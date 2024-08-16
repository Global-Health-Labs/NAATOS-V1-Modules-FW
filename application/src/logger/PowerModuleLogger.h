#pragma once
#include "../naatos_config.h"
#include "loggerInterface.h"

extern LoggerInterface powerModuleLogger_I;

void powerModuleGetLogFileName(const char *_logFileName, calendar_time_t time);
uint32_t powerModuleConstructSensorDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent);
uint32_t powerModuleConstructEventDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent);