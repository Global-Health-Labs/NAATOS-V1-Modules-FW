#pragma once

#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "../pid.h"
#include "../pwm/pwm.h"
#include "../usb.h"
#include "samplePrepHeater.h"
#include "powerModuleHeater.h"
#include "heaterInterface.h"

// --------Common Functions--------
void heater_task(void *pvParameters);
void sendWdtHeaterValid();
