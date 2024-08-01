#include "cycle_state_fsm.h"

cycle_state_t current_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t next_state = VALIDATE_INIT_CONDITIONS;
cycle_state_t last_state;

cycle_state_exit_t exitInfo;



void run_cycle_state_machine() {
  last_state = current_state;
  current_state = next_state;

  switch(current_state) {
    case VALIDATE_INIT_CONDITIONS:
      exitInfo = CYCLE_RUNNING;
      // Perform initialization condition checks
      if (config.recovery_power_thresh > percent_recv) {
        printf("MAIN_TASK: Unable to begin sample run, battery percent is less than the recovery threshold.\n");
        // Tell Log that temperature is not stabalized yet
        xReturned = xQueueSend(logger_logMessageQueue, &recovery_batt_msg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send recovery battery percentage event to logging task.\n");
        }
        next_state = MAIN_STANDBY;
        sendUpdatedMainTaskState(next_state);
        // Set error during run and wait alert timeout
        updateLedState(LED_DECLINE, true);
        error_during_run = true;
        exitInfo = CYCLE_ERROR_POWER_LOW;
        next_state = EXIT_CYCLE;
        break;
      } 

      next_state = START_CYCLE_1;

      break;

    case START_CYCLE_1:
      
            // Send start amplification message to heater queue
      if (!begin_amplification_zone()) {
        printf("MAIN_TASK: Unable to begin sample run, temperatures have not yet stabalized.\n");
        // Tell Log that temperature is not stabalized yet
        xReturned = xQueueSend(logger_logMessageQueue, &temps_not_stablized_msg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send sample interruption event to logging task.\n");
        }
        // Stop amplification zone
        end_amplification_zone();
        next_state = MAIN_STANDBY;
        sendUpdatedMainTaskState(next_state);
        // Set error during run and wait alert timeout
        updateLedState(LED_DECLINE, true);
        error_during_run = true;
        exitInfo = CYCLE_ERROR_START_TEMP_TOO_HIGH;
        next_state = EXIT_CYCLE;
        break;
      }

      updateLedState(LED_RUN, true);

      next_state = CYCLE_1_RAMP_TO_TEMP;
      break;

    case CYCLE_1_RAMP_TO_TEMP:
      // Ramp to the required temperature for cycle 1
      if (/* temperature reached */) {
        next_state = CYCLE_1_TIMER;
      }
      break;

    case CYCLE_1_TIMER:
      // Run the timer for cycle 1
      if (/* timer finished */) {
        next_state = START_CYCLE_2;
      }
      break;

    case START_CYCLE_2:
      // Initialize cycle 2
      // Perform actions to start cycle 2
      next_state = CYCLE_2_RAMP_TO_TEMP;
      break;

    case CYCLE_2_RAMP_TO_TEMP:
      // Ramp to the required temperature for cycle 2
      if (/* temperature reached */) {
        next_state = CYCLE_2_TIMER;
      }
      break;

    case CYCLE_2_TIMER:
      // Run the timer for cycle 2
      if (/* timer finished */) {
        next_state = EXIT_CYCLE;
      }
      break;

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
}