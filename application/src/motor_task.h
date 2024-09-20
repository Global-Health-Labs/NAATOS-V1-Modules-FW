#pragma once

#include "motor.h"
#include "bsp.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "nrf_gpio.h"

void motorTask(void *pvParameters);

void init_motor_gpio();

void motorTaskSetup(void);
double readMotorSpeed(void);
void handleMotorSleep(void);
void handleMotorWake(void);
void handleMotorConfigUpdated(void);
void sensorMotorCollection(void);
