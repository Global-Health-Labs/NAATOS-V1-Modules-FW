#pragma once

#include "FreeRTOS.h"
#include "fuel.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "queue.h"

void battery_task(void *pvParameters);