#pragma once

#include "naatos_queues.h"
#include "naatos_config.h"
#include "pid.h"
#include "pwm.h"

#define VALVE_SETPOINT    85.0
#define AMP0_SETPOINT     65.0
#define AMP1_SETPOINT     65.0
#define AMP2_SETPOINT     65.0

#define V_KP    5.0
#define V_KI    1.0
#define V_KD    1.0

#define A0_KP   5.0
#define A0_KI   0
#define A0_KD   0

#define A1_KP   5.0
#define A1_KI   1.0
#define A1_KD   1.0

#define A2_KP   5.0
#define A2_KI   1.0
#define A2_KD   1.0

void heater_task(void * pvParameters);
