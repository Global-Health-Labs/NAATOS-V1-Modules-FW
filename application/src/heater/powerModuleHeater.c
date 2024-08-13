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
    .valve_zone_pwm = 0,
    .amp0_zone_pwm = 0,
    .amp1_zone_pwm = 0,
    .amp2_zone_pwm = 0,
    .sample_prep_heater_pwm = 0};

bool pm_greater_than_max = false;
bool pm_heater_run = false;

void handle_valve_stopstart_heater(bool heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  // Handle case where amplification zone is on already, dont want to send stop
  if (amplification_zone_running && !heating)
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

void handle_amplification_stopstart_heater(bool heating) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  // Handle case where valve zone is on already, dont want to send stop
  if (valve_zone_running && !heating)
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
  if (wdtTimeout++ > (1 / config.sample_rate)) { // send out once a second
    wdtTimeout = 0;
    sendWdtHeaterValid(); // update watchdog
  }

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
  if (amplification_zone_running) {
    // Update Amplification 0 PID loop with new temperatures
    pid_controller_compute(&amp0_pid, temperature_data.amp0_zone_temp);
    // Update Amplification 0 PWM with PID output
    pid_controller_compute(&amp1_pid, temperature_data.amp1_zone_temp);
    // Update Amplification 2 PID loop with new temperatures
    pid_controller_compute(&amp2_pid, temperature_data.amp2_zone_temp);
    // Update Valve PID loop with new temperatures
    pid_controller_compute(&valve_pid, temperature_data.valve_zone_temp);

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = valve_pid.out,
        .amp0_zone_pwm = amp0_pid.out,
        .amp1_zone_pwm = amp1_pid.out,
        .amp2_zone_pwm = amp2_pid.out};

    updateDutyCycles(pwmData);

    h_pwm_data.valve_zone_pwm = valve_pid.out;
    // Set the PWMs for the logger
    h_pwm_data.amp0_zone_pwm = amp0_pid.out;
    h_pwm_data.amp1_zone_pwm = amp1_pid.out;
    h_pwm_data.amp2_zone_pwm = amp2_pid.out;
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if ((config.amp0_max_temp < temperature_data.amp0_zone_temp) || temperature_data.amp0_zone_temp < 0 || temperature_data.amp0_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.amp1_max_temp < temperature_data.amp1_zone_temp || temperature_data.amp1_zone_temp < 0 || temperature_data.amp1_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.amp2_max_temp < temperature_data.amp2_zone_temp || temperature_data.amp2_zone_temp < 0 || temperature_data.amp2_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.valve_zone_temp || temperature_data.valve_zone_temp < 0 || temperature_data.valve_zone_temp > 110) {
      greater_than_max = true;
    }
  }
  if (valve_zone_running) {
    pid_controller_compute(&amp0_pid_2, temperature_data.amp0_zone_temp);
    pid_controller_compute(&amp1_pid_2, temperature_data.amp1_zone_temp);
    pid_controller_compute(&amp2_pid_2, temperature_data.amp2_zone_temp);
    pid_controller_compute(&valve_pid_2, temperature_data.valve_zone_temp);

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = valve_pid_2.out,
        .amp0_zone_pwm = amp0_pid_2.out,
        .amp1_zone_pwm = amp1_pid_2.out,
        .amp2_zone_pwm = amp2_pid_2.out};

    updateDutyCycles(pwmData);

    // Set the PWMs for the logger
    h_pwm_data.valve_zone_pwm = valve_pid_2.out;
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if ((config.amp0_max_temp < temperature_data.amp0_zone_temp) || temperature_data.amp0_zone_temp < 0 || temperature_data.amp0_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.amp1_max_temp < temperature_data.amp1_zone_temp || temperature_data.amp1_zone_temp < 0 || temperature_data.amp1_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.amp2_max_temp < temperature_data.amp2_zone_temp || temperature_data.amp2_zone_temp < 0 || temperature_data.amp2_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.valve_zone_temp || temperature_data.valve_zone_temp < 0 || temperature_data.valve_zone_temp > 110) {
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

#if VERBOSE_PID
  if (amplification_zone_running) {
    printf("Amp0: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp0_zone_temp, amp0_pid.out);
    printf("Amp1: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp1_zone_temp, amp1_pid.out);
    printf("Amp2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp2_zone_temp, amp2_pid.out);
    printf("Valve: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.valve_zone_temp, valve_pid.out);
  }
  if (valve_zone_running) {
    printf("Amp0_2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp0_zone_temp, amp0_pid_2.out);
    printf("Amp1_2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp1_zone_temp, amp1_pid_2.out);
    printf("Amp2_2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp2_zone_temp, amp2_pid_2.out);
    printf("Valve_2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.valve_zone_temp, valve_pid_2.out);
  }
#endif
#endif
}

void powerModuleHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
#ifndef SAMPLE_PREP_BOARD
  // Set zones enabled
  if (heaterRxMessage.zoneSelect == AMPLIFICATION) {
    amplification_zone_running = heaterRxMessage.zoneEnabled;
    if (!amplification_zone_running) {
      amp0_pid.out = 0;
      amp1_pid.out = 0;
      amp2_pid.out = 0;
      valve_pid.out = 0;

      temperature_pwm_data_t pwmData = {
          .valve_zone_pwm = valve_pid.out,
          .amp0_zone_pwm = amp0_pid.out,
          .amp1_zone_pwm = amp1_pid.out,
          .amp2_zone_pwm = amp2_pid.out};
      updateDutyCycles(pwmData);

      pid_controller_init(&valve_pid, config.valve_setpoint, config.valve_kp, config.valve_ki, config.valve_kd); // TODO: Implement defaults

      // Reinitalize PID Values
      pid_controller_init(&amp0_pid, config.amp0_setpoint, config.amp0_kp, config.amp0_ki, config.amp0_kd);
      pid_controller_init(&amp1_pid, config.amp1_setpoint, config.amp1_kp, config.amp1_ki, config.amp1_kd);
      pid_controller_init(&amp2_pid, config.amp2_setpoint, config.amp2_kp, config.amp2_ki, config.amp2_kd);

      // Send stop heater to sensors task
      heater_run = false;
      starting_run = false;
      handle_amplification_stopstart_heater(heater_run);
    } else {
      starting_run = true;
      heater_run = true;
      heater_reset_all_pids();
      // Send starting heater to sensors task
      handle_amplification_stopstart_heater(heater_run);
    }
  } else if (heaterRxMessage.zoneSelect = VALVE) {
    valve_zone_running = heaterRxMessage.zoneEnabled;
    if (!valve_zone_running) {
      valve_pid.out = 0;
      amp0_pid.out = 0;
      amp1_pid.out = 0;
      amp2_pid.out = 0;
      temperature_pwm_data_t pwmData = {
          .valve_zone_pwm = valve_pid.out,
          .amp0_zone_pwm = amp0_pid.out,
          .amp1_zone_pwm = amp1_pid.out,
          .amp2_zone_pwm = amp2_pid.out};
      updateDutyCycles(pwmData);
      // Reinitalize PID Values
      pid_controller_init(&valve_pid_2, config.valve_setpoint_2, config.valve_kp_2, config.valve_ki_2, config.valve_kd_2);
      pid_controller_init(&amp0_pid_2, config.amp0_setpoint_2, config.amp0_kp_2, config.amp0_ki_2, config.amp0_kd_2);
      pid_controller_init(&amp1_pid_2, config.amp1_setpoint_2, config.amp1_kp_2, config.amp1_ki_2, config.amp1_kd_2);
      pid_controller_init(&amp2_pid_2, config.amp2_setpoint_2, config.amp2_kp_2, config.amp2_ki_2, config.amp2_kd_2);
      // Send stop heater to sensors task
      heater_run = false;
      handle_valve_stopstart_heater(heater_run);
    } else {
      heater_run = true;
      heater_reset_all_pids();
      handle_valve_stopstart_heater(heater_run);
    }
  }
#endif
}

void powerModuleResetHeaterPIDs(void) {
#ifndef SAMPLE_PREP_BOARD
  // Create PID Controllers
  if (use_default_configuration_parameters) {
    pid_controller_init(&valve_pid, VALVE_SETPOINT, V_KP, V_KI, V_KD);
    pid_controller_init(&amp0_pid, AMP0_SETPOINT, A0_KP, A0_KI, A0_KD);
    pid_controller_init(&amp1_pid, AMP1_SETPOINT, A1_KP, A1_KI, A1_KD);
    pid_controller_init(&amp2_pid, AMP1_SETPOINT, A2_KP, A2_KI, A2_KD);
  } else {
    pid_controller_init(&valve_pid, config.valve_setpoint, config.valve_kp, config.valve_ki, config.valve_kd);
    pid_controller_init(&amp0_pid, config.amp0_setpoint, config.amp0_kp, config.amp0_ki, config.amp0_kd);
    pid_controller_init(&amp1_pid, config.amp1_setpoint, config.amp1_kp, config.amp1_ki, config.amp1_kd);
    pid_controller_init(&amp2_pid, config.amp2_setpoint, config.amp2_kp, config.amp2_ki, config.amp2_kd);
  }

  // Create PID Controllers
  if (use_default_configuration_parameters) {
    pid_controller_init(&valve_pid_2, VALVE_SETPOINT_2, V_KP_2, V_KI_2, V_KD_2);
    pid_controller_init(&amp0_pid_2, AMP0_SETPOINT_2, A0_KP_2, A0_KI_2, A0_KD_2);
    pid_controller_init(&amp1_pid_2, AMP1_SETPOINT_2, A1_KP_2, A1_KI_2, A1_KD_2);
    pid_controller_init(&amp2_pid_2, AMP1_SETPOINT_2, A2_KP_2, A2_KI_2, A2_KD_2);
  } else {
    pid_controller_init(&valve_pid_2, config.valve_setpoint_2, config.valve_kp_2, config.valve_ki_2, config.valve_kd_2);
    pid_controller_init(&amp0_pid_2, config.amp0_setpoint_2, config.amp0_kp_2, config.amp0_ki_2, config.amp0_kd_2);
    pid_controller_init(&amp1_pid_2, config.amp1_setpoint_2, config.amp1_kp_2, config.amp1_ki_2, config.amp1_kd_2);
    pid_controller_init(&amp2_pid_2, config.amp2_setpoint_2, config.amp2_kp_2, config.amp2_ki_2, config.amp2_kd_2);
  }
#endif
}

temperature_pwm_data_t getPowerModulePwmData(void) {
  return pm_h_pwm_data;
}

bool getPowerModuleOverTempStatus(void) {
  return pm_greater_than_max;
}

bool getPowerModuleHeaterRunningStatus(void) {
  return pm_heater_run;
}