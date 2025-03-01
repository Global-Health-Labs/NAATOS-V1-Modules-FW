#include "cycle_state_fsm.h"
#include "naatos_messages.h"

cycle_state_t current_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t next_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t last_state;

cycle_state_exit_t exitInfo = CYCLE_COMPLETE;
char exitString[256] = {};

sensor_switches_t switch_data = {.optical_tiggered = false, .hal_triggered = false};
sensor_switches_t last_switch_data;
button_update_t buttonData = {.event = NONE};

uint32_t start_time = 0;
uint32_t end_time = 0;
uint32_t time_left = 0;
float time_s_in_cycle_elapsed = 0;
bool setReachedRx = false;

fuel_batt_info_t batt_info_recv =  {
  .batt_percent = 0,
  .batt_voltage = 0,
  .batt_temp = 0,
};

MainStateErrorQueueMsg_t main_err_msg;
volatile bool runThrough = true;
int current_cycle_index = 0;
float extraLogData = 0.0;

temperature_data_t over_temp_data;

BaseType_t xReturned;

void reset_cycle_state_machine(void) {
  current_state = VALIDATE_INIT_CONDITIONS;
  next_state = VALIDATE_INIT_CONDITIONS;
  exitInfo = CYCLE_RUNNING;
}

cycle_state_t handleBatteryMessage() {
  xReturned = xQueueSend(batteryRxQueue, &batt_req, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.\r\n");
  }

  if (xQueueReceive(main_batteryDataQueue, &batt_info_recv, pdMS_TO_TICKS(0)) == pdPASS) {
    if(batt_info_recv.batt_temp >= 59.0) {
      return EXIT_CYCLE;
    }
  }
  return current_state;
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
        send_debug_log_message("MAIN_TASK: Unable to send start amplification zone event to logging task.\r\n");
      }

      exitInfo = CYCLE_RUNNING;
      time_s_in_cycle_elapsed = 0;

      vTaskDelay(pdMS_TO_TICKS(150));

      // Request the battery percentage from the bettery task
      xReturned = xQueueSend(batteryRxQueue, &batt_req, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.\r\n");
      }

      // Check for Battery Data in Battery Queue
      if (xQueueReceive(main_batteryDataQueue, &batt_info_recv, pdMS_TO_TICKS(1000)) == pdPASS) {
        if (batt_recovering) {
          //updateLedState(LED_ABORT_YELLOW, true);
          exitInfo = CYCLE_ERROR_POWER_LOW;
          runThrough = true;
          next_state = RUN_ERROR_OR_FINISHED;
          break;
        }

        if(batt_info_recv.batt_temp >= 59.0) {
          //updateLedState(LED_ABORT_YELLOW, true);
          exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY;
          runThrough = true;
          next_state = RUN_ERROR_OR_FINISHED;
          break;
        }
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
      if (limitSwitchFreed(switch_data)) {
        //end_cycle((cycle_t)(current_cycle_index + 1), true);
        //updateLedState(LED_ABORT_YELLOW, true);
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
        next_state = RUN_ERROR_OR_FINISHED;
        break;
      }

      // Check Temperatures Are Low-enough If Applicable
      // (was previously set in main_standby())
      if(!conditions_for_run.bit.temperature_zones_in_range)  {
        exitInfo = CYCLE_ERROR_START_TEMP_TOO_HIGH;
        runThrough = true;
        next_state = RUN_ERROR_OR_FINISHED;
        break;
      }

      // Check Other Conditions We Didn't Handle
      // (was previously set in main_standby())
      if(!conditions_can_we_start_a_run())  {
        exitInfo = CYCLE_ERROR_OTHERFLAGS;
        runThrough = true;
        next_state = RUN_ERROR_OR_FINISHED;
        break;
      }

    
      // Reset ERROR-QUEUE
      // ensure we start with this runErrorQueue as empty
      if(uxQueueMessagesWaiting(main_runErrorQueue) > 0)  {
        sprintf(exitString,"CYCLE_FSM: Startup state, runErrorQueue has %d waiting messages. Consume them now.",uxQueueMessagesWaiting(main_runErrorQueue));
        send_debug_log_message(exitString);
        while(xQueueReceive(main_runErrorQueue, &main_err_msg, 0) == pdPASS) {
          // consume all remaining items
          //send_debug_log_message("CYCLE_FSM: consumed a message from main_runErrorQueue and discarded");
        }
      }

      // Reset SETPOint reacheD QUEUE
      if(uxQueueMessagesWaiting(main_setPointReached) > 0)  {
        sprintf(exitString,"CYCLE_FSM: Startup state, main_setPointReached has %d waiting messages. Consume them now.",uxQueueMessagesWaiting(main_setPointReached));
        send_debug_log_message(exitString);
        while(xQueueReceive(main_setPointReached, &main_err_msg, 0) == pdPASS) {
          // consume all remaining items
          //send_debug_log_message("CYCLE_FSM: consumed a message from main_runErrorQueue and discarded");
        }
      }


      next_state = START_CYCLE;
      break;
    }

    case START_CYCLE: {
      // Send start cycle 1 message to heater queue
      if (!begin_cycle((cycle_t)(current_cycle_index + 1))) {
        send_debug_log_message("MAIN_TASK: Unable to begin sample run, temperatures have not yet stabalized.\r\n");

        // TODO: We'll probably rework this. Cycle refused to start (NOTE: This seems to be legacy, checking if temperature too high, etc) Move out i think

        exitInfo = CYCLE_ERROR_START_TEMP_TOO_HIGH;
        runThrough = true;
        next_state = RUN_ERROR_OR_FINISHED;
        break;
      }

      updateLedState(LED_RUN_HEATER, true);

      next_state = CYCLE_IS_RUNNING;

      setReachedRx = false;
      time_s_in_cycle_elapsed = 0;

      break;
    }

    case CYCLE_IS_RUNNING: {
      // Initial entry into this state
      if (last_state != current_state) {
        start_time = xTaskGetTickCount();

      }

      // Time left on the timer (float in seconds)
      time_s_in_cycle_elapsed = (xTaskGetTickCount() - start_time)/configTICK_RATE_HZ;
      //time_left = xTaskGetTickCount() - start_time;
      
      // Get Battery Data
      next_state = handleBatteryMessage();
      if(next_state == RUN_ERROR_OR_FINISHED){
        exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY;
        runThrough = true;
        break;
      }

      // Get Switch Data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
      if (limitSwitchFreed(switch_data)) {
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      // Get Button Data
      buttonData.event = NONE;
      xQueueReceive(button_mainStateQueue, &buttonData, 0);
      if (buttonData.event == ON_EVENT ) {
        exitInfo = CYCLE_ERROR_BUTTON_EXIT;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      // Are there any other errors from within the run ?
      if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
        next_state = handleMainErrorMessage();
        // should result in EXIT_CYCLE
        break;
      }

      // Check if we have met temperature requirement for this cycle (exceeded the ramp to temp number)
      if( cycle_configs[current_cycle_index].ramp_to_temp_timeout > 0 ) {
        if(!setReachedRx) {
          xReturned = xQueueReceive(main_setPointReached, &setReachedRx, 0);
          if (xReturned != pdPASS) {
            setReachedRx = false;
          }
        }
        
        #if 0
        if(setReachedRx)
          send_debug_log_message("cycle_fsm: setReachedRx=TRUE");
        else
          send_debug_log_message("cycle_fsm: setReachedRx=FALSE");
        #endif

        // Did we not reach the temperature in the specified amount of time?? Error-out if so...
        if(!setReachedRx && (time_s_in_cycle_elapsed > cycle_configs[current_cycle_index].ramp_to_temp_timeout))  {
          exitInfo = CYCLE_ERROR_TIMEOUT_DURING_RAMP;
          runThrough = true;
          next_state = EXIT_CYCLE;
          break;
        }
      }

      // Do we want to have ramped to temp before the specified cycle-time starts?
      if( cycle_configs[current_cycle_index].ramp_to_temp_before_start_cycle && (cycle_configs[current_cycle_index].ramp_to_temp_timeout > 0) )  {

          if (setReachedRx) {
            // we have just reached (exceeded) the setpoint
            xReturned = xQueueSend(logger_logMessageQueue, &ramp_to_temp_complete_log_msg, 0);
            if (xReturned != pdPASS) {
              send_debug_log_message("MAIN_TASK: Unable to send ramp_to_temp_complete_log_msg interruption event to logging task.");
            }
          }

          // change the start-time to right-now, so cycle timer will measure from this point
          start_time = xTaskGetTickCount();

      }

      // Check if cycle has completed normally
      if ( time_s_in_cycle_elapsed >= cycle_configs[current_cycle_index].cycle_run_time_s ) {
        next_state = CYCLE_COMPLETE_DELAY;
        //end_cycle((cycle_t)(current_cycle_index + 1), false);
        break;
      }

      break;
    }

    case CYCLE_COMPLETE_DELAY: {
      if (last_state != current_state) {
        start_time = xTaskGetTickCount();
        end_time = ((cycle_configs[current_cycle_index].cycle_delay_time) * 1000);
      }

      next_state = handleBatteryMessage();
      if(next_state == RUN_ERROR_OR_FINISHED){
        exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      time_left = (xTaskGetTickCount() - start_time);

      if (time_left >= end_time) {
        next_state = EXIT_CYCLE;
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
      if (limitSwitchFreed(switch_data)) {
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }
      // Get Button Data
      buttonData.event = NONE;
      xQueueReceive(button_mainStateQueue, &buttonData, 0);
      if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
        exitInfo = CYCLE_ERROR_BUTTON_EXIT;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      break;
    }

    case EXIT_CYCLE: {
      // First-entry
      if (last_state != current_state) {

        // CHECK IF AN ERROR OCCURRED
        if(exitInfo!=CYCLE_RUNNING) {
          
          // CYCLE IS ENDING BECAUSE AN ERROR OCCURRED
          end_cycle((cycle_t)(current_cycle_index + 1), true);
          runThrough = true;
                    
          next_state = RUN_ERROR_OR_FINISHED;

        } else{
          // CYCLE IS ENDING NORMALLY
          end_cycle((cycle_t)(current_cycle_index + 1), false);

          // Check to see if there are more cycles to be completed 
          if ((current_cycle_index + 1) == total_cycles){
            
            // we are really all finished, no more cycles
            next_state = CYCLE_SAMPLE_VALID_HOLD;
          } else {
            current_cycle_index++;  // Increase the current cycle index to get the next cycle information
            next_state = START_CYCLE;
          }
        }
        

      }
      break;
    }

    case CYCLE_SAMPLE_VALID_HOLD: {
      if (last_state != current_state) {
        updateLedState(LED_COMPLETE, true);
        start_time = xTaskGetTickCount();
        end_time = ((config.sample_valid_timeout_s) * 1000);

      #ifdef POWER_MODULE_BOARD
        // PLAY SOUND
        PwmRxQueueMsg_t pwmmsg = {
            .type = PWM_MSG_BUZZER_SONG_mariointro,
            .buzzer_pwm_duty = config.sound_volume_complete
        };
        BaseType_t xReturned;
        xReturned = xQueueSend(pwmRxQueue, &pwmmsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("CYCLEFSM: Unable to send buzzer message to pwmRxQueue.");
        }
      #endif
        send_event_log_message(SAMPLE_UNKNOWN, "Exiting with DoubleGreenHold");
      }
     
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
      if (limitSwitchFreed(switch_data)) {
        exitInfo = CYCLE_COMPLETE;
        runThrough = true;
        next_state = RUN_ERROR_OR_FINISHED;
        break;
      }

      buttonData.event = NONE;
      xQueueReceive(button_mainStateQueue, &buttonData, 0);

      time_left = (xTaskGetTickCount() - start_time);
      if (time_left >= end_time) {
        exitInfo = CYCLE_SAMPLE_INVALIDATED;
        runThrough = true;
        next_state = RUN_ERROR_OR_FINISHED;
      }

      break;
    }

    case RUN_ERROR_OR_FINISHED: {
      runThrough = false;

      // Update LED's
      if(exitInfo != CYCLE_COMPLETE)  {
        // exited due to an error
        if(
          (time_s_in_cycle_elapsed < cycle_configs[current_cycle_index].double_yellow_grace_period_s)
            && (cycle_configs[current_cycle_index].double_yellow_grace_period_s>0.0)
        ) {
          // within graceperiod, flash YELLOW (sample okay)
          updateLedState(LED_ABORT_YELLOW, true);
          updateLedState(LED_INVALID, false);

        #ifdef POWER_MODULE_BOARD
          // PLAY SOUND (graceperiod sound)
          PwmRxQueueMsg_t pwmmsg = {
              .type = PWM_MSG_BUZZER_TONE_500ms_1,
              .buzzer_pwm_duty = config.sound_volume_graceperiod
          };
          BaseType_t xReturned;
          xReturned = xQueueSend(pwmRxQueue, &pwmmsg, 0);
          if (xReturned != pdPASS) {
            send_debug_log_message("CYCLEFSM: Unable to send buzzer message to pwmRxQueue.");
          }
        #endif
        send_event_log_message(SAMPLE_UNKNOWN, "Exiting with DoubleYellow");

        } else  {
          // outside graceperiod, flash RED (sample invalidated)
          updateLedState(LED_ABORT, true);
          updateLedState(LED_INVALID, true);

        #ifdef POWER_MODULE_BOARD
          // PLAY SOUND (error sound)
          PwmRxQueueMsg_t pwmmsg = {
              .type = PWM_MSG_BUZZER_TONE_1SEC_2,
              .buzzer_pwm_duty = config.sound_volume_abort
          };
          BaseType_t xReturned;
          xReturned = xQueueSend(pwmRxQueue, &pwmmsg, 0);
          if (xReturned != pdPASS) {
            send_debug_log_message("CYCLEFSM: Unable to send buzzer message to pwmRxQueue.");
          }
        #endif
        send_event_log_message(SAMPLE_UNKNOWN, "Exiting with DoubleRed and SampleInvalidated");
        }
      } else  {
        // ENDED DUE TO CYCLE_COMPLETE
        updateLedState(LED_INVALID, false);
      }

      // Perform exit operations
      // Cleanup or final actions
      current_cycle_index = 0;
      handle_exit_notifications();
      next_state = VALIDATE_INIT_CONDITIONS;
      break;
    }

    default:
      // Handle unexpected states
      next_state = RUN_ERROR_OR_FINISHED;
      break;
    }
  }

  return exitInfo;
}

