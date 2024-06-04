#pragma once

#include "nrf_delay.h"
#include "bsp.h"
#include "FreeRTOS.h"
#include "nrf.h"
#include "naatos_config.h"
#include "task.h"

#define BUTTON_INPUT_PIN NRF_GPIO_PIN_MAP(1,6)


void button_init(void);

void buttonTask(void * pvParameters);