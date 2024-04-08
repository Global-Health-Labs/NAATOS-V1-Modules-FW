#include "sensors.h"
#include "timers.h"

sensor_switches_t switches;
temperature_data_t temperatures;

static tsys01_errors_t tsys01_err;

static double valve_temperature;
static double amp0_temperature;
static double amp1_temperature;
static double amp2_temperature;

xQueueHandle sensor_mainStateQueue;
main_state_t s_main_state = STANDBY;

static log_data_message_t log_msg = {
  .data_type = TEMPERATURE_DATA,
  .event_data = NULL,
  .temperature_data = NULL
};

void sensors_task(void * pvParameters) {
  BaseType_t xReturned;
  uint32_t sample_log_index = 0;
  uint32_t sample_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE); // TODO: Update to be based on config.txt

  // TODO: Get Configuration Settings

  /* Get TSYS01 Calibration Values */
#if I2C_CONNECTED
  tsys01_err = tsys01_getCalibrationValues(valve_zone);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_0);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_1);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_2);
#endif

  for (;;) {
    // Get Start Time in ms, testing for timings
    //uint32_t time = pdTICKS_TO_MS(xTaskGetTickCount());
    //printf("Start Time: %dms \n", time);

    // Check to see if the main task state has changed
    if (uxQueueMessagesWaiting(sensor_mainStateQueue) > 0) {
      xReturned = xQueueReceive(sensor_mainStateQueue, &s_main_state, 0);
      if (xReturned != pdPASS) {
        printf("BATT_TASK: Unable to receive state change from sensor_mainStateQueue.\n");
      }
      continue;
    }
   
 #if (GO_STRAIGHT_TO_RUNNING)
    switches.optical_tiggered = true;
    switches.hal_triggered = true;
 #else 
    // ADC Read for Optical Sensors
    bool prev = switches.optical_tiggered;
    switches.optical_tiggered = get_optical_triggered();
    if (prev != switches.optical_tiggered) {
      if (switches.optical_tiggered) {
        printf("Optical sensor triggered!\n");
      }
      else {
        printf("Optical sensor no longer triggered!\n");
      }
    }
    // GPIO Read for Hall Sensor
    prev = switches.hal_triggered;
    if (nrf_gpio_pin_read(HAL_INPUT_PIN))    
     switches.hal_triggered = false;   
    else                                      
      switches.hal_triggered = true;
    if (prev != switches.hal_triggered) {
      if (switches.hal_triggered) {
        printf("Hal sensor triggered!\n");
      }
      else {
        printf("Hal sensor no longer triggered!\n");
      }
    }
 #endif

 #if I2C_CONNECTED
    // I2C Read for Valve Zone
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
#else
    // Set temps to their setpoints if i2c is not connected
    temperatures.valve_zone_temp = 85;
    temperatures.amp0_zone_temp = 65;
    temperatures.amp1_zone_temp = 65;
    temperatures.amp2_zone_temp = 65;
    vTaskDelay(pdMS_TO_TICKS(12 * 4)); // Simulate 12ms delay for each reading
#endif

    // Put Switch Data into queue
    xReturned = xQueueSend(main_switchQueue, (void *)&switches, 0);
    if (xReturned != pdPASS) {
      printf("SENSORS_TASK: Unable to send switch data in main_switchQueue.\n");
    }
    
    // Only send temperature data when we are running 
    if (s_main_state == RUNNING) {
      // Put Temperature Data into queue
      xReturned = xQueueSend(heater_temperatureDataQueue, (void *)&temperatures, 0);
      if (xReturned != pdPASS) {
        printf("SENSORS_TASK: Unable to send temperature data in heater_temperatureDataQueue. Error: %d\n", xReturned);
      }
    }

    // Send temperature data to Log Data queue
    if (switches.hal_triggered && switches.optical_tiggered) 
      sample_log_index++;
    if (sample_log_index >= sample_log_max) {
      log_msg.temperature_data = temperatures;
      xReturned = xQueueSend(logger_logMessageQueue, (void *)&log_msg, 0);
      if (xReturned != pdPASS) {
        printf("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.\n");
      }
      sample_log_index = 0;
    }
    
    // Delay based on the given sample rate
    // Remove 48 ms delay when running for temperature read delays
    vTaskDelay(pdMS_TO_TICKS((DEFAULT_SAMPLE_RATE*1000) - (12 * 4) + 1)); // TODO: Update sample rate to be based on config file
    
    // Get Stop Time in ms, testing for timings
    //time = pdTICKS_TO_MS(xTaskGetTickCount());
    //printf("End Time: %dms\n", time);
  }
}

void init_sensors_gpios(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(HAL_INPUT_PIN, NRF_GPIO_PIN_PULLDOWN);
}

void readTemp(sensor_selection_t sensor, double * temperature) {
  tsys01_errors_t tsys_err;

  tsys01_startConversion(sensor);
  vTaskDelay(pdMS_TO_TICKS(12));  // 12ms conversion time
  tsys_err = tsys01_getTemp(sensor, &temperature);
  if (tsys_err != tsys01_success) {
    printf("HEATER_TASK: Unable to read temperature!\n");
  }
}