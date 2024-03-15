/*********************************************************************
*                               NAATOS                               *
**********************************************************************
File    : main.c
Purpose : NAATOS Application Start
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "bsp.h"
#include "nordic_common.h"
#include "nrf_drv_clock.h"
#include "sdk_errors.h"
#include "app_error.h"

#include "states.h"
#include "battery.h"
#include "logger.h"
#include "sensors.h"
#include "heater.h"
#include "usb.h"
#include "naatos_queues.h"
#include "naatos_config.h"

// Task Handles
xTaskHandle mainTaskHandle;
xTaskHandle heaterTaskHandle;
xTaskHandle loggerTaskHandle;
xTaskHandle sensorsTaskHandle;
xTaskHandle batteryTaskHandle;
xTaskHandle usbTaskHandle;

xQueueHandle main_batteryDataQueue;
xQueueHandle main_switchQueue;

/*********************************************************************
*
*       main_task()
*
*   Main Task of NAATOS Application
*/
void main_task(void * pvParameters) {
  BaseType_t xReturned;
  uint8_t queue_size;
  sensor_switches_t switch_data;
  int percent_recv;   
  bool hal_triggered = false, optical_triggered = false; 

  // Set Start up state to standby
  main_state_t main_state = STANDBY;
  // TODO: Get Configuration Settings
  
  // Main State Loop
  for (;;) {
    switch(main_state) {
      // In Standby State
      case STANDBY:
        // Check for Battery Data in Battery Queue
        printf("Current size of main_batteryDataQueue: %d\n", uxQueueMessagesWaiting(main_batteryDataQueue));
        if (xQueueReceive(main_batteryDataQueue, &percent_recv, 0) == pdPASS) {
          if (percent_recv < LOW_POWER_THRESHOLD) {
             main_state = LOW_POWER;
             break;
          }
          printf("Got battery percentage in main_batteryDataQueue: %d \n",percent_recv);
        }
        else {
          printf("Error receiving battery percentage from main_batteryDataQueue\n");
        }
        // Wait for Sensor Switch Data 
        xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
        if (xReturned != pdPASS) {
          printf("Error receiving switch data from main_switchQueue\n");
        }
        printf("Got switch data in main_switchQueue\n");
        // Update switches triggered
        hal_triggered = switch_data.hal_triggered;
        optical_triggered = switch_data.optical_tiggered;
        // Check if we can go to RUN state
        if (hal_triggered && optical_triggered) {
          main_state = RUNNING;
        }
      break;
      // In Running State
      case RUNNING:

      break;
      // In Low Power State
      case LOW_POWER:

      break;
      // Shouldnt Get here
      default:
      break;
    }
  }
}

/*********************************************************************
*
*       create_tasks()
*
*   Creates all of the FreeRTOS Tasks
*/
void create_tasks() {
  BaseType_t xReturned;
  // Main Task
  xReturned = xTaskCreate(main_task, "MainTask", 100, NULL, 0, &mainTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating main task. Error: %d\n", xReturned);
      vTaskDelete( mainTaskHandle );
  }
  // Heater Task
  xReturned = xTaskCreate(heater_task, "HeaterTask", 100, NULL, 0, &heaterTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating heater task. Error: %d\n", xReturned);
      vTaskDelete( heaterTaskHandle );
  }
  // Logger Task
  xReturned = xTaskCreate(logger_task, "LoggerTask", 100, NULL, 0, &loggerTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating logger task. Error: %d\n", xReturned);
      vTaskDelete( loggerTaskHandle );
  }
  // Sensors Task
  xReturned = xTaskCreate(sensors_task, "SensorsTask", 100, NULL, 0, &sensorsTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating sensors task. Error: %d\n", xReturned);
      vTaskDelete( sensorsTaskHandle );
  }
  // Battery Management Task
  xReturned = xTaskCreate(battery_task, "BatteryTask", 100, NULL, 0, &batteryTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating battery management task. Error: %d\n", xReturned);
      vTaskDelete( batteryTaskHandle );
  }
  // USB Management Task
  xReturned = xTaskCreate(usb_task, "USBTask", 100, NULL, 0, &usbTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating usb management task. Error: %d\n", xReturned);
      vTaskDelete( usbTaskHandle );
  }
}

/*********************************************************************
*
*       create_queues()
*
*   Creates all of the FreeRTOS Queues
*/
void create_queues() {
  // Main Task Queues
  main_batteryDataQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));        // TODO: Update sizes, int is just the placeholder 
  if (main_batteryDataQueue == NULL)
    printf("Unable to create main_batteryDataQueue queue\n");
  main_switchQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (main_switchQueue == NULL)
    printf("Unable to create main_switchQueue queue\n");

  // Heater Task Queues
  heater_zoneRunQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (heater_zoneRunQueue == NULL)
    printf("Unable to create heater_zoneRunQueue queue\n");
  heater_temperatureDataQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (heater_temperatureDataQueue == NULL)
    printf("Unable to create heater_temperatureDataQueue queue\n");

  // Battery Management Task Queues
  battery_requestPercentQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (battery_requestPercentQueue == NULL)
    printf("Unable to create battery_requestPercentQueue queue\n");

  // USB Management Task Queues
  usb_stateChangeQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (usb_stateChangeQueue == NULL)
    printf("Unable to create usb_stateChangeQueue queue\n");

  // Logger Task Queues
  logger_recvBattPercentQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (logger_recvBattPercentQueue == NULL)
    printf("Unable to create logger_recvBattPercentQueue queue\n");
  logger_logMessageQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (logger_logMessageQueue == NULL)
    printf("Unable to create logger_logMessageQueue queue\n");
}

/*********************************************************************
*
*       main()
*
*   Application entry point.
*/
int main(void) {
  ret_code_t err_code;

  // Initialize clock driver for better time accuracy in FREERTOS
  err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  // TODO: Full Peripheral Initalizations

  // Create Tasks
  create_tasks();

  // Create Queues
  create_queues();

  // Start Tasks
  vTaskStartScheduler();
}

/*************************** End of file ****************************/

