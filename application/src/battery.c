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
fuel_batt_info_t batt_info = {
  .batt_percent = 100,
  .batt_voltage = 8.0,
  .batt_temp = 20.0
};
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
    send_debug_log_message("Battery: Unable to send timer update to batteryRxQueue queue.");
  }
}

void startBatteryTimer(void) {
  TickType_t sampleRateTicks = pdMS_TO_TICKS(BATT_SEND_WDT_EVENT_MS);

  if (xTimerChangePeriod(batteryTimer, sampleRateTicks, 100) != pdPASS) {
    send_debug_log_message("Cannot change period of battery timer.");
  }

  if (xTimerStart(batteryTimer, 0) != pdPASS) {
    send_debug_log_message("Failed to start sensor timer.");
  }
}

void stopBatteryTimer(void) {
  if (xTimerStop(batteryTimer, 100) != pdPASS) {
    send_debug_log_message("Failed to stop sensor timer.");
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
      send_debug_log_message("Unable to Rx data to sensor queue");
    } else {
      switch (batteryRxMessage.type) {
      case BATTERY_MSG_TIMER_EVENT: {
        xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
        }
        break;
      }

      case BATTERY_SOC_REQUEST: {
        char tmp[100];
        // I2C Fuel Gauge read
        batt_info.batt_percent = fuelGauge_getSOC(NULL);
        vTaskDelay(pdMS_TO_TICKS(25));
        batt_info.batt_voltage = (((float)fuelGauge_getBattVoltage(NULL)) / 1000.0);
        vTaskDelay(pdMS_TO_TICKS(25));
        batt_info.batt_temp = (float)fuelGauge_getBattTemperature(NULL);
        vTaskDelay(pdMS_TO_TICKS(25));
        if (batteryRxMessage.sendTo == BATTERY_MSG_SOC_MAIN) {
          xReturned = xQueueSend(main_batteryDataQueue, (void *)&batt_info, 1000); // TODO: Probably want to send full BMS information instead
          if (xReturned != pdPASS) {
            sprintf(tmp, "BATT_TASK: Was unable to send battery percentage to main queue. Error:%d", xReturned);
            send_debug_log_message(tmp);
          }
        } else if (batteryRxMessage.sendTo == BATTERY_MSG_SOC_LOG) {
          xReturned = xQueueSend(logger_recvBattPercentQueue, &batt_info, 0); // TODO: Probably want to send full BMS information instead
          if (xReturned != pdPASS) {
            send_debug_log_message("BATT_TASK: Was unable to send battery information to logger queue");
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
          send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
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
          send_debug_log_message("USB: Unable to send main state response to main_mainStateRespQueue queue.");
        }
        break;

      default:
        break;
      }
    }
  }
}