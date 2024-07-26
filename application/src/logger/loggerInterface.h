#pragma once
#include "../naatos_config.h"

typedef struct {
  void (*getLogFileName)(const char *_logFileName);
  uint32_t (*constructSensorDataLogLine)(char*logLineBuffer);
  uint32_t (*constructEventDataLogLine)(char*logLineBuffer);
} LoggerInterface;
