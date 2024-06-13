#pragma once

#include "naatos_config.h"
#include "naatos_queues.h"
#include "pid.h"
#include "pwm.h"

#define VALVE_SETPOINT 67.6
#define AMP0_SETPOINT 67.2
#define AMP1_SETPOINT 67.4
#define AMP2_SETPOINT 68.5

#define VALVE_SETPOINT_2 95
#define AMP0_SETPOINT_2 67.0
#define AMP1_SETPOINT_2 67.0
#define AMP2_SETPOINT_2 67.0

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

void heater_task(void *pvParameters);
void sendWdtHeaterValid();