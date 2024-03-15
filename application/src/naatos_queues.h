#ifndef NAATOS_QUEUES_H
#define NAATOS_QUEUES_H

#include "FreeRTOS.h"
#include "queue.h"

#define QUEUE_SIZE  1

// Main Task Queue(s)
extern xQueueHandle main_batteryDataQueue;
extern xQueueHandle main_switchQueue;

// Heater Task Queue(s)
extern xQueueHandle heater_zoneRunQueue;
extern xQueueHandle heater_temperatureDataQueue;

// Battery Management Task Queue(s)
extern xQueueHandle battery_requestPercentQueue;

// USB Management Task Queue(s)
extern xQueueHandle usb_stateChangeQueue;

// Logger Task Queue(s)
extern xQueueHandle logger_recvBattPercentQueue;
extern xQueueHandle logger_logMessageQueue;

#endif