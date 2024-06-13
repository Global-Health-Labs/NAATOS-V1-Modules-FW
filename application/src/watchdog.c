#include "watchdog.h"
#include "boards.h"
#include "naatos_queues.h"
static nrfx_wdt_channel_id m_channel_id;

xQueueHandle watchdog_rxTimesQueue;

void wdt_event_handler(void) {
  // This function is called if the WDT event occurs (optional)
}

void watchdog_init(void) {
  uint32_t err_code = NRF_SUCCESS;

  nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
  config.behaviour = NRF_WDT_BEHAVIOUR_RUN_SLEEP_HALT; // Ensure this behaviour is supported
  config.reload_value = 6000;                          // 12 seconds
  err_code = nrf_drv_wdt_init(&config, wdt_event_handler);
  APP_ERROR_CHECK(err_code);

  err_code = nrf_drv_wdt_channel_alloc(&m_channel_id);
  APP_ERROR_CHECK(err_code);

  // Enable the WDT
  nrf_drv_wdt_enable();
}

#define WDT_TASK_DELAY 500 // msec
#define MAX_WDT_TASK_TIMEOUT (3 / (WDT_TASK_DELAY * 0.001)) // 3 sec

void wdtFeedTask(void *pvParameters) {
  /* We can handle this a few ways. We can just kick the dog in a task on an interval. That would support general system lockup 
 * We could also have all tasks we want to track update system time variable. If the time is too old then it means we dont kick dog
 * All tasks must be updated for the dog to be reset.
 *
 *
 *
 **/
  (void)pvParameters;
  watchdog_time_update_t recv_req;

  bool heaterValid = false;
  int heaterTicks = 0;

  bool batteryValid = false;
  int batteryTicks = 0;

  bool mainValid = false;
  int mainTicks = 0;

#if NAATOS_ENABLE_WATCHDOG
  watchdog_init();
#endif

  while (true) {

    //if msg in queue
    // Set watchdog time and or set valid flag
    while (xQueueReceive(watchdog_rxTimesQueue,
               &recv_req,
               (TickType_t)0) == pdPASS) {
      /* *pxRxedPointer now points to xMessage. */
      // reset specific task counter to 0
      // if set to invalid then we dont have to compare times

      switch (recv_req.taskName) {
      case BATTERY:
        batteryValid = recv_req.valid;
        batteryTicks = 0;
        break;

      case HEATER:
        heaterValid = recv_req.valid;
        heaterTicks = 0;
        break;

      case MAIN:
        mainValid = recv_req.valid;
        mainTicks = 0;
        break;

      default:
        break;
      }
    }
#if NAATOS_ENABLE_WATCHDOG
    if (heaterValid) {
      heaterTicks++;
    }

    if (batteryValid) {
      batteryTicks++;
    }

    if (mainValid) {
      mainTicks++;
    }

    if (heaterTicks < MAX_WDT_TASK_TIMEOUT && batteryTicks < MAX_WDT_TASK_TIMEOUT && mainTicks < MAX_WDT_TASK_TIMEOUT) {
      nrf_drv_wdt_channel_feed(m_channel_id);
    }
#endif
    // Delay for a period shorter than the watchdog timeout
    vTaskDelay(WDT_TASK_DELAY);
  }
}

void sendWatchdogKickFromTask(tasks_t task, bool valid) {
  BaseType_t xReturned;
  watchdog_time_update_t wdtUpdate;
  wdtUpdate.taskName = task;
  wdtUpdate.valid = valid;

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
  }
}