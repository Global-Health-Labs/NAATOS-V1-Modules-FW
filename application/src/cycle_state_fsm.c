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
    case CYCLE_ZERO:    return use_default_configuration_parameters ? DEFAULT_RUN_HEATER_1 : config.run_heater_1;
    case CYCLE_ONE:     return use_default_configuration_parameters ? DEFAULT_RUN_HEATER_1 : config.run_heater_1;
    case CYCLE_TWO:     return use_default_configuration_parameters ? DEFAULT_RUN_HEATER_2 : config.run_heater_2;
    case CYCLE_THREE:   return false;
    default:            return false;
  }
}

static bool doWeRunMotorInThisCycle() {
  switch(cycle_now) {
    case CYCLE_ZERO:    false;
    case CYCLE_ONE:     return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_1 : config.run_motor_1;
    case CYCLE_TWO:     return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_2 : config.run_motor_2;
    case CYCLE_THREE:   return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_2 : config.run_motor_2;
    default:            return false;
  }
}

float getCycleRuntime() {
  float runtime;
  switch(cycle_now) {
    case CYCLE_ZERO:    runtime = use_default_configuration_parameters ? DEFAULT_CYCLE_0_RUNTIME : config.cycle_0_run_time_s; break;
    case CYCLE_ONE:     runtime = use_default_configuration_parameters ? DEFAULT_CYCLE_1_RUNTIME : config.cycle_1_run_time_s; break;
    case CYCLE_TWO:     runtime = use_default_configuration_parameters ? DEFAULT_CYCLE_2_RUNTIME : config.cycle_2_run_time_s; break;
    case CYCLE_THREE:   runtime = use_default_configuration_parameters ? DEFAULT_CYCLE_3_RUNTIME : config.cycle_3_run_time_s; break;
    default:            runtime = 6.0;  break;
  }
  return runtime;
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
      return CYCLE_RUNNING_C_DONE_DECIDE_STOP;
    }
  }
  return current_state;
}

