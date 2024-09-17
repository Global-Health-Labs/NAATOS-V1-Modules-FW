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
bool starting_sample_prep_run = true;
bool h_pwm_req = false;
bool greater_than_max = false;
bool heater_run = false;
bool rampToTemp = false;
int last_motor_speed = 0;

temperature_data_t local_temp_data;

uint32_t samp_log_index = 0;
uint32_t samp_log_max = 0;

temperature_pwm_data_t h_pwm_data = {
    .heat_zone_0_pwm = 0,
    .heat_zone_1_pwm = 0,
    .heat_zone_2_pwm = 0,
    .heat_zone_3_pwm = 0,
    .sample_prep_heater_pwm = 0};

static log_data_message_t logMsg = {
    .data_type = TEMPERATURE_DATA,
    .event_data = NULL,
    .temperature_data = NULL};

void handle_cycle2_stopstart_heater(bool heating) {
  BaseType_t xReturned;

  // Handle case where cycle one is already running, dont want to send stop
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
#ifdef SAMPLE_PREP_BOARD
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
  #endif
}

void samplePrepHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  local_temp_data = temperature_data;
  // Ensure temperatures are below the minimum run zone temperature
  if (config.min_run_zone_temp_en) {
    if (starting_sample_prep_run &&
        (temperature_data.heat_zone_0_temp > config.min_run_zone_temp || // TODO: Implement defaults
            temperature_data.heat_zone_1_temp > config.min_run_zone_temp ||
            temperature_data.heat_zone_2_temp > config.min_run_zone_temp ||
            temperature_data.heat_zone_3_temp > config.min_run_zone_temp)) {
      starting_sample_prep_run = false;
      // Send cannot start
      xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: Unable to send cannot start run response.\n");
      }
      return;
    } else if (starting_sample_prep_run &&
               (temperature_data.heat_zone_0_temp <= config.min_run_zone_temp && // TODO: Implement defaults
                   temperature_data.heat_zone_1_temp <= config.min_run_zone_temp &&
                   temperature_data.heat_zone_2_temp <= config.min_run_zone_temp &&
                   temperature_data.heat_zone_3_temp <= config.min_run_zone_temp)) {
      // Send can start
      xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: Unable to send cannot start run response.\n");
      }
      starting_sample_prep_run = false;
    }
  } else if (!config.min_run_zone_temp_en && starting_sample_prep_run) {
    // Send can start
    xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
    if (xReturned != pdPASS) {
      printf("HEATER_TASK: Unable to send cannot start run response.\n");
    }
    starting_sample_prep_run = false;
  }

  // Update PID and PWM
  if (heater_cycle1_running) {
    // Update Heater Zone 3 PID loop with new temperatures
    if (config.run_heater_1) {
      pid_controller_compute(&heater_pid_1, temperature_data.heat_zone_3_temp);
      if (rampToTemp && (temperature_data.heat_zone_3_temp >= heater1SetPoint)) {
        xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send set point reached message.\n");
        }
        rampToTemp = false;
      }
    }

    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = 0,
        .heat_zone_1_pwm = heater_pid_1.out,
        .sample_prep_heater_pwm = heater_pid_1.out,
        .heat_zone_2_pwm = 0,
        .heat_zone_3_pwm = 0};

    // Update Heater Zone 3 PWM with PID output
    h_pwm_data.heat_zone_0_pwm = pwmData.heat_zone_0_pwm;
    h_pwm_data.heat_zone_1_pwm = pwmData.heat_zone_1_pwm;
    // Dont Heater Zone 2, its from the motor task
    h_pwm_data.heat_zone_3_pwm = pwmData.heat_zone_3_pwm;

    updateDutyCycles(h_pwm_data);

    char tmp[150];
    sprintf(tmp, "Heat Zones: %0.2f,%0.2f,%0.2f,%0.2f; PWM: %0.2f,%0.2f,%0.2f,%0.2f\r\n", 
                  temperature_data.heat_zone_0_temp, temperature_data.heat_zone_1_temp, temperature_data.heat_zone_2_temp, temperature_data.heat_zone_3_temp, 
                  h_pwm_data.heat_zone_0_pwm, h_pwm_data.heat_zone_1_pwm, h_pwm_data.heat_zone_2_pwm, h_pwm_data.heat_zone_3_pwm);
    printf(tmp);

    if ((config.heater_max_temp < temperature_data.heat_zone_3_temp) || temperature_data.heat_zone_3_temp < 0 || temperature_data.heat_zone_3_temp > 120) {
      greater_than_max = true;
    }
  }

  if (heater_cycle2_running) { //AMP 1 will be used for motor
    // Update Heater Zone 3 PID loop with new temperatures
    if (config.run_heater_2) {
      pid_controller_compute(&heater_pid_2, temperature_data.heat_zone_3_temp);
      if (rampToTemp && (temperature_data.heat_zone_3_temp >= heater2SetPoint)) {
        xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send set point reached message.\n");
        }
        rampToTemp = false;
      }
    }

    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = 0,
        .heat_zone_1_pwm = heater_pid_2.out,
        .heat_zone_2_pwm = 0,
        .heat_zone_3_pwm = 0};

    h_pwm_data.heat_zone_0_pwm = pwmData.heat_zone_0_pwm;
    h_pwm_data.heat_zone_1_pwm = pwmData.heat_zone_1_pwm;
    h_pwm_data.heat_zone_3_pwm = pwmData.heat_zone_3_pwm;

    updateDutyCycles(h_pwm_data);

    char tmp[150];
    sprintf(tmp, "Heat Zones: %0.2f,%0.2f,%0.2f,%0.2f; PWM: %0.2f,%0.2f,%0.2f,%0.2f\r\n", 
                  temperature_data.heat_zone_0_temp, temperature_data.heat_zone_1_temp, temperature_data.heat_zone_2_temp, temperature_data.heat_zone_3_temp, 
                  h_pwm_data.heat_zone_0_pwm, h_pwm_data.heat_zone_1_pwm, h_pwm_data.heat_zone_2_pwm, h_pwm_data.heat_zone_3_pwm);
    printf(tmp);

    if ((config.heater_max_temp < temperature_data.heat_zone_3_temp) || temperature_data.heat_zone_3_temp < 0 || temperature_data.heat_zone_3_temp > 120) {
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

  logMsg.temperature_data.heat_zone_1_temp = temperature_data.heat_zone_1_temp;
  logMsg.temperature_data.heat_zone_2_temp = temperature_data.heat_zone_2_temp;
  logMsg.temperature_data.heat_zone_3_temp = temperature_data.heat_zone_3_temp;
  logMsg.temperature_data.heat_zone_0_temp = temperature_data.heat_zone_0_temp;
  logMsg.temperature_data.heat_zone_1_pwm = h_pwm_data.heat_zone_1_pwm;
  logMsg.temperature_data.heat_zone_2_pwm = h_pwm_data.heat_zone_2_pwm;
  logMsg.temperature_data.heat_zone_3_pwm = h_pwm_data.heat_zone_3_pwm;
  logMsg.temperature_data.heat_zone_0_pwm = h_pwm_data.heat_zone_0_pwm;
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
  #endif
}

