#pragma once

#include "naatos_queues.h"
#include "naatos_config.h"
#include "states.h"
#include "task.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "sd_card.h"

void usb_task(void * pvParameters);