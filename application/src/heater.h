#pragma once

#include "naatos_queues.h"
#include "naatos_config.h"
#include "pid.h"
#include "pwm.h"

#define VERBOSE_PID       1

#define VALVE_SETPOINT    85.0
#define AMP0_SETPOINT     65.0
#define AMP1_SETPOINT     65.0
#define AMP2_SETPOINT     65.0

#define V_KP    2.250
#define V_KI    0.021
#define V_KD    2.255

#define A0_KP   2.185
#define A0_KI   0.016
#define A0_KD   1.258

#define A1_KP   2.122
#define A1_KI   0.012
#define A1_KD   1.277

#define A2_KP   2.157
#define A2_KI   0.014
#define A2_KD   1.262

void heater_task(void * pvParameters);