void handleSampleMotorDataRx(int motor_speed) {
#ifdef SAMPLE_PREP_BOARD
  if (heater_cycle1_running) {
    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = 0,
        .heat_zone_1_pwm = heater_pid_1.out,
        .heat_zone_2_pwm = 0,
        .heat_zone_3_pwm = 0};

    if (config.run_motor_1) {
      pid_controller_compute(&motor_pid_1, motor_speed);
      pwmData.heat_zone_2_pwm = motor_pid_1.out;
    }

    h_pwm_data.heat_zone_2_pwm = pwmData.heat_zone_2_pwm;
    last_motor_speed = motor_speed;

    updateDutyCycles(pwmData);
  } else if (heater_cycle2_running) {
    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = 0,
        .heat_zone_1_pwm = heater_pid_2.out,
        .heat_zone_2_pwm = 0,
        .heat_zone_3_pwm = 0};

    if (config.run_motor_2) {
      pid_controller_compute(&motor_pid_2, motor_speed);
      pwmData.heat_zone_2_pwm = motor_pid_2.out;
    }

    h_pwm_data.heat_zone_2_pwm = pwmData.heat_zone_2_pwm;
    last_motor_speed = motor_speed;

    updateDutyCycles(pwmData);
  }
  #endif
}

void samplePrepHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
#ifdef SAMPLE_PREP_BOARD
  // Set zones enabled
  if (heaterRxMessage.cycleSelect == CYCLE_ONE) {
    heater_cycle1_running = heaterRxMessage.cycleEnabled;
    rampToTemp = config.ramp_to_temp_before_start_cycle_1;
    if (!heater_cycle1_running) { //AMP2 will be used in sample prep for heating
      heater_pid_1.out = 0;
      temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = 0,
          .heat_zone_1_pwm = heater_pid_1.out,
          .sample_prep_heater_pwm = heater_pid_1.out,
          .heat_zone_2_pwm = 0,
          .heat_zone_3_pwm = 0};
      updateDutyCycles(pwmData);

      // Send stop heater to sensors task
      heater_run = false;
      starting_sample_prep_run = false;
      handle_cycle1_stopstart_heater(heater_run);
    } else {
      starting_sample_prep_run = true;
      heater_run = true;
      samplePrepResetHeaterPIDs();
      // Send starting heater to sensors task
      handle_cycle1_stopstart_heater(heater_run);
    }
  } else if (heaterRxMessage.cycleSelect = CYCLE_TWO) {
    heater_cycle2_running = heaterRxMessage.cycleEnabled;
    rampToTemp = config.ramp_to_temp_before_start_cycle_2;
    if (!heater_cycle2_running) {
      heater_pid_2.out = 0;
      temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = 0,
          .heat_zone_1_pwm = heater_pid_2.out,
          .sample_prep_heater_pwm = heater_pid_2.out,
          .heat_zone_2_pwm = 0,
          .heat_zone_3_pwm = 0};
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
  #endif
}

temperature_pwm_data_t getSamplePrepPwmData(void) {
  return h_pwm_data;
}

bool getSamplePrepOverTempStatus(void) {
  return greater_than_max;
}

temperature_data_t getSamplePrepOverTempData(void) {
  return local_temp_data;
}

bool getSamplePrepHeaterRunningStatus(void) {
  return heater_run;
}