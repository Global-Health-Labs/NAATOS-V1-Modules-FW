#pragma once
#include "../naatos_config.h"
#include "loggerInterface.h"

extern LoggerInterface powerModuleLogger_I;

void powerModuleGetLogFileName(const char *_logFileName);
uint32_t powerModuleConstructSensorDataLogLine(char*logLineBuffer);
uint32_t powerModuleConstructEventDataLogLine(char*logLineBuffer);