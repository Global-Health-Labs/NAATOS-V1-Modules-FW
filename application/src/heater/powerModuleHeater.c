#include "powerModuleHeater.h"
#include "timers.h"

pid_controller_t valve_pid;
pid_controller_t amp0_pid;
pid_controller_t amp1_pid;
pid_controller_t amp2_pid;

pid_controller_t valve_pid_2;
pid_controller_t amp0_pid_2;
pid_controller_t amp1_pid_2;
pid_controller_t amp2_pid_2;

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
    printf("HEATER_TASK: Unable to send heater state to sensorRxQueue.\n");
  }

  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  wdtUpdate.valid = heating;

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
  }

  if (!heating) {
    printf("Sent valve heater stop");
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
#endif
}

void handle_cycle_one_stopstart_heater(bool heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  // Handle case where valve zone is on already, dont want to send stop
  if (cycle_one_running && !heating)
    return;

  if (heating) {
    //nrf_gpio_pin_set(BOOST_CONTROL_ENABLE_PIN);
    //vTaskDelay(pdMS_TO_TICKS(400));
  } else {
    nrf_gpio_pin_clear(BOOST_CONTROL_ENABLE_PIN);
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
        printf("HEATER_TASK: Unable to send cannot start run response.\n");
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
  if (amplification_zone_running) {
    // Update Amplification 0 PID loop with new temperatures
    pid_controller_compute(&amp0_pid, temperature_data.heat_zone_1_temp);
    // Update Amplification 0 PWM with PID output
    pid_controller_compute(&amp1_pid, temperature_data.heat_zone_2_temp);
    // Update Amplification 2 PID loop with new temperatures
    pid_controller_compute(&amp2_pid, temperature_data.heat_zone_3_temp);
    // Update Valve PID loop with new temperatures
    pid_controller_compute(&valve_pid, temperature_data.heat_zone_0_temp);

    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = valve_pid.out,
        .heat_zone_1_pwm = amp0_pid.out,
        .heat_zone_2_pwm = amp1_pid.out,
        .heat_zone_3_pwm = amp2_pid.out};

    updateDutyCycles(pwmData);

    pm_h_pwm_data.heat_zone_0_pwm = valve_pid.out;
    // Set the PWMs for the logger
    pm_h_pwm_data.heat_zone_1_pwm = amp0_pid.out;
    pm_h_pwm_data.heat_zone_2_pwm = amp1_pid.out;
    pm_h_pwm_data.heat_zone_3_pwm = amp2_pid.out;
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if ((config.amp0_max_temp < temperature_data.heat_zone_1_temp) || temperature_data.heat_zone_1_temp < 0 || temperature_data.heat_zone_1_temp > 110) {
      pm_greater_than_max = true;
    }
    if (config.amp1_max_temp < temperature_data.heat_zone_2_temp || temperature_data.heat_zone_2_temp < 0 || temperature_data.heat_zone_2_temp > 110) {
      pm_greater_than_max = true;
    }
    if (config.amp2_max_temp < temperature_data.heat_zone_3_temp || temperature_data.heat_zone_3_temp < 0 || temperature_data.heat_zone_3_temp > 110) {
      pm_greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.heat_zone_0_temp || temperature_data.heat_zone_0_temp < 0 || temperature_data.heat_zone_0_temp > 110) {
      pm_greater_than_max = true;
    }
  }
  if (cycle_two_running) {
    pid_controller_compute(&amp0_pid_2, temperature_data.heat_zone_1_temp);
    pid_controller_compute(&amp1_pid_2, temperature_data.heat_zone_2_temp);
    pid_controller_compute(&amp2_pid_2, temperature_data.heat_zone_3_temp);
    pid_controller_compute(&valve_pid_2, temperature_data.heat_zone_0_temp);

    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = valve_pid_2.out,
        .heat_zone_1_pwm = amp0_pid_2.out,
        .heat_zone_2_pwm = amp1_pid_2.out,
        .heat_zone_3_pwm = amp2_pid_2.out};

    updateDutyCycles(pwmData);

    // Set the PWMs for the logger
    pm_h_pwm_data.heat_zone_0_pwm = valve_pid_2.out;
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if ((config.amp0_max_temp < temperature_data.heat_zone_1_temp) || temperature_data.heat_zone_1_temp < 0 || temperature_data.heat_zone_1_temp > 110) {
      pm_greater_than_max = true;
    }
    if (config.amp1_max_temp < temperature_data.heat_zone_2_temp || temperature_data.heat_zone_2_temp < 0 || temperature_data.heat_zone_2_temp > 110) {
      pm_greater_than_max = true;
    }
    if (config.amp2_max_temp < temperature_data.heat_zone_3_temp || temperature_data.heat_zone_3_temp < 0 || temperature_data.heat_zone_3_temp > 110) {
      pm_greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.heat_zone_0_temp || temperature_data.heat_zone_0_temp < 0 || temperature_data.heat_zone_0_temp > 110) {
      pm_greater_than_max = true;
    }
  }

  // Handle being greater than the maximum temperature
  if (pm_greater_than_max) {
    // Send alert message to main task
    xReturned = xQueueSend(main_runErrorQueue, &pm_greater_than_max, 0);
    if (xReturned != pdPASS) {
      printf("HEATER_TASK: Unable to send run error for greater than max temp to main_runErrorQueue.\n");
    }
    pm_greater_than_max = false;
  }

