#include "watchdog.h"
#include "boards.h"
static nrfx_wdt_channel_id m_channel_id;

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

  while (true) {
      // Feed the watchdog
      nrf_drv_wdt_channel_feed(m_channel_id);

      // Delay for a period shorter than the watchdog timeout
      vTaskDelay(1000); // Adjust the delay as necessary
  }

}