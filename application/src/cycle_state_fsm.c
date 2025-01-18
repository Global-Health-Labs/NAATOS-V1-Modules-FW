#include "cycle_state_fsm.h"
#include "naatos_messages.h"

cycle_state_t current_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t next_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t last_state;

cycle_t cycle_now = CYCLE_NONE;
cycle_t cycle_next = CYCLE_NONE;
cycle_t cycle_last = CYCLE_NONE;

cycle_state_exit_t exitInfo = CYCLE_COMPLETE;
char exitString[256] = {};
static char tmp[120];

sensor_switches_t switch_data = {.optical_tiggered = false, .hal_triggered = false};
button_update_t buttonData = {.event = NONE};

uint32_t start_time = 0;
uint32_t end_time = 0;
uint32_t time_left = 0;
fuel_batt_info_t batt_info_recv =  {
  .batt_percent = 0,
  .batt_voltage = 0,
  .batt_temp = 0,
};

MainStateErrorQueueMsg_t main_err_msg;
volatile bool runThrough = true;
float extraLogData = 0.0;

temperature_data_t over_temp_data;

BaseType_t xReturned;

static bool doWeRunHeaterInThisCycle() {
  switch(cycle_now) {
    case CYCLE_ZERO:    return true;
    case CYCLE_ONE:     return use_default_configuration_parameters ? DEFAULT_RUN_HEATER_1 : config.run_heater_1;
    case CYCLE_ONE_B:   return use_default_configuration_parameters ? DEFAULT_RUN_HEATER_2 : config.run_heater_2;
    case CYCLE_TWO:     return false;
    default:            return false;
  }
}

static bool doWeRunMotorInThisCycle() {
  switch(cycle_now) {
    case CYCLE_ZERO:    return false;
    case CYCLE_ONE:     return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_1 : config.run_motor_1;
    case CYCLE_ONE_B:   return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_2 : config.run_motor_2;
    case CYCLE_TWO:     return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_2 : config.run_motor_2;
    default:            return false;
  }
}

bool getCycleRuntime() {
  switch(cycle_now) {
    case CYCLE_ZERO:    return use_default_configuration_parameters ? DEFAULT_CYCLE_0_RUNTIME : config.cycle_0_run_time_s;
    case CYCLE_ONE:     return use_default_configuration_parameters ? DEFAULT_CYCLE_1_RUNTIME : config.cycle_1_run_time_s;
    case CYCLE_ONE_B:   return use_default_configuration_parameters ? DEFAULT_CYCLE_2_RUNTIME : config.cycle_2_run_time_s;
    case CYCLE_TWO:     return use_default_configuration_parameters ? DEFAULT_CYCLE_3_RUNTIME : config.cycle_3_run_time_s;
    default:            return false;
  }
}

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
      exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY;
      //return EXIT_CYCLE;
      return CYCLE_RUNNING_D_DONE;
    }
  }
  return current_state;
}

