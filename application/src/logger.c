#include "logger.h"

xQueueHandle logger_recvBattPercentQueue;
xQueueHandle logger_logMessageQueue;
xQueueHandle logger_mainStateChangeQueue;

const battery_percent_req_t batt_req = {
  .task_req = LOGGER
};

// Puts log file name in char pointer
void getLogFileName(const char * _logFileName) {
  uint8_t minute=52, hour=13, day=5, month=4, year=24;
  // TODO: Get current Date and Time and update vars
  sprintf(_logFileName, "sample.csv");
}

void logger_task(void * pvParameters) {
  BaseType_t xReturned;
  main_state_t main_state = STANDBY;
  log_data_message_t log_message;
  int battery_percent;
  bool new_temp = false;
  bool uart_only = false;
  char logFileName[50];
  FRESULT res;

  // TODO: Get the configuration settings 
  // TODO: Check the log rate (if its less than 1 set to UART Logging only)
  for (;;) {
    // Wait for running state from main task update (blocking)
    xReturned = xQueueReceive(logger_mainStateChangeQueue, &main_state, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("LOG_TASK: Unable to receive state change from logger_mainStateChangeQueue.\n");
    }
    // Create Log File based on the UTC Time of the Sample preperation
    getLogFileName(logFileName);
    res = sd_card_create_log_file(logFileName);
    if (res != FR_OK) {
      printf("LOG_TASK: Unable to create log file for current sample preperation.\n");
    }

    // Log sample data while we are running
    while (main_state == RUNNING) {
      // Check if state has changed
      if (uxQueueMessagesWaiting(logger_mainStateChangeQueue) > 0) {
        xReturned = xQueueReceive(logger_mainStateChangeQueue, &main_state, 0);
        if (xReturned != pdPASS) {
          printf("LOG_TASK: Unable to receive state change from logger_mainStateChangeQueue.\n");
        }
        continue;
      }

      // Request the battery percentage from the bettery task
      xReturned = xQueueSend(battery_requestPercentQueue, &batt_req, 0);
      if (xReturned != pdPASS) {
        printf("LOG_TASK: Unable to send battery percentage request to battery_requestPercentQueue.\n");
      }
      // Wait for response 
      xReturned = xQueueReceive(logger_recvBattPercentQueue, &battery_percent, portMAX_DELAY);
      if (xReturned != pdPASS) {
        printf("LOG_TASK: Unable to get battery percentage from battery_requestPercentQueue.\n");
      }
      
      while (!new_temp) {
        // Wait for a log data message (either temperature data or event data)
        xReturned = xQueueReceive(logger_logMessageQueue, &log_message, portMAX_DELAY);
        if (xReturned != pdPASS) {
          printf("LOG_TASK: Unable to get log message from logger_logMessageQueue.\n");
        }

        if (log_message.data_type == TEMPERATURE_DATA) {
          new_temp = true;
          // TODO: Setup Log Message for UART and File
        }
        else if (log_message.data_type == EVENT_DATA) {
          // TODO: Setup Log Message for UART and File
        }
        
        // TODO: Get current time
        // TODO: Format Log String to have HH-MM-SS Message
        
        // Check UART Only 
        if (!uart_only) {
          // TODO: Write to sample log file
        }
      } 
    }
   }
 }
