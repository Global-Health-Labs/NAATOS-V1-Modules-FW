/*********************************************************************
*
*       battery_task()
*
*  Function description
*   Application entry point.
*/

#include "battery.h"
#include "timers.h"

xQueueHandle batteryRxQueue;
TimerHandle_t batteryTimer;

charge_state_t charge_state;
int battery_percentage = 100;
main_state_t batt_main_state = MAIN_STANDBY;

int sendBattWDT = 0;

#define BATT_SEND_WDT_EVENT_MS 500 // 500 msec

void vBatteryTimerCallback(TimerHandle_t xTimer) {
  BaseType_t xReturned;
  BatteryRxQueueMsg_t msg;
  msg.type = BATTERY_MSG_TIMER_EVENT;

  xReturned = xQueueSend(batteryRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("Battery: Unable to send timer update to batteryRxQueue queue.\n");
  }
}

void startBatteryTimer(void) {
  TickType_t sampleRateTicks = pdMS_TO_TICKS(BATT_SEND_WDT_EVENT_MS);

  if (xTimerChangePeriod(batteryTimer, sampleRateTicks, 100) != pdPASS) {
    printf("Cannot change period of battery timer. \n");
  }

  if (xTimerStart(batteryTimer, 0) != pdPASS) {
    printf("Failed to start sensor timer. \n");
  }
}

void stopBatteryTimer(void) {
  if (xTimerStop(batteryTimer, 100) != pdPASS) {
    printf("Failed to stop sensor timer. \n");
  }
}

void battery_task(void *pvParameters) {
  BaseType_t xReturned;
  BatteryRxQueueMsg_t batteryRxMessage;

  watchdog_time_update_t wdtUpdate = {
      .taskName = BATTERY,
      .valid = true};

  bool inc = false; //temp
  tasks_t batt_task = BATTERY;
  bool cont;

  usb_suspend_req_t sus_req;
  usb_suspend_acpt_t sus_acpt = {
      .task = BATTERY,
      .suspended = true};
  usb_suspend_over_t sus_over = {
      .task = BATTERY,
      .over = true};

  TickType_t sampleRateTicks = pdMS_TO_TICKS(500);

  batteryTimer = xTimerCreate("BatteryTimer", sampleRateTicks, pdTRUE, (void *)0, vBatteryTimerCallback);
  startBatteryTimer();

  for (;;) {
    // TODO: Read NRF Temperature
    // TODO: If NRF Temp > MAX_TEMP (TBD)
    // TODO: Send BATTERY_TEMP_HIGH event to logger
    // TODO: GPIO Read Charger STAT pin
    // TODO: Compare pin status and update the charger status
    // TODO: if changed, put state change message in usb_stateChangeQueue

    xReturned = xQueueReceive(batteryRxQueue, &batteryRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("Unable to Rx data to sensor queue\n");
    } else {
      switch (batteryRxMessage.type) {
      case BATTERY_MSG_TIMER_EVENT: {
        xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
        if (xReturned != pdPASS) {
          printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
        }
        break;
      }

      case BATTERY_SOC_REQUEST: {
        // I2C Fuel Gauge read
        battery_percentage = fuelGauge_getSOC(NULL);
        if (batteryRxMessage.sendTo == BATTERY_MSG_SOC_MAIN) {
          xReturned = xQueueSend(main_batteryDataQueue, (void *)&battery_percentage, 1000); // TODO: Probably want to send full BMS information instead
          if (xReturned != pdPASS) {
            printf("BATT_TASK: Was unable to send battery percentage to main queue. Error:%d\n", xReturned);
          }
        } else if (batteryRxMessage.sendTo == BATTERY_MSG_SOC_LOG) {
          xReturned = xQueueSend(logger_recvBattPercentQueue, &battery_percentage, 0); // TODO: Probably want to send full BMS information instead
          if (xReturned != pdPASS) {
            printf("BATT_TASK: Was unable to send battery information to logger queue\n");
          }
        }

        break;
      }

      case BATTERY_MSG_SLEEP:
        if (xTimerIsTimerActive(batteryTimer) == pdTRUE) {
          stopBatteryTimer();
        }

        wdtUpdate.taskName = BATTERY;
        wdtUpdate.valid = false;
        xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
        if (xReturned != pdPASS) {
          printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
        }

        //send to main queue that we are asleep
        break;

      case BATTERY_MSG_WAKEUP:
        if (xTimerIsTimerActive(batteryTimer) == pdFALSE) {
          startBatteryTimer();
        }
        break;

      case BATTERY_CONFIG_UPDATED:

        break;

      case BATTERY_MSG_MAIN_STATE_CHANGE:
        // Respond to main state change
        xReturned = xQueueSend(main_mainStateRespQueue, &batt_task, 0);
        if (xReturned != pdPASS) {
          printf("USB: Unable to send main state response to main_mainStateRespQueue queue.\n");
        }
        break;

      default:
        break;
      }
    }
  }
}