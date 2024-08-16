#include "cycle_state_fsm.h"
#include "naatos_messages.h"

cycle_state_t current_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t next_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t last_state;

cycle_state_exit_t exitInfo = CYCLE_COMPLETE;

sensor_switches_t switch_data = {.optical_tiggered = false, .hal_triggered = false};
button_update_t buttonData = {.event = NONE};

uint32_t start_time = 0;
uint32_t end_time = 0;
uint32_t time_left = 0;
int percent_recv;
bool over_temp = false;

BaseType_t xReturned;

void reset_cycle_state_machine(void) {
  current_state = VALIDATE_INIT_CONDITIONS;
  next_state = VALIDATE_INIT_CONDITIONS;
  exitInfo = CYCLE_RUNNING;
}

cycle_state_exit_t run_cycle_state_machine(void) {
  last_state = current_state;
  current_state = next_state;

  switch (current_state) {
  case VALIDATE_INIT_CONDITIONS: {

    xReturned = xQueueSend(logger_logMessageQueue, &new_log_msg, 10);
    if (xReturned != pdPASS) {
      printf("MAIN_TASK: Unable to send start amplification zone event to logging task.\n");
    }

    exitInfo = CYCLE_RUNNING;

    // Request the battery percentage from the bettery task
    xReturned = xQueueSend(batteryRxQueue, &batt_req, 0);
    if (xReturned != pdPASS) {
      printf("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.\n");
    }

    // Check for Battery Data in Battery Queue
    if (xQueueReceive(main_batteryDataQueue, &percent_recv, pdMS_TO_TICKS(1000)) == pdPASS) {
      if ((percent_recv < DEFAULT_LOW_POWER_THRESHOLD && use_default_configuration_parameters) || (!use_default_configuration_parameters && percent_recv < config.low_power_threshold)) {
        exitInfo = CYCLE_ERROR_POWER_LOW;
        next_state = EXIT_CYCLE;
      }
    }

    // Perform initialization condition checks
    if (percent_recv < config.recovery_power_thresh) {
      printf("MAIN_TASK: Unable to begin sample run, battery percent is less than the recovery threshold.\n");
      // Tell Log that temperature is not stabalized yet
      xReturned = xQueueSend(logger_logMessageQueue, &recovery_batt_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send recovery battery percentage event to logging task.\n");
      }
      // Set error during run and wait alert timeout
      updateLedState(LED_DECLINE, true);
      exitInfo = CYCLE_ERROR_POWER_LOW;
      next_state = EXIT_CYCLE;
      break;
    }

    updateLedState(LED_RUN, true);
    next_state = START_CYCLE_1;

    break;
  }

  case START_CYCLE_1: {
    // Send start amplification message to heater queue
    if (!begin_cycle_1()) {
      printf("MAIN_TASK: Unable to begin sample run, temperatures have not yet stabalized.\n");
      // Tell Log that temperature is not stabalized yet
      xReturned = xQueueSend(logger_logMessageQueue, &temps_not_stablized_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send sample interruption event to logging task.\n");
      }
      // Stop amplification zone
      end_cycle_1();
      next_state = MAIN_STANDBY;
      // Set error during run and wait alert timeout
      updateLedState(LED_DECLINE, true);
      exitInfo = CYCLE_ERROR_START_TEMP_TOO_HIGH;
      next_state = EXIT_CYCLE;
      break;
    }

    updateLedState(LED_RUN, true);

    if (config.ramp_to_temp_before_start_cycle_1 && config.run_heater_1) {
      start_time = xTaskGetTickCount();
      if (use_default_configuration_parameters) {
        end_time = pdMS_TO_TICKS((DEFAULT_RAMP_TO_TEMP_TIMEOUT)*1000);
      } else {
        end_time = pdMS_TO_TICKS((config.ramp_to_temp_c1_timeout) * 1000);
      }
      next_state = CYCLE_1_RAMP_TO_TEMP;
    } else {
      next_state = CYCLE_1_TIMER;
    }
    break;
  }

  case CYCLE_1_RAMP_TO_TEMP: {
    // Ramp to the required temperature for cycle 1
    time_left = pdTICKS_TO_MS(xTaskGetTickCount() - start_time);

    bool setReachedRx = false;
    xReturned = xQueueReceive(main_setPointReached, &setReachedRx, 0);

    if (setReachedRx) {
      xReturned = xQueueSend(logger_logMessageQueue, &ramp_to_temp_complete_log_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send ramp_to_temp_complete_log_msg interruption event to logging task.\n");
      }
      next_state = CYCLE_1_TIMER;
      break;
    } else if (time_left >= end_time) {
      end_cycle_1();
      xReturned = xQueueSend(logger_logMessageQueue, &ramp_to_temp_timeout_log_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send ramp_to_temp_timeout_log_msg interruption event to logging task.\n");
      }
      exitInfo = CYCLE_ERROR_TIMEOUT_DURING_RAMP;
      next_state = EXIT_CYCLE;
      break;
    } else if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
      end_cycle_1();
      xReturned = xQueueReceive(main_runErrorQueue, &over_temp, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to receive run error from main_runErrorQueue queue.\n");
      }
      xReturned = xQueueSend(logger_logMessageQueue, &over_temp_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send sample over temp event to logging task.\n");
      }
      exitInfo = CYCLE_ERROR_OVER_TEMP;
      next_state = EXIT_CYCLE;
      break;
    }

    // Get Switch data
    xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

    if (limitSwitchFreed(switch_data)) {
      end_cycle_1();
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_SENSOR_BREAK;
      next_state = EXIT_CYCLE;
      break;
    }

    buttonData.event = NONE;
    xQueueReceive(button_mainStateQueue, &buttonData, 0);
    //TODO test also including off_event
    if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
      end_cycle_1();
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_BUTTON_EXIT;
      next_state = EXIT_CYCLE;
      break;
    }

    break;
  }

  case CYCLE_1_TIMER: {
    if (last_state != current_state) {
      start_time = xTaskGetTickCount();
      if (use_default_configuration_parameters) {
        end_time = pdMS_TO_TICKS((DEFAULT_AMPLIFICATION_ZONE_ON_TIME)*1000);
      } else {
        end_time = pdMS_TO_TICKS((config.cycle_1_run_time_m) * 1000);
      }
    }

    time_left = pdTICKS_TO_MS(xTaskGetTickCount() - start_time);

    if (time_left >= end_time) {
      next_state = START_CYCLE_2;
      end_cycle_1();
      break;
    }

    if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
      end_cycle_1();
      xReturned = xQueueReceive(main_runErrorQueue, &over_temp, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to receive run error from main_runErrorQueue queue.\n");
      }
      xReturned = xQueueSend(logger_logMessageQueue, &over_temp_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send sample over temp event to logging task.\n");
      }
      exitInfo = CYCLE_ERROR_OVER_TEMP;
      next_state = EXIT_CYCLE;
      break;
    }

    // Get Switch data
    xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

    if (limitSwitchFreed(switch_data)) {
      end_cycle_1();
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_SENSOR_BREAK;
      next_state = EXIT_CYCLE;
      break;
    }

    buttonData.event = NONE;
    xQueueReceive(button_mainStateQueue, &buttonData, 0);
    //TODO test also including off_event
    if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
      end_cycle_1();
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_BUTTON_EXIT;
      next_state = EXIT_CYCLE;
      break;
    }

    break;
  }

  case START_CYCLE_2: {
    begin_cycle_2();

    if (config.ramp_to_temp_before_start_cycle_2 && config.run_heater_2) {
      if (!config.ramp_to_temp_before_start_cycle_1 || (config.heater_setpoint_1 != config.heater_setpoint_2)) {
        next_state = CYCLE_2_RAMP_TO_TEMP;
      }
    } else {
      next_state = CYCLE_2_TIMER;
    }

    break;
  }

  case CYCLE_2_RAMP_TO_TEMP:
    // Ramp to the required temperature for cycle 1
    time_left = pdTICKS_TO_MS(xTaskGetTickCount() - start_time);

    bool setReachedRx = false;
    xReturned = xQueueReceive(main_setPointReached, &setReachedRx, 0);

    if (setReachedRx) {
      xReturned = xQueueSend(logger_logMessageQueue, &ramp_to_temp_complete_log_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send ramp_to_temp_complete_log_msg interruption event to logging task.\n");
      }
      next_state = CYCLE_2_TIMER;
      break;
    } else if (time_left >= end_time) {
      end_cycle_2();
      xReturned = xQueueSend(logger_logMessageQueue, &ramp_to_temp_timeout_log_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send ramp_to_temp_timeout_log_msg interruption event to logging task.\n");
      }
      exitInfo = CYCLE_ERROR_TIMEOUT_DURING_RAMP;
      next_state = EXIT_CYCLE;
      break;
    } else if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
      end_cycle_2();
      xReturned = xQueueReceive(main_runErrorQueue, &over_temp, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to receive run error from main_runErrorQueue queue.\n");
      }
      xReturned = xQueueSend(logger_logMessageQueue, &over_temp_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send sample over temp event to logging task.\n");
      }
      exitInfo = CYCLE_ERROR_OVER_TEMP;
      next_state = EXIT_CYCLE;
      break;
    }

    // Get Switch data
    xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

    if (limitSwitchFreed(switch_data)) {
      end_cycle_2();
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_SENSOR_BREAK;
      next_state = EXIT_CYCLE;
      break;
    }

    buttonData.event = NONE;
    xQueueReceive(button_mainStateQueue, &buttonData, 0);
    //TODO test also including off_event
    if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
      end_cycle_2();
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_BUTTON_EXIT;
      next_state = EXIT_CYCLE;
      break;
    }

    break;

  case CYCLE_2_TIMER: {
    if (last_state != current_state) {
      start_time = xTaskGetTickCount();
      if (use_default_configuration_parameters) {
        end_time = pdMS_TO_TICKS((DEFAULT_VALVE_ZONE_ON_TIME)*1000);
      } else {
        end_time = pdMS_TO_TICKS((config.cycle_2_run_time_m) * 1000);
      }
    }

    time_left = pdTICKS_TO_MS(xTaskGetTickCount() - start_time);

    if (time_left >= end_time) {
      next_state = CYCLE_COMPLETE_DELAY;
      end_cycle_2();
      break;
    }

    if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
      end_cycle_2();
      xReturned = xQueueReceive(main_runErrorQueue, &over_temp, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to receive run error from main_runErrorQueue queue.\n");
      }
      xReturned = xQueueSend(logger_logMessageQueue, &over_temp_msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send sample over temp event to logging task.\n");
      }
      exitInfo = CYCLE_ERROR_OVER_TEMP;
      next_state = EXIT_CYCLE;
      break;
    }

    // Get Switch data
    xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

    if (limitSwitchFreed(switch_data)) {
      end_cycle_2();
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_SENSOR_BREAK;
      next_state = EXIT_CYCLE;
      break;
    }

    buttonData.event = NONE;
    xQueueReceive(button_mainStateQueue, &buttonData, 0);
    //TODO test also including off_event
    if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
      end_cycle_2();
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_BUTTON_EXIT;
      next_state = EXIT_CYCLE;
      break;
    }
    break;
  }

  case CYCLE_COMPLETE_DELAY: {
    if (last_state != current_state) {
      start_time = xTaskGetTickCount();
      if (use_default_configuration_parameters) {
        end_time = pdMS_TO_TICKS((DEFAULT_CYCLES_COMPLETE_DELAY_S)*1000);
      } else {
        end_time = pdMS_TO_TICKS((config.sample_complete_delay_s) * 1000);
      }
    }

    time_left = pdTICKS_TO_MS(xTaskGetTickCount() - start_time);

    if (time_left >= end_time) {
      next_state = CYCLE_SAMPLE_VALID_HOLD;
    }

    // Get Switch data
    xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

    if (limitSwitchFreed(switch_data)) {
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_SENSOR_BREAK;
      next_state = EXIT_CYCLE;
      break;
    }

    buttonData.event = NONE;
    xQueueReceive(button_mainStateQueue, &buttonData, 0);
    //TODO test also including off_event
    if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_ERROR_BUTTON_EXIT;
      next_state = EXIT_CYCLE;
      break;
    }

    break;
  }
 
  case CYCLE_SAMPLE_VALID_HOLD: {
    if (last_state != current_state) {
      updateLedState(LED_COMPLETE, true);
      start_time = xTaskGetTickCount();
      if (use_default_configuration_parameters) {
        end_time = pdMS_TO_TICKS((DEFAULT_VALID_TIMEOUT_S)*1000);
      } else {
        end_time = pdMS_TO_TICKS((config.sample_valid_timeout_s)*1000);
      }
    }

    xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

    if (limitSwitchFreed(switch_data)) {
      exitInfo = CYCLE_COMPLETE;
      next_state = EXIT_CYCLE;
      break;
    }

    buttonData.event = NONE;
    xQueueReceive(button_mainStateQueue, &buttonData, 0);

    time_left = pdTICKS_TO_MS(xTaskGetTickCount() - start_time);

    if (time_left >= end_time) {
      updateLedState(LED_ABORT, true);
      exitInfo = CYCLE_SAMPLE_INVALIDATED;
      next_state = EXIT_CYCLE;
    }

    break;
  }

  case EXIT_CYCLE:
    // Perform exit operations
    // Cleanup or final actions
    next_state = VALIDATE_INIT_CONDITIONS;
    break;

  default:
    // Handle unexpected states
    next_state = EXIT_CYCLE;
    break;
  }

  return exitInfo;
}

