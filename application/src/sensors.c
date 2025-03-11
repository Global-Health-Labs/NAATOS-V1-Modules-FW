#include "sensors.h"
#include "motor.h"
#include "nrf_drv_timer.h"
#include "timers.h"
#include "logger/logger.h"

void sensorCollection(void);

sensor_switches_t switches;
temperature_data_t temperatures = {
#ifdef SAMPLE_PREP_BOARD
    .heater_pwm = 0.0,
    .heater_temp = 0.0,
    .motor_speed = 0.0,
    .motor_pwm = 0.0
#else 
    .amp_pwm = 0.0,
    .amp_temp = 0.0,
    .valve_pwm = 0.0,
    .valve_temp = 0.0
#endif
    };

temperature_pwm_data_t pwm_data = {
#ifdef SAMPLE_PREP_BOARD
    .heater_pwm = 0.0,
    .motor_pwm = 0.0
#else
    .amp_pwm = 0.0,
    .valve_pwm = 0.0
#endif
    };


static tsys01_errors_t tsys01_err;

static long double valve_temperature = 0.0;
static long double amp0_temperature = 0.0;
static long double amp1_temperature = 0.0;
static long double amp2_temperature = 0.0;

xQueueHandle sensorRxQueue;

TimerHandle_t sensorTempTimer;
TickType_t sampleRateTicks;

bool oneshot_temperature_acquisition = false;
bool heaterRunning = false;
bool p_heaterRunningValve = false;
bool s_motorRunning = false;
bool usb_suspend = false;
uint32_t sample_log_index = 0;
uint32_t sample_log_max = 0;


uint32_t tickstart1;
uint32_t tickstop1;

uint32_t tickstart2;
uint32_t tickstop2;

sensor_public_data_t PUBLIC_SENSOR_DATA = {
  .temperatures = &temperatures,
  .oneshot_temperature_acquisition = &oneshot_temperature_acquisition
};

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

  sampleRateTicks = pdMS_TO_TICKS((config.sample_rate * 1000.0));

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
#ifdef SAMPLE_PREP_BOARD
  /* Get TSYS01 Calibration Values */
  tsys01_err = tsys01_getCalibrationValues(heater_zone);
  if (tsys01_err == tsys01_i2c_error) {
    NVIC_SystemReset();
    asm volatile("nop");
  }
  sensorTempTimer = xTimerCreate("SensorTempTimer", sampleRateTicks, pdTRUE, (void *)0, vSensorTempTimerCallback);
#endif
}

void powerModuleSensorTaskSetup(void) {
#ifndef SAMPLE_PREP_BOARD
  tsys01_err = tsys01_getCalibrationValues(valve_zone);
  tsys01_err = tsys01_getCalibrationValues(amp_zone);
  sensorTempTimer = xTimerCreate("SensorTempTimer", sampleRateTicks, pdTRUE, (void *)0, vSensorTempTimerCallback);
#endif
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
  sample_log_max = (config.logging_rate / config.sample_rate);

  SensorRxQueueMsg_t sensorRxMessage;

  sampleRateTicks = pdMS_TO_TICKS((config.sample_rate * 1000.0) - 12.0);

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
        //char tmp[100];
        // dbg
        //snprintf(tmp,100,"rx SENSOR_MSG_HEATER_STATE: heater=%d->%d ",heaterRunning,sensorRxMessage.heaterRunning);
        //send_event_log_message(SAMPLE_I2C_READ_ERROR,tmp);
        // dbg

        heaterRunning = sensorRxMessage.heaterRunning;
        #if defined(POWER_MODULE_BOARD)
          p_heaterRunningValve = sensorRxMessage.heaterRunningValve;
        #endif

        HeaterRxQueueMsg_t heaterMsg = {
            .type = HEATER_MSG_SENSOR_CONFIRM,
            .heaterRunning = heaterRunning
            };

        //send_debug_log_message("Sensor: handle SENSOR_MSG_HEATER_STATE");

        // Respond to heater change
        xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("USB: Unable to send main state response to main_mainStateRespQueue queue.\r\n");
        }
        break;
      }

#ifdef SAMPLE_PREP_BOARD
      case SENSOR_MSG_MOTOR_STATE: {
        s_motorRunning = sensorRxMessage.motorRunning;
        break;
      }
#endif
      case SENSOR_MSG_COLLECTION_LOOP_ONESHOT_TEMPERATURES:
        oneshot_temperature_acquisition = true;
        break;

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
  sample_log_max = (config.logging_rate / config.sample_rate);
}

