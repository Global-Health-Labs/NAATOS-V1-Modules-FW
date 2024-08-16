#include "logger.h"
#include "../naatos_messages.h"
#include "PowerModuleLogger.h"
#include "SamplePrepLogger.h"
#include "loggerInterface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

xQueueHandle logger_recvBattPercentQueue;
xQueueHandle logger_logMessageQueue;

calendar_time_t time = {
    .second = 0,
    .minute = 0,
    .hour = 0,
    .day = 0,
    .week_day = 0,
    .month = 0,
    .year = 0};

LoggerInterface samplePrepLogger_I = {
    .getLogFileName = &samplePrepGetLogFileName,
    .constructSensorDataLogLine = &samplePrepConstructSensorDataLogLine,
    .constructEventDataLogLine = &samplePrepConstructEventDataLogLine};

LoggerInterface powerModuleLogger_I = {
    .getLogFileName = &powerModuleGetLogFileName,
    .constructSensorDataLogLine = &powerModuleConstructSensorDataLogLine,
    .constructEventDataLogLine = &powerModuleConstructEventDataLogLine};

void normalize_pwm_data(log_data_message_t *rxLogMsg);

int getBatteryPercent(void) {
  BaseType_t xReturned;
  int battery_percent = 0;
  // Request the battery percentage from the bettery task
  xReturned = xQueueSend(batteryRxQueue, &batt_req_log, 0);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.\n");
  }
  // Wait for response
  xReturned = xQueueReceive(logger_recvBattPercentQueue, &battery_percent, portMAX_DELAY);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to get battery percentage from battery_requestPercentQueue.\n");
  }

  return battery_percent;
}

// Puts log file name in char pointer
void getLogFileName(const char *_logFileName) {
  if (!(calendar_get_time(&time))) {
    printf("LOG_TASK: Unable to get time for log file name!\n");
    sprintf(_logFileName, "unknown.csv");
  }
  // Get current Date and Time and update vars
  sprintf(_logFileName, "sample_%02d-%02d-%02d_%02d%02d%02d.csv",
      time.month, time.day, time.year, // Assuming 'year' is the full year, so we use % 100 to get last two digits
      time.hour, time.minute, time.second);
}

void logger_task(void *pvParameters) {
  BaseType_t xReturned;
  log_data_message_t last_temp_message = {};
  bool uart_only = false;
  char logFileName[50];
  char logFileLine[256];
  uint32_t logFileLineSize;
  FRESULT res;
  int batteryPercent = 0;

  log_data_message_t rxLogMsg;

#ifdef SAMPLE_PREP_BOARD
  LoggerInterface *loggerInterface = &samplePrepLogger_I;
#else
  LoggerInterface *loggerInterface = &powerModuleLogger_I;
#endif

  for (;;) {
    xReturned = xQueueReceive(logger_logMessageQueue, &rxLogMsg, portMAX_DELAY);

    switch (rxLogMsg.data_type) {
    case LOGGER_START_CYCLE_LOG: {
      if (config.logging_rate < 1.0) {
        uart_only = true;
      }
      // Create Log File based on the UTC Time of the Sample preparation
      getLogFileName(logFileName);
      //loggerInterface->getLogFileName(logFileName);
      res = sd_card_create_log_file(logFileName);
      if (res == FR_EXIST) {
        printf("LOG_TASK: Warning! Log file with name already exist, will be overwritting that file.\n");
      } else if (res != FR_OK) {
        printf("LOG_TASK: Unable to create log file for current sample preperation.\n");
      }
      break;
    }
    case TEMPERATURE_DATA: {
      // Get current time
      if (!(calendar_get_time(&time))) {
        printf("LOG_TASK: Unable to retreive time!");
      }

      batteryPercent = getBatteryPercent();

      normalize_pwm_data(&rxLogMsg);

      logFileLineSize = loggerInterface->constructSensorDataLogLine(logFileLine, time, rxLogMsg, batteryPercent);
      last_temp_message = rxLogMsg;

      // Check UART Only
      if (!uart_only) {
        // Write to sample log file
        FRESULT res = sd_card_write_log_line(logFileName, logFileLine, logFileLineSize);
        if (res != FR_OK) {
          printf("LOG_TASK: Unable to write last log line!\n");
        }
      }
      break;
    }

    case EVENT_DATA: {
      // Get current time
      if (!(calendar_get_time(&time))) {
        printf("LOG_TASK: Unable to retreive time!");
      }

      batteryPercent = getBatteryPercent();

      normalize_pwm_data(&rxLogMsg);

      last_temp_message.event_data = rxLogMsg.event_data;

      logFileLineSize = loggerInterface->constructEventDataLogLine(logFileLine, time, last_temp_message, batteryPercent);
      if (rxLogMsg.event_data.event == SAMPLE_VALV_ENDED ||
          rxLogMsg.event_data.event == SAMPLE_INTERRUPTED ||
          rxLogMsg.event_data.event == SAMPLE_TEMPS_NOT_STABALIZED ||
          rxLogMsg.event_data.event == SAMPLE_RECOVERY_BATT ||
          rxLogMsg.event_data.event == SAMPLE_OVER_TEMP) {

        //Zero out the last_temp_message elements so that they all start at zero for the next log file
        last_temp_message.temperature_data.amp2_zone_temp = 0;
        last_temp_message.temperature_data.amp0_zone_pwm = 0;
        last_temp_message.temperature_data.motorSpeed = 0;
        last_temp_message.temperature_data.amp1_zone_pwm = 0;
      }

      // Check UART Only
      if (!uart_only) {
        // Write to sample log file
        FRESULT res = sd_card_write_log_line(logFileName, logFileLine, logFileLineSize);
        if (res != FR_OK) {
          printf("LOG_TASK: Unable to write last log line!\n");
        }
      }

      write_to_com(logFileLine, logFileLineSize);

      break;
    }
    case UART_DATA: {
      normalize_pwm_data(&rxLogMsg);
      logFileLineSize = loggerInterface->constructSensorDataLogLine(logFileLine, time, rxLogMsg, 0);
      write_to_com(logFileLine, logFileLineSize);
      break;
    }
    case LOGGER_LOG_DEBUG_EVENT:

      break;
    default:
      break;
    }
  }
}

float normalize(float value, float min_old_range, float max_old_range, float min_new_range, float max_new_range) {
  return ((value - min_old_range) / (max_old_range - min_old_range)) * (max_new_range - min_new_range) + min_new_range;
}

void normalize_pwm_data(log_data_message_t *rxLogMsg) {
  //DEFAULT_MAX_HEATER_PID
  //MAX_MOTOR_PID

#ifdef SAMPLE_PREP_BOARD
  rxLogMsg->temperature_data.amp0_zone_pwm = normalize(rxLogMsg->temperature_data.amp0_zone_pwm, 0, DEFAULT_MAX_HEATER_PID, 0.0, 100.0);
  rxLogMsg->temperature_data.amp1_zone_pwm = normalize(rxLogMsg->temperature_data.amp1_zone_pwm, 0, MAX_MOTOR_PID, 0.0, 100.0);
  rxLogMsg->temperature_data.amp2_zone_pwm = normalize(rxLogMsg->temperature_data.amp2_zone_pwm, 0, DEFAULT_MAX_HEATER_PID, 0.0, 100.0);
  rxLogMsg->temperature_data.valve_zone_pwm = normalize(rxLogMsg->temperature_data.valve_zone_pwm, 0, DEFAULT_MAX_HEATER_PID, 0.0, 100.0);
#else

#endif
}