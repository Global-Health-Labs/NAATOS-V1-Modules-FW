#include "samplePrepHeater.h"
#include "../motor.h"
#include "../usb.h"
#include "timers.h"

const MainStateErrorQueueMsg_t motor_stall_percent_err_msg = {
  .errType = ERR_MOTOR_STALLED_PERCENT,
  .overTempData = NULL
};

const MainStateErrorQueueMsg_t motor_stall_pwm_err_msg = {
  .errType = ERR_MOTOR_STALLED_PWM,
  .overTempData = NULL
};

cycle_config_parameters *s_cycle_config;

pid_controller_t heater_pid;
pid_controller_t motor_pid;

bool motor_running = false;
bool heater_running = false;
bool starting_sample_prep_run = true;
bool h_pwm_req = false;
bool greater_than_max = false;
bool s_rampToTemp = false;
bool motorStalledPercent = false;
bool motorStalledPWM = false;
bool motorReachedSpeed = false;
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
#ifdef SAMPLE_PREP_BOARD
void handle_sample_cycle_stopstart_heater(bool heating) {

  BaseType_t xReturned;
  
  // Update the Heating Enable
  if (!heating) { //need to stop supply before stopping pwm
    nrf_gpio_pin_clear(HEATER_PWR_EN);
  } else {
    nrf_gpio_pin_set(MOTOR_PWR_EN);
  }

  // Check if we want to Ramp To Temperature
  if (s_cycle_config->ramp_to_temp_before_start_cycle) {
    s_rampToTemp = true;
  }
  
  // Send the heater status
  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  msg.heaterRunning = heating;
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
  }

  // Enable/Disable the PWM if it has not been updated already
  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_HEATER_DISABLE};
  if (heating) {
    pwmMsg.type = PWM_MSG_HEATER_ENABLE;
  }
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
  }
 
  // Update the Watchdog
  watchdog_time_update_t wdtUpdate = {
    .taskName = HEATER,
    .valid = false};
  wdtUpdate.valid = heating;
  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }

  // Set the last sample based on config
  samp_log_max = (config.logging_rate / config.sample_rate);
}
#endif

void handle_cycle_stopstart_motor(bool motor_running) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  // update the motor enable
  if (motor_running) {
    nrf_gpio_pin_set(MOTOR_PWR_EN);
  } else {
    nrf_gpio_pin_clear(MOTOR_PWR_EN);
  }

  // Send the motor status
  MotorRxQueueMsg_t motorMsg;
  motorMsg.type = MOTOR_MSG_MOTOR_STATE;
  motorMsg.motorRunning = motor_running;
  if (motorMsg.motorRunning) {
    motor_running = true;
  }
  xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to motorRxQueue.");
  }

  // Update the PWM status for the motor
  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_MOTOR_DISABLE};
  if (motor_running) {
    pwmMsg.type = PWM_MSG_MOTOR_ENABLE;
  }
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
  }

  // Set the last sample based on config
  samp_log_max = (config.logging_rate / config.sample_rate);
#endif
}

void samplePrepResetHeaterPIDs(void) {
#ifdef SAMPLE_PREP_BOARD
  // Create PID Controllers
  pid_controller_init(&heater_pid, s_cycle_config->heater_setpoint, s_cycle_config->heater_kp, s_cycle_config->heater_ki, s_cycle_config->heater_kd, config.max_heater_pid_pwm);
  pid_controller_init(&motor_pid, s_cycle_config->motor_setpoint, s_cycle_config->motor_kp, s_cycle_config->motor_ki, s_cycle_config->motor_kd, 100); // Default 100% max PWM
  #endif
}

void samplePrepHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  local_temp_data = temperature_data;
  // Ensure temperatures are below the minimum run zone temperature
  if (s_cycle_config->min_run_zone_temp_en) {
    if (starting_sample_prep_run &&
        (temperature_data.heat_zone_0_temp > s_cycle_config->min_run_zone_temp || 
            temperature_data.heat_zone_1_temp > s_cycle_config->min_run_zone_temp ||
            temperature_data.heat_zone_2_temp > s_cycle_config->min_run_zone_temp ||
            temperature_data.heat_zone_3_temp > s_cycle_config->min_run_zone_temp)) {
      starting_sample_prep_run = false;
      // Send cannot start
      xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
      }
      return;
    } else if (starting_sample_prep_run &&
               (temperature_data.heat_zone_0_temp <= s_cycle_config->min_run_zone_temp && 
                   temperature_data.heat_zone_1_temp <= s_cycle_config->min_run_zone_temp &&
                   temperature_data.heat_zone_2_temp <= s_cycle_config->min_run_zone_temp &&
                   temperature_data.heat_zone_3_temp <= s_cycle_config->min_run_zone_temp)) {
      // Send can start
      xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
      }
      starting_sample_prep_run = false;
    }
  } else if (!s_cycle_config->min_run_zone_temp_en && starting_sample_prep_run) {
    // Send can start
    xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
    }
    starting_sample_prep_run = false;
  }

  // Update PID and PWM
  if (heater_running) {
    // Update Heater Zone 3 PID loop with new temperatures
    pid_controller_compute(&heater_pid, temperature_data.heat_zone_3_temp);
    
    // Check TO See if we have Ramped to Temperature if Enabled
    if (s_rampToTemp) {
      if ((temperature_data.heat_zone_3_temp >= s_cycle_config->heater_setpoint)) {
        xReturned = xQueueSend(main_setPointReached, &s_rampToTemp, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("HEATER_TASK: Unable to send set point reached message.");
        }
        s_rampToTemp = false;
      }
    }

    temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = 0,
          .heat_zone_1_pwm = 0,
          .sample_prep_heater_pwm = heater_pid.out,
          .heat_zone_2_pwm = 0,
          .heat_zone_3_pwm = heater_pid.out};

    // Update Heater Zone 3 PWM with PID output
    h_pwm_data.heat_zone_0_pwm = pwmData.heat_zone_0_pwm;
    h_pwm_data.heat_zone_1_pwm = pwmData.heat_zone_1_pwm;
    // Dont Heater Zone 2, its from the motor task
    h_pwm_data.heat_zone_3_pwm = pwmData.heat_zone_3_pwm;

    updateDutyCycles(h_pwm_data);

#if VERBOSE_HEATING
    char tmp[150];
    sprintf(tmp, "Heat Zones: %0.2f,%0.2f,%0.2f,%0.2f; PWM: %0.2f,%0.2f,%0.2f,%0.2f", 
                  temperature_data.heat_zone_0_temp, temperature_data.heat_zone_1_temp, temperature_data.heat_zone_2_temp, temperature_data.heat_zone_3_temp, 
                  h_pwm_data.heat_zone_0_pwm, h_pwm_data.heat_zone_1_pwm, h_pwm_data.heat_zone_2_pwm, h_pwm_data.heat_zone_3_pwm);
    send_debug_log_message(tmp);
#endif

    if ((config.heater_max_temp < temperature_data.heat_zone_3_temp) || temperature_data.heat_zone_3_temp < 0) {
      greater_than_max = true;
    }

    // Handle being greater than the maximum temperature
    if (greater_than_max) {
      // Send alert message to main task
      xReturned = xQueueSend(main_runErrorQueue, &greater_than_max, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send run error for greater than max temp to main_runErrorQueue.");
      }
      greater_than_max = false;
    }
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
    send_debug_log_message("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.");
  }

  samp_log_index++;
  if (samp_log_index >= samp_log_max) {
    logMsg.data_type = TEMPERATURE_DATA;
    samp_log_index = 0;
    // Send the Log message
    xReturned = xQueueSend(logger_logMessageQueue, (void *)&logMsg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.");
    }
  }
  #endif
}

