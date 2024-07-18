#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

xQueueHandle logger_recvBattPercentQueue;
xQueueHandle logger_logMessageQueue;
xQueueHandle logger_mainStateChangeQueue;
xQueueHandle logger_mainStateContinueQueue;

calendar_time_t time = {
    .second = 0,
    .minute = 0,
    .hour = 0,
    .day = 0,
    .week_day = 0,
    .month = 0,
    .year = 0};

const BatteryRxQueueMsg_t batt_req = {
    .type = BATTERY_SOC_REQUEST,
    .sendTo = BATTERY_MSG_SOC_LOG};

// Puts log file name in char pointer
void getLogFileName(const char *_logFileName) {
  if (!(calendar_get_time(&time))) {
    printf("LOG_TASK: Unable to get time for log file name!\n");
    sprintf(_logFileName, "unknown.csv");
  }
  // Get current Date and Time and update vars
  sprintf(_logFileName, "sample_%d-%d-%d_%d%d%d.csv", time.month, time.day, time.year, time.hour, time.minute, time.second);
}

void logger_task(void *pvParameters) {
  BaseType_t xReturned;
  main_state_t main_state = MAIN_STANDBY;
  log_data_message_t log_message;
  log_data_message_t last_temp_message;
  int battery_percent;
  bool new_temp = false;
  bool uart_only = false;
  char logFileName[50];
  char logFileLine[256];
  uint32_t logFileLineSize;
  FRESULT res;
  bool run_stopped = false;
  tasks_t logger_task = LOGGER;
  bool cont;
  bool interrupted = true;

  // If logging rate is less than once a second, do UART only
  if (config.logging_rate < 1.0) {
    uart_only = true;
  }

  for (;;) {
    run_stopped = false;
    // Wait for running state from main task update (blocking)
    xReturned = xQueueReceive(logger_mainStateChangeQueue, &main_state, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("LOG_TASK: Unable to receive state change from logger_mainStateChangeQueue.\n");
    }

    printf("LOG_TASK:Passed send to main\n");
    // Respond to main state change
    xReturned = xQueueSend(main_mainStateRespQueue, &logger_task, 0);
    if (xReturned != pdPASS) {
      printf("USB: Unable to send main state response to main_mainStateRespQueue queue.\n");
    }
    // Wait for Continue
    xReturned = xQueueReceive(logger_mainStateContinueQueue, &cont, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("USB: Unable to recevive continue to logger_mainStateContinueQueue queue.\n");
    }

    printf("LOG_TASK:Passed continue\n");

    if (main_state == MAIN_RUNNING) {
      // Create Log File based on the UTC Time of the Sample preparation
      getLogFileName(logFileName);
      res = sd_card_create_log_file(logFileName);
      if (res == FR_EXIST) {
        printf("LOG_TASK: Warning! Log file with name already exist, will be overwritting that file.\n");
      } else if (res != FR_OK) {
        printf("LOG_TASK: Unable to create log file for current sample preperation.\n");
      }
    }

    // Log sample data while we are running
    while (main_state == MAIN_RUNNING && !run_stopped) {
      new_temp = false;
      // Check if state has changed
      if (uxQueueMessagesWaiting(logger_mainStateChangeQueue) > 0) {
        //xReturned = xQueueReceive(logger_mainStateChangeQueue, &main_state, 0);
        //if (xReturned != pdPASS) {
        //  printf("LOG_TASK: Unable to receive state change from logger_mainStateChangeQueue.\n");
        //}
        //continue;
        break;
      }

      // Request the battery percentage from the bettery task
      xReturned = xQueueSend(batteryRxQueue, &batt_req, 0);
      if (xReturned != pdPASS) {
        printf("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.\n");
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

        // Get current time
        if (!(calendar_get_time(&time))) {
          printf("LOG_TASK: Unable to retreive time!");
        }

        // Setup Log Message for UART and File based on log message type
        if (log_message.data_type == TEMPERATURE_DATA) {
          // Set new temp to true
          new_temp = true;
          // Format: Time,ValveTemp,ValvePWM,Amp0Temp,Amp0PWM,Amp1Temp,Amp1PWM,Amp2Temp,Amp2PWM,Batt,Event
          logFileLineSize = sprintf(logFileLine, "%d:%d:%d,%0.2f,%0.2f,%0.2f,%0.2f,%d, \n",
              time.hour,
              time.minute,
              time.second,
              log_message.temperature_data.amp2_zone_temp,
              log_message.temperature_data.amp0_zone_pwm,
              log_message.temperature_data.motorSpeed,
              log_message.temperature_data.amp1_zone_pwm,
              battery_percent);
          last_temp_message = log_message;
        } else if (log_message.data_type == EVENT_DATA) {

          // Format: Time,ValveTemp,ValvePWM,Amp0Temp,Amp0PWM,Amp1Temp,Amp1PWM,Amp2Temp,Amp2PWM,Batt,Event
          logFileLineSize = sprintf(logFileLine, "%d:%d:%d,%0.2f,%0.2f,%0.2f,%0.2f,%d,%s\n",
              time.hour,
              time.minute,
              time.second,
              last_temp_message.temperature_data.amp2_zone_temp,
              last_temp_message.temperature_data.amp0_zone_pwm,
              last_temp_message.temperature_data.motorSpeed,
              last_temp_message.temperature_data.amp1_zone_pwm,
              battery_percent,
              log_message.event_data.message);
          if (log_message.event_data.event == SAMPLE_VALV_ENDED ||
              log_message.event_data.event == SAMPLE_INTERRUPTED ||
              log_message.event_data.event == SAMPLE_TEMPS_NOT_STABALIZED ||
              log_message.event_data.event == SAMPLE_RECOVERY_BATT ||
              log_message.event_data.event == SAMPLE_OVER_TEMP) {
            run_stopped = true;

            //Zero out the last_temp_message elements so that they all start at zero for the next log file
            last_temp_message.temperature_data.amp2_zone_temp = 0;
            last_temp_message.temperature_data.amp0_zone_pwm = 0;
            last_temp_message.temperature_data.motorSpeed = 0;
            last_temp_message.temperature_data.amp1_zone_pwm = 0;
          }
        }

        // Check UART Only
        if (!uart_only) {
          // Write to sample log file
          FRESULT res = sd_card_write_log_line(logFileName, logFileLine, logFileLineSize);
          if (res != FR_OK) {
            printf("LOG_TASK: Unable to write last log line!\n");
          }
        }
      }
    }
  }
}