#include "powerModuleHeater.h"
#include "timers.h"

pid_controller_t valve_pid;
pid_controller_t amp_pid;

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

bool starting_run = true;

temperature_data_t pm_local_temp_data;
uint32_t p_samp_log_index = 0;
uint32_t p_samp_log_max = 0;

static log_data_message_t logMsg = {
    .data_type = TEMPERATURE_DATA,
    .event_data = NULL,
    .temperature_data = NULL};

void handle_cycle_stopstart_heater(bool amp_heating, bool valve_heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  if (!amp_heating && !valve_heating)
    return;

  if (amp_heating) {
    nrf_gpio_pin_set(AMP_PWR_EN);
  } else {
    nrf_gpio_pin_clear(AMP_PWR_EN);
  }

  if (valve_heating) {
    enable_valve_boost();
  } else {
    disable_valve_boost();
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
  p_samp_log_max = (config.logging_rate / config.sample_rate);
#endif
}

void powerModuleHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  pm_local_temp_data = temperature_data;

  // Ensure temperatures are below the minimum run zone temperature
  if (cycle_config->min_run_zone_temp_en) {
    if (starting_run &&
        (temperature_data.heat_zone_0_temp > cycle_config->min_run_zone_temp || 
            temperature_data.heat_zone_1_temp > cycle_config->min_run_zone_temp ||
            temperature_data.heat_zone_2_temp > cycle_config->min_run_zone_temp ||
            temperature_data.heat_zone_3_temp > cycle_config->min_run_zone_temp)) {
      starting_run = false;
      // Send cannot start
      xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
      }
      return;
    } else if (starting_run &&
               (temperature_data.heat_zone_0_temp <= cycle_config->min_run_zone_temp && 
                   temperature_data.heat_zone_1_temp <= cycle_config->min_run_zone_temp &&
                   temperature_data.heat_zone_2_temp <= cycle_config->min_run_zone_temp &&
                   temperature_data.heat_zone_3_temp <= cycle_config->min_run_zone_temp)) {
      // Send can start
      xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
      }
      starting_run = false;
    }
  } else if (!cycle_config->min_run_zone_temp_en && starting_run) {
    // Send can start
    xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
    }
    starting_run = false;
  }

  // Update Amplification PID loop with new temperatures
  if (cycle_config->run_amp) { 
    pid_controller_compute(&amp_pid, temperature_data.heat_zone_2_temp);
  } else {
    amp_pid.out = 0.0;
  }
  // Update Valve PID loop with new temperatures
  if (cycle_config->run_valve) {
    pid_controller_compute(&valve_pid, temperature_data.heat_zone_0_temp);
  } else {
    valve_pid.out = 0.0;
  }

  temperature_pwm_data_t pwmData = {
      .heat_zone_0_pwm = valve_pid.out, // Heat Zone 0 controls Valve
      .heat_zone_1_pwm = 0,
      .heat_zone_2_pwm = amp_pid.out,   // Heat Zone 2 controls Amplification
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
  sprintf(w_buff, "V Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.heat_zone_0_temp, valve_pid.out);
  send_debug_log_message(w_buff);
  sprintf(w_buff, "A Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.heat_zone_2_temp, amp_pid.out);
  send_debug_log_message(w_buff);
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
   /* Disable Heater */
   if (!heaterRxMessage.cycleEnabled) {
    // Set PWMs to Zero
    amp_pid.out = 0;
    valve_pid.out = 0;
    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = valve_pid.out,
        .heat_zone_1_pwm = 0,
        .heat_zone_2_pwm = amp_pid.out,
        .heat_zone_3_pwm = 0};
    updateDutyCycles(pwmData);

    // Send stop heater to sensors task
    pm_amp_heater_running = false;
    pm_valve_heater_running = false;
    starting_run = false;
    handle_cycle_stopstart_heater(pm_amp_heater_running, pm_valve_heater_running);
  } 

  /* Enable Heater */
  else {
    // Update the cycle config to the current cycle
    cycle_config = &cycle_configs[(uint16_t)(heaterRxMessage.cycleSelect - 1)]; // Index = cycle - 1
    // Set Parameters
    starting_run = true;
    pm_amp_heater_running = cycle_config->run_amp;
    pm_valve_heater_running = cycle_config->run_valve;
    powerModuleResetHeaterPIDs();
    // Send starting heater to sensors task
    handle_cycle_stopstart_heater(pm_amp_heater_running, pm_valve_heater_running);
  }
#endif
}

void powerModuleResetHeaterPIDs(void) {
#ifndef SAMPLE_PREP_BOARD
  // Create PID Controllers
  pid_controller_init(&valve_pid, cycle_config->valve_setpoint, cycle_config->valve_kp, cycle_config->valve_ki, cycle_config->valve_kd, config.max_valve_pid_pwm);
  pid_controller_init(&amp_pid, cycle_config->amp_setpoint, cycle_config->amp_kp, cycle_config->amp_ki, cycle_config->amp_kd, config.max_amp_pid_pwm);
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