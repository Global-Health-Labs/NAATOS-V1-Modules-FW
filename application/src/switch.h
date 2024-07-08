#pragma once

#include "FreeRTOS.h"
#include "bsp.h"
#include "naatos_config.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "task.h"

void button_init(void);

void buttonTask(void *pvParameters);