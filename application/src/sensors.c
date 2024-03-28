#include "sensors.h"

sensor_switches_t switches;
temperature_data_t temperatures;

void tsys01_callback(tsys01_errors_t err, tsys01_callback_data_t cb_data);

static tsys01_errors_t tsys01_err;
static tsys01_callback_data_t tsys01_cb_data;
tsys01_opDoneCallback_t savedUserCallback = tsys01_callback;
bool called = false;

void sensors_task(void * pvParameters) {
  BaseType_t xReturned;

  // TODO: Get Configuration Settings

  for (;;) {
    // ADC Read for Optical Sensors
    switches.optical_tiggered = get_optical_triggered();
     
    // GPIO Read for Hall Sensor
    if (nrf_gpio_pin_read(HAL_INPUT_PIN))
     switches.hal_triggered = false;   // Temp
    else
     switches.hal_triggered = true;

    // TODO: I2C Read for Temp Sensor 1
    tsys01_startConversion(valve_zone, savedUserCallback);
    vTaskDelay(12);  // 12ms conversion time
    tsys01_getTemp(valve_zone, savedUserCallback);
    while(!called);
    temperatures.valve_zone_temp = tsys01_cb_data.data.temperature;  // Temp

    // TODO: I2C Read for Temp Sensor 2
    temperatures.amplification_zone_temp = 26.5;    // Temp

    // Put Switch Data into queue
    xReturned = xQueueSend(main_switchQueue, (void *)&switches, 0);
    if (xReturned != pdPASS) {
      printf("SENSORS_TASK: Unable to send switch data in main_switchQueue.\n");
    }

    // Put Temperature Data into queue
    xReturned = xQueueSend(heater_temperatureDataQueue, (void *)&temperatures, 0);
    if (xReturned != pdPASS) {
      printf("SENSORS_TASK: Unable to send temperature data in heater_temperatureDataQueue. Error: %d\n", xReturned);
    }

    // TODO: Add in sending of temperature data to Log Data queue if: index = (log rate * sample rate) - 1

    vTaskDelay(500);
  }
}

void init_sensors_gpios(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(HAL_INPUT_PIN, NRF_GPIO_PIN_PULLDOWN);
}

void tsys01_callback(tsys01_errors_t err, tsys01_callback_data_t cb_data) {
  called = true;
  tsys01_err = err;
  tsys01_cb_data = cb_data;
}