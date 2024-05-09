/*********************************************************************
*
*       battery_task()
*
*  Function description
*   Application entry point.
*/

#include "battery.h"
#include "timers.h"

xQueueHandle battery_requestPercentQueue;
xQueueHandle battery_mainStateQueue;
xQueueHandle battery_usbWaitQueue;

charge_state_t charge_state;
int battery_percentage = 100;
main_state_t batt_main_state = STANDBY;

void battery_task(void * pvParameters) {
  BaseType_t xReturned;
  battery_percent_req_t recv_req;
  bool inc = false; //temp

  usb_suspend_req_t sus_req;
  usb_suspend_acpt_t sus_acpt = {
    .task = BATTERY,
    .suspended = true
  };
  usb_suspend_over_t sus_over = {
    .task = BATTERY,
    .over = true
  };

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

    // Check to see if the main task state has changed
    if (uxQueueMessagesWaiting(battery_mainStateQueue) > 0) {
      xReturned = xQueueReceive(battery_mainStateQueue, &batt_main_state, 0);
      if (xReturned != pdPASS) {
        printf("BATT_TASK: Unable to receive state change from battery_mainStateQueue.\n");
      }
      continue;
    }

    // Check to see if we need to suspend for USB to be enabled
    if (uxQueueMessagesWaiting(battery_usbWaitQueue) > 0) {
      xReturned = xQueueReceive(battery_usbWaitQueue, &sus_req, 0) ;
      if (xReturned != pdPASS) {
        printf("BATTERY: Unable to receive usb suspend request from battery_usbWaitQueue\n");
      }
      // Send Suspend Accepted
      xReturned = xQueueSend(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0); 
      if (xReturned != pdPASS) {
        printf("BATTERY: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue\n");
      }
      printf("BATTERY: Suspending for 15 seconds.\n");
      // Delay Task for 15 Seconds
      vTaskDelay(pdMS_TO_TICKS(15000));
      // Send Suspend Over
      xReturned = xQueueSend(usb_usbWaitOverQueue, &sus_over, 0); 
      if (xReturned != pdPASS) {
        printf("BATTERY: Unable to send usb suspend over to usb_usbWaitOverQueue\n");
      }
    }

    // Send Battery Percentage to main task if we are in standby
    if (batt_main_state == STANDBY) {
      xReturned = xQueueSend(main_batteryDataQueue, (void *)&battery_percentage, 1000); // TODO: Probably want to send full BMS information instead
      if (xReturned != pdPASS) {
        printf("BATT_TASK: Was unable to send battery percentage to main queue. Error:%d\n", xReturned);
      }
    }

    // Check the battery request queue (logging)
    if (uxQueueMessagesWaiting(battery_requestPercentQueue) == 0) {
      vTaskDelay(100);
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

    vTaskDelay(100);
  }
}