#pragma once
#include "../naatos_config.h"
#include "loggerInterface.h"

extern LoggerInterface samplePrepLogger_I;

void samplePrepGetLogFileName(const char *_logFileName, calendar_time_t time);
uint32_t samplePrepConstructSensorDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent);
uint32_t samplePrepConstructEventDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent);