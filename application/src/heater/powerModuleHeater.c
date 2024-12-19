#include "powerModuleHeater.h"
#include "timers.h"

pid_controller_t valve_pid_1;
pid_controller_t amp_pid_1;

pid_controller_t valve_pid_2;
pid_controller_t amp_pid_2;

temperature_pwm_data_t pm_h_pwm_data = {
    .heat_zone_0_pwm = 0,
    .heat_zone_1_pwm = 0,
    .heat_zone_2_pwm = 0,
    .heat_zone_3_pwm = 0,
    .sample_prep_heater_pwm = 0};

bool pm_amp_greater_than_max = false;
bool pm_amp_heater_running = false;
bool pm_valve_greater_than_max = false;
bool pm_valve_heater_running = false;

bool cycle_one_running = false;
bool cycle_two_running = false;
bool starting_run = true;

temperature_data_t pm_local_temp_data;
uint32_t p_samp_log_index = 0;
uint32_t p_samp_log_max = 0;

static log_data_message_t logMsg = {
    .data_type = TEMPERATURE_DATA,
    .event_data = NULL,
    .temperature_data = NULL};

void handle_cycle_two_stopstart_heater(bool amp_heating, bool valve_heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  if (cycle_one_running && !amp_heating && !valve_heating)
    return;

  if (amp_heating) {
    nrf_gpio_pin_set(AMP_PWR_EN);
  } else {
    nrf_gpio_pin_clear(AMP_PWR_EN);
  }

  if (valve_heating) {
    nrf_gpio_pin_set(VALVE_PWR_EN);
  } else {
    nrf_gpio_pin_clear(VALVE_PWR_EN);
  }
  
  // Send the heater status
  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  msg.heaterRunning = false;
  if (amp_heating || valve_heating) {
    msg.heaterRunning = true;
  }
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
  }
  
  // Update the watchdog status
  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  if (amp_heating || valve_heating) {
    wdtUpdate.valid = true;
  }
  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }
  
  // Respond to heater change
  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};
  if (amp_heating || valve_heating) {
    pwmMsg.type = PWM_MSG_ENABLE;
  }
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
  }

  // Set the last sample based on config
  if (use_default_configuration_parameters) {
    p_samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
  } else {
    p_samp_log_max = (config.logging_rate / config.sample_rate);
  }
#endif
}

void handle_cycle_one_stopstart_heater(bool amp_heating, bool valve_heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  if (cycle_one_running && !amp_heating && !valve_heating)
    return;

  if (amp_heating) {
    nrf_gpio_pin_set(AMP_PWR_EN);
  } else {
    nrf_gpio_pin_clear(AMP_PWR_EN);
  }

  if (valve_heating) {
    nrf_gpio_pin_set(VALVE_PWR_EN);
  } else {
    nrf_gpio_pin_clear(VALVE_PWR_EN);
  }
  
  // Send the heater status
  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  msg.heaterRunning = false;
  if (amp_heating || valve_heating) {
    msg.heaterRunning = true;
  }
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
  }
  
  // Update the watchdog status
  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  if (amp_heating || valve_heating) {
    wdtUpdate.valid = true;
  }
  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }
  
  // Respond to heater change
  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};
  if (amp_heating || valve_heating) {
    pwmMsg.type = PWM_MSG_ENABLE;
  }
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
  }

  // Set the last sample based on config
  if (use_default_configuration_parameters) {
    p_samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
  } else {
    p_samp_log_max = (config.logging_rate / config.sample_rate);
  }
#endif
}

void powerModuleHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  pm_local_temp_data = temperature_data;

  // Ensure temperatures are below the minimum run zone temperature
  if (config.min_run_zone_temp_en) {
    if (starting_run &&
        (temperature_data.heat_zone_0_temp > config.min_run_zone_temp || 
            temperature_data.heat_zone_1_temp > config.min_run_zone_temp ||
            temperature_data.heat_zone_2_temp > config.min_run_zone_temp ||
            temperature_data.heat_zone_3_temp > config.min_run_zone_temp)) {
      starting_run = false;
      // Send cannot start
      xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
      }
      return;
    } else if (starting_run &&
               (temperature_data.heat_zone_0_temp <= config.min_run_zone_temp && 
                   temperature_data.heat_zone_1_temp <= config.min_run_zone_temp &&
                   temperature_data.heat_zone_2_temp <= config.min_run_zone_temp &&
                   temperature_data.heat_zone_3_temp <= config.min_run_zone_temp)) {
      // Send can start
      xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
      }
      starting_run = false;
    }
  } else if (!config.min_run_zone_temp_en && starting_run) {
    // Send can start
    xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
    }
    starting_run = false;
  }

  // Update PID and PWM
  if (cycle_one_running) {
    // Update Amplification PID loop with new temperatures
    pid_controller_compute(&amp_pid_1, temperature_data.heat_zone_2_temp);
    // Update Valve PID loop with new temperatures
    pid_controller_compute(&valve_pid_1, temperature_data.heat_zone_0_temp);

    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = valve_pid_1.out, // Heat Zone 0 controls Valve
        .heat_zone_1_pwm = 0,
        .heat_zone_2_pwm = amp_pid_1.out,   // Heat Zone 2 controls Amplification
        .heat_zone_3_pwm = 0};
    updateDutyCycles(pwmData);
    
    // Set the PWMs for the logger
    pm_h_pwm_data.heat_zone_0_pwm = pwmData.heat_zone_0_pwm;
    pm_h_pwm_data.heat_zone_1_pwm = pwmData.heat_zone_1_pwm;
    pm_h_pwm_data.heat_zone_2_pwm = pwmData.heat_zone_2_pwm;
    pm_h_pwm_data.heat_zone_3_pwm = pwmData.heat_zone_3_pwm;
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if (config.amp_max_temp < temperature_data.heat_zone_2_temp || temperature_data.heat_zone_2_temp < 0) {
      pm_amp_greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.heat_zone_0_temp || temperature_data.heat_zone_0_temp < 0) {
      pm_valve_greater_than_max = true;
    }
  }
  if (cycle_two_running) {
    // Update Amplification PID loop with new temperatures
    pid_controller_compute(&amp_pid_2, temperature_data.heat_zone_2_temp);
    // Update Valve PID loop with new temperatures
    pid_controller_compute(&valve_pid_2, temperature_data.heat_zone_0_temp);

    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = valve_pid_2.out,
        .heat_zone_1_pwm = 0,
        .heat_zone_2_pwm = amp_pid_2.out,
        .heat_zone_3_pwm = 0};
    updateDutyCycles(pwmData);

    // Set the PWMs for the logger
    pm_h_pwm_data.heat_zone_0_pwm = pwmData.heat_zone_0_pwm;
    pm_h_pwm_data.heat_zone_1_pwm = pwmData.heat_zone_1_pwm;
    pm_h_pwm_data.heat_zone_2_pwm = pwmData.heat_zone_2_pwm;
    pm_h_pwm_data.heat_zone_3_pwm = pwmData.heat_zone_3_pwm;
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if (config.amp_max_temp < temperature_data.heat_zone_2_temp || temperature_data.heat_zone_2_temp < 0) {
      pm_amp_greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.heat_zone_0_temp || temperature_data.heat_zone_0_temp < 0) {
      pm_valve_greater_than_max = true;
    }
  }

  // Handle being greater than the maximum temperature
  if (pm_amp_greater_than_max || pm_valve_greater_than_max) {
    bool msg = true;
    // Send alert message to main task
    xReturned = xQueueSend(main_runErrorQueue, &msg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send run error for greater than max temp to main_runErrorQueue.");
    }
  }