#if VERBOSE_PID
  if (cycle_one_running) {
    printf("HeatZone0: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.heat_zone_0_temp, valve_pid.out);
    printf("HeatZone1: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.heat_zone_1_temp, amp0_pid.out);
    printf("HeatZone2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.heat_zone_2_temp, amp1_pid.out);
    printf("HeatZone3: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.heat_zone_3_temp, amp2_pid.out);
  }
  if (cycle_two_running) {
    printf("HeatZone0: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.heat_zone_0_temp, valve_pid.out);
    printf("HeatZone1: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.heat_zone_1_temp, amp0_pid.out);
    printf("HeatZone2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.heat_zone_2_temp, amp1_pid.out);
    printf("HeatZone3: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.heat_zone_3_temp, amp2_pid.out);
  }
#endif
#endif
}

void powerModuleHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
#ifndef SAMPLE_PREP_BOARD
  int pid_max = DEFAULT_MAX_HEATER_PID;

  if (!use_default_configuration_parameters) {
    if (config.max_heater_pid_pwm > 0 && config.max_heater_pid_pwm <= DEFAULT_MAX_HEATER_PID) {
      pid_max = config.max_heater_pid_pwm;
    }
  }
  // Set zones enabled
  if (heaterRxMessage.zoneSelect == CYCLE_ONE) {
    cycle_one_running = heaterRxMessage.zoneEnabled;
    if (!cycle_one_running) {
      amp0_pid.out = 0;
      amp1_pid.out = 0;
      amp2_pid.out = 0;
      valve_pid.out = 0;

      temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = valve_pid.out,
          .heat_zone_1_pwm = amp0_pid.out,
          .heat_zone_2_pwm = amp1_pid.out,
          .heat_zone_3_pwm = amp2_pid.out};
      updateDutyCycles(pwmData);

      pid_controller_init(&valve_pid, config.valve_setpoint, config.valve_kp, config.valve_ki, config.valve_kd, pid_max); // TODO: Implement defaults

      // Reinitalize PID Values
      pid_controller_init(&amp0_pid, config.amp0_setpoint, config.amp0_kp, config.amp0_ki, config.amp0_kd, pid_max);
      pid_controller_init(&amp1_pid, config.amp1_setpoint, config.amp1_kp, config.amp1_ki, config.amp1_kd, pid_max);
      pid_controller_init(&amp2_pid, config.amp2_setpoint, config.amp2_kp, config.amp2_ki, config.amp2_kd, pid_max);

      // Send stop heater to sensors task
      pm_heater_run = false;
      starting_run = false;
      handle_amplification_stopstart_heater(pm_heater_run);
    } else {
      starting_run = true;
      pm_heater_run = true;
      powerModuleResetHeaterPIDs();
      // Send starting heater to sensors task
      handle_amplification_stopstart_heater(pm_heater_run);
    }
  } else if (heaterRxMessage.zoneSelect = CYCLE_TWO) {
    cycle_two_running = heaterRxMessage.zoneEnabled;
    if (!cycle_two_running) {
      valve_pid.out = 0;
      amp0_pid.out = 0;
      amp1_pid.out = 0;
      amp2_pid.out = 0;
      temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = valve_pid.out,
          .heat_zone_1_pwm = amp0_pid.out,
          .heat_zone_2_pwm = amp1_pid.out,
          .heat_zone_3_pwm = amp2_pid.out};
      updateDutyCycles(pwmData);
      // Reinitalize PID Values
      pid_controller_init(&valve_pid_2, config.valve_setpoint_2, config.valve_kp_2, config.valve_ki_2, config.valve_kd_2, pid_max);
      pid_controller_init(&amp0_pid_2, config.amp0_setpoint_2, config.amp0_kp_2, config.amp0_ki_2, config.amp0_kd_2, pid_max);
      pid_controller_init(&amp1_pid_2, config.amp1_setpoint_2, config.amp1_kp_2, config.amp1_ki_2, config.amp1_kd_2, pid_max);
      pid_controller_init(&amp2_pid_2, config.amp2_setpoint_2, config.amp2_kp_2, config.amp2_ki_2, config.amp2_kd_2, pid_max);
      // Send stop heater to sensors task
      pm_heater_run = false;
      handle_valve_stopstart_heater(pm_heater_run);
    } else {
      pm_heater_run = true;
      powerModuleResetHeaterPIDs();
      handle_valve_stopstart_heater(pm_heater_run);
    }
  }
#endif
}

void powerModuleResetHeaterPIDs(void) {
#ifndef SAMPLE_PREP_BOARD
  int pid_max = DEFAULT_MAX_HEATER_PID;

  if (!use_default_configuration_parameters) {
    if (config.max_heater_pid_pwm > 0 && config.max_heater_pid_pwm <= DEFAULT_MAX_HEATER_PID) {
      pid_max = config.max_heater_pid_pwm;
    }
  }
  // Create PID Controllers
  if (use_default_configuration_parameters) {
    pid_controller_init(&valve_pid, VALVE_SETPOINT, V_KP, V_KI, V_KD, pid_max);
    pid_controller_init(&amp0_pid, AMP0_SETPOINT, A0_KP, A0_KI, A0_KD, pid_max);
    pid_controller_init(&amp1_pid, AMP1_SETPOINT, A1_KP, A1_KI, A1_KD, pid_max);
    pid_controller_init(&amp2_pid, AMP1_SETPOINT, A2_KP, A2_KI, A2_KD, pid_max);
  } else {
    pid_controller_init(&valve_pid, config.valve_setpoint, config.valve_kp, config.valve_ki, config.valve_kd, pid_max);
    pid_controller_init(&amp0_pid, config.amp0_setpoint, config.amp0_kp, config.amp0_ki, config.amp0_kd, pid_max);
    pid_controller_init(&amp1_pid, config.amp1_setpoint, config.amp1_kp, config.amp1_ki, config.amp1_kd, pid_max);
    pid_controller_init(&amp2_pid, config.amp2_setpoint, config.amp2_kp, config.amp2_ki, config.amp2_kd, pid_max);
  }

  // Create PID Controllers
  if (use_default_configuration_parameters) {
    pid_controller_init(&valve_pid_2, VALVE_SETPOINT_2, V_KP_2, V_KI_2, V_KD_2, pid_max);
    pid_controller_init(&amp0_pid_2, AMP0_SETPOINT_2, A0_KP_2, A0_KI_2, A0_KD_2, pid_max);
    pid_controller_init(&amp1_pid_2, AMP1_SETPOINT_2, A1_KP_2, A1_KI_2, A1_KD_2, pid_max);
    pid_controller_init(&amp2_pid_2, AMP1_SETPOINT_2, A2_KP_2, A2_KI_2, A2_KD_2, pid_max);
  } else {
    pid_controller_init(&valve_pid_2, config.valve_setpoint_2, config.valve_kp_2, config.valve_ki_2, config.valve_kd_2, pid_max);
    pid_controller_init(&amp0_pid_2, config.amp0_setpoint_2, config.amp0_kp_2, config.amp0_ki_2, config.amp0_kd_2, pid_max);
    pid_controller_init(&amp1_pid_2, config.amp1_setpoint_2, config.amp1_kp_2, config.amp1_ki_2, config.amp1_kd_2, pid_max);
    pid_controller_init(&amp2_pid_2, config.amp2_setpoint_2, config.amp2_kp_2, config.amp2_ki_2, config.amp2_kd_2, pid_max);
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