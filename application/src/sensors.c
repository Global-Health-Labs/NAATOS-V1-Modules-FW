#include "sensors.h"
#include "motor.h"
#include "nrf_drv_timer.h"
#include "timers.h"
#include "logger/logger.h"

void sensorCollection(void);

sensor_switches_t switches;
temperature_data_t temperatures = {
    .heat_zone_0_pwm = 0,
    .heat_zone_0_temp = 0,
    .heat_zone_1_pwm = 0,
    .heat_zone_1_temp = 0,
    .heat_zone_2_pwm = 0,
    .heat_zone_2_temp = 0,
    .heat_zone_3_pwm = 0,
    .heat_zone_3_temp = 0};

temperature_pwm_data_t pwm_data = {
    .heat_zone_0_pwm = 0,
    .heat_zone_1_pwm = 0,
    .heat_zone_2_pwm = 0,
    .heat_zone_3_pwm = 0};


static tsys01_errors_t tsys01_err;

static long double valve_temperature = 0.0;
static long double amp0_temperature = 0.0;
static long double amp1_temperature = 0.0;
static long double amp2_temperature = 0.0;

xQueueHandle sensorRxQueue;

TimerHandle_t sensorTempTimer;
TickType_t sampleRateTicks;

bool heaterRunning = false;
bool usb_suspend = false;
uint32_t sample_log_index = 0;
uint32_t sample_log_max = 0;

static log_data_message_t log_msg = {
    .data_type = TEMPERATURE_DATA,
    .event_data = NULL,
    .temperature_data = NULL};

void updateSampleLogMax(void);

void vSensorTempTimerCallback(TimerHandle_t xTimer) {
  BaseType_t xReturned;
  SensorRxQueueMsg_t msg;

  msg.type = SENSOR_MSG_TIMER_TEMP_EVENT;
  if (!usb_suspend) {
    xReturned = xQueueSend(sensorRxQueue, &msg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("Sensor: Unable to send timer update to sensorRxQueue queue. from temp callback\r\n");
    }
  }
}

void startSensorTempTimer(void) {
  TickType_t sampleRateTicks;

  if (use_default_configuration_parameters) {
    sampleRateTicks = pdMS_TO_TICKS((DEFAULT_SAMPLE_RATE * 1000.0));
  } else {
    sampleRateTicks = pdMS_TO_TICKS((config.sample_rate * 1000.0));
  }

  if (xTimerChangePeriod(sensorTempTimer, sampleRateTicks, 100) != pdPASS) {
    send_debug_log_message("Cannot change period of sensor timer. \n");
  }

  if (xTimerStart(sensorTempTimer, 0) != pdPASS) {
    send_debug_log_message("Failed to start sensor timer. \r\n");
  }
}

void stopSensorTempTimer(void) {
  if (xTimerStop(sensorTempTimer, 100) != pdPASS) {
    send_debug_log_message("Failed to stop sensor timer. \r\n");
  }
}

void samplePrepSensorTaskSetup(void) {
  /* Get TSYS01 Calibration Values */
  tsys01_err = tsys01_getCalibrationValues(heat_zone_3);
  if (tsys01_err == tsys01_i2c_error) {
    NVIC_SystemReset();
    asm volatile("nop");
  }
  sensorTempTimer = xTimerCreate("SensorTempTimer", sampleRateTicks, pdTRUE, (void *)0, vSensorTempTimerCallback);
}

void powerModuleSensorTaskSetup(void) {
  tsys01_err = tsys01_getCalibrationValues(heat_zone_0);
  tsys01_err = tsys01_getCalibrationValues(heat_zone_2);
  sensorTempTimer = xTimerCreate("SensorTempTimer", sampleRateTicks, pdTRUE, (void *)0, vSensorTempTimerCallback);
}

