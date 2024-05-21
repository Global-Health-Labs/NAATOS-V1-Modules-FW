#include "watchdog.h"
#include "boards.h"
#include "naatos_queues.h""
static nrfx_wdt_channel_id m_channel_id;

xQueueHandle watchdog_rxTimesQueue;

void wdt_event_handler(void) {
    // This function is called if the WDT event occurs (optional)
}


void watchdog_init(void) {
  uint32_t err_code = NRF_SUCCESS;

  // Configure the WDT with a timeout of 5000 ms
  nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
  config.behaviour = NRF_WDT_BEHAVIOUR_RUN_SLEEP_HALT; // Ensure this behaviour is supported
  config.reload_value = 10000; // 10 seconds
  err_code = nrf_drv_wdt_init(&config, wdt_event_handler);
  APP_ERROR_CHECK(err_code);

  err_code = nrf_drv_wdt_channel_alloc(&m_channel_id);
  APP_ERROR_CHECK(err_code);

  // Enable the WDT
  nrf_drv_wdt_enable();
}


void wdtFeedTask(void * pvParameters) {
/* We can handle this a few ways. We can just kick the dog in a task on an interval. That would support general system lockup 
 * We could also have all tasks we want to track update system time variable. If the time is too old then it means we dont kick dog
 * All tasks must be updated for the dog to be reset.
 *
 *
 *
 **/
  (void) pvParameters;

 watchdog_time_update_t recv_req;

  while (true) {

    //if msg in queue
    // Set watchdog time and or set valid flag
    if( xQueueReceive( watchdog_rxTimesQueue,
                         &recv_req,
                         ( TickType_t ) 0 ) == pdPASS )
      {
         /* *pxRxedPointer now points to xMessage. */
         // reset specific task counter to 0
         // if set to invalid then we dont have to compare times
      }

    // Consider all valid tasks for timeout. This means if their  tick has been incremented past the max  value we dont kick the dog
    // If task not valid skip and dont consider it for WDT 

    // if no tasks are valid just kick the dog
    // if one of the tasks is missed then dont allow the dog to be kicked

    // Feed the watchdog
    nrf_drv_wdt_channel_feed(m_channel_id);

    // Delay for a period shorter than the watchdog timeout
    vTaskDelay(1000); 
  }

}