#include "heater.h"
#include "motor.h"
#include "timers.h"

xQueueHandle heaterRxQueue;

bool amplification_zone_running = false;
bool valve_zone_running = false;
bool starting_run = true;
bool h_pwm_req = false;
bool greater_than_max = false;
bool heater_run = false;
int wdtTimeout = 0;

zone_run_req_t zone_req;

temperature_pwm_data_t h_pwm_data = {
    .valve_zone_pwm = 0,
    .amp0_zone_pwm = 0,
    .amp1_zone_pwm = 0,
    .amp2_zone_pwm = 0};

temperature_pwm_data_t outputPwmData = {
    .valve_zone_pwm = 0,
    .amp0_zone_pwm = 0,
    .amp1_zone_pwm = 0,
    .amp2_zone_pwm = 0};

pid_controller_t heater_pid_1;
pid_controller_t heater_pid_2;
pid_controller_t motor_pid_1;
pid_controller_t motor_pid_2;

pid_controller_t valve_pid;
pid_controller_t amp0_pid;
pid_controller_t amp1_pid;
pid_controller_t amp2_pid;

pid_controller_t valve_pid_2;
pid_controller_t amp0_pid_2;
pid_controller_t amp1_pid_2;
pid_controller_t amp2_pid_2;

void handleSensorDataRx(temperature_data_t temperature_data);