cycle_state_exit_t run_cycle_state_machine(void) {

  float MY_H_SP; float MY_H_KP; float MY_H_KI; float MY_H_KD;
  float MY_M_SP; float MY_M_KP; float MY_M_KI; float MY_M_KD;
  bool MY_RAMP_TO_TEMP;

  runThrough = true;
  while (runThrough) {
    runThrough = false;
    last_state = current_state;
    current_state = next_state;

    log_event_t logevent;
    log_data_message_t logdatamessage;
    HeaterRxQueueMsg_t heaterrxqueuemsg;
    bool start_run;

    switch(cycle_now) {
      case CYCLE_ZERO:
        MY_H_SP = use_default_configuration_parameters ? DEFAULT_HEATER_SETPOINT_0 : config.heater_setpoint_0;
        MY_H_KP = use_default_configuration_parameters ? H_KP_1 : config.heater_kp_1;
        MY_H_KI = use_default_configuration_parameters ? H_KI_1 : config.heater_ki_1;
        MY_H_KD = use_default_configuration_parameters ? H_KD_1 : config.heater_kd_1;

        MY_M_SP = use_default_configuration_parameters ? MOTOR_SETPOINT_1 : config.motor_setpoint_1;
        MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_1;
        MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_1;
        MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_1;

        MY_RAMP_TO_TEMP = use_default_configuration_parameters ? M_KD : config.ramp_to_temp_before_start_cycle_1;
        break;
      case CYCLE_ONE:
        MY_H_SP = use_default_configuration_parameters ? DEFAULT_HEATER_SETPOINT_1 : config.heater_setpoint_1;
        MY_H_KP = use_default_configuration_parameters ? H_KP_1 : config.heater_kp_1;
        MY_H_KI = use_default_configuration_parameters ? H_KI_1 : config.heater_ki_1;
        MY_H_KD = use_default_configuration_parameters ? H_KD_1 : config.heater_kd_1;

        MY_M_SP = use_default_configuration_parameters ? MOTOR_SETPOINT_1 : config.motor_setpoint_1;
        MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_1;
        MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_1;
        MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_1;

        MY_RAMP_TO_TEMP = false;
        break;
      case CYCLE_ONE_B:
        MY_H_SP = use_default_configuration_parameters ? DEFAULT_HEATER_SETPOINT_1 : config.heater_setpoint_1;
        MY_H_KP = use_default_configuration_parameters ? H_KP_2 : config.heater_kp_2;
        MY_H_KI = use_default_configuration_parameters ? H_KI_2 : config.heater_ki_2;
        MY_H_KD = use_default_configuration_parameters ? H_KD_2 : config.heater_kd_2;

        MY_M_SP = use_default_configuration_parameters ? MOTOR_SETPOINT_1 : config.motor_setpoint_1;
        MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_1;
        MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_1;
        MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_1;

        MY_RAMP_TO_TEMP = MY_RAMP_TO_TEMP = use_default_configuration_parameters ? M_KD : config.ramp_to_temp_before_start_cycle_2;
        break;
      case CYCLE_TWO:
        MY_H_SP = 0;
        MY_H_KP = use_default_configuration_parameters ? H_KP_2 : config.heater_kp_2;
        MY_H_KI = use_default_configuration_parameters ? H_KI_2 : config.heater_ki_2;
        MY_H_KD = use_default_configuration_parameters ? H_KD_2 : config.heater_kd_2;

        MY_M_SP = use_default_configuration_parameters ? MOTOR_SETPOINT_2 : config.motor_setpoint_2;
        MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_2;
        MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_2;
        MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_2;

        MY_RAMP_TO_TEMP = false;
        break;
      default:
        send_debug_log_message("FSM: defineconstants DEFAULT CASE1");
        break;
    }

    switch (current_state) {
    case VALIDATE_INIT_CONDITIONS: {

      xReturned = xQueueSend(logger_logMessageQueue, &new_log_msg, 10);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send start amplification zone event to logging task.\r\n");
      }

      exitInfo = CYCLE_RUNNING;

      vTaskDelay(pdMS_TO_TICKS(150));

      // Request the battery percentage from the bettery task
      xReturned = xQueueSend(batteryRxQueue, &batt_req, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.\r\n");
      }

      // Check for Battery Data in Battery Queue
      if (xQueueReceive(main_batteryDataQueue, &batt_info_recv, pdMS_TO_TICKS(1000)) == pdPASS) {
        if (batt_recovering) {
          updateLedState(LED_DECLINE, true);
          exitInfo = CYCLE_ERROR_POWER_LOW;
          runThrough = true;
          next_state = EXIT_CYCLE;
          break;
        }

        if(batt_info_recv.batt_temp >= 59.0) {
          updateLedState(LED_DECLINE, true);
          exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY;
          runThrough = true;
          next_state = EXIT_CYCLE;
          break;
        }
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

      next_state = CYCLE_RUNNING_A_START;
      cycle_now = (cycle_t) 0;
      cycle_last= (cycle_t) 0;

      break;
    }

    case CYCLE_RUNNING_A_START: {
      if(cycle_now == CYCLE_NONE) {
        sprintf(tmp,"FSM CYCLE_RUNNING_A_START - CYCLE=%d",(uint8_t) cycle_now);
        send_debug_log_message(tmp);
      }

      // Send start zone request
      heaterrxqueuemsg.type = HEATER_MSG_ZONE_STATE;
      heaterrxqueuemsg.cycleSelect = cycle_now;
      heaterrxqueuemsg.cycleEnabled = true;

      heaterrxqueuemsg.tempSetpoint1 = MY_H_SP;
      heaterrxqueuemsg.HEATER_KP = MY_H_KP;
      heaterrxqueuemsg.HEATER_KI = MY_H_KI;
      heaterrxqueuemsg.HEATER_KD = MY_H_KD;

      heaterrxqueuemsg.motorSpeed = MY_M_SP;
      heaterrxqueuemsg.MOTOR_KP = MY_M_KP;
      heaterrxqueuemsg.MOTOR_KI = MY_M_KI;
      heaterrxqueuemsg.MOTOR_KD = MY_M_KD;
      xReturned = xQueueSend(heaterRxQueue, &heaterrxqueuemsg, 0);
      if (xReturned != pdPASS) {
        //send_debug_log_message("MAIN_TASK: Unable to send run amplification zone request.\n");
        send_debug_log_message("FSM CYCLE_RUNNING_A_START: GHL Unable to send heaterRxQueue.\n");
      }


      // Send Start Cycle String message to logging task
      logevent.event = SAMPLE_CYCLE_ONE_STARTED;
      sprintf(logevent.message, "Cycle %d started.",(uint8_t) cycle_now);
      logdatamessage.data_type = EVENT_DATA;
      //logdatamessage.temperature_data = NULL;
      logdatamessage.event_data = logevent;
      xReturned = xQueueSend(logger_logMessageQueue, &logdatamessage, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: GHL Unable to send to logger_logMessageQueue.\n");
      }

      // Wait for run confirmation response
      xReturned = xQueueReceive(main_runConfRespQueue, &start_run, portMAX_DELAY);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_runConfRespQueue queue.\n");
      }
      // Wait for run ok to start response
      xReturned = xQueueReceive(main_runRespQueue, &start_run, portMAX_DELAY);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
      }

      if (!start_run) {
        send_debug_log_message("FSM CYCLE_RUNNING_A_START: Unable to begin sample run (temperatures have not yet stabilized?)\n");

        // Stop cycle one
        //end_cycle_0();
        //next_state = MAIN_STANDBY;

        // Set error during run and wait alert timeout
        //updateLedState(LED_DECLINE, true);
        exitInfo = CYCLE_ERROR_START_TEMP_TOO_HIGH;
        runThrough = true;
        next_state = CYCLE_RUNNING_D_DONE;
        break;
      }
      
      runThrough = true;
      next_state = CYCLE_RUNNING_C_TIMER;
      if(MY_RAMP_TO_TEMP) {
        //next_state = CYCLE_RUNNING_B_RAMP_TO_TEMP
        send_debug_log_message("GHL Ramp To Temp Not Implemented Yet");
      }

      break;
    }

    case CYCLE_RUNNING_B_RAMP_TO_TEMP: {
      // GHL TODO: implement
      next_state = CYCLE_RUNNING_C_TIMER;
      break;
    }

    case CYCLE_RUNNING_C_TIMER: {
      if (last_state != current_state) {
        // First Time In This State

        // Set Timing
        start_time = xTaskGetTickCount();
        //if (use_default_configuration_parameters) {
        //  end_time = (DEFAULT_CYCLE_0_RUNTIME)*configTICK_RATE_HZ;
        //} else {
        //  end_time = (config.cycle_0_run_time_s) * configTICK_RATE_HZ;
        //}
        end_time = (getCycleRuntime()) * configTICK_RATE_HZ;

  #ifdef SAMPLE_PREP_BOARD
        // Set LEDs
        if (doWeRunHeaterInThisCycle()) {
          updateLedState(LED_RUN_HEATER, true);
          updateLedState(LED_RUN_MOTOR, false);
        }
        if (doWeRunMotorInThisCycle()) {
          updateLedState(LED_RUN_MOTOR, true);
          updateLedState(LED_RUN_HEATER, false);
        }
  #else
        updateLedState(LED_RUN_HEATER, true);

        next_state = CYCLE_1_TIMER;
  #endif
      } // end first time run

      // Check Battery Temperature
      // handleBatteryMessage() will set exitInfo, and return CYCLE_RUNNING_D_DONE
      next_state = handleBatteryMessage();
      if(next_state == CYCLE_RUNNING_D_DONE){
        //end_cycle_0();
        //updateLedState(LED_ABORT, true);
        //exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY; // set exit info so CYCLE_RUNNING_D_DONE knows to not continue
        runThrough = true;
        break;
      }

      // Check Normal Runtime Expirationi
      time_left = xTaskGetTickCount() - start_time;
      if (time_left >= end_time) {
        next_state = CYCLE_RUNNING_D_DONE;  // do not set exit info, so cycle will continue to run
        //end_cycle_0();
        break;
      }

      // Check for errors occuring during run
      if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
        //end_cycle_0();
        // handleMainErrorMessage() sets exitInfo, and returns CYCLE_RUNNING_D_DONE next_state
        next_state = handleMainErrorMessage();    //next_state = CYCLE_RUNNING_D_DONE;
        runThrough = true;
        // set exit info
        break;
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

      if (limitSwitchFreed(switch_data)) {
        //end_cycle_0();
        //updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
        //next_state = EXIT_CYCLE;
        next_state = CYCLE_RUNNING_D_DONE;
        break;
      }

      buttonData.event = NONE;
      xQueueReceive(button_mainStateQueue, &buttonData, 0);
      //TODO test also including off_event
      if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
        //end_cycle_0();
        //updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_BUTTON_EXIT;
        runThrough = true;
        //next_state = EXIT_CYCLE;
        next_state = CYCLE_RUNNING_D_DONE;
        break;
      }

      break;
    }

    case CYCLE_RUNNING_D_DONE: {
      bool stop = true;

      sprintf(tmp,"FSM CYCLE_RUNNING_D_DONE - CYCLE=%d",(uint8_t) cycle_now);
      send_debug_log_message(tmp);

      // Send Stop Cycle String message to logging task
      logevent.event = SAMPLE_CYCLE_ONE_ENDED;
      sprintf(logevent.message, "Cycle %d stopped.",(uint8_t) cycle_now);
      logdatamessage.data_type = EVENT_DATA;
      //logdatamessage.temperature_data = NULL;
      logdatamessage.event_data = logevent;
      xReturned = xQueueSend(logger_logMessageQueue, &logdatamessage, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: GHL Unable to send to logger_logMessageQueue.\n");
      }

      if(exitInfo == CYCLE_RUNNING) {
        //---- NORMAL RUNNING
        // we would only be here if this cycle was done

        // Decide If We Are Totally Done, Or If We Have More Cycles
        if( (uint8_t) cycle_now + 1 >= (uint8_t) CYCLE_NONE )  {
          // we were on the last cycle, no more cycles to run
          // normal finish
          next_state = CYCLE_COMPLETE_DELAY;

        } else{
          // there were more cycles to run
          stop = false;
          next_state = CYCLE_RUNNING_A_START;

          // run the next cycle
          cycle_last= cycle_now;
          cycle_now = (cycle_t) ((uint8_t) cycle_now + 1);
        }



      } else{
        //---- EXCEPTION OR ERROR OCCURED WHILE RUNNING OR STARTING
        sprintf(tmp,"FSM CYCLE_RUNNING_D_DONE - Exception Ocurred ExitInfo=%d",(uint8_t) exitInfo);
        send_debug_log_message(tmp);

        updateLedState(LED_ABORT, true);
        next_state = EXIT_CYCLE;
      }

      if(stop)  {
        sprintf(tmp,"FSM CYCLE_RUNNING_D_DONE - Stop Decision");
        send_debug_log_message(tmp);
        // Send stop zone request
        //heaterrxqueuemsg = {
        //    .type = HEATER_MSG_ZONE_STATE,
        //    .cycleSelect = cycle_now,
        //    //.cycleEnabled = true,
        //    .cycleEnabled = false,

        //    //.tempSetpoint1 = MY_H_SP,
        //    //.H_KP = MY_H_KP,
        //    //.H_KI = MY_H_KI,
        //    //.H_KD = MY_H_KD,

        //    //.motorSpeed = MY_M_SP,
        //    //.M_KP = MY_M_KP,
        //    //.M_KI = MY_M_KI,
        //    //.M_KD = MY_M_KD,
        //    };  //GHL
        xReturned = xQueueSend(heaterRxQueue, &heaterrxqueuemsg, 0);
        if (xReturned != pdPASS) {
          //send_debug_log_message("MAIN_TASK: Unable to send run amplification zone request.\n");
          send_debug_log_message("FSM CYCLE_RUNNING_D_Done: GHL Unable to send heaterRxQueue.\n");
        }

        // Wait for run confirmation response
        xReturned = xQueueReceive(main_runConfRespQueue, &stop, portMAX_DELAY);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_runConfRespQueue queue.\n");
        }
        // Wait for run ok to start response
        xReturned = xQueueReceive(main_runRespQueue, &stop, portMAX_DELAY);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
        }
      }
      break;
    }

