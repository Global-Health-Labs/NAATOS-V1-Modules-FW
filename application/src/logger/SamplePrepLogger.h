#pragma once
#include "../naatos_config.h"
#include "loggerInterface.h"

extern LoggerInterface samplePrepLogger_I;

void samplePrepGetLogFileName(const char *_logFileName);
uint32_t samplePrepConstructSensorDataLogLine(char*logLineBuffer);
uint32_t samplePrepConstructEventDataLogLine(char*logLineBuffer);