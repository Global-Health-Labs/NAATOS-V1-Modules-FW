#pragma once

#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "../pid.h"
#include "../pwm.h"
#include "../usb.h"
#include "samplePrepHeater.h"
#include "powerModuleHeater.h"
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


// --------Common Functions--------
void heater_task(void *pvParameters);
void sendWdtHeaterValid();

// --------Power Module Functions--------
void powerModuleHandleHeaterSensorDataRx(temperature_data_t temperature_data);

extern HeaterInterface powerModuleHeater_I;

extern HeaterVariables heaterVariables;