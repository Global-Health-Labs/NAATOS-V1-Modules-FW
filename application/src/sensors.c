#include "sensors.h"

sensor_switches_t switches;
temperature_data_t temperatures;

void sensors_task(void * pvParameters) {
  BaseType_t xReturned;

  // Initalize GPIOs
  init_gpios();

  // TODO: Get Configuration Settings
  for (;;) {
    // TODO: ADC Read for Optical Sensors
    switches.optical_tiggered = false;  // Temp
    // GPIO Read for Hall Sensor
    if (nrf_gpio_pin_read(HAL_INPUT_PIN))
     switches.hal_triggered = false;   // Temp
    else
     switches.hal_triggered = true;
    // TODO: I2C Read for Temp Sensor 1
    temperatures.amplification_zone_temp = 26.7;  // Temp
    // TODO: I2C Read for Temp Sensor 2
    temperatures.valve_zone_temp = 26.5;    // Temp
    // Put Switch Data into queue
    xReturned = xQueueSend(main_switchQueue, (void *)&switches, 0);
    if (xReturned != pdPASS) {
      printf("SENSORS_TASK: Unable to send switch data in main_switchQueue.\n");
    }
    // Put Temperature Data into queue
    xReturned = xQueueSend(heater_temperatureDataQueue, (void *)&temperatures, 0);
    if (xReturned != pdPASS) {
      printf("SENSORS_TASK: Unable to send temperature data in heater_temperatureDataQueue.\n");
    }

    // TODO: Add in sending of temperature data to Log Data queue if: index = (log rate * sample rate) - 1

    vTaskDelay(500);
  }
}

void init_gpios(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(HAL_INPUT_PIN, NRF_GPIO_PIN_PULLDOWN);
}