#pragma once

#include "FreeRTOS.h"
#include "fuel.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "queue.h"
#include "logger/logger.h"

// PUBLIC/GLOBAL ACCESS TO THE LATEST FUELGAUGE DATA
typedef struct {
  const fuel_batt_info_t* batt_info;
} battery_public_data_t;
extern battery_public_data_t PUBLIC_FUELGAUGE_DATA;

// FUNCTIONS
void battery_task(void *pvParameters);