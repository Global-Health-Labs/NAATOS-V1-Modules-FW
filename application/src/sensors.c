#include "sensors.h"
#include "timers.h"

void sensorCollection(void) ;

const bool pwm_req = true;

sensor_switches_t switches;
temperature_data_t temperatures = {
  .amp0_zone_pwm = 0,
  .amp0_zone_temp = 0,
  .amp1_zone_pwm = 0,
  .amp1_zone_temp = 0,
  .amp2_zone_pwm = 0,
  .amp2_zone_temp = 0,
  .valve_zone_pwm = 0,
  .valve_zone_temp = 0
};

temperature_pwm_data_t pwm_data = {
  .valve_zone_pwm = 0,
  .amp0_zone_pwm = 0,
  .amp1_zone_pwm = 0,
  .amp2_zone_pwm = 0
};

static tsys01_errors_t tsys01_err;

static long double valve_temperature = 0.0;
static long double amp0_temperature = 0.0;
static long double amp1_temperature = 0.0;
static long double amp2_temperature = 0.0;

xQueueHandle sensorRxQueue;

TimerHandle_t sensorTimer;

bool heaterRunning = false;
bool usb_suspend = false;
uint32_t sample_log_index = 0;
uint32_t sample_log_max = 0;

static log_data_message_t log_msg = {
  .data_type = TEMPERATURE_DATA,
  .event_data = NULL,
  .temperature_data = NULL
};

void vSensorTimerCallback( TimerHandle_t xTimer ) {
  BaseType_t xReturned;
  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_TIMER_EVENT;
  if(!usb_suspend){
    xReturned = xQueueSend(sensorRxQueue, &msg, 0);
    if (xReturned != pdPASS) {
      printf("Sensor: Unable to send timer update to sensorRxQueue queue.\n");
    }
  }
}

void startTimer(void) {
  TickType_t sampleRateTicks;

  if (use_default_configuration_parameters) {
    sampleRateTicks = pdMS_TO_TICKS((DEFAULT_SAMPLE_RATE*1000.0) - (12.0 * 4.0) + 1.0); 
  }
  else {
    sampleRateTicks = pdMS_TO_TICKS((config.sample_rate*1000.0) - (12.0 * 4.0) + 1.0);
  }

  if(xTimerChangePeriod(sensorTimer, sampleRateTicks, 100) != pdPASS) {
    printf("Cannot change period of sensor timer. \n");
  }

  if( xTimerStart( sensorTimer, 0 ) != pdPASS ){
     printf("Failed to start sensor timer. \n");
  }
}

void stopTimer(void) {
  if( xTimerStop(sensorTimer, 100) != pdPASS ){
    printf("Failed to stop sensor timer. \n");
  }
}

