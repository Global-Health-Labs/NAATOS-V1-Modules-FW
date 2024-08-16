#include "samplePrepHeater.h"
#include "../motor.h"
#include "../usb.h"
#include "timers.h"

pid_controller_t heater_pid_1;
pid_controller_t heater_pid_2;
pid_controller_t motor_pid_1;
pid_controller_t motor_pid_2;

float heater1SetPoint = 0;
float heater2SetPoint = 0;

bool heater_cycle1_running = false;
bool heater_cycle2_running = false;
bool starting_run = true;
bool h_pwm_req = false;
bool greater_than_max = false;
bool heater_run = false;
bool rampToTemp = false;
int last_motor_speed = 0;

uint32_t samp_log_index = 0;
uint32_t samp_log_max = 0;

temperature_pwm_data_t h_pwm_data = {
    .valve_zone_pwm = 0,
    .amp0_zone_pwm = 0,
    .amp1_zone_pwm = 0,
    .amp2_zone_pwm = 0,
    .sample_prep_heater_pwm = 0};

static log_data_message_t logMsg = {
    .data_type = TEMPERATURE_DATA,
    .event_data = NULL,
    .temperature_data = NULL};

void handle_cycle2_stopstart_heater(bool heating) {
  BaseType_t xReturned;

  // Handle case where amplification zone is on already, dont want to send stop
  if (heater_cycle1_running && !heating)
    return;

  if (!heating) { //need to stop supply before stopping pwm
    nrf_gpio_pin_clear(MOTOR_POWER_ENABLE);
  }

  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  msg.heaterRunning = heating;

  // Send the heater status
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("HEATER_TASK: Unable to send heater state to sensorRxQueue.\n");
  }

  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  wdtUpdate.valid = heating;

  if (heating) {
    nrf_gpio_pin_set(MOTOR_POWER_ENABLE);
  }

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
  }

  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

  if (heating) {
    pwmMsg.type = PWM_MSG_ENABLE;
  }

  // Respond to heater change
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    printf("heater: Unable to send stop to pwmRxQueue.\n");
  }

  // Set the last sample based on config
  if (use_default_configuration_parameters) {
    samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
  } else {
    samp_log_max = (config.logging_rate / config.sample_rate);
  }
}

void handle_cycle1_stopstart_heater(bool heating) {
  BaseType_t xReturned;

  // Handle case where valve zone is on already, dont want to send stop
  if (heater_cycle2_running && !heating)
    return;

  if (!heating) { //need to stop supply before stopping pwm
    nrf_gpio_pin_clear(MOTOR_POWER_ENABLE);
  }

  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  msg.heaterRunning = heating;

  // Send the heater status
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("HEATER_TASK: Unable to send heater state to sensorRxQueue.\n");
  }

  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  wdtUpdate.valid = heating;

  if (heating) {
    nrf_gpio_pin_set(MOTOR_POWER_ENABLE);
    //nrf_gpio_pin_set(BOOST_CONTROL_ENABLE_PIN);
  }

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
  }

  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

  if (heating) {
    pwmMsg.type = PWM_MSG_ENABLE;
  }

  // Respond to heater change
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    printf("heater: Unable to send stop to pwmRxQueue.\n");
  }

  // Set the last sample based on config
  if (use_default_configuration_parameters) {
    samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
  } else {
    samp_log_max = (config.logging_rate / config.sample_rate);
  }
}

void samplePrepResetHeaterPIDs(void) {
  int pid_max = DEFAULT_MAX_HEATER_PID;

  if (!use_default_configuration_parameters) {
    if (config.max_heater_pid_pwm > 0 && config.max_heater_pid_pwm <= DEFAULT_MAX_HEATER_PID) {
      pid_max = config.max_heater_pid_pwm;
    }
  }
  // Create PID Controllers
  if (use_default_configuration_parameters) {
    heater1SetPoint = DEFAULT_HEATER_SETPOINT;
    pid_controller_init(&heater_pid_1, DEFAULT_HEATER_SETPOINT, H_KP, H_KI, H_KD, pid_max);
  } else {
    heater1SetPoint = config.heater_setpoint_1;
    pid_controller_init(&heater_pid_1, config.heater_setpoint_1, config.heater_kp_1, config.heater_ki_1, config.heater_kd_1, pid_max);
  }

  if (use_default_configuration_parameters) {
    heater2SetPoint = DEFAULT_HEATER_SETPOINT;
    pid_controller_init(&heater_pid_2, DEFAULT_HEATER_SETPOINT, H_KP, H_KI, H_KD, pid_max);
  } else {
    heater2SetPoint = config.heater_setpoint_2;
    pid_controller_init(&heater_pid_2, config.heater_setpoint_2, config.heater_kp_2, config.heater_ki_2, config.heater_kd_2, pid_max);
  }

  if (use_default_configuration_parameters) {
    pid_controller_init(&motor_pid_1, MOTOR_SETPOINT_1, M_KP, M_KI, M_KD, MAX_MOTOR_PID);
  } else {
    pid_controller_init(&motor_pid_1, config.motor_setpoint_1, config.motor_kp_1, config.motor_ki_1, config.motor_kd_1, MAX_MOTOR_PID);
  }

  if (use_default_configuration_parameters) {
    pid_controller_init(&motor_pid_2, MOTOR_SETPOINT_2, M_KP, M_KI, M_KD, MAX_MOTOR_PID);
  } else {
    pid_controller_init(&motor_pid_2, config.motor_setpoint_2, config.motor_kp_2, config.motor_ki_2, config.motor_kd_2, MAX_MOTOR_PID);
  }
}

void samplePrepHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
  BaseType_t xReturned;

  // Ensure temperatures are below the minimum run zone temperature
  if (config.min_run_zone_temp_en) {
    if (starting_run &&
        (temperature_data.valve_zone_temp > config.min_run_zone_temp || // TODO: Implement defaults
            temperature_data.amp0_zone_temp > config.min_run_zone_temp ||
            temperature_data.amp1_zone_temp > config.min_run_zone_temp ||
            temperature_data.amp2_zone_temp > config.min_run_zone_temp)) {
      starting_run = false;
      // Send cannot start
      xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: Unable to send cannot start run response.\n");
      }
      return;
    } else if (starting_run &&
               (temperature_data.valve_zone_temp <= config.min_run_zone_temp && // TODO: Implement defaults
                   temperature_data.amp0_zone_temp <= config.min_run_zone_temp &&
                   temperature_data.amp1_zone_temp <= config.min_run_zone_temp &&
                   temperature_data.amp2_zone_temp <= config.min_run_zone_temp)) {
      // Send can start
      xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: Unable to send cannot start run response.\n");
      }
      starting_run = false;
    }
  } else if (!config.min_run_zone_temp_en && starting_run) {
    // Send can start
    xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
    if (xReturned != pdPASS) {
      printf("HEATER_TASK: Unable to send cannot start run response.\n");
    }
    starting_run = false;
  }

  // Update PID and PWM
  if (heater_cycle1_running) {
    // Update Amplification 2 PID loop with new temperatures
    if (config.run_heater_1) {
      pid_controller_compute(&heater_pid_1, temperature_data.amp2_zone_temp);
      if (rampToTemp && (temperature_data.amp2_zone_temp >= heater1SetPoint)) {
        xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send set point reached message.\n");
        }
        rampToTemp = false;
      }
    }

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = 0,
        .amp0_zone_pwm = heater_pid_1.out,
        .sample_prep_heater_pwm = heater_pid_1.out,
        .amp1_zone_pwm = 0,
        .amp2_zone_pwm = 0};

    // Update Amplification 2 PWM with PID output
    h_pwm_data.valve_zone_pwm = pwmData.valve_zone_pwm;
    h_pwm_data.amp0_zone_pwm = pwmData.amp0_zone_pwm;
    // Dont want Amp1, its from the motor task
    h_pwm_data.amp2_zone_pwm = pwmData.amp2_zone_pwm;

    updateDutyCycles(h_pwm_data);

    if ((config.heater_max_temp < temperature_data.amp2_zone_temp) || temperature_data.amp2_zone_temp < 0 || temperature_data.amp2_zone_temp > 120) {
      greater_than_max = true;
    }
  }
  if (heater_cycle2_running) { //AMP 1 will be used for motor
    // Update Amplification 2 PID loop with new temperatures
    if (config.run_heater_2) {
      pid_controller_compute(&heater_pid_2, temperature_data.amp2_zone_temp);
      if (rampToTemp && (temperature_data.amp2_zone_temp >= heater2SetPoint)) {
        xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send set point reached message.\n");
        }
        rampToTemp = false;
      }
    }

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = 0,
        .amp0_zone_pwm = heater_pid_2.out,
        .amp1_zone_pwm = 0,
        .amp2_zone_pwm = 0};

    h_pwm_data.valve_zone_pwm = pwmData.valve_zone_pwm;
    h_pwm_data.amp0_zone_pwm = pwmData.amp0_zone_pwm;
    h_pwm_data.amp2_zone_pwm = pwmData.amp2_zone_pwm;

    updateDutyCycles(h_pwm_data);

    if ((config.heater_max_temp < temperature_data.amp2_zone_temp) || temperature_data.amp2_zone_temp < 0 || temperature_data.amp2_zone_temp > 120) {
      greater_than_max = true;
    }
  }

  // Handle being greater than the maximum temperature
  if (greater_than_max) {
    // Send alert message to main task
    xReturned = xQueueSend(main_runErrorQueue, &greater_than_max, 0);
    if (xReturned != pdPASS) {
      printf("HEATER_TASK: Unable to send run error for greater than max temp to main_runErrorQueue.\n");
    }
    greater_than_max = false;
  }

  logMsg.temperature_data.amp0_zone_temp = temperature_data.amp0_zone_temp;
  logMsg.temperature_data.amp1_zone_temp = temperature_data.amp1_zone_temp;
  logMsg.temperature_data.amp2_zone_temp = temperature_data.amp2_zone_temp;
  logMsg.temperature_data.valve_zone_temp = temperature_data.valve_zone_temp;
  logMsg.temperature_data.amp0_zone_pwm = h_pwm_data.amp0_zone_pwm;
  logMsg.temperature_data.amp1_zone_pwm = h_pwm_data.amp1_zone_pwm;
  logMsg.temperature_data.amp2_zone_pwm = h_pwm_data.amp1_zone_pwm;
  logMsg.temperature_data.valve_zone_pwm = h_pwm_data.valve_zone_pwm;
  logMsg.temperature_data.motorSpeed = last_motor_speed;
  
  logMsg.data_type = UART_DATA;
  // Send the Log message
  xReturned = xQueueSend(logger_logMessageQueue, (void *)&logMsg, 0);
  if (xReturned != pdPASS) {
    printf("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.\n");
  }

  samp_log_index++;
  if (samp_log_index >= samp_log_max) {
    logMsg.data_type = TEMPERATURE_DATA;
    samp_log_index = 0;
    // Send the Log message
    xReturned = xQueueSend(logger_logMessageQueue, (void *)&logMsg, 0);
    if (xReturned != pdPASS) {
      printf("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.\n");
    }
  }
}

