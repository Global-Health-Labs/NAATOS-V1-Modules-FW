#pragma once
#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "../pid.h"
#include "../pwm/pwm.h"
#include "heaterInterface.h"
#include "../tps55288.h"

//-------- Power Module Defualt PID Configs ----------
#define V_KP_1 2.250
#define V_KI_1 0.025
#define V_KD_1 2.25

#define A_KP_1 2.25
#define A_KI_1 0.025
#define A_KD_1 2.25

#define V_KP_2 2.25
#define V_KI_2 0.025
#define V_KD_2 2.25

#define A_KP_2 2.25
#define A_KI_2 0.025
#define A_KD_2 2.25

extern HeaterInterface powerModuleHeater_I;

void powerModuleHandleHeaterSensorDataRx(temperature_data_t temperature_data);

void powerModuleHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage);
void powerModuleResetHeaterPIDs(void);
temperature_pwm_data_t getPowerModulePwmData(void);
bool getPowerModuleOverTempStatus(void);
bool getPowerModuleHeaterRunningStatus(void);
temperature_data_t getPowerModuleOverTempData(void);