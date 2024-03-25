#pragma once

#include "naatos_config.h"
#include "naatos_queues.h"
#include "nrf_gpio.h"
#include "bsp.h"

#define HAL_INPUT_PIN       2
#define OPTICAL_INPUT_PIN   3

void sensors_task(void * pvParameters);
void init_gpios(void);