#pragma once

#include "naatos_queues.h"
#include "naatos_config.h"
#include "pid.h"
#include "pwm.h"

#define MOTOR_SETPOINT    1000 //RP

//TODO: Figure out what these values should actually be
#define MOTOR_P    2.250
#define MOTOR_I    0.021
#define MOTOR_D    2.255

//void motor_task(void * pvParameters);
