#pragma once

#include "adc.h"
#include "bsp.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "nrf_gpio.h"
#include "tsys01.h"

void sensors_task(void *pvParameters);
void init_sensors_gpios(void);
bool readTemp(sensor_selection_t sensor, float *temperature);
double readMotorSpeed(void);
void sensorTempCollection(void);
void sensorMotorCollection(void);