bool limitSwitchFreed(sensor_switches_t data) {
  bool returnValue = false;

#ifdef SAMPLE_PREP_BOARD
  returnValue = !data.hal_triggered;
#else
  returnValue = !data.hal_triggered || !data.optical_tiggered;
#endif
}

bool begin_cycle_1(void) {
  BaseType_t xRet;
  bool start_run = false;
  // Send start zone request

  xRet = xQueueSend(heaterRxQueue, &run_amplification_zone, 0);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to send run amplification zone request.\n");
  }
  // Send Start Amplification Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &amplification_start_log_msg, 0);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to send start amplification zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &start_run, portMAX_DELAY);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
  // Wait for run ok to start response
  xRet = xQueueReceive(main_runRespQueue, &start_run, portMAX_DELAY);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }

  return start_run;
}

void begin_cycle_2(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send start valve message to heater queue
  xRet = xQueueSend(heaterRxQueue, &run_valve_zone, 0);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to send run valve zone request.\n");
  }
  // Send Start Valve Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &valve_start_log_msg, 0);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to send start valve zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
}

void end_cycle_1(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send stop amplification message to heater queue
  xRet = xQueueSend(heaterRxQueue, &stop_amplification_zone, 50);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to send stop amplification zone request.\n");
  }
  // Send stop amplification Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &amplification_stop_log_msg, 50);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to send stop amplification zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
}

void end_cycle_2(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send valve zone stop request
  xRet = xQueueSend(heaterRxQueue, &stop_valve_zone, 0);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to send stop valve zone request.\n");
  }
  // Send stop valve Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &valve_stop_log_msg, 0);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to send stop valve zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
}