void handle_exit_notifications(void) {
  event_t eventType;

  switch (exitInfo) {
  case CYCLE_COMPLETE:
    updateLedState(LED_INVALID, false);
    break;
  case CYCLE_ERROR_POWER_LOW:
    sprintf(exitString, "%s%d", RECOVERY_BATT, batt_info_recv.batt_percent);
    eventType = SAMPLE_RECOVERY_BATT;
    break;
  case CYCLE_ERROR_SENSOR_BREAK:
    eventType = SAMPLE_HAL_CANCEL;
#ifdef SAMPLE_PREP_BOARD
      sprintf(exitString, HALL_SENSOR_BRAKE_MSG);
#else
      sprintf(exitString, OPTICAL_SENSOR_BRAKE_MSG);
#endif
    break;
  case CYCLE_ERROR_BUTTON_EXIT:
    eventType = SAMPLE_BUTTON_CANCEL;
    sprintf(exitString, "Cycle cancled via button click.");
    break;
  case CYCLE_ERROR_OVER_TEMP_BATTERY:
    eventType = SAMPLE_BATTERY_OVERTEMP;
    sprintf(exitString, "%s%f", SAMPLE_BATTERY_OVER_TEMP, batt_info_recv.batt_temp);
    break;
  case CYCLE_ERROR_OVER_TEMP:
    sprintf(exitString, "%s", SAMPLE_OVER_TEMPERATURE);
    eventType = SAMPLE_OVER_TEMP;
    break;
  case CYCLE_ERROR_START_TEMP_TOO_HIGH:
    sprintf(exitString, "%s", TEMPS_NOT_STABLE);
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
  case CYCLE_ERROR_MOTOR_STALLED_PERCENT:
    eventType = SAMPLE_MOTOR_STALLED_PERCENT;
    sprintf(exitString, SAMPLE_MOTOR_STALLED_PERCENT_ERROR_MSG);
    break;
  case CYCLE_ERROR_MOTOR_STALLED_PWM:
    eventType = SAMPLE_MOTOR_STALLED_PWM;
    sprintf(exitString, SAMPLE_MOTOR_STALLED_PWM_ERROR_MSG);
    break;
  case CYCLE_ERROR_OTHERFLAGS:
    eventType = SAMPLE_UNKNOWN;
    sprintf(exitString, "Cannot run due to unhandled flags. runflags=0x%x machine=0x%x",conditions_for_run.reg,conditions_for_machine.reg);
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
  returnValue = !data.optical_tiggered;
#endif
}

bool begin_cycle(cycle_t cycle) {
  BaseType_t xRet;
  bool start_run = false;

  // RESET I2C ERROR COUNTER
  GLOBAL_I2C_RECOVERY_COUNTER = 0;

  // Create Cycle Zones Request For New Cycle
  HeaterRxQueueMsg_t run_cycle_zones = {
    .type = HEATER_MSG_ZONE_STATE,
    .cycleSelect = cycle,
    .cycleEnabled = true};
  // Send start zone request
  xRet = xQueueSend(heaterRxQueue, &run_cycle_zones, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send start heater zone request.\n");
  }

  // Create New Cycle Started Logging Task
  log_event_t cycle_start_event = {
    .event = SAMPLE_CYCLE_STARTED,
    .message = NULL};
  sprintf(cycle_start_event.message, "Cycle %d Started.", (uint16_t)cycle);
  log_data_message_t cycle_start_log_msg = {
    .data_type = EVENT_DATA,
    .temperature_data = NULL,
    .event_data = cycle_start_event};
  // Send Start Cycle to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_start_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send start heater zone event to logging task.\n");
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

void end_cycle(cycle_t cycle, bool end_from_error) {
  BaseType_t xRet;
  bool heat_conf = false;

  // Create Cycle Heating Request
  HeaterRxQueueMsg_t stop_cycle_zone_heating = {
    .type = HEATER_MSG_ZONE_STATE,
    .cycleSelect = cycle,
    .cycleEnabled = false,
    .fromError = end_from_error};
  // Send Stop Cycle to logging task
  xRet = xQueueSend(heaterRxQueue, &stop_cycle_zone_heating, 50);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop heater zone request.\r\n");
  }
  
  // Create Cycle Stopped Logging Task
  log_event_t cycle_stop_event = {
    .event = SAMPLE_CYCLE_ENDED,
    .message = NULL};
  if(!end_from_error)
    sprintf(cycle_stop_event.message, "Cycle %d Stopped. I2CERRCOUNT=%u", (uint16_t)cycle, GLOBAL_I2C_RECOVERY_COUNTER);
  else
    sprintf(cycle_stop_event.message, "Cycle %d Stopped early. I2CERRCOUNT=%u", (uint16_t)cycle, GLOBAL_I2C_RECOVERY_COUNTER);
  log_data_message_t cycle_stop_log_msg = {
    .data_type = EVENT_DATA,
    .temperature_data = NULL,
    .event_data = cycle_stop_event};

  // Send Stop Cycle to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_stop_log_msg, 50);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop heater zone event to logging task.\r\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\r\n");
  }
}

cycle_state_t handleMainErrorMessage() {
    xReturned = xQueueReceive(main_runErrorQueue, &main_err_msg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("MAIN_TASK: Unable to receive run error from main_runErrorQueue queue.\r\n");
    }

    if(main_err_msg.errType == ERR_TEMP_SENSOR_READ) {
      exitInfo = CYCLE_ERROR_I2C_FAIL;
    } 
    else if (main_err_msg.errType == ERR_OVERTEMP_EVENT) {
      over_temp_data = main_err_msg.overTempData;
      exitInfo = CYCLE_ERROR_OVER_TEMP;
    } 
    else if (main_err_msg.errType == ERR_MOTOR_STALLED_PERCENT) {
      exitInfo = CYCLE_ERROR_MOTOR_STALLED_PERCENT;
    } 
    else if (main_err_msg.errType == ERR_MOTOR_STALLED_PWM) {
      exitInfo = CYCLE_ERROR_MOTOR_STALLED_PWM;
    }

    runThrough = true;
    return EXIT_CYCLE;
}