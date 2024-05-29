#pragma once

#include "nrf_drv_wdt.h"
#include "nrf_drv_clock.h"
#include "nrf_delay.h"
#include "bsp.h"
#include "FreeRTOS.h"
#include "nrf.h"
#include "naatos_config.h"
#include "task.h"

#define NAATOS_ENABLE_WATCHDOG 0

void wdt_event_handler(void);

void watchdog_init(void);

void wdtFeedTask(void * pvParameters);