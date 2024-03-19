/*********************************************************************
*
*       battery_task()
*
*  Function description
*   Application entry point.
*/

#include "battery.h"

xQueueHandle battery_requestPercentQueue;
charge_state_t charge_state;
int battery_percentage = 100;

void battery_task(void * pvParameters) {
  BaseType_t xReturned;
  battery_percent_req_t recv_req;
  bool inc = false; //temp

  // Set the charge state
  charge_state = NOT_CHARGING;
  // TODO: Get the configuration settings

  for (;;) {
    // TODO: Read NRF Temperature
    // TODO: If NRF Temp > MAX_TEMP (TBD)
      // TODO: Send BATTERY_TEMP_HIGH event to logger
    // TODO: GPIO Read Charger STAT pin
      // TODO: Compare pin status and update the charger status
      // TODO: if changed, put state change message in usb_stateChangeQueue
    // TODO: I2C Fuel Gauge read (on second i2c bus to ensure no task collisions)


    // Send Battery Percentage to main task
    xReturned = xQueueSend(main_batteryDataQueue, (void *)&battery_percentage, 1000); // TODO: Probably want to send full BMS information instead
    if (xReturned != pdPASS) {
      printf("BATT_TASK: Was unable to send battery percentage to main queue. Error:%d\n", xReturned);
    }
    else {
      printf("BATT_TASK: Sent battery data to main task.\n");
    }
    // Check the battery request queue (logging)
    if (uxQueueMessagesWaiting(battery_requestPercentQueue) == 0) {
      vTaskDelay(500);
      continue;
    }

    // Pull off the battery request
    xReturned = xQueueReceive(battery_requestPercentQueue, &recv_req, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("BATT_TASK: Was unable to pull off battery request for logger queue\n");
    }
    // Send to correct task
    if (recv_req.task_req == LOGGER) {
      // Send the Battery information to logger task
      xReturned = xQueueSend(logger_recvBattPercentQueue, &battery_percentage, 0); // TODO: Probably want to send full BMS information instead
      if (xReturned != pdPASS) {
        printf("BATT_TASK: Was unable to send battery information to logger queue\n");
      }
    }

    vTaskDelay(500);
  }
}