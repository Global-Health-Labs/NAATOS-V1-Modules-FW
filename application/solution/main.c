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
#include "heater.h"
#include "logger.h"
#include "sensors.h"
#include "usb.h"

// Main Task Handle
xTaskHandle mainTaskHandle;

// Main State
main_state_t main_state;

/*********************************************************************
*
*       main_task()
*
*   Main Task of NAATOS Application
*/
void main_task(void * pvParameters) {
  for (;;) {

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

  xReturned = xTaskCreate(main_task, "MainTask", 2048, NULL, 2, &mainTaskHandle);
}

/*********************************************************************
*
*       create_queues()
*
*   Creates all of the FreeRTOS Queues
*/
void create_queues() {

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
  

  // Set Start up state to standby
  main_state = STANDBY;

  // Create Tasks
  create_tasks();

  // Create Queues
  create_queues();

  // Start Tasks
  vTaskStartScheduler();
}

/*************************** End of file ****************************/

