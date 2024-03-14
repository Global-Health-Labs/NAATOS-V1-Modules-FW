#pragma once

#include "FreeRTOS.h"
#include "queue.h"

#define QUEUE_SIZE  5

// Main Task Queue(s)
xQueueHandle main_batteryDataQueue;
xQueueHandle main_switchQueue;

// Heater Task Queue(s)
xQueueHandle heater_zoneRunQueue;
xQueueHandle heater_temperatureDataQueue;

// Battery Management Task Queue(s)
xQueueHandle battery_requestPercentQueue;

// USB Management Task Queue(s)
xQueueHandle usb_stateChangeQueue;

// Logger Task Queue(s)
xQueueHandle logger_recvBattPercentQueue;
xQueueHandle logger_logMessageQueue;
