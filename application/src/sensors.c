#include "sensors.h"
#include "motor.h"
#include "nrf_drv_timer.h"
#include "timers.h"

void sensorCollection(void);

sensor_switches_t switches;
temperature_data_t temperatures = {
    .amp0_zone_pwm = 0,
    .amp0_zone_temp = 0,
    .amp1_zone_pwm = 0,
    .amp1_zone_temp = 0,
    .amp2_zone_pwm = 0,
    .amp2_zone_temp = 0,
    .valve_zone_pwm = 0,
    .valve_zone_temp = 0};

temperature_pwm_data_t pwm_data = {
    .valve_zone_pwm = 0,
    .amp0_zone_pwm = 0,
    .amp1_zone_pwm = 0,
    .amp2_zone_pwm = 0};

#if USE_MOTOR
static const nrf_drv_timer_t *p_counter1;
static uint32_t motor_speed_read_t1 = 0;
static uint32_t motor_speed_read_t2;
static bool skipped_last_call = false;
static long double motor_speed = 0.0; //RPM
static long double avg_speed[3] = {0,0,0};
static long double moving_avg_speed = 0.0;
#endif

static tsys01_errors_t tsys01_err;

static long double valve_temperature = 0.0;
static long double amp0_temperature = 0.0;
static long double amp1_temperature = 0.0;
static long double amp2_temperature = 0.0;

xQueueHandle sensorRxQueue;
double motorSpeed = 0;

TimerHandle_t sensorTempTimer;
TimerHandle_t sensorMotorTimer;

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
      printf("Sensor: Unable to send timer update to sensorRxQueue queue. from temp callback\n");
    }
  }
}

void vSensorMotorTimerCallback(TimerHandle_t xTimer) {
  BaseType_t xReturned;
  SensorRxQueueMsg_t msg;

  msg.type = SENSOR_MSG_TIMER_MOTOR_EVENT;
  if (!usb_suspend) {
    xReturned = xQueueSend(sensorRxQueue, &msg, 0);
    if (xReturned != pdPASS) {
      printf("Sensor: Unable to send timer update to sensorRxQueue queue. from motor timer callback\n");
    }
  }
}

void startSensorTempTimer(void) {
  TickType_t sampleRateTicks;

  if (use_default_configuration_parameters) {
    sampleRateTicks = pdMS_TO_TICKS((DEFAULT_SAMPLE_RATE * 1000.0) - (12.0 * 4.0) + 1.0);
  } else {
    sampleRateTicks = pdMS_TO_TICKS((config.sample_rate * 1000.0) - (12.0 * 4.0) + 1.0);
  }

  if (xTimerChangePeriod(sensorTempTimer, sampleRateTicks, 100) != pdPASS) {
    printf("Cannot change period of sensor timer. \n");
  }
  
  if (xTimerStart(sensorTempTimer, 0) != pdPASS) {
    printf("Failed to start sensor timer. \n");
  }
}

void startSensorMotorTimer(void) {
  TickType_t sampleRateTicks;
  
  sampleRateTicks = pdMS_TO_TICKS(50);

  if (xTimerChangePeriod(sensorMotorTimer, sampleRateTicks, 100) != pdPASS) {
    printf("Cannot change period of sensor timer. \n");
  }
  
  if (xTimerStart(sensorMotorTimer, 0) != pdPASS) {
    printf("Failed to start sensor timer. \n");
  }
}

void stopSensorTempTimer(void) {
  if (xTimerStop(sensorTempTimer, 100) != pdPASS) {
    printf("Failed to stop sensor timer. \n");
  }
}

void stopSensorMotorTimer(void) {
  if (xTimerStop(sensorMotorTimer, 100) != pdPASS) {
    printf("Failed to stop sensor timer. \n");
  }
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

  /* Get TSYS01 Calibration Values */
#if I2C_CONNECTED
#ifdef SAMPLE_PREP_BOARD
  tsys01_err = tsys01_getCalibrationValues(amp_zone_2);
  if (tsys01_err == tsys01_i2c_error) {
    NVIC_SystemReset();
    asm volatile("nop");
  }
#else
  tsys01_err = tsys01_getCalibrationValues(valve_zone);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_0);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_1);
  tsys01_err = tsys01_getCalibrationValues(amp_zone_2);