//#ifdef SAMPLE_PREP_BOARD
//    case CYCLE_2_MOTOR_STOP_WAIT: {
//      if (last_state != current_state) {
//        char w_buff[100];
//        sprintf(w_buff, "Delaying motor turn off for %d seconds", config.motor_end_wait_time_s);
//        send_debug_log_message(w_buff);
//        start_time = xTaskGetTickCount();
//        if (use_default_configuration_parameters) {
//          end_time = ((DEFAULT_MOTOR_WAIT_TIME_S)*1000);
//        } else {
//          end_time = ((config.motor_end_wait_time_s) * 1000);
//        }
//      }

//      next_state = handleBatteryMessage();
//      if(next_state == EXIT_CYCLE){
//        end_cycle_2();
//        updateLedState(LED_ABORT, true);
//        exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY;
//        runThrough = true;
//        break;
//      }

//      time_left = (xTaskGetTickCount() - start_time);

//      if (time_left >= end_time) {
//        // Turn off the motor
//        MotorRxQueueMsg_t motorMsg;
//        motorMsg.type = MOTOR_MSG_HEATER_STATE;
//        motorMsg.motorRunning = false;
        
//        // stop motor PID
//        xReturned = xQueueSend(heaterRxQueue, &stop_cycle_two_zone_motor, 0);
//        if (xReturned != pdPASS) {
//          send_debug_log_message("MAIN_TASK: Unable to send stop valve zone request.\r\n");
//        }

