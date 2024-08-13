#pragma once
#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "../pid.h"
#include "../pwm/pwm.h"
#include "heaterInterface.h"

//-------- Power Module Defualt PID Configs ----------
#define V_KP 2.250
#define V_KI 0.025
#define V_KD 2.25

#define A0_KP 2.25
#define A0_KI 0.025
#define A0_KD 2.25

#define A1_KP 2.25
#define A1_KI 0.025
#define A1_KD 2.25

#define A2_KP 2.25
#define A2_KI 0.025
#define A2_KD 2.25

#define V_KP_2 2.25
#define V_KI_2 0.025
#define V_KD_2 2.25

#define A0_KP_2 2.25
#define A0_KI_2 0.025
#define A0_KD_2 2.25

#define A1_KP_2 2.25
#define A1_KI_2 0.025
#define A1_KD_2 2.25

#define A2_KP_2 2.25
#define A2_KI_2 0.025
#define A2_KD_2 2.25

extern HeaterInterface powerModuleHeater_I;

void powerModuleHandleHeaterSensorDataRx(temperature_data_t temperature_data);

void powerModuleHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage);
void powerModuleResetHeaterPIDs(void);
temperature_pwm_data_t getPowerModulePwmData(void);
bool getPowerModuleOverTempStatus(void);
bool getPowerModuleHeaterRunningStatus(void);