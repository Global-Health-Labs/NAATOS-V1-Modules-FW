#pragma once

#include "FreeRTOS.h"
#include "bsp.h"
#include "naatos_config.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_wdt.h"
#include "task.h"

#define NAATOS_ENABLE_WATCHDOG 1

void wdt_event_handler(void);

void watchdog_init(void);

void sendWatchdogKickFromTask(tasks_t task, bool valid);

void wdtFeedTask(void *pvParameters);