void sensors_task(void *pvParameters) {
  BaseType_t xReturned;
  tasks_t sensor_task = SENSORS;
  bool cont;

  usb_suspend_req_t sus_req;
  usb_suspend_acpt_t sus_acpt = {
      .task = SENSORS,
      .suspended = true};
  usb_suspend_over_t sus_over = {
      .task = SENSORS,
      .over = true};

  // Set the last sample based on config
  if (use_default_configuration_parameters) {
    sample_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
  } else {
    sample_log_max = (config.logging_rate / config.sample_rate);
  }

  SensorRxQueueMsg_t sensorRxMessage;

  if (use_default_configuration_parameters) {
    sampleRateTicks = pdMS_TO_TICKS((DEFAULT_SAMPLE_RATE * 1000.0) - 12.0);
  } else {
    sampleRateTicks = pdMS_TO_TICKS((config.sample_rate * 1000.0) - 12.0);
  }

#ifdef SAMPLE_PREP_BOARD
  samplePrepSensorTaskSetup();
#else
  powerModuleSensorTaskSetup();
#endif
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
      send_debug_log_message("Unable to Rx data to sensor queue\r\n");
    } else {
      switch (sensorRxMessage.type) {
      case SENSOR_MSG_HEATER_STATE: {
        heaterRunning = sensorRxMessage.heaterRunning;

        HeaterRxQueueMsg_t heaterMsg = {
            .type = HEATER_MSG_SENSOR_CONFIRM,
            .heaterRunning = heaterRunning};

        // Respond to heater change
        xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("USB: Unable to send main state response to main_mainStateRespQueue queue.\r\n");
        }
        break;
      }

      case SENSOR_MSG_USB_SUSPEND: {
        // Send Suspend Accepted
        xReturned = xQueueSend(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("SENSORS: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue\r\n");
        }

        usb_suspend = true;

        // Delay Task for 15 Seconds
        vTaskDelay(pdMS_TO_TICKS(USB_SUSPEND_TASKS_TIME));
        // Send Suspend Over
        xReturned = xQueueSend(usb_usbWaitOverQueue, &sus_over, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("SENSORS: Unable to send usb suspend over to usb_usbWaitOverQueue\r\n");
        }
        usb_suspend = false;
        break;
      }

      case SENSOR_MSG_TIMER_TEMP_EVENT: {
#ifdef SAMPLE_PREP_BOARD
        runSamplePrepSensorCollection();
#else
        runPowerModuleSensorCollection();
#endif
        break;
      }
      case SENSOR_MSG_SLEEP: {
        handleSensorSleep();
        //send to main queue that we are asleep
        break;
      }

      case SENSOR_MSG_WAKEUP: {
        handleSensorWake();
        break;
      }

      case CONFIG_UPDATED: {
        handleConfigUpdated();
        break;
      }

      default:
        break;
      }
    }
  }
}

void handleSensorSleep(void) {
  if (xTimerIsTimerActive(sensorTempTimer) == pdTRUE) {
    stopSensorTempTimer();
  }
}

void handleSensorWake(void) {
  if (xTimerIsTimerActive(sensorTempTimer) == pdFALSE) {
    startSensorTempTimer();
  }
}

void handleConfigUpdated(void) {
  if (xTimerIsTimerActive(sensorTempTimer) == pdTRUE) {
    stopSensorTempTimer();
  }
  updateSampleLogMax();

  if (xTimerIsTimerActive(sensorTempTimer) == pdFALSE) {
    startSensorTempTimer();
  }
}

void updateSampleLogMax(void) {
  // Set the last sample based on config
  if (use_default_configuration_parameters) {
    sample_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
  } else {
    sample_log_max = (config.logging_rate / config.sample_rate);
  }
}

void runPowerModuleSensorCollection(void) {
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterMsg;

  // ADC Read for Optical Sensors
  bool prev = switches.optical_tiggered;
  switches.optical_tiggered = get_optical_triggered();
  if (prev != switches.optical_tiggered) {
    if (switches.optical_tiggered) {
      send_debug_log_message("Optical sensor triggered!\r\n");
    } else {
      send_debug_log_message("Optical sensor no longer triggered!\r\n");
    }
  }
  // GPIO Read for Hall Sensor
  // No Hal Sensor
  switches.hal_triggered = false;

  // Put Switch Data into queue
  xReturned = xQueueSend(main_switchQueue, (void *)&switches, 10);
  if (xReturned != pdPASS) {
    send_debug_log_message("SENSORS_TASK: Unable to send switch data in main_switchQueue.\r\n");
  }
  // Send temperature data to Log Data queue
  if (heaterRunning) {
    bool readTempSuccess = false;
    bool dis = true;
    heaterMsg.readTempFailed = false;
    // I2C Read for Heater Zone 0
    dis = disable_valve_boost();
    while (!dis) { // Reset the Valve Boost
      nrf_gpio_pin_clear(VALVE_PWR_EN); 
      nrf_gpio_pin_set(VALVE_PWR_EN); 
      valve_zone_set_6v();
      dis = disable_valve_boost();
    }
    readTempSuccess = readTemp(heat_zone_0, &temperatures.heat_zone_0_temp);
    enable_valve_boost();
    if (!readTempSuccess) {
      heaterMsg.readTempFailed = true;
    }

    // I2C Read for Heater Zone 1, Not Connected
    temperatures.heat_zone_1_temp = 0.0;

    // I2C Read for Heater Zone 2
    dis = disable_valve_boost();
    while (!dis) {  // Reset the Valve Boost
      nrf_gpio_pin_clear(VALVE_PWR_EN); 
      nrf_gpio_pin_set(VALVE_PWR_EN); 
      valve_zone_set_6v();
      dis = disable_valve_boost();
    }
    readTempSuccess = readTemp(heat_zone_2, &temperatures.heat_zone_2_temp);
    enable_valve_boost();
    if (!readTempSuccess) {
      heaterMsg.readTempFailed = true;
    }

    // I2C Read for Heater Zone 3, Not Connected
    temperatures.heat_zone_3_temp = 0.0;

    heaterMsg.type = HEATER_MSG_TEMPERATURE_DATA;
    heaterMsg.tempData = temperatures;

    xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
    if (xReturned != pdPASS) {
      char errorString[100];
      sprintf(errorString, "SENSORS_TASK: Unable to send temperature data in heaterRxQueue. Error: %d\r\n", xReturned);
      send_debug_log_message(errorString);
    }
  }
}