#endif
#endif

  SensorRxQueueMsg_t sensorRxMessage;

  TickType_t sampleRateTicks;

  if (use_default_configuration_parameters) {
    sampleRateTicks = pdMS_TO_TICKS((DEFAULT_SAMPLE_RATE * 1000.0) - (12.0 * 4.0) + 1.0);
  } else {
    sampleRateTicks = pdMS_TO_TICKS((config.sample_rate * 1000.0) - (12.0 * 4.0) + 1.0);
  }
  sensorTempTimer = xTimerCreate("SensorTempTimer", sampleRateTicks, pdTRUE, (void *)0, vSensorTempTimerCallback);
  sensorMotorTimer = xTimerCreate("SensorMotorTimer", 2, pdTRUE, (void *)0, vSensorMotorTimerCallback);

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
      switch (sensorRxMessage.type) {
      case SENSOR_MSG_HEATER_STATE: {
        heaterRunning = sensorRxMessage.heaterRunning;

        HeaterRxQueueMsg_t heaterMsg = {
            .type = HEATER_MSG_SENSOR_CONFIRM,
            .heaterRunning = heaterRunning};

        // Respond to heater change
        xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
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
        log_msg.temperature_data.amp0_zone_pwm = sensorRxMessage.pwmData.amp0_zone_pwm;
        log_msg.temperature_data.amp1_zone_pwm = sensorRxMessage.pwmData.amp1_zone_pwm;
        log_msg.temperature_data.amp2_zone_pwm = sensorRxMessage.pwmData.amp1_zone_pwm;
        log_msg.temperature_data.valve_zone_pwm = sensorRxMessage.pwmData.valve_zone_pwm;
        log_msg.temperature_data.motorSpeed = moving_avg_speed;
        // Send the Log message
        xReturned = xQueueSend(logger_logMessageQueue, (void *)&log_msg, 0);
        if (xReturned != pdPASS) {
          printf("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.\n");
        }
        sample_log_index = 0;
        break;

      case SENSOR_MSG_TIMER_TEMP_EVENT: {
        sensorTempCollection();
        break;
      }

      case SENSOR_MSG_TIMER_MOTOR_EVENT: {
        sensorMotorCollection();
        break;
      } 

      case SENSOR_MSG_SLEEP: {
        if (xTimerIsTimerActive(sensorTempTimer) == pdTRUE) {
          stopSensorTempTimer();
        }
        if (xTimerIsTimerActive(sensorMotorTimer) == pdTRUE) {
          stopSensorMotorTimer();
        }

        //send to main queue that we are asleep
        break;
      }

      case SENSOR_MSG_WAKEUP:
        if (xTimerIsTimerActive(sensorTempTimer) == pdFALSE) {
          startSensorTempTimer();
        }
        if (xTimerIsTimerActive(sensorMotorTimer) == pdFALSE) {
          startSensorMotorTimer();
        }
        break;

      case CONFIG_UPDATED:
        if (xTimerIsTimerActive(sensorTempTimer) == pdTRUE) {
          stopSensorTempTimer();
        }
        if (xTimerIsTimerActive(sensorMotorTimer) == pdTRUE) {
          stopSensorMotorTimer();
        }

        updateSampleLogMax();

        if (xTimerIsTimerActive(sensorTempTimer) == pdFALSE) {
          startSensorTempTimer();
        }
        if (xTimerIsTimerActive(sensorMotorTimer) == pdFALSE) {
          startSensorMotorTimer();
        }
        break;

      default:
        break;
      }
    }
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

void sensorTempCollection(void) {
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterMsg;
  bool readTempSuccess = false;
  heaterMsg.readTempFailed = false;
  bool prev;

  #ifndef SAMPLE_PREP_BOARD
  // ADC Read for Optical Sensors
  prev = switches.optical_tiggered;
  switches.optical_tiggered = get_optical_triggered();
  if (prev != switches.optical_tiggered) {
    if (switches.optical_tiggered) {
      printf("Optical sensor triggered!\n");
    } else {
      printf("Optical sensor no longer triggered!\n");
    }
  }
#endif

  // GPIO Read for Hall Sensor
  prev = switches.hal_triggered;
  if (nrf_gpio_pin_read(HAL_INPUT_PIN))
    switches.hal_triggered = false;
  else
    switches.hal_triggered = true;
  if (prev != switches.hal_triggered) {
    if (switches.hal_triggered) {
      printf("Hal sensor triggered!\n");
    } else {
      printf("Hal sensor no longer triggered!\n");
    }
  }

  #if I2C_CONNECTED
  #ifdef SAMPLE_PREP_BOARD
    readTempSuccess = readTemp(amp_zone_2, &temperatures.amp2_zone_temp);
    if(!readTempSuccess) {
      heaterMsg.readTempFailed = true;
    }
  #else
    // I2C Read for Valve Zone
    temperatures.valve_zone_temp = readTemp(valve_zone);

    // I2C Read for Amplification Zone 0
    temperatures.amp0_zone_temp = readTemp(amp_zone_0);

    // I2C Read for Amplification Zone 1
    temperatures.amp1_zone_temp = readTemp(amp_zone_1);

    // I2C Read for Amplification Zone 2
    temperatures.amp2_zone_temp = readTemp(amp_zone_2);
  #endif
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

    if (heaterRunning) {

    #if I2C_CONNECTED
      readTempSuccess = readTemp(amp_zone_2, &temperatures.amp2_zone_temp);
      if(!readTempSuccess) {
        heaterMsg.readTempFailed = true;
      }
    #else
      // Set temps to their setpoints if i2c is not connected
      temperatures.valve_zone_temp = 85;
      temperatures.amp0_zone_temp = 65;
      temperatures.amp1_zone_temp = 65;
      temperatures.amp2_zone_temp = 65;
      vTaskDelay(pdMS_TO_TICKS(12 * 4)); // Simulate 12ms delay for each reading
    #endif
      heaterMsg.type = HEATER_MSG_TEMPERATURE_DATA;
      heaterMsg.tempData = temperatures;

      xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
      if (xReturned != pdPASS) {
        printf("SENSORS_TASK: Unable to send temperature data in heaterRxQueue. Error: %d\n", xReturned);
      }
      sample_log_index++;
      if (sample_log_index >= sample_log_max) {
        HeaterRxQueueMsg_t heaterPwmMsg = {.type = HEATER_MSG_PWM_REQUEST};
        // Request PWM from heater
        xReturned = xQueueSend(heaterRxQueue, &heaterPwmMsg, 0);
        if (xReturned != pdPASS) {
          printf("SENSOR_TASK: Unable to send PWM request to heaterRxQueue queue.\n");
        }
      }
  }

}

