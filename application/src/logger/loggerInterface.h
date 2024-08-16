#pragma once
#include "../naatos_config.h"

typedef struct {
  void (*getLogFileName)(const char *_logFileName, calendar_time_t time);
  uint32_t (*constructSensorDataLogLine)(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent);
  uint32_t (*constructEventDataLogLine)(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent);
} LoggerInterface;