void runSamplePrepSensorCollection(void) {
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterMsg;
  bool readTempSuccess = false;
  heaterMsg.readTempFailed = false;
  bool prev = false;
  int consecutive_failures = 0; // Counter for consecutive failures

  // GPIO Read for Hall Sensor
  prev = switches.hal_triggered;
  if (get_hal_triggered()) {
    switches.hal_triggered = true;
  } else {
    switches.hal_triggered = false;
  }
  if (prev != switches.hal_triggered) {
    if (switches.hal_triggered) {
      send_debug_log_message("Hal sensor triggered!\r\n");
    } else {
      send_debug_log_message("Hal sensor no longer triggered!\r\n");
    }
  }

  // ADC Read for Optical Sensors
  // No optical sensor, force true
  switches.optical_tiggered = true;

  // Array to store temperature samples
  float temp_samples[6];
  float temp;

  // Collect 10 temperature samples
  for (int i = 0; i < 6; i++) {
      bool readTempSuccess = readTemp(heat_zone_3, &temp);
      if (readTempSuccess) {
          temp_samples[i] = temp;
          consecutive_failures = 0; // Reset the failure counter on success
      } else {
          consecutive_failures++; // Increment failure counter
          send_debug_log_message("Failed to read temperature sample!\r\n");
          if (consecutive_failures > 4) {
              heaterMsg.readTempFailed = true;
              send_debug_log_message("Temperature reading failed more than 6 times in a row!\r\n");
              return; // Exit early if we have more than 6 consecutive failures
          }
      }
      //vTaskDelay(pdMS_TO_TICKS(10)); // Optional delay between samples
  }

  // Sort the temperature samples (bubble sort for simplicity)
  for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5 - i; j++) {
          if (temp_samples[j] > temp_samples[j + 1]) {
              float temp_swap = temp_samples[j];
              temp_samples[j] = temp_samples[j + 1];
              temp_samples[j + 1] = temp_swap;
          }
      }
  }

  // Calculate the average of the middle 3 values (indexes 3, 4, 5 after sorting)
  float avg_temp = (temp_samples[2] + temp_samples[3]) / 2.0;

  // Update the temperature reading with the averaged value
  temperatures.heat_zone_3_temp = avg_temp;

  // Put Switch Data into queue
  xReturned = xQueueSend(main_switchQueue, (void *)&switches, 10);
  if (xReturned != pdPASS) {
    send_debug_log_message("SENSORS_TASK: Unable to send switch data in main_switchQueue.\r\n");
  }

  if (heaterRunning) {

    heaterMsg.type = HEATER_MSG_TEMPERATURE_DATA;
    heaterMsg.tempData = temperatures;

    xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
    if (xReturned != pdPASS) {
      char errorString[100];
      sprintf(errorString, "SENSORS_TASK: Unable to send temperature data in heaterRxQueue. Error: %d\r\n", xReturned);
      send_debug_log_message(errorString);
    }
  }
}

bool readTemp(sensor_selection_t sensor, float *temperature) {
  int i2cRetry = 0;
  tsys01_errors_t tsys_err;

  tsys_err = tsys01_startConversion(sensor);
  if (tsys_err != tsys01_success) {
    send_debug_log_message("HEATER_TASK: Unable to trigger temperature conversion!\r\n");
    return false;
  } else {
    vTaskDelay(pdMS_TO_TICKS(12)); // 12ms conversion time
    tsys_err = tsys01_getTemp(sensor, temperature);
    if (tsys_err != tsys01_success) {
      send_debug_log_message("HEATER_TASK: Unable to read temperature! 1\r\n");
      return false;
    }
    return true;
  }
}