cycle_state_exit_t run_cycle_state_machine(void) {

  float MY_H_SP; float MY_H_KP; float MY_H_KI; float MY_H_KD;
  uint16_t MY_M_SP; float MY_M_KP; float MY_M_KI; float MY_M_KD;
  bool MY_RAMP_TO_TEMP;

  log_event_t logevent;
  log_data_message_t logdatamessage;
  HeaterRxQueueMsg_t heaterrxqueuemsg;

  runThrough = true;
  while (runThrough) {
    runThrough = false;
    last_state = current_state;
    current_state = next_state;
    
    #if 0
    if(last_state != current_state) {
      sprintf(tmp,"FSM STATE %d -> %d",(uint8_t) last_state, (uint8_t) current_state);
      send_debug_log_message(tmp);
    }
    #endif


    bool start_run;

    switch(cycle_now) {
      case CYCLE_ZERO:
        MY_H_SP = use_default_configuration_parameters ? DEFAULT_HEATER_SETPOINT_0 : config.heater_setpoint_0;
        MY_H_KP = use_default_configuration_parameters ? H_KP_1 : config.heater_kp_1;
        MY_H_KI = use_default_configuration_parameters ? H_KI_1 : config.heater_ki_1;
        MY_H_KD = use_default_configuration_parameters ? H_KD_1 : config.heater_kd_1;

        MY_M_SP = 0;
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

        MY_M_SP = 0;
        MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_1;
        MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_1;
        MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_1;

        MY_RAMP_TO_TEMP = false;
        break;
      case CYCLE_TWO:
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
      case CYCLE_THREE:
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

      //sprintf(tmp,"FSM VALIDATE_INIT_CONDITIONS - CYCLE=%d",(uint8_t) cycle_now);
      //send_debug_log_message(tmp);

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
        //end_cycle_1();
        updateLedState(LED_ABORT, true);
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
        next_state = EXIT_CYCLE;
        break;
      }

      next_state = CYCLE_RUNNING_A_START;
      cycle_now = (cycle_t) 0;      // start with the first cycle (whatever it is)
      cycle_last= (cycle_t) 0;

      break;
    }

    case CYCLE_RUNNING_A_START: {
      //sprintf(tmp,"FSM CYCLE_RUNNING_A_START - CYCLE=%d",(uint8_t) cycle_now);
      //send_debug_log_message(tmp);

      // TODO SG/GHL: skip this cycle if the runtime is ZERO?

      // Send start zone request
      heaterrxqueuemsg.type = HEATER_MSG_ZONE_STATE;
      heaterrxqueuemsg.cycleSelect = cycle_now;
      heaterrxqueuemsg.cycleEnabled = true;

      heaterrxqueuemsg.heaterRunning = doWeRunHeaterInThisCycle();
      heaterrxqueuemsg.tempSetpoint1 = MY_H_SP;
      heaterrxqueuemsg.HEATER_KP = MY_H_KP;
      heaterrxqueuemsg.HEATER_KI = MY_H_KI;
      heaterrxqueuemsg.HEATER_KD = MY_H_KD;
      heaterrxqueuemsg.rampToTemp = false;

      heaterrxqueuemsg.motorSpeed = doWeRunMotorInThisCycle() ? (double) MY_M_SP : 0.0;
      heaterrxqueuemsg.MOTOR_KP = MY_M_KP;
      heaterrxqueuemsg.MOTOR_KI = MY_M_KI;
      heaterrxqueuemsg.MOTOR_KD = MY_M_KD;
      xReturned = xQueueSend(heaterRxQueue, &heaterrxqueuemsg, 0);
      if (xReturned != pdPASS) {
        //send_debug_log_message("MAIN_TASK: Unable to send run amplification zone request.\n");
        send_debug_log_message("FSM CYCLE_RUNNING_A_START: GHL Unable to send heaterRxQueue.\n");
      }


      // Send Start Cycle String message to logging task
      logevent.event = SAMPLE_CYCLE_STARTED;
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
      //send_debug_log_message("FSM CYCLE_RUNNING_A_START: rx confirm1\n");
      
      if((uint8_t) cycle_now == 0)  {
        // THIS BLOCK only runs when starting the very first cycle (cycle==0)

        // Wait for run ok to start response, if CYCLE==0
        xReturned = xQueueReceive(main_runRespQueue, &start_run, portMAX_DELAY);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
        }
        //send_debug_log_message("FSM CYCLE_RUNNING_A_START: rx confirm2\n");

        if (!start_run) {
          send_debug_log_message("FSM CYCLE_RUNNING_A_START: Unable to begin sample run (temperatures have not yet stabilized?)\n");

          // Stop cycle one
          //end_cycle_0();
          //next_state = MAIN_STANDBY;

          // Set error during run and wait alert timeout
          //updateLedState(LED_DECLINE, true);
          exitInfo = CYCLE_ERROR_START_TEMP_TOO_HIGH;
          runThrough = true;
          next_state = CYCLE_RUNNING_C_DONE_DECIDE_STOP;
          break;
        }
      }
      
      runThrough = true;
      next_state = CYCLE_RUNNING_B_INPROGRESS;
      if(MY_RAMP_TO_TEMP) {
        send_debug_log_message("GHL Ramp To Temp Not Implemented Yet");
      }

      break;
    }

    case CYCLE_RUNNING_B_INPROGRESS: {
      if (last_state != current_state) {
        // First Time In This State
        //sprintf(tmp,"FSM CYCLE_RUNNING_B_INPROGRESS - CYCLE=%d - first run",(uint8_t) cycle_now);
        //send_debug_log_message(tmp);


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
      // handleBatteryMessage() will set exitInfo, and return CYCLE_RUNNING_C_DONE_DECIDE_STOP
      next_state = handleBatteryMessage();
      if(next_state == CYCLE_RUNNING_C_DONE_DECIDE_STOP){
        //end_cycle_0();
        //updateLedState(LED_ABORT, true);
        //exitInfo = CYCLE_ERROR_OVER_TEMP_BATTERY; // set exit info so CYCLE_RUNNING_C_DONE_DECIDE_STOP knows to not continue
        send_debug_log_message("in battery sub");
        runThrough = true;
        break;
      }

      // TODO: SG/GHL Ramping was not setup yet with this new cycle system (Chin said we weren't using it for the "hacked" 4-cycles)'
      // but i think it would be handled best here, and setting the start_time variable once temperature reached
      //// Handle Ramping (not setup yet)
      //bool setReachedRx = false;
      //xReturned = xQueueReceive(main_setPointReached, &setReachedRx, 0);
      //if (setReachedRx) {
      //  if(!send_event_log_message_struct(&ramp_to_temp_complete_log_msg)) {
      //    send_debug_log_message("MAIN_TASK: Unable to send ramp_to_temp_complete_log_msg interruption event to logging task.\r\n");
      //  }
      //  next_state = CYCLE_2_TIMER;
      //  break;
      //} else if (time_left >= end_time) {
      //  end_cycle_2();
      //  exitInfo = CYCLE_ERROR_TIMEOUT_DURING_RAMP;
      //  runThrough = true;
      //  next_state = EXIT_CYCLE;
      //  break;
      //}

      // Check Normal Runtime Expirationi
      time_left = xTaskGetTickCount() - start_time;
      if (time_left >= end_time) {
        //send_debug_log_message("in time expiry sub");
        next_state = CYCLE_RUNNING_C_DONE_DECIDE_STOP;  // do not set exit info, so cycle will continue to run
        //end_cycle_0();
        break;
      }

      // Check for errors occuring during run
      if (uxQueueMessagesWaiting(main_runErrorQueue) > 0) {
        //end_cycle_0();
        // handleMainErrorMessage() sets exitInfo, and returns CYCLE_RUNNING_C_DONE_DECIDE_STOP next_state
        send_debug_log_message("in errorcheck sub");
        next_state = handleMainErrorMessage();    //next_state = CYCLE_RUNNING_C_DONE_DECIDE_STOP;
        runThrough = true;
        // set exit info
        break;
      }

      // Get Switch data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);

      if (limitSwitchFreed(switch_data)) {
        //end_cycle_0();
        //updateLedState(LED_ABORT, true);
        send_debug_log_message("in limitswitchfreed sub");
        exitInfo = CYCLE_ERROR_SENSOR_BREAK;
        runThrough = true;
        //next_state = EXIT_CYCLE;
        next_state = CYCLE_RUNNING_C_DONE_DECIDE_STOP;
        break;
      }

      buttonData.event = NONE;
      xQueueReceive(button_mainStateQueue, &buttonData, 0);
      //TODO test also including off_event
      if (buttonData.event == ON_EVENT /*|| buttonData.even == OFF_EVENT*/) {
        //end_cycle_0();
        //updateLedState(LED_ABORT, true);
        send_debug_log_message("in buttondata sub");
        exitInfo = CYCLE_ERROR_BUTTON_EXIT;
        runThrough = true;
        //next_state = EXIT_CYCLE;
        next_state = CYCLE_RUNNING_C_DONE_DECIDE_STOP;
        break;
      }

      break;
    }

    case CYCLE_RUNNING_C_DONE_DECIDE_STOP: {
      bool stop = true;

      sprintf(tmp,"FSM CYCLE_RUNNING_C... - CYCLE=%d EXITINFO=%d",(uint8_t) cycle_now, (uint8_t) exitInfo);
      send_debug_log_message(tmp);

      // Send Stop Cycle String message to logging task
      logevent.event = SAMPLE_CYCLE_ENDED;
      if(exitInfo == CYCLE_RUNNING) {
        // normal
        sprintf(logevent.message, "Cycle %d stopped.",(uint8_t) cycle_now);
      } else  {
        // exitInfo was set to something, we stopped early
        sprintf(logevent.message, "Cycle %d stopped early.",(uint8_t) cycle_now);
      }
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
        sprintf(tmp,"FSM CYCLE_RUNNING_C_DONE_DECIDE_STOP - Exception Ocurred ExitInfo=%d",(uint8_t) exitInfo);
        send_debug_log_message(tmp);

        handle_exit_notifications();

        updateLedState(LED_ABORT, true);
        next_state = EXIT_CYCLE;
      }

      if(stop)  {
        //sprintf(tmp,"FSM CYCLE_RUNNING_C_DONE_DECIDE_STOP - Stop Decision");
        //send_debug_log_message(tmp);

        // Send stop zone request
        heaterrxqueuemsg.type = HEATER_MSG_ZONE_STATE;
        heaterrxqueuemsg.cycleSelect = cycle_now;
        heaterrxqueuemsg.cycleEnabled = false;

        //heaterrxqueuemsg.tempSetpoint1 = MY_H_SP;
        //heaterrxqueuemsg.HEATER_KP = MY_H_KP;
        //heaterrxqueuemsg.HEATER_KI = MY_H_KI;
        //heaterrxqueuemsg.HEATER_KD = MY_H_KD;
        //heaterrxqueuemsg.rampToTemp = false;

        //heaterrxqueuemsg.motorSpeed = MY_M_SP;
        //heaterrxqueuemsg.MOTOR_KP = MY_M_KP;
        //heaterrxqueuemsg.MOTOR_KI = MY_M_KI;
        //heaterrxqueuemsg.MOTOR_KD = MY_M_KD;
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
//#ifdef SAMPLE_PREP_BOARD
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
//#endif
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
  event_t eventType;

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
    return CYCLE_RUNNING_C_DONE_DECIDE_STOP;
}