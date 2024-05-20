#pragma once

#include "nrf_drv_wdt.h"
#include "nrf_drv_clock.h"
#include "nrf_delay.h"

#include "FreeRTOS.h"

void wdt_event_handler(void);

void watchdog_init(void);

void wdt_task(void * pvParameters);