void handle_valve_stopstart_heater(bool heating) {
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

  if (heating) {
    nrf_gpio_pin_set(MOTOR_POWER_ENABLE);
  } else {
    nrf_gpio_pin_clear(MOTOR_POWER_ENABLE);
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
}

void handle_amplification_stopstart_heater(bool heating) {
  BaseType_t xReturned;

  // Handle case where valve zone is on already, dont want to send stop
  if (valve_zone_running && !heating)
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

  if (heating) {
    nrf_gpio_pin_set(MOTOR_POWER_ENABLE);
  } else {
    nrf_gpio_pin_clear(MOTOR_POWER_ENABLE);
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
}

void heater_reset_all_pids(void) {
  // Create PID Controllers
  if (use_default_configuration_parameters) {
    pid_controller_init(&heater_pid_1, DEFAULT_HEATER_SETPOINT, H_KP, H_KI, H_KD);
  } else {
    pid_controller_init(&heater_pid_1, config.heater_setpoint_1, config.heater_kp_1, config.heater_ki_1, config.heater_kd_1);
  }

  if (use_default_configuration_parameters) {
    pid_controller_init(&heater_pid_2, DEFAULT_HEATER_SETPOINT, H_KP, H_KI, H_KD);
  } else {
    pid_controller_init(&heater_pid_2, config.heater_setpoint_2, config.heater_kp_2, config.heater_ki_2, config.heater_kd_2);
  }

  if (true /*use_default_configuration_parameters*/) {   // No config parameters yet
    pid_controller_init(&motor_pid_1, MOTOR_SETPOINT, M_KP, M_KI, M_KD);
  }

  if (true /*use_default_configuration_parameters*/) {   // No config parameters yet
    pid_controller_init(&motor_pid_2, MOTOR_SETPOINT, M_KP, M_KI, M_KD);
  }

}

void sendWdtHeaterValid() {
  BaseType_t xReturned;
  watchdog_time_update_t wdtUpdate = {};
  wdtUpdate.taskName = HEATER;
  wdtUpdate.valid = true;

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
  }
}

void heater_task(void *pvParameters) {
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterRxMessage;

  heater_reset_all_pids();

  for (;;) {
    xReturned = xQueueReceive(heaterRxQueue, &heaterRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("Unable to Rx data to heater queue\n");
    } else {
      switch (heaterRxMessage.type) {
      case HEATER_MSG_SLEEP:
        // Handle sleep message
        break;
      case HEATER_MSG_WAKE:
        // Handle wake message
        break;
      case HEATER_MSG_ZONE_STATE: {
        // Set zones enabled
        if (heaterRxMessage.zoneSelect == AMPLIFICATION) {
          amplification_zone_running = heaterRxMessage.zoneEnabled;
          if (!amplification_zone_running) { //AMP2 will be used in sample prep fro heating
            heater_pid_1.out = 0;
            temperature_pwm_data_t pwmData = {
                .valve_zone_pwm = 0,
                .amp0_zone_pwm = heater_pid_1.out,
                .amp1_zone_pwm = 0,
                .amp2_zone_pwm = 0};
            updateDutyCycles(pwmData);

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
            heater_pid_2.out = 0;
            temperature_pwm_data_t pwmData = {
                .valve_zone_pwm = 0,
                .amp0_zone_pwm = heater_pid_2.out,
                .amp1_zone_pwm = 0,
                .amp2_zone_pwm = 0};
            updateDutyCycles(pwmData);

            // Send stop heater to sensors task
            heater_run = false;
            handle_valve_stopstart_heater(heater_run);
          } else {
            heater_run = true;
            heater_reset_all_pids();
            handle_valve_stopstart_heater(heater_run);
          }
        }
        break;
      }
      case HEATER_MSG_TEMPERATURE_DATA:
        handleSensorDataRx(heaterRxMessage.tempData);
        break;
      case HEATER_MSG_MOTOR_DATA:
        handleMotorDataRx(heaterRxMessage.motorSpeed);
        break;
      case HEATER_MSG_USB_SUSPEND:
        // Handle USB suspend message
        break;
      case HEATER_MSG_SENSOR_CONFIRM:
        // Rx confirmation that sensor got heater update
        // Send the confirmation to the main state
        xReturned = xQueueSend(main_runConfRespQueue, &heater_run, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send run response to main_runConfRespQueue.\n");
        }
        break;
      case HEATER_MSG_PWM_REQUEST: {
        SensorRxQueueMsg_t msg;
        msg.type = SENSOR_MSG_PWM_RESPONSE;
        msg.pwmData = h_pwm_data;

        // Send back the pwm data
        xReturned = xQueueSend(sensorRxQueue, &msg, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: unable to send pwm data to sensorRxQueue queue.\n");
        }
        break;
      }
      case HEATER_MSG_CONFIG_UPDATED:
        // Handle config updated message

        break;
      case HEATER_MSG_WDT_UPDATE:
        // Handle watchdog timer update message
        break;
      default:
        // Handle unknown message
        break;
      }
    }
  }
}

void handleMotorDataRx(int motor_speed) {
  if (amplification_zone_running) {
    temperature_pwm_data_t pwmData = {
      .valve_zone_pwm = 0,
      .amp0_zone_pwm = heater_pid_1.out,
      .amp1_zone_pwm = 0,
      .amp2_zone_pwm = 0
    };

    if (config.run_motor_1) {
      pid_controller_compute(&motor_pid_1, motor_speed);
      pwmData.amp1_zone_pwm = motor_pid_1.out;
    } 

    updateDutyCycles(pwmData);
  }
  else if (valve_zone_running) {
    temperature_pwm_data_t pwmData = {
      .valve_zone_pwm = 0,
      .amp0_zone_pwm = heater_pid_1.out,
      .amp1_zone_pwm = 0,
      .amp2_zone_pwm = 0
    };

    if (config.run_motor_2) {
      pid_controller_compute(&motor_pid_2, motor_speed);
      pwmData.amp1_zone_pwm = motor_pid_2.out;
    } 

    updateDutyCycles(pwmData);
  }
}

void handleSensorDataRx(temperature_data_t temperature_data) {
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
#ifdef SAMPLE_PREP_BOARD
    // Update Amplification 2 PID loop with new temperatures
    if (config.run_heater_1) {
      pid_controller_compute(&heater_pid_1, temperature_data.amp2_zone_temp);
    }

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = 0,
        .amp0_zone_pwm = heater_pid_1.out,
        .amp1_zone_pwm = 0,
        .amp2_zone_pwm = 0};

    // Update Amplification 2 PWM with PID output
    updateDutyCycles(pwmData);
    h_pwm_data.valve_zone_pwm = pwmData.valve_zone_pwm;
    h_pwm_data.amp0_zone_pwm = pwmData.amp0_zone_pwm;
    h_pwm_data.amp1_zone_pwm = pwmData.amp1_zone_pwm;
    h_pwm_data.amp2_zone_pwm = pwmData.amp2_zone_pwm;

    if (config.heater_max_temp < temperature_data.amp2_zone_temp) {
      greater_than_max = true;
    }
#else
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
    if (config.amp0_max_temp < temperature_data.amp0_zone_temp) {
      greater_than_max = true;
    }
    if (config.amp1_max_temp < temperature_data.amp1_zone_temp) {
      greater_than_max = true;
    }
    if (config.amp2_max_temp < temperature_data.amp2_zone_temp) {
      greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.valve_zone_temp) {
      greater_than_max = true;
    }
#endif
  }
  if (valve_zone_running) { //AMP 1 will be used for motor
#ifdef SAMPLE_PREP_BOARD
    // Update Amplification 2 PID loop with new temperatures
    if (config.run_heater_2) {
      pid_controller_compute(&heater_pid_2, temperature_data.amp2_zone_temp);
    }

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = 0,
        .amp0_zone_pwm = heater_pid_2.out,
        .amp1_zone_pwm = 0,
        .amp2_zone_pwm = 0};

