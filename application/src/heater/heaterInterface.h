#pragma once
#include "../naatos_config.h"

typedef struct {
  void (*handleHeaterSensorDataRx)(temperature_data_t temperature_data);
  void (*handleMotorDataRx)(int motor_speed);
  void (*handleHeaterZoneStateUpdate)(HeaterRxQueueMsg_t heaterRxMessage);
  void (*resetHeaterPIDs)(void);
} HeaterInterface;

typedef struct {
  bool amplification_zone_running;
  bool valve_zone_running;
  bool starting_run;
  bool h_pwm_req;
  bool greater_than_max;
  bool heater_run;
  bool rampToTemp;
  int wdtTimeout;
  float heater1SetPoint;
  float heater2SetPoint;
  int last_motor_speed;
} HeaterVariables;