//        // Stop motor enable
//        xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
//        if (xReturned != pdPASS) {
//          send_debug_log_message("HEATER_TASK: Unable to send heater state to motorRxQueue.");
//        }
//        next_state = CYCLE_SAMPLE_VALID_HOLD;
//      }

//      // Get Switch data
//      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
//      if (limitSwitchFreed(switch_data)) {
//        end_cycle_2();
//        updateLedState(LED_ABORT, true);
//        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
//        runThrough = true;
//        next_state = EXIT_CYCLE;
//        break;
//      }
      
//      // Handle Button data in this state
//      buttonData.event = NONE;
//      xQueueReceive(button_mainStateQueue, &buttonData, 0);
//      //TODO test also including off_event
//      if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
//        updateLedState(LED_ABORT, true);
//        exitInfo = CYCLE_ERROR_BUTTON_EXIT;
//        runThrough = true;
//        next_state = EXIT_CYCLE;
//        break;
//      }

//      break;
//    }
//#endif

    case CYCLE_COMPLETE_DELAY: {
      if (last_state != current_state) {
        start_time = xTaskGetTickCount();
        if (use_default_configuration_parameters) {
          end_time = ((DEFAULT_CYCLES_COMPLETE_DELAY_S)*1000);
        } else {
          end_time = ((config.sample_complete_delay_s) * 1000);
        }
      }

      next_state = handleBatteryMessage();
      if(next_state == EXIT_CYCLE){
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY;
        runThrough = true;
        break;
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
      /* Confirm motor is turned off might not be if we exit early due to end cycle not calling this for cycle 2 */
      if (last_state != current_state) {
#ifdef SAMPLE_PREP_BOARD
        // Turn off the motor
        MotorRxQueueMsg_t motorMsg;
        motorMsg.type = MOTOR_MSG_HEATER_STATE;
        motorMsg.motorRunning = false;
      
        // stop motor PID
        xReturned = xQueueSend(heaterRxQueue, &stop_cycle_two_zone_motor, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to send stop valve zone request.\r\n");
        }

        // Stop motor enable
        xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
        if (xReturned != pdPASS) {
          send_debug_log_message("HEATER_TASK: Unable to send heater state to motorRxQueue.");
        }
#endif
        next_state = CYCLE_SAMPLE_VALID_HOLD;
      }

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
  log_event_t exit_event_info = {
      .event = SAMPLE_CYCLE_ONE_ENDED,
      .message = CYCLE_ONE_END_MSG};

  log_data_message_t exit_log_message = {
      .data_type = EVENT_DATA,
      .temperature_data = NULL,
      .event_data = exit_event_info};
  event_t eventType = SAMPLE_CYCLE_ONE_ENDED;

  switch (exitInfo) {
  case CYCLE_COMPLETE:
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
    sprintf(exitString, "%s: %d", SAMPLE_OVER_TEMPERATURE, over_temp_data.heat_zone_1_temp);
    eventType = SAMPLE_OVER_TEMP;
    //exit_event_info.message = exitString;
    //exit_log_message.event_data = exit_event_info;
    break;

  case CYCLE_ERROR_START_TEMP_TOO_HIGH:
    sprintf(exitString, "%s: %d", TEMPS_NOT_STABLE, over_temp_data.heat_zone_1_temp);
    eventType = SAMPLE_TEMPS_NOT_STABALIZED;
    //exit_event_info.message = exitString;
    //exit_log_message.event_data = exit_event_info;
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

// GHL NOTE: copied from original begin_cycle_0
bool begin_cycle_0(void) {
  BaseType_t xRet;
  bool start_run = false;
  // Send start zone request
  xRet = xQueueSend(heaterRxQueue, &run_cycle_zero_zone_heating, 0);
  if (xRet != pdPASS) {
    //send_debug_log_message("MAIN_TASK: Unable to send run amplification zone request.\n");
    send_debug_log_message("MAIN_TASK: GHL Unable to send msg:run_cycle_zero_zone_heating to queue:heaterRxQueue .\n");
  }
  // Send Start Cycle One to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_zero_start_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: GHL Unable to send msg:cycle_zero_start_log_msg to queue:logger_logMessageQueue .\n");
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

// GHL NOTE: copied from begin_cycle_2
void begin_cycle_1(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send Start Cycle Two to heater queue
  xRet = xQueueSend(heaterRxQueue, &run_cycle_one_zone_heating, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send run valve zone request.\r\n");
  }
  // Send Start Cycle Two Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_one_start_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send start valve zone event to logging task.\r\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\r\n");
  }
}

void begin_cycle_1b(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send Start Cycle 1b to heater queue
  xRet = xQueueSend(heaterRxQueue, &run_cycle_one_b_zone_heating, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send run valve zone request.\r\n");
  }
  // Send Start Cycle Two Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_oneB_start_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send start valve zone event to logging task.\r\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\r\n");
  }
}

void begin_cycle_2(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send Start Cycle Two to heater queue
  xRet = xQueueSend(heaterRxQueue, &run_cycle_two_zone_heating, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send run valve zone request.\r\n");
  }
  // Send Start Cycle Two Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_two_start_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send start valve zone event to logging task.\r\n");
  }
  // Wait for run confirmation response
  xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\r\n");
  }
}


