#include "cycle_state_fsm.h"
#include "naatos_messages.h"

cycle_state_t current_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t next_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t last_state;

cycle_state_exit_t exitInfo = CYCLE_COMPLETE;
char exitString[256] = {};

sensor_switches_t switch_data = {.optical_tiggered = false, .hal_triggered = false};
button_update_t buttonData = {.event = NONE};

uint32_t start_time = 0;
uint32_t end_time = 0;
uint32_t time_left = 0;
int percent_recv = 0;

MainStateErrorQueueMsg_t main_err_msg;
volatile bool runThrough = true;
float extraLogData = 0.0;

temperature_data_t over_temp_data;

BaseType_t xReturned;

void reset_cycle_state_machine(void) {
  current_state = VALIDATE_INIT_CONDITIONS;
  next_state = VALIDATE_INIT_CONDITIONS;
  exitInfo = CYCLE_RUNNING;
}

cycle_state_exit_t run_cycle_state_machine(void) {
  runThrough = true;
  while (runThrough) {
    runThrough = false;
    last_state = current_state;
    current_state = next_state;

    switch (current_state) {
    case VALIDATE_INIT_CONDITIONS: {

      xReturned = xQueueSend(logger_logMessageQueue, &new_log_msg, 10);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send start amplification zone event to logging task.\n");
      }

      exitInfo = CYCLE_RUNNING;

      // Request the battery percentage from the bettery task
      xReturned = xQueueSend(batteryRxQueue, &batt_req, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.\n");
      }

      // Check for Battery Data in Battery Queue
      if (xQueueReceive(main_batteryDataQueue, &percent_recv, pdMS_TO_TICKS(1000)) == pdPASS) {
        if ((percent_recv < DEFAULT_LOW_POWER_THRESHOLD && use_default_configuration_parameters) || (!use_default_configuration_parameters && percent_recv < config.low_power_threshold)) {
          updateLedState(LED_DECLINE, true);
          exitInfo = CYCLE_ERROR_POWER_LOW;
          runThrough = true;
          next_state = EXIT_CYCLE;
          break;
        }
      }

      updateLedState(LED_RUN, true);
      next_state = START_CYCLE_1;

      break;
    }

    case START_CYCLE_1: {
      // Send start amplification message to heater queue
      if (!begin_cycle_1()) {
        send_debug_log_message("MAIN_TASK: Unable to begin sample run, temperatures have not yet stabalized.\n");

        // Stop amplification zone
        end_cycle_1();
        next_state = MAIN_STANDBY;
        // Set error during run and wait alert timeout
        updateLedState(LED_DECLINE, true);
        exitInfo = CYCLE_ERROR_START_TEMP_TOO_HIGH;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      updateLedState(LED_RUN, true);
#ifdef SAMPLE_PREP_BOARD
      if (config.ramp_to_temp_before_start_cycle_1 && config.run_heater_1) {
        start_time = xTaskGetTickCount();
        if (use_default_configuration_parameters) {
          end_time = (DEFAULT_RAMP_TO_TEMP_TIMEOUT)*configTICK_RATE_HZ;
        } else {
          end_time = (config.ramp_to_temp_c1_timeout) * configTICK_RATE_HZ;
        }
        next_state = CYCLE_1_RAMP_TO_TEMP;
      } else {
        next_state = CYCLE_1_TIMER;
      }
#else
      next_state = CYCLE_1_TIMER;
#endif
      break;
    }

    case CYCLE_1_RAMP_TO_TEMP: {
      // Ramp to the required temperature for cycle 1
      time_left = xTaskGetTickCount() - start_time;

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
        exitInfo = CYCLE_ERROR_TIMEOUT_DURING_RAMP;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      } else if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
        end_cycle_1();
        next_state = handleMainErrorMessage();
        break;
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

      if (limitSwitchFreed(switch_data)) {
        end_cycle_1();
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
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
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      break;
    }

    case CYCLE_1_TIMER: {
      if (last_state != current_state) {
        start_time = xTaskGetTickCount();
        if (use_default_configuration_parameters) {
          end_time = (DEFAULT_AMPLIFICATION_ZONE_ON_TIME)*configTICK_RATE_HZ;
        } else {
          end_time = (config.cycle_1_run_time_m) * configTICK_RATE_HZ;
        }
      }

      time_left = xTaskGetTickCount() - start_time;

      if (time_left >= end_time) {
        next_state = START_CYCLE_2;
        end_cycle_1();
        break;
      }

      if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
        end_cycle_1();
        next_state = handleMainErrorMessage();
        break;
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

      if (limitSwitchFreed(switch_data)) {
        end_cycle_1();
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
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
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      break;
    }

    case START_CYCLE_2: {
      begin_cycle_2();
#ifdef SAMPLE_PREP_BOARD
      if (config.ramp_to_temp_before_start_cycle_2 && config.run_heater_2) {
        if (!config.ramp_to_temp_before_start_cycle_1 || (config.heater_setpoint_1 != config.heater_setpoint_2)) {
          next_state = CYCLE_2_RAMP_TO_TEMP;
        }
      } else {
        next_state = CYCLE_2_TIMER;
      }
#else
      next_state = CYCLE_2_TIMER;
#endif
      break;
    }

    case CYCLE_2_RAMP_TO_TEMP:
      // Ramp to the required temperature for cycle 1
      time_left = (xTaskGetTickCount() - start_time);

      bool setReachedRx = false;
      xReturned = xQueueReceive(main_setPointReached, &setReachedRx, 0);

      if (setReachedRx) {
        if(!send_event_log_message_struct(&ramp_to_temp_complete_log_msg)) {
          send_debug_log_message("MAIN_TASK: Unable to send ramp_to_temp_complete_log_msg interruption event to logging task.\n");
        }
        next_state = CYCLE_2_TIMER;
        break;
      } else if (time_left >= end_time) {
        end_cycle_2();
        exitInfo = CYCLE_ERROR_TIMEOUT_DURING_RAMP;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      } else if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
        end_cycle_2();
        next_state = handleMainErrorMessage();
        break;
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

      if (limitSwitchFreed(switch_data)) {
        end_cycle_2();
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
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
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      break;

    case CYCLE_2_TIMER: {
      if (last_state != current_state) {
        start_time = xTaskGetTickCount();
        if (use_default_configuration_parameters) {
          end_time = ((DEFAULT_VALVE_ZONE_ON_TIME)*configTICK_RATE_HZ);
        } else {
          end_time = ((config.cycle_2_run_time_m) * configTICK_RATE_HZ);
        }
      }

      time_left = (xTaskGetTickCount() - start_time);

      if (time_left >= end_time) {
        next_state = CYCLE_COMPLETE_DELAY;
        end_cycle_2();
        break;
      }

      if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
        end_cycle_2();
        next_state = handleMainErrorMessage();
        break;
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

      if (limitSwitchFreed(switch_data)) {
        end_cycle_2();
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
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
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }
      break;
    }

    case CYCLE_COMPLETE_DELAY: {
      if (last_state != current_state) {
        start_time = xTaskGetTickCount();
        if (use_default_configuration_parameters) {
          end_time = ((DEFAULT_CYCLES_COMPLETE_DELAY_S)*1000);
        } else {
          end_time = ((config.sample_complete_delay_s) * 1000);
        }
      }

      time_left = (xTaskGetTickCount() - start_time);

      if (time_left >= end_time) {
        next_state = CYCLE_SAMPLE_VALID_HOLD;
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

      if (limitSwitchFreed(switch_data)) {
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      buttonData.event = NONE;
      xQueueReceive(button_mainStateQueue, &buttonData, 0);
      //TODO test also including off_event
      if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_BUTTON_EXIT;
        runThrough = true;
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
          end_time = ((DEFAULT_VALID_TIMEOUT_S)*1000);
        } else {
          end_time = ((config.sample_valid_timeout_s) * 1000);
        }
      }

      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

      if (limitSwitchFreed(switch_data)) {
        exitInfo = CYCLE_COMPLETE;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      buttonData.event = NONE;
      xQueueReceive(button_mainStateQueue, &buttonData, 0);

      time_left = (xTaskGetTickCount() - start_time);

      if (time_left >= end_time) {
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_SAMPLE_INVALIDATED;
        runThrough = true;
        next_state = EXIT_CYCLE;
      }

      break;
    }

    case EXIT_CYCLE:
      runThrough = false;
      // Perform exit operations
      // Cleanup or final actions
      handle_exit_notifications();
      next_state = VALIDATE_INIT_CONDITIONS;
      break;

    default:
      // Handle unexpected states
      next_state = EXIT_CYCLE;
      break;
    }
  }

  return exitInfo;
}

void handle_exit_notifications(void) {
  event_t eventType = SAMPLE_AMP_ENDED;

  switch (exitInfo) {
  case CYCLE_COMPLETE:
    break;

  case CYCLE_ERROR_POWER_LOW:
    sprintf(exitString, "%s%d", RECOVERY_BATT, percent_recv);
    eventType = SAMPLE_RECOVERY_BATT;
    break;

  case CYCLE_ERROR_SENSOR_BREAK:
    eventType = SAMPLE_HAL_CANCEL;
    sprintf(exitString, HALL_SENSOR_BRAKE_MSG);
    break;

  case CYCLE_ERROR_BUTTON_EXIT:
    eventType = SAMPLE_BUTTON_CANCEL;
    sprintf(exitString, "Cycle cancled via button click.");
    break;

  case CYCLE_ERROR_OVER_TEMP:
    sprintf(exitString, "%s: %d", SAMPLE_OVER_TEMP, over_temp_data.amp2_zone_temp);
    eventType = SAMPLE_OVER_TEMP;
    break;

  case CYCLE_ERROR_START_TEMP_TOO_HIGH:
    sprintf(exitString, "%s: %d", TEMPS_NOT_STABLE, over_temp_data.amp2_zone_temp);
    eventType = SAMPLE_TEMPS_NOT_STABALIZED;
    break;

  case CYCLE_ERROR_TIMEOUT_DURING_RAMP:
    eventType = SAMPLE_RAMP_TO_TEMP_TIMEOUT;
    sprintf(exitString, SAMPLE_RAMP_TO_TEMP_TIMEOUT_MSG);
    break;

  case CYCLE_SAMPLE_INVALIDATED:
    eventType = SAMPLE_INVALID_TIMEOUT;
    sprintf(exitString, SAMPLE_VALID_TIMEOUT_MSG);
    break;

  case CYCLE_ERROR_I2C_FAIL:
    eventType = SAMPLE_I2C_READ_ERROR;
    sprintf(exitString, SAMPLE_I2C_READ_ERROR_MSG);
    break;

  case CYCLE_ERROR_UNKNOWN: // drop to default
  default:
    eventType = SAMPLE_UNKNOWN;
    sprintf(exitString, UNKNOWN_ERROR_MESSAGE);
    // Handle unexpected cycle state
    break;
  }

  if (exitInfo != CYCLE_COMPLETE) {
    send_event_log_message(eventType, exitString);
  }
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
    send_debug_log_message("MAIN_TASK: Unable to send run amplification zone request.\n");
  }
  // Send Start Amplification Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &amplification_start_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send start amplification zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &start_run, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
  // Wait for run ok to start response
  xRet = xQueueReceive(main_runRespQueue, &start_run, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }

  return start_run;
}

void begin_cycle_2(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send start valve message to heater queue
  xRet = xQueueSend(heaterRxQueue, &run_valve_zone, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send run valve zone request.\n");
  }
  // Send Start Valve Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &valve_start_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send start valve zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
}

void end_cycle_1(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send stop amplification message to heater queue
  xRet = xQueueSend(heaterRxQueue, &stop_amplification_zone, 50);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop amplification zone request.\n");
  }
  // Send stop amplification Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &amplification_stop_log_msg, 50);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop amplification zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
}

void end_cycle_2(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send valve zone stop request
  xRet = xQueueSend(heaterRxQueue, &stop_valve_zone, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop valve zone request.\n");
  }
  // Send stop valve Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &valve_stop_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop valve zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
}

cycle_state_t handleMainErrorMessage() {
    xReturned = xQueueReceive(main_runErrorQueue, &main_err_msg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("MAIN_TASK: Unable to receive run error from main_runErrorQueue queue.\n");
    }

    if(main_err_msg.errType == ERR_TEMP_SENSOR_READ) {
      exitInfo = CYCLE_ERROR_I2C_FAIL;
    } else if (main_err_msg.errType == ERR_OVERTEMP_EVENT) {
      over_temp_data = main_err_msg.overTempData;
      exitInfo = CYCLE_ERROR_OVER_TEMP;
    }

    runThrough = true;
    return EXIT_CYCLE;
}