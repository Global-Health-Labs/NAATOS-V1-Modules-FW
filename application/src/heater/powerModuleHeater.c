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

bool pm_greater_than_max = false;
bool pm_heater_run = false;

bool cycle_one_running = false;
bool cycle_two_running = false;
bool starting_run = true;

temperature_data_t pm_local_temp_data;

void handle_cycle_two_stopstart_heater(bool heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  if (cycle_two_running && !heating)
    return;

  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  msg.heaterRunning = heating;

  // Send the heater status
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
  }

  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  wdtUpdate.valid = heating;

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }

  if (!heating) {
    send_debug_log_message("Sent valve heater stop");
  }

  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

  if (heating) {
    pwmMsg.type = PWM_MSG_ENABLE;
  }

  // Respond to heater change
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
  }
#endif
}

void handle_cycle_one_stopstart_heater(bool heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  // Handle case where valve zone is on already, dont want to send stop
  if (cycle_one_running && !heating)
    return;

  if (heating) {
    //nrf_gpio_pin_set(HEATER_PWR_EN);
    //vTaskDelay(pdMS_TO_TICKS(400));
  } else {
    nrf_gpio_pin_clear(HEATER_PWR_EN);
  }

  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  msg.heaterRunning = heating;

  // Send the heater status
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
  }

  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  wdtUpdate.valid = heating;

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }

  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

  if (heating) {
    pwmMsg.type = PWM_MSG_ENABLE;
  }

  // Respond to heater change
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
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
        (temperature_data.heat_zone_0_temp > config.min_run_zone_temp || // TODO: Implement defaults
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
               (temperature_data.heat_zone_0_temp <= config.min_run_zone_temp && // TODO: Implement defaults
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
      pm_greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.heat_zone_0_temp || temperature_data.heat_zone_0_temp < 0) {
      pm_greater_than_max = true;
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
      pm_greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.heat_zone_0_temp || temperature_data.heat_zone_0_temp < 0) {
      pm_greater_than_max = true;
    }
  }

  // Handle being greater than the maximum temperature
  if (pm_greater_than_max) {
    // Send alert message to main task
    xReturned = xQueueSend(main_runErrorQueue, &pm_greater_than_max, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send run error for greater than max temp to main_runErrorQueue.");
    }
    pm_greater_than_max = false;
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
    sprintf(w_buff, "V Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.heat_zone_0_temp, valve_pid_1.out);
    send_debug_log_message(w_buff);
    sprintf(w_buff, "A Zone: Temp: %0.2f\tDuty: %0.2f", temperature_data.heat_zone_2_temp, amp_pid_1.out);
    send_debug_log_message(w_buff);
  }
#endif
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
      pm_heater_run = false;
      starting_run = false;
      handle_cycle_one_stopstart_heater(pm_heater_run);
    } else {
      starting_run = true;
      pm_heater_run = true;
      powerModuleResetHeaterPIDs();
      // Send starting heater to sensors task
      handle_cycle_one_stopstart_heater(pm_heater_run);
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
      pm_heater_run = false;
      handle_cycle_two_stopstart_heater(pm_heater_run);
    } else {
      pm_heater_run = true;
      powerModuleResetHeaterPIDs();
      handle_cycle_two_stopstart_heater(pm_heater_run);
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
  return pm_greater_than_max;
}

temperature_data_t getPowerModuleOverTempData(void) {
  return pm_local_temp_data;
}

bool getPowerModuleHeaterRunningStatus(void) {
  return pm_heater_run;
}