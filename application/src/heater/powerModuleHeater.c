#include "powerModuleHeater.h"
#include "timers.h"

pid_controller_t valve_pid;
pid_controller_t amp_pid;

cycle_config_parameters *p_cycle_config;

bool pm_amp_greater_than_max = false;
bool pm_amp_heater_running = false;
bool pm_valve_greater_than_max = false;
bool pm_valve_heater_running = false;
bool starting_run = true;
bool p_rampToTemp = false;

temperature_data_t pm_local_temp_data;
uint32_t p_samp_log_index = 0;
uint32_t p_samp_log_max = 0;

static log_data_message_t logMsg = {
    .data_type = TEMPERATURE_DATA,
    .event_data = NULL,
    .temperature_data = NULL};

void handle_power_cycle_stopstart_heater(bool amp_heating, bool valve_heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  
  // Set the Amplification Zone Boost Enable
  if (amp_heating) {
    nrf_gpio_pin_set(AMP_PWR_EN);
  } else {
    nrf_gpio_pin_clear(AMP_PWR_EN);
  }

  // Set the Valve Zone Boost Enable
  if (valve_heating) {
    enable_valve_boost();
  } else {
    disable_valve_boost();
  }

  // Check if we want to Ramp To Temperature
  if ( (p_cycle_config->ramp_to_temp_timeout > 0.0) && (amp_heating||valve_heating)) {
    p_rampToTemp = true;
  }
  
  // Send the heater status
  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  msg.heaterRunning = false;
  if (amp_heating || valve_heating) {
    msg.heaterRunning = true;
  }
  #if 0
  // some debug messages
  if(msg.heaterRunning)
    send_debug_log_message("powerModuleHeater: sending sensor.heatRun=TRUE");
  else
    send_debug_log_message("powerModuleHeater: sending sensor.heatRun=FALSE");
  #endif
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
  #if 0
  // some debug messages
  if(wdtUpdate.valid)
    send_debug_log_message("powerModuleHeater: sending wdtUpdate.valid=TRUE");
  else
    send_debug_log_message("powerModuleHeater: sending wdtUpdate.valid=FALSE");
  #endif
  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }
  
  // Respond to heater change
  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_HEATER_DISABLE};
  if (amp_heating || valve_heating) {
    pwmMsg.type = PWM_MSG_HEATER_ENABLE;
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
  temperature_pwm_data_t pwmData ;
  pm_local_temp_data = temperature_data;

  // Check to see if we have Ramped to Temperature if Enabled (All Active zones have hit there setpoint)
  if (p_rampToTemp) {
    bool rampComplete = ((temperature_data.amp_temp >= p_cycle_config->amp_setpoint) && (temperature_data.valve_temp >= p_cycle_config->valve_setpoint) && p_cycle_config->run_amp && p_cycle_config->run_valve) || // Both On
                        ((temperature_data.amp_temp >= p_cycle_config->amp_setpoint) && p_cycle_config->run_amp && !p_cycle_config->run_valve) || // Amplification Only On
                        ((temperature_data.valve_temp >= p_cycle_config->valve_setpoint) && !p_cycle_config->run_amp && p_cycle_config->run_valve); // Valve Only On
    
    #if 0
    if(rampComplete)
      send_debug_log_message("heater: rampComplete=TRUE");
    else
      send_debug_log_message("heater: rampComplete=FALSE");
    #endif

    if (rampComplete) {
      xReturned = xQueueSend(main_setPointReached, &p_rampToTemp, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send set point reached message.");
      }
      p_rampToTemp = false;
    }
  }

  // Update Amplification PID loop with new temperatures
  if (p_cycle_config->run_amp) { 
    pid_controller_compute(&amp_pid, temperature_data.amp_temp);
  } else {
    amp_pid.out = 0.0;
  }
  // Update Valve PID loop with new temperatures
  if (p_cycle_config->run_valve) {
    pid_controller_compute(&valve_pid, temperature_data.valve_temp);
  } else {
    valve_pid.out = 0.0;
  }

  pwmData.valve_pwm = valve_pid.out; // Heat Zone 0 controls Valve
  pwmData.amp_pwm = amp_pid.out;   // Heat Zone 2 controls Amplification
  updateDutyCycles(pwmData);
  
  // Ensure that the temperatures are not greater than the max temperatures allowed
  if (config.amp_max_temp < temperature_data.amp_temp || temperature_data.amp_temp < 0) {
    pm_amp_greater_than_max = true;
  }
  if (config.valve_max_temp < temperature_data.valve_temp || temperature_data.valve_temp < 0) {
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
  sprintf(w_buff, "V Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.valve_temp, valve_pid.out);
  send_debug_log_message(w_buff);
  sprintf(w_buff, "A Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.amp_temp, amp_pid.out);
  send_debug_log_message(w_buff);
#endif

  logMsg.temperature_data.amp_temp = temperature_data.amp_temp;
  logMsg.temperature_data.valve_temp = temperature_data.valve_temp;
  logMsg.temperature_data.amp_pwm = pwmData.amp_pwm;
  logMsg.temperature_data.valve_pwm = pwmData.valve_pwm;

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
  BaseType_t xReturned;
  temperature_pwm_data_t pwmData;

  /* Disable Heaters */
  if (!heaterRxMessage.cycleEnabled) {
    // Set PWMs to Zero
    amp_pid.out = 0;
    valve_pid.out = 0;
    pwmData.amp_pwm = amp_pid.out;
    pwmData.valve_pwm = valve_pid.out;
    updateDutyCycles(pwmData);

    // Send stop heater to sensors task
    pm_amp_heater_running = false;
    pm_valve_heater_running = false;
    handle_power_cycle_stopstart_heater(pm_amp_heater_running, pm_valve_heater_running);
  } 

  /* Enable Heater(s) */
  else {
    // Update the cycle config to the current cycle
    p_cycle_config = &cycle_configs[(uint16_t)(heaterRxMessage.cycleSelect - 1)]; // Index = cycle - 1
    // Set Parameters
    pm_amp_heater_running = p_cycle_config->run_amp;
    pm_valve_heater_running = p_cycle_config->run_valve;
    // Reset PIDs if this is the start of the cycle
    if (heaterRxMessage.cycleSelect == 1) {
      powerModuleResetHeaterPIDs();
    } else {
      // Update the setpoints of the next cycle if it is being run in the cycle
      if (p_cycle_config->run_amp) { 
        pid_controller_update(&amp_pid, p_cycle_config->amp_setpoint, p_cycle_config->amp_kp, p_cycle_config->amp_ki, p_cycle_config->amp_kd); 
      }
      if (p_cycle_config->run_valve ) { 
        pid_controller_update(&valve_pid, p_cycle_config->valve_setpoint, p_cycle_config->valve_kp, p_cycle_config->valve_ki, p_cycle_config->valve_kd); 
      }
    }
    
    // Send starting heater to sensors task
    handle_power_cycle_stopstart_heater(pm_amp_heater_running, pm_valve_heater_running);
    
    // Send can start back to cycle_fsm (which is waiting on us)
    xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send can start run response.");
    }

  }
#endif
}

void powerModuleResetHeaterPIDs(void) {
#ifndef SAMPLE_PREP_BOARD
  // Create PID Controllers
  pid_controller_init(&valve_pid, p_cycle_config->valve_setpoint, p_cycle_config->valve_kp, p_cycle_config->valve_ki, p_cycle_config->valve_kd, config.max_valve_pid_pwm);
  pid_controller_init(&amp_pid, p_cycle_config->amp_setpoint, p_cycle_config->amp_kp, p_cycle_config->amp_ki, p_cycle_config->amp_kd, config.max_amp_pid_pwm);
#endif
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