void handleSampleMotorDataRx(int motor_speed) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  char tmp[100];
  
  if (motor_running) {
    // Maintain Current Heater PID PWM
    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = 0,
        .heat_zone_1_pwm = heater_pid.out,
        .heat_zone_2_pwm = 0,
        .heat_zone_3_pwm = 0};

    if (s_cycle_config->run_motor) {
      pid_controller_compute(&motor_pid, motor_speed);
      pwmData.heat_zone_2_pwm = motor_pid.out;
    }

    h_pwm_data.heat_zone_2_pwm = pwmData.heat_zone_2_pwm;
    last_motor_speed = motor_speed;

    updateDutyCycles(pwmData);

  #if VERBOSE_MOTOR
    sprintf(tmp, "Motor Speed: %d rpm; Motor PWM: %0.2f", motor_speed, pwmData.heat_zone_2_pwm);
    send_debug_log_message(tmp);
  #endif
  
    // Check if Motor has come up to speed yet
    if (motor_speed >= s_cycle_config->motor_setpoint && config.motor_stall_en) {
      motorReachedSpeed = true;
    }

    // Motor Stall Percentage Check
    if (motorReachedSpeed && motor_speed < (s_cycle_config->motor_setpoint - ((float)s_cycle_config->motor_setpoint * ((float)config.motor_stall_percent / 100.0 )))) {
      motorStalledPercent = true;
      motorReachedSpeed = false;
    }
    // Motor Stall PWM Check
    if (motorReachedSpeed && motor_pid.out >= config.motor_stall_pwm) {
      motorStalledPWM = true;
      motorReachedSpeed = false;
    }
    // Handle motor Stall Detection
    if (motorStalledPercent || motorStalledPWM) {
      // Set LEDs
      updateLedState(LED_ABORT, true);
      // Send alert message to main task
      if (motorStalledPercent) {
        xReturned = xQueueSend(main_runErrorQueue, &motor_stall_percent_err_msg, 0);
      } else {
        xReturned = xQueueSend(main_runErrorQueue, &motor_stall_pwm_err_msg, 0);
      }
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send run error for motor stalled to main_runErrorQueue.");
      }
      motorStalledPWM = false;
      motorStalledPercent = false;
    }
  }
  #endif
}

void samplePrepHandleZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  /* Cycle Ending */
  if (!heaterRxMessage.cycleEnabled) { //AMP2 will be used in sample prep for heating
    // Set IUD outputs to 0
    heater_pid.out = 0;
    motor_pid.out = 0;
    // Update PWM Duty Cycles
    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = 0,
        .heat_zone_1_pwm = heater_pid.out,
        .sample_prep_heater_pwm = heater_pid.out,
        .heat_zone_2_pwm = motor_pid.out,
        .heat_zone_3_pwm = 0};
    updateDutyCycles(pwmData);
    // Reset Control Vars
    motorReachedSpeed = false;
    starting_sample_prep_run = false;
    // Send stop heater and motor
    heater_running = false;
    motor_running = false;
    handle_sample_cycle_stopstart_heater(heater_running);
    handle_cycle_stopstart_motor(motor_running);

  /* Cycle Starting */
  } else {
    // Update the cycle config to the current cycle
    s_cycle_config = &cycle_configs[(uint16_t)(heaterRxMessage.cycleSelect - 1)]; // Index = cycle - 1
    // Reset PIDs
    samplePrepResetHeaterPIDs();
    // Reset Control Vars
    motorReachedSpeed = false;
    starting_sample_prep_run = true;
    // Set running based on new config
    heater_running = s_cycle_config->run_heater;
    motor_running = s_cycle_config->run_motor;
    // Send start heater and motor
    handle_sample_cycle_stopstart_heater(heater_running);
    handle_cycle_stopstart_motor(motor_running);
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
  return heater_running;
}