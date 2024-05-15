#include "sensors.h"
#include "timers.h"

sensor_switches_t switches;
temperature_data_t temperatures;

static tsys01_errors_t tsys01_err;

static long double valve_temperature = 0.0;
static long double amp0_temperature = 0.0;
static long double amp1_temperature = 0.0;
static long double amp2_temperature = 0.0;

xQueueHandle sensor_mainStateQueue;
xQueueHandle sensor_usbWaitQueue;
xQueueHandle sensor_mainStateContinueQueue;

main_state_t s_main_state = STANDBY;

static log_data_message_t log_msg = {
  .data_type = TEMPERATURE_DATA,
  .event_data = NULL,
  .temperature_data = NULL
};

void sensors_task(void * pvParameters) {
  BaseType_t xReturned;
  uint32_t sample_log_index = 0;
  uint32_t sample_log_max = 0;
  tasks_t sensor_task = SENSORS;
  bool cont;

  usb_suspend_req_t sus_req;
  usb_suspend_acpt_t sus_acpt = {
    .task = SENSORS,
    .suspended = true
  };
  usb_suspend_over_t sus_over = {
    .task = SENSORS,
    .over = true
  };

  // Set the last sample based on config
  if (use_default_configuration_parameters) {
    sample_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE); 
  }
  else {
    sample_log_max = (config.logging_rate / config.sample_rate);
  }

  /* Get TSYS01 Calibration Values */
#if I2C_CONNECTED
  tsys01_err = tsys01_getCalibrationValues(valve_zone);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_0);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_1);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_2);
#endif

  for (;;) {
    // Check to see if the main task state has changed
    if (uxQueueMessagesWaiting(sensor_mainStateQueue) > 0) {
      xReturned = xQueueReceive(sensor_mainStateQueue, &s_main_state, 0);
      if (xReturned != pdPASS) {
        printf("BATT_TASK: Unable to receive state change from sensor_mainStateQueue.\n");
      }
      // Respond to main state change
      xReturned = xQueueSend(main_mainStateRespQueue, &sensor_task, 0);
      if (xReturned != pdPASS) {
        printf("USB: Unable to send main state response to main_mainStateRespQueue queue.\n");
      }
      // Wait for Continue
      xReturned = xQueueReceive(sensor_mainStateContinueQueue, &cont, portMAX_DELAY);
      if (xReturned != pdPASS) {
        printf("USB: Unable to recevive continue to sensor_mainStateContinueQueue queue.\n");
      }
    }

    // Check to see if we need to suspend for USB to be enabled
    if (uxQueueMessagesWaiting(sensor_usbWaitQueue) > 0) {
      xReturned = xQueueReceive(sensor_usbWaitQueue, &sus_req, 0) ;
      if (xReturned != pdPASS) {
        printf("SENSORS: Unable to receive usb suspend request from sensor_usbWaitQueue\n");
      }
      // Send Suspend Accepted
      xReturned = xQueueSend(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0); 
      if (xReturned != pdPASS) {
        printf("SENSORS: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue\n");
      }
      printf("SENSORS: Suspending for 15 seconds.\n");
      // Delay Task for 15 Seconds
      vTaskDelay(pdMS_TO_TICKS(USB_SUSPEND_TASKS_TIME));
      // Send Suspend Over
      xReturned = xQueueSend(usb_usbWaitOverQueue, &sus_over, 0); 
      if (xReturned != pdPASS) {
        printf("SENSORS: Unable to send usb suspend over to usb_usbWaitOverQueue\n");
      }
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
    temperatures.valve_zone_temp = readTemp(valve_zone);

    // I2C Read for Amplification Zone 0 
    temperatures.amp0_zone_temp = readTemp(amp_zone_0); 
  
    // I2C Read for Amplification Zone 1 
    temperatures.amp1_zone_temp = readTemp(amp_zone_1);

    // I2C Read for Amplification Zone 2 
    temperatures.amp2_zone_temp = readTemp(amp_zone_2);
    
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
    if (s_main_state == RUNNING) 
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
    if (use_default_configuration_parameters) {
      vTaskDelay(pdMS_TO_TICKS((DEFAULT_SAMPLE_RATE*1000.0) - (12.0 * 4.0) + 1.0)); 
    }
    else {
      vTaskDelay(pdMS_TO_TICKS((config.sample_rate*1000.0) - (12.0 * 4.0) + 1.0));
    }
  }
}

void init_sensors_gpios(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(HAL_INPUT_PIN, NRF_GPIO_PIN_PULLDOWN);
  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(1,7));
  nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(1,7), 1);
  nrf_gpio_cfg_output(SENSORS_EN);
  nrf_gpio_pin_set(SENSORS_EN);
}

long double readTemp(sensor_selection_t sensor) {
  tsys01_errors_t tsys_err;
  long double temperature;

  tsys01_startConversion(sensor);
  vTaskDelay(pdMS_TO_TICKS(12));  // 12ms conversion time
  tsys_err = tsys01_getTemp(sensor, &temperature);
  if (tsys_err != tsys01_success) {
    printf("HEATER_TASK: Unable to read temperature!\n");
  }
  return temperature;
}