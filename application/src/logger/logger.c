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

uint32_t constructDebugLogLine(char *logLineBuffer, char *message, calendar_time_t time);

fuel_batt_info_t battery_info_recv = {
  .batt_percent = 0,
  .batt_voltage = 0.0,
  .batt_temp = 0.0
};

fuel_batt_info_t getBatteryPercent(void) {
  BaseType_t xReturned;
  // Request the battery percentage from the bettery task
  xReturned = xQueueSend(batteryRxQueue, &batt_req_log, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.");
  }
  // Wait for response
  xReturned = xQueueReceive(logger_recvBattPercentQueue, &battery_info_recv, portMAX_DELAY);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to get battery percentage from battery_requestPercentQueue.");
  }

  return battery_info_recv;
}

// Puts log file name in char pointer
void getLogFileName(const char *_logFileName) {
  if (!(calendar_get_time(&time))) {
    send_debug_log_message("LOG_TASK: Unable to get time for log file name!");
    sprintf(_logFileName, "unknown.csv");
  }
  if (!(calendar_get_time(&time))) {
    send_debug_log_message("LOG_TASK: Unable to get time for log file name!");
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
  fuel_batt_info_t battery_info = {
    .batt_percent = 0,
    .batt_voltage = 0.0,
    .batt_temp = 0.0
  };

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
      res = create_log_file(logFileName);
      if (res == FR_EXIST) {
        send_debug_log_message("LOG_TASK: Warning! Log file with name already exist, will be overwritting that file.");
      } else if (res != FR_OK) {
        send_debug_log_message("LOG_TASK: Unable to create log file for current sample preperation.");
      }
      break;
    }
    case TEMPERATURE_DATA: {
      // Get current time
      if (!(calendar_get_time(&time))) {
        send_debug_log_message("LOG_TASK: Unable to retreive time!");
      }

      battery_info = getBatteryPercent();

      normalize_pwm_data(&rxLogMsg);

      logFileLineSize = loggerInterface->constructSensorDataLogLine(logFileLine, time, rxLogMsg, battery_info.batt_percent, battery_info.batt_voltage, battery_info.batt_temp);
      last_temp_message = rxLogMsg;

      // Check UART Only
      if (!uart_only) {
        // Write to sample log file
        FRESULT res = write_log_line(logFileName, logFileLine, logFileLineSize);
        if (res != FR_OK) {
          send_debug_log_message("LOG_TASK: Unable to write last log line!");
        }
      }
      break;
    }

    case EVENT_DATA: {
      // Get current time
      if (!(calendar_get_time(&time))) {
        send_debug_log_message("LOG_TASK: Unable to retreive time!");
      }

      battery_info = getBatteryPercent();

      normalize_pwm_data(&rxLogMsg);

      last_temp_message.event_data = rxLogMsg.event_data;

      logFileLineSize = loggerInterface->constructEventDataLogLine(logFileLine, time, last_temp_message, battery_info.batt_percent, battery_info.batt_voltage, battery_info.batt_temp);
      if (rxLogMsg.event_data.event == SAMPLE_CYCLE_TWO_ENDED ||
          rxLogMsg.event_data.event == SAMPLE_INTERRUPTED ||
          rxLogMsg.event_data.event == SAMPLE_TEMPS_NOT_STABALIZED ||
          rxLogMsg.event_data.event == SAMPLE_RECOVERY_BATT ||
          rxLogMsg.event_data.event == SAMPLE_OVER_TEMP) {

        //Zero out the last_temp_message elements so that they all start at zero for the next log file
        last_temp_message.temperature_data.heat_zone_3_temp = 0;
        last_temp_message.temperature_data.heat_zone_1_pwm = 0;
        last_temp_message.temperature_data.motorSpeed = 0;
        last_temp_message.temperature_data.heat_zone_2_pwm = 0;
      }

      // Check UART Only
      if (!uart_only) {
        // Write to sample log file
        FRESULT res = write_log_line(logFileName, logFileLine, logFileLineSize);
        if (res != FR_OK) {
          send_debug_log_message("LOG_TASK: Unable to write last log line!");
        }
      }

      write_to_com(logFileLine, logFileLineSize);

      break;
    }
    case UART_DATA: {
      normalize_pwm_data(&rxLogMsg);
      //if (config.debug_to_com_en) { Not sure if this is considered debug statement or not, going to keep it printing to com for now
        logFileLineSize = loggerInterface->constructSensorDataLogLine(logFileLine, time, rxLogMsg, battery_info.batt_percent, battery_info.batt_voltage, battery_info.batt_temp);
        write_to_com(logFileLine, logFileLineSize);
      //}
      break;
    }
    case LOGGER_LOG_DEBUG_EVENT:
      logFileLineSize = constructDebugLogLine(logFileLine, rxLogMsg.event_data.message, time);
      write_to_com(logFileLine, logFileLineSize);
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
#ifdef SAMPLE_PREP_BOARD
if (use_default_configuration_parameters) {
    rxLogMsg->temperature_data.heat_zone_0_pwm = normalize(rxLogMsg->temperature_data.heat_zone_0_pwm, 0, DEFAULT_MAX_HEATER_PID, 0.0, 100.0);
    rxLogMsg->temperature_data.heat_zone_1_pwm = normalize(rxLogMsg->temperature_data.heat_zone_1_pwm, 0, DEFAULT_MAX_HEATER_PID, 0.0, 100.0);
    rxLogMsg->temperature_data.heat_zone_2_pwm = normalize(rxLogMsg->temperature_data.heat_zone_2_pwm, 0, MAX_MOTOR_PID, 0.0, 100.0);
    rxLogMsg->temperature_data.heat_zone_3_pwm = normalize(rxLogMsg->temperature_data.heat_zone_3_pwm, 0, DEFAULT_MAX_HEATER_PID, 0.0, 100.0);
}
else {
    rxLogMsg->temperature_data.heat_zone_0_pwm = normalize(rxLogMsg->temperature_data.heat_zone_0_pwm, 0, config.max_heater_pid_pwm, 0.0, 100.0);
    rxLogMsg->temperature_data.heat_zone_1_pwm = normalize(rxLogMsg->temperature_data.heat_zone_1_pwm, 0, config.max_heater_pid_pwm, 0.0, 100.0);
    rxLogMsg->temperature_data.heat_zone_2_pwm = normalize(rxLogMsg->temperature_data.heat_zone_2_pwm, 0, MAX_MOTOR_PID, 0.0, 100.0);
    rxLogMsg->temperature_data.heat_zone_3_pwm = normalize(rxLogMsg->temperature_data.heat_zone_3_pwm, 0, config.max_heater_pid_pwm, 0.0, 100.0);
}
#else
  rxLogMsg->temperature_data.heat_zone_0_pwm = normalize(rxLogMsg->temperature_data.heat_zone_0_pwm, 0, DEFAULT_MAX_VALVE_PID, 0.0, 100.0);
  rxLogMsg->temperature_data.heat_zone_2_pwm = normalize(rxLogMsg->temperature_data.heat_zone_2_pwm, 0, DEFAULT_MAX_AMP_PID, 0.0, 100.0);
#endif
}

