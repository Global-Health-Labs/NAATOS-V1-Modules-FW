#pragma once

#include "adc.h"
#include "bsp.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "nrf_gpio.h"
#include "tsys01.h"

//TODO add header comment for sensor.h

// --------Common Functions--------
void init_sensors_gpios(void);
void sensors_task(void *pvParameters);
bool readTemp(sensor_selection_t sensor, float *temperature);
void handleSensorSleep(void);
void handleSensorWake(void);
void handleConfigUpdated(void);

// --------Sample Prep Functions--------
void samplePrepSensorTaskSetup(void);
double readMotorSpeed(void);
void runSamplePrepSensorCollection(void);
void sensorMotorCollection(void);

// --------Power Module Functions--------
void powerModuleSensorTaskSetup(void);
void runPowerModuleSensorCollection(void);