void sensors_task(void * pvParameters) {
  BaseType_t xReturned;
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

  SensorRxQueueMsg_t sensorRxMessage;

  TickType_t sampleRateTicks;

  if (use_default_configuration_parameters) {
    sampleRateTicks = pdMS_TO_TICKS((DEFAULT_SAMPLE_RATE*1000.0) - (12.0 * 4.0) + 1.0); 
  }
  else {
    sampleRateTicks = pdMS_TO_TICKS((config.sample_rate*1000.0) - (12.0 * 4.0) + 1.0);
  }
  sensorTimer = xTimerCreate ("SensorTimer", sampleRateTicks, pdTRUE, (void*)0,vSensorTimerCallback);

  //setup our message timer for sensor collection
  for (;;) {
    // one queue the rxs messages from other threads

    // if message rx then handle whatever it is
      // if msg = heater start/stop
        // send response to sensorConfQueue
      // if msg = usb suspend 
        // return response for suspend accepted
        // take note of time do not execute non watchdog messages or tasks until usb is past that 15 second timeout
    // if delay time passed since last time we wrote down the time take note of time now and run all the sensor collection stuff

    xReturned = xQueueReceive(sensorRxQueue, &sensorRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("Unable to Rx data to sensor queue\n");
    } else {
      switch(sensorRxMessage.type) {
        case SENSOR_MSG_HEATER_STATE: {
          heaterRunning = sensorRxMessage.heaterRunning;

          // Respond to heater change
          xReturned = xQueueSend(heater_sensorConfQueue, &heaterRunning, 0);
          if (xReturned != pdPASS) {
            printf("USB: Unable to send main state response to main_mainStateRespQueue queue.\n");
          }
          break;
        }

        case SENSOR_MSG_USB_SUSPEND:
          // Send Suspend Accepted
          xReturned = xQueueSend(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0); 
          if (xReturned != pdPASS) {
            printf("SENSORS: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue\n");
          }

          usb_suspend = true;

          // Delay Task for 15 Seconds
          vTaskDelay(pdMS_TO_TICKS(USB_SUSPEND_TASKS_TIME));
          // Send Suspend Over
          xReturned = xQueueSend(usb_usbWaitOverQueue, &sus_over, 0); 
          if (xReturned != pdPASS) {
            printf("SENSORS: Unable to send usb suspend over to usb_usbWaitOverQueue\n");
          }
          usb_suspend = false;
        break;

        case SENSOR_MSG_PWM_RESPONSE:
          // Update PWM in temerature data
          log_msg.temperature_data.amp0_zone_temp = temperatures.amp0_zone_temp;
          log_msg.temperature_data.amp1_zone_temp = temperatures.amp1_zone_temp;
          log_msg.temperature_data.amp2_zone_temp = temperatures.amp2_zone_temp;
          log_msg.temperature_data.valve_zone_temp = temperatures.valve_zone_temp;
          log_msg.temperature_data.amp0_zone_pwm = pwm_data.amp0_zone_pwm;
          log_msg.temperature_data.amp1_zone_pwm = pwm_data.amp1_zone_pwm;
          log_msg.temperature_data.amp2_zone_pwm = pwm_data.amp1_zone_pwm;
          log_msg.temperature_data.valve_zone_pwm = pwm_data.valve_zone_pwm;
          // Send the Log message
          xReturned = xQueueSend(logger_logMessageQueue, (void *)&log_msg, 0);
          if (xReturned != pdPASS) {
            printf("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.\n");
          }
          sample_log_index = 0;
        break;

        case SENSOR_MSG_TIMER_EVENT:{
          sensorCollection();
        }
          
        break;

        case SENSOR_MSG_SLEEP:
          if(xTimerIsTimerActive(sensorTimer) == pdTRUE) {
            stopTimer();
          }

          //send to main queue that we are asleep
        break;

        case SENSOR_MSG_WAKEUP:
          if(xTimerIsTimerActive(sensorTimer) == pdFALSE) {
            startTimer();
          }
        break;

        case CONFIG_UPDATED:

        break;

        default:
        break;
      }
    }
  }
}

void sensorCollection(void) {
     BaseType_t xReturned;

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

    // Put Switch Data into queue
    xReturned = xQueueSend(main_switchQueue, (void *)&switches, 0);
    if (xReturned != pdPASS) {
      printf("SENSORS_TASK: Unable to send switch data in main_switchQueue.\n");
    }
    // Send temperature data to Log Data queue
    if (heaterRunning) { 
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
      xReturned = xQueueSend(heater_temperatureDataQueue, (void *)&temperatures, 0);
      if (xReturned != pdPASS) {
        printf("SENSORS_TASK: Unable to send temperature data in heater_temperatureDataQueue. Error: %d\n", xReturned);
      }
      sample_log_index++;
      if (sample_log_index >= sample_log_max) {
        // Request PWM from heater
        xReturned = xQueueSend(heater_pwmReqQueue, &pwm_req, 0);
        if (xReturned != pdPASS) {
          printf("SENSOR_TASK: Unable to send PWM request to heater_pwmReqQueue queue.\n");
        }
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

  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(1,3));
  //nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(1,3), 1);
  nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(1,3));
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