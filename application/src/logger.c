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
  sprintf(_logFileName, "sample1.csv");
}

void logger_task(void * pvParameters) {
  BaseType_t xReturned;
  main_state_t main_state = STANDBY;
  log_data_message_t log_message;
  log_data_message_t last_temp_message;
  int battery_percent;
  bool new_temp = false;
  bool uart_only = false;
  char logFileName[50];
  char logFileLine[256];
  uint32_t logFileLineSize;
  FRESULT res;
  uint8_t hour = 12, minute = 59;
  bool run_stopped = false;

  // TODO: Get the configuration settings 
  // TODO: Check the log rate (if its less than 1 set to UART Logging only)
  for (;;) {
    run_stopped = false;
    // Wait for running state from main task update (blocking)
    xReturned = xQueueReceive(logger_mainStateChangeQueue, &main_state, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("LOG_TASK: Unable to receive state change from logger_mainStateChangeQueue.\n");
    }
    if (main_state == RUNNING) {
      // Create Log File based on the UTC Time of the Sample preperation
      getLogFileName(logFileName);
      res = sd_card_create_log_file(logFileName);
      if (res == FR_EXIST) {
        printf("LOG_TASK: Warning! Log file with name already exist, will be overwritting that file.\n");
      }
      else if (res != FR_OK) {
        printf("LOG_TASK: Unable to create log file for current sample preperation.\n");
      }
    }

    // Log sample data while we are running
    while (main_state == RUNNING && !run_stopped) {
      new_temp = false;
      // Check if state has changed
      if (uxQueueMessagesWaiting(logger_mainStateChangeQueue) > 0) {
        xReturned = xQueueReceive(logger_mainStateChangeQueue, &main_state, 0);
        if (xReturned != pdPASS) {
          printf("LOG_TASK: Unable to receive state change from logger_mainStateChangeQueue.\n");
        }
        //continue;
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
      
      while (!new_temp && !run_stopped) {
        // Wait for a log data message (either temperature data or event data)
        xReturned = xQueueReceive(logger_logMessageQueue, &log_message, portMAX_DELAY);
        if (xReturned != pdPASS) {
          printf("LOG_TASK: Unable to get log message from logger_logMessageQueue.\n");
        }
         
        // TODO: Get current time

        // Setup Log Message for UART and File based on log message type
         if (log_message.data_type == TEMPERATURE_DATA) {
          // Set new temp to true
          new_temp = true;
          // Format: Time,ValveTemp,Amp0Temp,Amp1Temp,Amp2Temp,BattPercent,Event
          logFileLineSize = sprintf(logFileLine, "%d:%d,%0.2f,%0.2f,%0.2f,%0.2f,%d,NONE\n", hour, minute, log_message.temperature_data.valve_zone_temp,
                                    log_message.temperature_data.amp0_zone_temp, log_message.temperature_data.amp1_zone_temp,
                                    log_message.temperature_data.amp2_zone_temp, battery_percent);
          last_temp_message = log_message;
        }
        else if (log_message.data_type == EVENT_DATA) {
          
          // Format: Time,ValveTemp,Amp0Temp,Amp1Temp,Amp2Temp,BattPercent,Event
          logFileLineSize = sprintf(logFileLine, "%d:%d,%0.2f,%0.2f,%0.2f,%0.2f,%d,%s\n", hour, minute, last_temp_message.temperature_data.valve_zone_temp,
                                    last_temp_message.temperature_data.amp0_zone_temp, last_temp_message.temperature_data.amp1_zone_temp,
                                    last_temp_message.temperature_data.amp2_zone_temp, battery_percent, log_message.event_data.message);
          if (log_message.event_data.event == SAMPLE_END || log_message.event_data.event == SAMPLE_INTERRUPTED) {
            run_stopped = true;
          }
        }
        
        // Check UART Only 
        if (!uart_only) {
          // Write to sample log file
          FRESULT res = sd_card_write_log_line(logFileName, logFileLine, logFileLineSize);
          if (res != FR_OK) {
            printf("LOG_TASK: Unable to write last log line!\n");
          }
          else {
            printf("LOG_TASK: Wrote line to log\n");
          }
        }
      } 
    }
   }
 }