void runPowerModuleSensorCollection(void) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterMsg;
  uint8_t nretries = 0;
  static uint8_t counter = 0;

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
  if (heaterRunning || oneshot_temperature_acquisition) {
    bool readTempSuccess = false;
    bool dis = true;
    heaterMsg.readTempFailed = false;

    /*
    // I2C Read for Heater Zone 0
    nretries = 0;
    while(nretries<3) {
      if(p_heaterRunningValve)  {
        tickstart1 = xTaskGetTickCount();
        dis = disable_valve_boost();
        while (!dis) { // Reset the Valve Boost
          nrf_gpio_pin_clear(VALVE_PWR_EN); 
          nrf_gpio_pin_set(VALVE_PWR_EN); 
          valve_zone_set_6v();
          dis = disable_valve_boost();
        }
      }
      readTempSuccess = readTemp(valve_zone, &temperatures.valve_temp);
      if(p_heaterRunningValve)  {
        enable_valve_boost();
        tickstop1 = xTaskGetTickCount();
      }
      if (!readTempSuccess) {
        send_event_log_message(SAMPLE_I2C_READ_ERROR,"temp read failure on valve zone");
        heaterMsg.readTempFailed = true;
        nretries++;
      } else{
        heaterMsg.readTempFailed = false;
        break;
      }
    }

    // I2C Read for Heater Zone 2
    nretries = 0;
    while(nretries<3) {
      if(p_heaterRunningValve)  {
        tickstart2 = xTaskGetTickCount();
        dis = disable_valve_boost();
        while (!dis) {  // Reset the Valve Boost
          nrf_gpio_pin_clear(VALVE_PWR_EN); 
          nrf_gpio_pin_set(VALVE_PWR_EN); 
          valve_zone_set_6v();
          dis = disable_valve_boost();
        }
      }
      readTempSuccess = readTemp(amp_zone, &temperatures.amp_temp);
      if(p_heaterRunningValve)  {
        enable_valve_boost();
        tickstop2 = xTaskGetTickCount();
      }
      if (!readTempSuccess) {
        send_event_log_message(SAMPLE_I2C_READ_ERROR,"temp read failure on amp zone");
        heaterMsg.readTempFailed = true;
        nretries++;
      } else{
        heaterMsg.readTempFailed = false;
        break;
      }
    }
    */

    // I2C Read for Temperature Sensors (try 3 times then return with error which will abandon cycle)
    if(p_heaterRunningValve)  {
      tickstart1 = xTaskGetTickCount();
      
      nrf_gpio_pin_clear(VALVE_PWR_EN);       //<-- turn off the VALVE power supply; this will clear it's registers
      //dis = disable_valve_boost();
      //while (!dis) { // Reset the Valve Boost
      //  nrf_gpio_pin_clear(VALVE_PWR_EN); 
      //  nrf_gpio_pin_set(VALVE_PWR_EN); 
      //  valve_zone_set_6v();
      //  dis = disable_valve_boost();
      //}
    }
    nretries = 0;
    while(nretries<3) {
      readTempSuccess = readTemp(valve_zone, &temperatures.valve_temp);
      if (!readTempSuccess) {
        send_event_log_message(SAMPLE_I2C_READ_ERROR,"saw a temperature read failure on valve zone");
        heaterMsg.readTempFailed = true;
        nretries++;
      } else{
        heaterMsg.readTempFailed = false;
        break;
      }
    }
    nretries = 0;
    while(nretries<3) {
      readTempSuccess = readTemp(amp_zone, &temperatures.amp_temp);
      if (!readTempSuccess) {
        send_event_log_message(SAMPLE_I2C_READ_ERROR,"saw a temperature read failure on amp zone");
        heaterMsg.readTempFailed = true;
        nretries++;
      } else{
        heaterMsg.readTempFailed = false;
        break;
      }
    }
    if(p_heaterRunningValve)  {
      // TURN ON VALVE HEATER POWER SUPPLY AGAIN
      nrf_gpio_pin_set(VALVE_PWR_EN);
      vTaskDelay(2);  //2 tick delay
      //valve_zone_set_6v();
      //enable_valve_boost();
      valve_zone_set_6v_and_configure_and_ENABLE();

      tickstop1 = xTaskGetTickCount();
    }

#if 0
    // Summarize down-time for no valve power supply
    if(p_heaterRunningValve && (counter%20 == 0) )  {
      char buf[100];
      counter = 1;
      snprintf(buf,100,"SENSORS: valve power supply downtimes 1=%lums  2=%lums",
        pdTICKS_TO_MS(tickstop1-tickstart1),
        pdTICKS_TO_MS(tickstop2-tickstart2)
      );
      send_event_log_message(SAMPLE_UNKNOWN,buf);
    } else{
      counter++;
    }
 #endif

    if(heaterRunning) {
      // Send Temperatures to heater
      heaterMsg.type = HEATER_MSG_TEMPERATURE_DATA;
      heaterMsg.tempData = temperatures;
      xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
      if (xReturned != pdPASS) {
        char errorString[100];
        sprintf(errorString, "SENSORS_TASK: Unable to send temperature data in heaterRxQueue. Error: %d\r\n", xReturned);
        send_debug_log_message(errorString);
      }
    }
    //send_debug_log_message("SENSORS_TASK: read temperatures");
    oneshot_temperature_acquisition = false;  // we did our 1-time temperature read, so set back to false
  }
#endif
}

void runSamplePrepSensorCollection(void) {
#ifdef SAMPLE_PREP_BOARD
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
      bool readTempSuccess = readTemp(heater_zone, &temp);
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
  temperatures.heater_temp = avg_temp;

  // Put Switch Data into queue
  xReturned = xQueueSend(main_switchQueue, (void *)&switches, 10);
  if (xReturned != pdPASS) {
    send_debug_log_message("SENSORS_TASK: Unable to send switch data in main_switchQueue.\r\n");
  }

#ifndef SAMPLE_PREP_BOARD
  if (heaterRunning) {
#else 
  
#endif
  if (heaterRunning || s_motorRunning) {
    heaterMsg.type = HEATER_MSG_TEMPERATURE_DATA;
    heaterMsg.tempData = temperatures;
    xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
    if (xReturned != pdPASS) {
      char errorString[100];
      sprintf(errorString, "SENSORS_TASK: Unable to send temperature data in heaterRxQueue. Error: %d\r\n", xReturned);
      send_debug_log_message(errorString);
    }
  }
#endif
  oneshot_temperature_acquisition = false;  // we did our 1-time temperature read, so set back to false
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