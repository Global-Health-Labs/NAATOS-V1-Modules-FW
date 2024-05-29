#pragma once

#include "naatos_config.h"
#include "naatos_queues.h"
#include "nrf_gpio.h"
#include "bsp.h"
#include "adc.h"
#include "tsys01.h"

#define SENSORS_EN          39

#define HAL_INPUT_PIN       2
#define OPTICAL_INPUT_PIN   3

//Pin P1.07 on eval board
#define MOTOR_INPUT_PIN     38 //TODO: Change to actual pin on sample prep main board

void sensors_task(void * pvParameters);
void init_sensors_gpios(void);
long double readTemp(sensor_selection_t sensor);