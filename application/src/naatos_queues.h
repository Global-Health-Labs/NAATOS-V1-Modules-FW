#ifndef NAATOS_QUEUES_H
#define NAATOS_QUEUES_H

#include "FreeRTOS.h"
#include "queue.h"

#define QUEUE_SIZE 1
#define WATCH_DOG_QUEUE_SIZE 30

// Main Task Queue(s)
extern xQueueHandle main_batteryDataQueue;
extern xQueueHandle main_switchQueue;
extern xQueueHandle main_mainStateRespQueue;
extern xQueueHandle main_runRespQueue;
extern xQueueHandle main_runErrorQueue;
extern xQueueHandle main_runConfRespQueue;
extern xQueueHandle main_usbConnRecvQueue;
extern xQueueHandle main_usbChangedConfQueue;
extern xQueueHandle main_wakeupTasksQueue;
extern xQueueHandle main_setPointReached;

// Sensor Task Queue
extern xQueueHandle sensorRxQueue;

//Battery Task Queue
extern xQueueHandle batteryRxQueue;

//Button Task Queue
extern xQueueHandle buttonRxQueue;

//Heater Task Queue
extern xQueueHandle heaterRxQueue;

// USB Management Task Queue(s)
extern xQueueHandle usbRxQueue;
extern xQueueHandle usb_recvUsbWaitAcceptQueue;
extern xQueueHandle usb_usbWaitOverQueue;

// Composite USB Task Queue(s)
extern xQueueHandle compositeRxQueue;

// Logger Task Queue(s)
extern xQueueHandle loggerRxQueue;
extern xQueueHandle logger_recvBattPercentQueue;
extern xQueueHandle logger_logMessageQueue;
extern xQueueHandle logger_mainStateChangeQueue;
extern xQueueHandle logger_mainStateContinueQueue;

// PWM Task Queue(s)
extern xQueueHandle pwmRxQueue;

//Watchdog Task Queue
extern xQueueHandle watchdog_rxTimesQueue;

extern xQueueHandle button_mainStateQueue;

extern xQueueHandle ledRxQueue;

#endif