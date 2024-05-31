#ifndef NAATOS_QUEUES_H
#define NAATOS_QUEUES_H

#include "FreeRTOS.h"
#include "queue.h"

#define QUEUE_SIZE  1
#define WATCH_DOG_QUEUE_SIZE 30

// Main Task Queue(s)
extern xQueueHandle main_batteryDataQueue;
extern xQueueHandle main_switchQueue;
extern xQueueHandle main_mainStateRespQueue;
extern xQueueHandle main_runRespQueue;
extern xQueueHandle main_runErrorQueue;
extern xQueueHandle main_runConfRespQueue;

// Heater Task Queue(s)
extern xQueueHandle heater_zoneRunQueue;
extern xQueueHandle heater_temperatureDataQueue;
extern xQueueHandle heater_usbWaitQueue;
extern xQueueHandle heater_pwmReqQueue;
extern xQueueHandle heater_sensorConfQueue;

// Sensor Task Queue
extern xQueueHandle sensorRxQueue;

//Battery Task Queue
extern xQueueHandle batteryRxQueue;

//Button Task Queue
extern xQueueHandle buttonRxQueue;

// USB Management Task Queue(s)
extern xQueueHandle usb_stateChangeQueue;
extern xQueueHandle usb_recvUsbWaitAcceptQueue;
extern xQueueHandle usb_usbWaitOverQueue;
extern xQueueHandle usb_mainStateContinueQueue;

// Logger Task Queue(s)
extern xQueueHandle logger_recvBattPercentQueue;
extern xQueueHandle logger_logMessageQueue;
extern xQueueHandle logger_mainStateChangeQueue;
extern xQueueHandle logger_mainStateContinueQueue;

// PWM Task Queue(s)
extern xQueueHandle pwm_usbWaitQueue;

//Watchdog Task Queue
extern xQueueHandle watchdog_rxTimesQueue;

extern xQueueHandle button_mainStateQueue;

#endif