#else
    pid_controller_compute(&amp0_pid_2, temperature_data.amp0_zone_temp);
    pid_controller_compute(&amp1_pid_2, temperature_data.amp1_zone_temp);
    pid_controller_compute(&amp2_pid_2, temperature_data.amp2_zone_temp);
    pid_controller_compute(&valve_pid_2, temperature_data.valve_zone_temp);

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = valve_pid_2.out,
        .amp0_zone_pwm = amp0_pid_2.out,
        .amp1_zone_pwm = amp1_pid_2.out,
        .amp2_zone_pwm = amp2_pid_2.out};

#endif

    updateDutyCycles(pwmData);

#ifndef SAMPLE_PREP_BOARD
    // Set the PWMs for the logger
    h_pwm_data.valve_zone_pwm = valve_pid.out;
    // Set the PWMs for the logger
    h_pwm_data.amp0_zone_pwm = amp0_pid.out;
    h_pwm_data.amp1_zone_pwm = amp1_pid.out;
    h_pwm_data.amp2_zone_pwm = amp2_pid.out;
#else
    h_pwm_data.valve_zone_pwm = pwmData.valve_zone_pwm;
    h_pwm_data.amp0_zone_pwm = pwmData.amp0_zone_pwm;
    h_pwm_data.amp1_zone_pwm = pwmData.amp1_zone_pwm;
    h_pwm_data.amp2_zone_pwm = pwmData.amp2_zone_pwm;
#endif

#ifdef SAMPLE_PREP_BOARD
    if (config.heater_max_temp < temperature_data.amp2_zone_temp) {
      greater_than_max = true;
    }
#else
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if (config.valve_max_temp < temperature_data.valve_zone_temp) {
      greater_than_max = true;
    }
    if (config.amp1_max_temp < temperature_data.amp1_zone_temp) {
      greater_than_max = true;
    }
    if (config.amp2_max_temp < temperature_data.amp2_zone_temp) {
      greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.valve_zone_temp) {
      greater_than_max = true;
    }
#endif
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
#ifndef SAMPLE_PREP_BOARD
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
#else
  if (amplification_zone_running) {
    int size;
    char buff[60];
    size = sprintf(buff, "Heater: Temp: %0.2f\tDuty: %0.2f\r\n", temperature_data.amp2_zone_temp, heater_pid_1.out);
    write_to_com(buff, size);
    size = sprintf(buff, "Motor: Speed: %0.2f\tDuty: %0.2f\r\n", temperature_data.motorSpeed, h_pwm_data.amp1_zone_pwm);
    write_to_com(buff, size);
  }
  if (valve_zone_running) {
    int size;
    char buff[60];
    size = sprintf(buff, "Heater: Temp: %0.2f\tDuty: %0.2f\r\n", temperature_data.amp2_zone_temp, heater_pid_2.out);
    write_to_com(buff, size);
    size = sprintf(buff, "Motor: Speed: %0.2f\tDuty: %0.2f\r\n", temperature_data.motorSpeed, h_pwm_data.amp1_zone_pwm);
    write_to_com(buff, size);
  }
#endif
#endif
}