// GHL NOTE: copied from original end_cycle_1()
void end_cycle_0(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  //// Send stop cycle one message to heater queue
  //xRet = xQueueSend(heaterRxQueue, &stop_cycle_zero_zone_heating, 50);
  //if (xRet != pdPASS) {
  //  send_debug_log_message("MAIN_TASK: Unable to send stop amplification zone request.\r\n");
  //}
  // Send stop amplification Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_zero_stop_log_msg, 50);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop amplification zone event to logging task.\r\n");
  }
  //// Wait for run confirmation response
  //xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  //if (xRet != pdPASS) {
  //  send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\r\n");
  //}
}

//GHL NOTE: copied from end_cycle_2()
void end_cycle_1(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  //// Send valve zone stop request
  //xRet = xQueueSend(heaterRxQueue, &stop_cycle_one_zone_heating, 0);
  //if (xRet != pdPASS) {
  //  send_debug_log_message("MAIN_TASK: Unable to send stop valve zone request.\r\n");
  //}
  // Send stop valve Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_one_stop_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop valve zone event to logging task.\r\n");
  }
  //// Wait for run confirmation response
  //xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  //if (xRet != pdPASS) {
  //  send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\r\n");
  //}
}

void end_cycle_1b(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  //// Send valve zone stop request
  //xRet = xQueueSend(heaterRxQueue, &stop_cycle_one_b_zone_heating, 0);
  //if (xRet != pdPASS) {
  //  send_debug_log_message("MAIN_TASK: Unable to send stop valve zone request.\r\n");
  //}
  // Send stop valve Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_oneB_stop_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop valve zone event to logging task.\r\n");
  }
  //// Wait for run confirmation response
  //xRet = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  //if (xRet != pdPASS) {
  //  send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\r\n");
  //}
}

void end_cycle_2(void) {
  BaseType_t xRet;
  bool heat_conf = false;
  // Send valve zone stop request
  xRet = xQueueSend(heaterRxQueue, &stop_cycle_two_zone_heating, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop valve zone request.\r\n");
  }
  // Send stop valve Event to logging task
  xRet = xQueueSend(logger_logMessageQueue, &cycle_two_stop_log_msg, 0);
  if (xRet != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send stop valve zone event to logging task.\r\n");
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
    } else if (main_err_msg.errType == ERR_OVERTEMP_EVENT) {
      over_temp_data = main_err_msg.overTempData;
      exitInfo = CYCLE_ERROR_OVER_TEMP;
    } else if (main_err_msg.errType == ERR_MOTOR_STALLED_PERCENT) {
      exitInfo = CYCLE_ERROR_MOTOR_STALLED_PERCENT;
    } else if (main_err_msg.errType == ERR_MOTOR_STALLED_PWM) {
      exitInfo = CYCLE_ERROR_MOTOR_STALLED_PWM;
    } else{
      // default
      exitInfo = CYCLE_ERROR_UNKNOWN;
    }

    runThrough = true;
    return CYCLE_RUNNING_D_DONE;
}