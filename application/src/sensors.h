#pragma once

#include "naatos_config.h"
#include "naatos_queues.h"
#include "nrf_gpio.h"
#include "bsp.h"
#include "adc.h"
#include "tsys01.h"
#include "motor.h"

#define SENSORS_EN          39

#define HAL_INPUT_PIN       2
#define OPTICAL_INPUT_PIN   3

void sensors_task(void * pvParameters);
void init_sensors_gpios(void);
long double readTemp(sensor_selection_t sensor);
double readMotorSpeed(void);