uint32_t constructDebugLogLine(char *logLineBuffer, char *message, calendar_time_t time) {
  return sprintf(logLineBuffer, "%02d:%02d:%02d - %s\n",
      time.hour,
      time.minute,
      time.second,
      message);
}

void send_event_log_message(event_t eventType, char *message) {
  log_event_t event_info = {
      .event = eventType};

  strncpy(event_info.message, message, sizeof(event_info.message) - 1);
  event_info.message[sizeof(event_info.message) - 1] = '\0';  // Ensure null termination

  log_data_message_t log_message = {
    .data_type = EVENT_DATA,
    .temperature_data = NULL,
    .event_data = event_info};

  BaseType_t xReturned = xQueueSend(logger_logMessageQueue, &log_message, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send event log message.");
  }
}

bool send_event_log_message_struct(const log_data_message_t *message) {
  BaseType_t xReturned = xQueueSend(logger_logMessageQueue, message, 0);
  if (xReturned != pdPASS) {
    return false;
  }
  return true;
}

void send_data_log_message() {

}


void send_debug_log_message(char *message) {
  char tmp_msg[256];
  
  sprintf(tmp_msg, "%s\r\n", message);
  printf(tmp_msg);

  if (!usb_started) {
    return;
  }
  
  if (config.debug_to_com_en) {
    log_event_t event_info = {
        .event = SAMPLE_UNKNOWN};

    strncpy(event_info.message, tmp_msg, sizeof(event_info.message) - 1);
    event_info.message[sizeof(event_info.message) - 1] = '\0';  // Ensure null termination

    log_data_message_t log_message = {
        .data_type = LOGGER_LOG_DEBUG_EVENT,
        .temperature_data = NULL,
        .event_data = event_info};

    BaseType_t xReturned = xQueueSend(logger_logMessageQueue, &log_message, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("MAIN_TASK: Unable to send debug log message.");
    }
  }
}


