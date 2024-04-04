#include "sensors.h"
#include "timers.h"

sensor_switches_t switches;
temperature_data_t temperatures;

static tsys01_errors_t tsys01_err;

static double valve_temperature;
static double amp0_temperature;
static double amp1_temperature;
static double amp2_temperature;


void sensors_task(void * pvParameters) {
  BaseType_t xReturned;

  // TODO: Get Configuration Settings
  tsys01_err = tsys01_getCalibrationValues(valve_zone);

  for (;;) {
    // ADC Read for Optical Sensors
    switches.optical_tiggered = get_optical_triggered();
     
    // GPIO Read for Hall Sensor
    if (nrf_gpio_pin_read(HAL_INPUT_PIN))    
     switches.hal_triggered = false;   
    else                                      
      switches.hal_triggered = true;

    // TODO: I2C Read for Temp Sensor 2
    readTemp(valve_zone, &valve_temperature);
    temperatures.valve_zone_temp = valve_temperature;    

    // I2C Read for Amplification Zone 0 (valve zone enum is acutally amp zone, changing the define was causing hard fault)
    readTemp(amp_zone_0, &amp0_temperature);
    temperatures.amp0_zone_temp = amp0_temperature; 
  
    // I2C Read for Amplification Zone 1 
    readTemp(amp_zone_1, &amp1_temperature);
    temperatures.amp1_zone_temp = amp1_temperature;  

    // I2C Read for Amplification Zone 2 
    readTemp(amp_zone_2, &amp2_temperature);
    temperatures.amp2_zone_temp = amp2_temperature;
    

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

void readTemp(sensor_selection_t sensor, double * temperature) {
  tsys01_errors_t tsys_err;

  tsys01_startConversion(sensor);
  vTaskDelay(12);  // 12ms conversion time
  tsys_err = tsys01_getTemp(sensor, &temperature);
  if (tsys_err != tsys01_success) {
    printf("HEATER_TASK: Unable to read temperature!\n");
  }
}