void sensorMotorCollection(void) {
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterMsg;

  if (heaterRunning) {
#if USE_MOTOR
    motorSpeed = readMotorSpeed();
    avg_speed[2] = avg_speed[1];
    avg_speed[1] = avg_speed[0];
    avg_speed[0] = motorSpeed;

    moving_avg_speed = (avg_speed[0] + avg_speed[1] + avg_speed[2]) / 3.0;

    heaterMsg.type = HEATER_MSG_MOTOR_DATA;
    heaterMsg.motorSpeed = moving_avg_speed;
    xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
    if (xReturned != pdPASS) {
      printf("SENSORS_TASK: Unable to send temperature data in heaterRxQueue. Error: %d\n", xReturned);
    }
#endif
  }
}

void init_sensors_gpios(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(HAL_INPUT_PIN, NRF_GPIO_PIN_PULLDOWN);
  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(1, 7));
  nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(1, 7), 1);
  nrf_gpio_cfg_output(SENSORS_EN);
  nrf_gpio_pin_set(SENSORS_EN);

  /* Setup Motor Speed Sensor Input*/
  p_counter1 = motor_tach_init();

  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(1, 3));
  nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(1, 3));
}

bool readTemp(sensor_selection_t sensor, float *temperature) {
  int i2cRetry = 0;
  tsys01_errors_t tsys_err;

  tsys_err = tsys01_startConversion(sensor);
  if (tsys_err != tsys01_success) {
    printf("HEATER_TASK: Unable to triggr temperature conversion!\n");
  }

  if(tsys_err != tsys01_success) {
    while((i2cRetry++ < 4) && (tsys_err != tsys01_success)) {
      tsys_err = tsys01_startConversion(sensor);
      if (tsys_err != tsys01_success) {
        printf("HEATER_TASK: Unable to triggr temperature conversion!\n");
      }
    }
  }

  if(i2cRetry >= 4){
    //TODO bail with error or possibly more robust recovery
    printf("HEATER_TASK: Too many i2c retries for conversion!\n");
  } else {
    i2cRetry = 0;
  }


  vTaskDelay(pdMS_TO_TICKS(12)); // 12ms conversion time
  tsys_err = tsys01_getTemp(sensor, temperature);
  if (tsys_err != tsys01_success) {
    printf("HEATER_TASK: Unable to read temperature!\n");
  }

  if(tsys_err != tsys01_success) {
    while((i2cRetry++ < 4) && (tsys_err != tsys01_success)) {
      tsys_err = tsys01_getTemp(sensor, temperature);
      if (tsys_err != tsys01_success) {
        printf("HEATER_TASK: Unable to read temperature!\n");
      }
    }
  }

  if(i2cRetry >= 4){
    //TODO bail with error or possibly more robust recovery
    printf("HEATER_TASK: Too many i2c retries for conversion!\n");
  } else {
    i2cRetry = 0;
  }
  return temperature;
}

double readMotorSpeed(void) {
  //Check how many pulses have been captured in elapsed time since last call
  motor_speed_read_t2 = xTaskGetTickCount();
  uint32_t delta_t = pdTICKS_TO_MS(motor_speed_read_t2 - motor_speed_read_t1);
  uint32_t pulse_count = (nrf_drv_timer_capture(p_counter1, NRF_TIMER_CC_CHANNEL0)) / 2; //Divide by two because counter increments for every rising AND falling edge

  //Convert pulse count to rotational speed
  double motor_speed_rpm = (1000 * 60 * ((double)pulse_count / (double)delta_t)) / 9;
  //printf("Motor speed: %f\r\n", motor_speed_rpm);

  //Clear the counter, update variable for tracking elapsed time
  nrf_drv_timer_clear(p_counter1);
  motor_speed_read_t1 = xTaskGetTickCount();
  skipped_last_call = false;

  return motor_speed_rpm;
}