#pragma once
#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "../pid.h"
#include "../pwm/pwm.h"
#include "heaterInterface.h"

//-------- Sample Prep Defualt PID Configs ----------
#define M_KP 0.006
#define M_KI 0.0005
#define M_KD 0.002

#define H_KP 2.0
#define H_KI 0.010
#define H_KD 0.0

extern HeaterInterface samplePrepHeater_I;

void handleSampleMotorDataRx(int motor_speed);
void samplePrepHandleHeaterSensorDataRx(temperature_data_t temperature_data);
void samplePrepHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage);
void samplePrepResetHeaterPIDs(void);
temperature_pwm_data_t getSamplePrepPwmData(void);
bool getSamplePrepOverTempStatus(void);
bool getSamplePrepHeaterRunningStatus(void);