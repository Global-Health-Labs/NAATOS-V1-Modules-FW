#include "motor_task.h"
#include "motor.h"
#include "nrf_drv_timer.h"
#include "timers.h"
#include "logger/logger.h"

static const nrf_drv_timer_t *p_counter1;
static uint32_t motor_speed_read_t1 = 0;
static uint32_t motor_speed_read_t2;
static bool skipped_last_call = false;
static long double avg_speed[3] = {0, 0, 0};
static long double moving_avg_speed = 0.0;

static long double motorSpeed = 0.0;

bool motorRunning = false;

TimerHandle_t sensorMotorTimer;

xQueueHandle motorRxQueue;

void init_motor_gpio() {

  /* Setup Motor Speed Sensor Input*/
  p_counter1 = motor_tach_init();
}


void vSensorMotorTimerCallback(TimerHandle_t xTimer) {
  BaseType_t xReturned;
  MotorRxQueueMsg_t msg;

  msg.type = MOTOR_MSG_TIMER_MOTOR_EVENT;
 // if (!usb_suspend) {
    xReturned = xQueueSend(motorRxQueue, &msg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("Sensor: Unable to send timer update to sensorRxQueue queue. from motor timer callback\n");
    }
 // }
}

void motorTask(void *pvParameters) {
  BaseType_t xReturned;

  MotorRxQueueMsg_t motorRxMessage;

  motorTaskSetup();

  for(;;) {
    xReturned = xQueueReceive(motorRxQueue, &motorRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      send_debug_log_message("Unable to Rx data to motor queue\n");
    } else {
      switch (motorRxMessage.type) {
         case MOTOR_MSG_HEATER_STATE: {
          motorRunning = motorRxMessage.motorRunning;
          send_debug_log_message("start/stop motor");
          break;
         }
         case MOTOR_MSG_TIMER_MOTOR_EVENT: {
          sensorMotorCollection();
          break;
         }

         case MOTOR_MSG_SLEEP: {
            handleMotorSleep();
            //send to main queue that we are asleep
            break;
          }

          case MOTOR_MSG_WAKEUP: {
            handleMotorWake();
            break;
          }

          case MOTOR_CONFIG_UPDATED: {
            handleMotorConfigUpdated();
            break;
          }

          default: {
            break;
          }
      }
    }
  }
}

void startSensorMotorTimer(void) {
  TickType_t sampleRateTicks;

  sampleRateTicks = pdMS_TO_TICKS(50);

  if (xTimerChangePeriod(sensorMotorTimer, sampleRateTicks, 100) != pdPASS) {
    send_debug_log_message("Cannot change period of sensor timer. \n");
  }

  if (xTimerStart(sensorMotorTimer, 0) != pdPASS) {
    send_debug_log_message("Failed to start sensor timer. \n");
  }
}


void stopSensorMotorTimer(void) {
  if (xTimerStop(sensorMotorTimer, 100) != pdPASS) {
    send_debug_log_message("Failed to stop sensor timer. \n");
  }
}

void motorTaskSetup(void) {
  sensorMotorTimer = xTimerCreate("SensorMotorTimer", 2, pdTRUE, (void *)0, vSensorMotorTimerCallback);
}

void sensorMotorCollection(void) {
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterMsg;

  if (motorRunning) {
    motorSpeed = readMotorSpeed();
    avg_speed[2] = avg_speed[1];
    avg_speed[1] = avg_speed[0];
    avg_speed[0] = motorSpeed;

    moving_avg_speed = (avg_speed[0] + avg_speed[1] + avg_speed[2]) / 3.0;

    heaterMsg.type = HEATER_MSG_MOTOR_DATA;
    heaterMsg.motorSpeed = moving_avg_speed;
    xReturned = xQueueSend(heaterRxQueue, &heaterMsg, 0);
    if (xReturned != pdPASS) {
      char errorString[100];
      sprintf(errorString, "SENSORS_TASK: Unable to send temperature data in heaterRxQueue. Error: %d\n", xReturned);
      send_debug_log_message(errorString);
    }
  } else {
    moving_avg_speed = 0.0;
    motorSpeed = 0.0;
  }
}

double readMotorSpeed(void) {
  //Check how many pulses have been captured in elapsed time since last call
  motor_speed_read_t2 = xTaskGetTickCount();
  uint32_t delta_t = pdTICKS_TO_MS(motor_speed_read_t2 - motor_speed_read_t1);
  uint32_t pulse_count = (nrf_drv_timer_capture(p_counter1, NRF_TIMER_CC_CHANNEL0)) / 2; //Divide by two because counter increments for every rising AND falling edge

  //Convert pulse count to rotational speed
  //double motor_speed_rpm = (1000 * 60 * ((double)pulse_count / (double)delta_t)) / 9; //Old motor
  double motor_speed_rpm = (1000 * 60 * ((double)pulse_count / (double)delta_t)) / 6; //New motor
  //send_debug_log_message("Motor speed: %f\r\n", motor_speed_rpm);

  //Clear the counter, update variable for tracking elapsed time
  nrf_drv_timer_clear(p_counter1);
  motor_speed_read_t1 = xTaskGetTickCount();
  skipped_last_call = false;

  return motor_speed_rpm;
} 


void handleMotorSleep(void) {
  if (xTimerIsTimerActive(sensorMotorTimer) == pdTRUE) {
    stopSensorMotorTimer();
  }
}

void handleMotorWake(void) {
  if (xTimerIsTimerActive(sensorMotorTimer) == pdFALSE) {
    startSensorMotorTimer();
  }
}

void handleMotorConfigUpdated(void) {
  if (xTimerIsTimerActive(sensorMotorTimer) == pdTRUE) {
    stopSensorMotorTimer();
  }

 // updateSampleLogMax();

  if (xTimerIsTimerActive(sensorMotorTimer) == pdFALSE) {
    startSensorMotorTimer();
  }
}
