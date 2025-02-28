#pragma once

#include "adc.h"
#include "bsp.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "nrf_gpio.h"
#include "tsys01.h"

//TODO add header comment for sensor.h

extern sensor_public_data_t PUBLIC_SENSOR_DATA;

// --------Common Functions--------
void sensors_task(void *pvParameters);
bool readTemp(sensor_selection_t sensor, float *temperature);
void handleSensorSleep(void);
void handleSensorWake(void);
void handleConfigUpdated(void);

// --------Sample Prep Functions--------
void samplePrepSensorTaskSetup(void);
void runSamplePrepSensorCollection(void);
void sensorMotorCollection(void);

// --------Power Module Functions--------
void powerModuleSensorTaskSetup(void);
void runPowerModuleSensorCollection(void);