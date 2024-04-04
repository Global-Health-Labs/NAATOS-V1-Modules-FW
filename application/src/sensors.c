#include "sensors.h"
#include "timers.h"

sensor_switches_t switches;
temperature_data_t temperatures;

static tsys01_errors_t tsys01_err;

static double valve_temperature;
static double amp1_temperature;
static double amp2_temperature;
static double amp3_temperature;


void sensors_task(void * pvParameters) {
  BaseType_t xReturned;
  tsys01_errors_t tsys_err;

  // TODO: Get Configuration Settings
  tsys01_err = tsys01_getCalibrationValues(valve_zone);

  for (;;) {
    // ADC Read for Optical Sensors
    //switches.optical_tiggered = get_optical_triggered(); PUT BACK
    switches.optical_tiggered = true;
     
    // GPIO Read for Hall Sensor
    //if (nrf_gpio_pin_read(HAL_INPUT_PIN))         PUT BACK
    // switches.hal_triggered = false;   // Temp    PUT BACK
    //else                                          PUT BACK
    switches.hal_triggered = true;

    // I2C Read for Amplification Zone 1 (valve zone enum is acutally amp zone, changing the define was causing hard fault)
    tsys01_startConversion(valve_zone);
    vTaskDelay(12);  // 12ms conversion time
    tsys_err = tsys01_getTemp(valve_zone, &amp1_temperature);
    if (tsys_err != tsys01_success) {
      printf("HEATER_TASK: Unable to read temperature from valve zone!\n");
    }
    temperatures.amplification_zone_temp = amp1_temperature;  // Temp

    // TODO: I2C Read for Temp Sensor 2
    temperatures.valve_zone_temp= 26.5;    // Temp

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

    vTaskDelay(100);
  }
}

void init_sensors_gpios(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(HAL_INPUT_PIN, NRF_GPIO_PIN_PULLDOWN);
}