#if VERBOSE_PID
  char w_buff[100];
  if (cycle_one_running) {
    sprintf(w_buff, "V Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.heat_zone_0_temp, valve_pid_1.out);
    send_debug_log_message(w_buff);
    sprintf(w_buff, "A Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.heat_zone_2_temp, amp_pid_1.out);
    send_debug_log_message(w_buff);
  }
  if (cycle_two_running) {
    sprintf(w_buff, "V Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.heat_zone_0_temp, valve_pid_2.out);
    send_debug_log_message(w_buff);
    sprintf(w_buff, "A Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.heat_zone_2_temp, amp_pid_2.out);
    send_debug_log_message(w_buff);
  }
#endif

  logMsg.temperature_data.heat_zone_1_temp = temperature_data.heat_zone_1_temp;
  logMsg.temperature_data.heat_zone_2_temp = temperature_data.heat_zone_2_temp;
  logMsg.temperature_data.heat_zone_3_temp = temperature_data.heat_zone_3_temp;
  logMsg.temperature_data.heat_zone_0_temp = temperature_data.heat_zone_0_temp;
  logMsg.temperature_data.heat_zone_1_pwm = pm_h_pwm_data.heat_zone_1_pwm;
  logMsg.temperature_data.heat_zone_2_pwm = pm_h_pwm_data.heat_zone_2_pwm;
  logMsg.temperature_data.heat_zone_3_pwm = pm_h_pwm_data.heat_zone_3_pwm;
  logMsg.temperature_data.heat_zone_0_pwm = pm_h_pwm_data.heat_zone_0_pwm; 

  logMsg.data_type = UART_DATA;
  // Send the Log message
  xReturned = xQueueSend(logger_logMessageQueue, (void *)&logMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.");
  }

  p_samp_log_index++;
  if (p_samp_log_index > p_samp_log_max) {
    logMsg.data_type = TEMPERATURE_DATA;
    p_samp_log_index = 0;
    // Send the Log message
    xReturned = xQueueSend(logger_logMessageQueue, (void *)&logMsg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.");
    }
  }
#endif
}

void powerModuleHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
#ifndef SAMPLE_PREP_BOARD
  // Set zones enabled
  if (heaterRxMessage.cycleSelect == CYCLE_ONE) {
    cycle_one_running = heaterRxMessage.cycleEnabled;
    if (!cycle_one_running) {
      amp_pid_1.out = 0;
      valve_pid_1.out = 0;

      temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = valve_pid_1.out,
          .heat_zone_1_pwm = 0,
          .heat_zone_2_pwm = amp_pid_1.out,
          .heat_zone_3_pwm = 0};
      updateDutyCycles(pwmData);

      // TODO: Implement defaults
      // Reinitalize PID Values
      pid_controller_init(&valve_pid_1, config.valve_setpoint_1, config.valve_kp_1, config.valve_ki_1, config.valve_kd_1, config.max_valve_pid_pwm);
      pid_controller_init(&amp_pid_1, config.amp_setpoint_1, config.amp_kp_1, config.amp_ki_1, config.amp_kd_1, config.max_amp_pid_pwm);

      // Send stop heater to sensors task
      pm_amp_heater_running = false;
      pm_valve_heater_running = false;
      starting_run = false;
      handle_cycle_one_stopstart_heater(pm_amp_heater_running, pm_valve_heater_running);
    } else {
      starting_run = true;
      pm_amp_heater_running = config.run_amp_cycle_1;
      pm_valve_heater_running = config.run_valve_cycle_1;
      powerModuleResetHeaterPIDs();
      // Send starting heater to sensors task
      handle_cycle_one_stopstart_heater(pm_amp_heater_running, pm_valve_heater_running);
    }
  } else if (heaterRxMessage.cycleSelect = CYCLE_TWO) {
    cycle_two_running = heaterRxMessage.cycleEnabled;
    if (!cycle_two_running) {
      valve_pid_2.out = 0;
      amp_pid_2.out = 0;

      temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = valve_pid_2.out,
          .heat_zone_1_pwm = 0,
          .heat_zone_2_pwm = amp_pid_2.out,
          .heat_zone_3_pwm = 0};
      updateDutyCycles(pwmData);

      // Reinitalize PID Values
      pid_controller_init(&valve_pid_2, config.valve_setpoint_2, config.valve_kp_2, config.valve_ki_2, config.valve_kd_2, config.max_valve_pid_pwm);
      pid_controller_init(&amp_pid_2, config.amp_setpoint_2, config.amp_kp_2, config.amp_ki_2, config.amp_kd_2, config.max_amp_pid_pwm);

      // Send stop heater to sensors task
      pm_amp_heater_running = false;
      pm_valve_heater_running = false;
      handle_cycle_two_stopstart_heater(pm_amp_heater_running, pm_valve_heater_running);
    } else {
      pm_amp_heater_running = config.run_amp_cycle_2;
      pm_valve_heater_running = config.run_valve_cycle_2;
      powerModuleResetHeaterPIDs();
      handle_cycle_two_stopstart_heater(pm_amp_heater_running, pm_valve_heater_running);
    }
  }
#endif
}

void powerModuleResetHeaterPIDs(void) {
#ifndef SAMPLE_PREP_BOARD
  // Create PID Controllers
  if (use_default_configuration_parameters) {
    pid_controller_init(&valve_pid_1, VALVE_SETPOINT_1, V_KP_1, V_KI_1, V_KD_1, DEFAULT_MAX_VALVE_PID);
    pid_controller_init(&amp_pid_1, AMP_SETPOINT_1, A_KP_1, A_KI_1, A_KD_1, DEFAULT_MAX_AMP_PID);
  } else {
    pid_controller_init(&valve_pid_1, config.valve_setpoint_1, config.valve_kp_1, config.valve_ki_1, config.valve_kd_1, config.max_valve_pid_pwm);
    pid_controller_init(&amp_pid_1, config.amp_setpoint_1, config.amp_kp_1, config.amp_ki_1, config.amp_kd_1, config.max_amp_pid_pwm);
  }
  // Create PID Controllers
  if (use_default_configuration_parameters) {
    pid_controller_init(&valve_pid_2, VALVE_SETPOINT_2, V_KP_2, V_KI_2, V_KD_2, DEFAULT_MAX_VALVE_PID);
    pid_controller_init(&amp_pid_2, AMP_SETPOINT_2, A_KP_2, A_KI_2, A_KD_2, DEFAULT_MAX_AMP_PID);
  } else {
    pid_controller_init(&valve_pid_2, config.valve_setpoint_2, config.valve_kp_2, config.valve_ki_2, config.valve_kd_2, config.max_valve_pid_pwm);
    pid_controller_init(&amp_pid_2, config.amp_setpoint_2, config.amp_kp_2, config.amp_ki_2, config.amp_kd_2, config.max_amp_pid_pwm);
  }
#endif
}

temperature_pwm_data_t getPowerModulePwmData(void) {
  return pm_h_pwm_data;
}

bool getPowerModuleOverTempStatus(void) {
  return (pm_valve_greater_than_max || pm_amp_greater_than_max);
}

temperature_data_t getPowerModuleOverTempData(void) {
  return pm_local_temp_data;
}

bool getPowerModuleHeaterRunningStatus(void) {
  return (pm_amp_heater_running || pm_valve_heater_running);
}