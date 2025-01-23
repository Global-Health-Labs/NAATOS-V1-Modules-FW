#pragma once
#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "../pid.h"
#include "../pwm/pwm.h"
#include "heaterInterface.h"

//-------- Sample Prep Defualt PID Configs ----------
#define M_KP 0.005
#define M_KI 0.0005
#define M_KD 0.020

#define H_KP_1 20.0
#define H_KI_1 0.05
#define H_KD_1 50.0

#define H_KP_2 20.0
#define H_KI_2 0.05
#define H_KD_2 50.0

#define VERBOSE_HEATING 0
#define VERBOSE_MOTOR   0

extern HeaterInterface samplePrepHeater_I;

void handleSampleMotorDataRx(int motor_speed);
void samplePrepHandleHeaterSensorDataRx(temperature_data_t temperature_data);
void samplePrepHandleZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage);
void samplePrepResetHeaterPIDs(void);
temperature_pwm_data_t getSamplePrepPwmData(void);
bool getSamplePrepOverTempStatus(void);
bool getSamplePrepHeaterRunningStatus(void);
temperature_data_t getSamplePrepOverTempData(void);