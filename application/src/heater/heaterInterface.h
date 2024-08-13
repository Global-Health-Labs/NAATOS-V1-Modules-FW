#pragma once
#include "../naatos_config.h"

typedef struct {
  void (*handleHeaterSensorDataRx)(temperature_data_t temperature_data);
  void (*handleMotorDataRx)(int motor_speed);
  void (*handleHeaterZoneStateUpdate)(HeaterRxQueueMsg_t heaterRxMessage);
  void (*resetHeaterPIDs)(void);
  temperature_pwm_data_t (*getPwmData)(void);
  bool (*getOverTempStatus)(void);
  bool (*getHeaterRunningStatus)(void);
} HeaterInterface;