void handleSampleMotorDataRx(int motor_speed) {
  if (heater_cycle1_running) {
    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = 0,
        .amp0_zone_pwm = heater_pid_1.out,
        .amp1_zone_pwm = 0,
        .amp2_zone_pwm = 0};

    if (config.run_motor_1) {
      pid_controller_compute(&motor_pid_1, motor_speed);
      pwmData.amp1_zone_pwm = motor_pid_1.out;
    }

    h_pwm_data.amp1_zone_pwm = pwmData.amp1_zone_pwm;
    last_motor_speed = motor_speed;

    updateDutyCycles(pwmData);
  } else if (heater_cycle2_running) {
    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = 0,
        .amp0_zone_pwm = heater_pid_2.out,
        .amp1_zone_pwm = 0,
        .amp2_zone_pwm = 0};

    if (config.run_motor_2) {
      pid_controller_compute(&motor_pid_2, motor_speed);
      pwmData.amp1_zone_pwm = motor_pid_2.out;
    }

    h_pwm_data.amp1_zone_pwm = pwmData.amp1_zone_pwm;
    last_motor_speed = motor_speed;

    updateDutyCycles(pwmData);
  }
}

void samplePrepHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
  // Set zones enabled
  if (heaterRxMessage.zoneSelect == AMPLIFICATION) {
    heater_cycle1_running = heaterRxMessage.zoneEnabled;
    rampToTemp = config.ramp_to_temp_before_start_cycle_1;
    if (!heater_cycle1_running) { //AMP2 will be used in sample prep for heating
      heater_pid_1.out = 0;
      temperature_pwm_data_t pwmData = {
          .valve_zone_pwm = 0,
          .amp0_zone_pwm = heater_pid_1.out,
          .sample_prep_heater_pwm = heater_pid_1.out,
          .amp1_zone_pwm = 0,
          .amp2_zone_pwm = 0};
      updateDutyCycles(pwmData);

      // Send stop heater to sensors task
      heater_run = false;
      starting_run = false;
      handle_cycle1_stopstart_heater(heater_run);
    } else {
      starting_run = true;
      heater_run = true;
      samplePrepResetHeaterPIDs();
      // Send starting heater to sensors task
      handle_cycle1_stopstart_heater(heater_run);
    }
  } else if (heaterRxMessage.zoneSelect = VALVE) {
    heater_cycle2_running = heaterRxMessage.zoneEnabled;
    rampToTemp = config.ramp_to_temp_before_start_cycle_2;
    if (!heater_cycle2_running) {
      heater_pid_2.out = 0;
      temperature_pwm_data_t pwmData = {
          .valve_zone_pwm = 0,
          .amp0_zone_pwm = heater_pid_2.out,
          .sample_prep_heater_pwm = heater_pid_2.out,
          .amp1_zone_pwm = 0,
          .amp2_zone_pwm = 0};
      updateDutyCycles(pwmData);

      // Send stop heater to sensors task
      heater_run = false;
      handle_cycle2_stopstart_heater(heater_run);
    } else {
      heater_run = true;
      samplePrepResetHeaterPIDs();
      handle_cycle2_stopstart_heater(heater_run);
    }
  }
}

temperature_pwm_data_t getSamplePrepPwmData(void) {
  return h_pwm_data;
}

bool getSamplePrepOverTempStatus(void) {
  return greater_than_max;
}

bool getSamplePrepHeaterRunningStatus(void) {
  return heater_run;
}