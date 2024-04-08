/*********************************************************************
*                               NAATOS                               *
**********************************************************************
File    : main.c
Purpose : NAATOS Application Start
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "bsp.h"
#include "nordic_common.h"
#include "nrf_drv_clock.h"
#include "sdk_errors.h"
#include "app_error.h"

#include "states.h"
#include "battery.h"
#include "logger.h"
#include "sensors.h"
#include "heater.h"
#include "usb.h"
#include "naatos_queues.h"
#include "naatos_config.h"
#include "pid.h"
#include "pwm.h"
#include "adc.h"
#include "i2c_hal_freertos.h"
#include "spi.h"
#include "sd_card.h"

// Task Handles
xTaskHandle mainTaskHandle;
xTaskHandle heaterTaskHandle;
xTaskHandle loggerTaskHandle;
xTaskHandle sensorsTaskHandle;
xTaskHandle batteryTaskHandle;
xTaskHandle usbTaskHandle;
xTaskHandle pwmTaskHandle;

xQueueHandle main_batteryDataQueue;
xQueueHandle main_switchQueue;

// Zone Request Constants
const zone_run_req_t run_amplification_zone = {
  .on = true,
  .zone = AMPLIFICATION
};
const zone_run_req_t run_valve_zone = {
  .on = true,
  .zone = VALVE
};
const zone_run_req_t stop_amplification_zone = {
  .on = false, 
  .zone = AMPLIFICATION
};
const zone_run_req_t stop_valve_zone = {
  .on = false,
  .zone = VALVE
};

// USB Main State Update Constants
const usb_message_t standby_update = {
  .message_type = MAIN_STATE_TYPE,
  .current_state = STANDBY,
  .charge_state = NULL
};
const usb_message_t running_update = {
  .message_type = MAIN_STATE_TYPE,
  .current_state = RUNNING,
  .charge_state = NULL
};

// Function defs
void sendUpdatedMainTaskState(main_state_t new_state);

/*********************************************************************
*
*       main_task()
*
*   Main Task of NAATOS Application
*/
void main_task(void * pvParameters) {
  BaseType_t xReturned;
  uint8_t queue_size;
  sensor_switches_t switch_data;
  int percent_recv;   
  bool hal_triggered = false, optical_triggered = false; 

  // Set Start up state to standby
  main_state_t main_state = STANDBY;
  // TODO: Get Configuration Settings
  
  // Main State Loop
  for (;;) {
    switch(main_state) {
      // In Standby State
      case STANDBY:
        // Check for Battery Data in Battery Queue
        if (xQueueReceive(main_batteryDataQueue, &percent_recv, 0) == pdPASS) {
          if (percent_recv < LOW_POWER_THRESHOLD) {
             main_state = LOW_POWER;
             sendUpdatedMainTaskState(main_state);
             break;
          }
        }
        // Wait for Sensor Switch Data 
        xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Error receiving switch data from main_switchQueue\n");
        }
        // Update switches triggered
        hal_triggered = switch_data.hal_triggered;
        optical_triggered = switch_data.optical_tiggered;
        // Check if we can go to RUN state
        if (hal_triggered && optical_triggered) {
          // Set the new main state
          main_state = RUNNING;
          sendUpdatedMainTaskState(main_state);
        }
      break;

      // In Running State (Will block task for the duration of the test)
      case RUNNING:
        /* ***** Start Sample Preperation ***** */
        // TODO: Send Start Test Preperation to log 
#if GO_STRAIGHT_TO_RUNNING
        // Allow for other tasks to get ready to receive main state change
        vTaskDelay(2000);
#endif
        /* ***** Amplification Zone Run ***** */
        // Send start amplification message to heater queue
        xReturned = xQueueSend(heater_zoneRunQueue, &run_amplification_zone, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send run amplification zone request.\n");
        }
        // TODO: Get the amplification start time
        // Get sensor switch data ensuring sample is still in position
        do {
          // TODO: Get the current time
          xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
          hal_triggered = switch_data.hal_triggered;
          optical_triggered = switch_data.optical_tiggered;
          if (!hal_triggered || !optical_triggered) {
            // TODO: Send Log Error about sample removed during run
            main_state = STANDBY;
            sendUpdatedMainTaskState(main_state);
            break;
          }
        } while (1/*TODO: Compare current time against AMPLIFICATION_ON_TIME*/);
        // Send amplification zone stop request
        xReturned = xQueueSend(heater_zoneRunQueue, &stop_amplification_zone, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send stop amplification zone request.\n");
        }
        // TODO: might have to do small delay here for heater task to update

        /* ***** Valve Zone Run **** */
        // Send start valve message to heater queue
        xReturned = xQueueSend(heater_zoneRunQueue, &run_valve_zone, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send run valve zone request.\n");
        }
        // TODO: Get the amplification start time
        // Get sensor switch data ensuring sample is still in position
        do {
          // TODO: Get the current time
          xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
          hal_triggered = switch_data.hal_triggered;
          optical_triggered = switch_data.optical_tiggered;
          if (!hal_triggered || !optical_triggered) {
            // TODO: Send Log Error about sample removed during run
            main_state = STANDBY;
            sendUpdatedMainTaskState(main_state);
            break;
          }
        } while (1/*TODO: Compare current time against VALVE_ON_TIME*/);
        // Send valve zone stop request
        xReturned = xQueueSend(heater_zoneRunQueue, &stop_valve_zone, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send stop valve zone request.\n");
        }

        // TODO: might have to do small delay here for heater task to update
      
        /* ***** End Sample Preperation ***** */
        if (!hal_triggered || !optical_triggered) {
          break;
        }
        // Wait for the sample to be removed prior to going back to STANDBY state
        do {
          xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
          hal_triggered = switch_data.hal_triggered;
          optical_triggered = switch_data.optical_tiggered;
        } while(hal_triggered || optical_triggered);
        // Update current main state
        main_state = STANDBY;
        sendUpdatedMainTaskState(main_state);
        break;

      // In Low Power State
      case LOW_POWER:

      break;
      // Shouldnt Get here
      default:
      break;
    }
  }
}

void sendUpdatedMainTaskState(main_state_t new_state) {
  BaseType_t xReturned;
  usb_message_t msg;
  
  // Get the update to send
  if (new_state == RUNNING)
    msg = running_update;
  else if (new_state == STANDBY)
    msg = standby_update;

  // Send main state update to usb task
  xReturned = xQueueSend(usb_stateChangeQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state change to usb_stateChangeQueue.\n");
  }
  
  // Send the state to the logger
  xReturned = xQueueSend(logger_mainStateChangeQueue, &new_state, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state change to logger_mainStateChangeQueue.\n");
  }

  // Send the state to the battery task
  xReturned = xQueueSend(battery_mainStateQueue, &new_state, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state change to battery_mainStateQueue.\n");
  }

  // TODO: might have to do small delay here for usb task to update
}

/*********************************************************************
*
*       create_tasks()
*
*   Creates all of the FreeRTOS Tasks
*/
void create_tasks() {
  BaseType_t xReturned;
  // Main Task
  xReturned = xTaskCreate(main_task, "MainTask", 300, NULL, 0, &mainTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating main task. Error: %d\n", xReturned);
      vTaskDelete( mainTaskHandle );
  }
  // Heater Task
  xReturned = xTaskCreate(heater_task, "HeaterTask", 300, NULL, 0, &heaterTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating heater task. Error: %d\n", xReturned);
      vTaskDelete( heaterTaskHandle );
  }
  // Logger Task
  xReturned = xTaskCreate(logger_task, "LoggerTask", 300, NULL, 0, &loggerTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating logger task. Error: %d\n", xReturned);
      vTaskDelete( loggerTaskHandle );
  }
  // Sensors Task
  xReturned = xTaskCreate(sensors_task, "SensorsTask", 300, NULL, 0, &sensorsTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating sensors task. Error: %d\n", xReturned);
      vTaskDelete( sensorsTaskHandle );
  }
  // Battery Management Task
  xReturned = xTaskCreate(battery_task, "BatteryTask", 100, NULL, 0, &batteryTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating battery management task. Error: %d\n", xReturned);
      vTaskDelete( batteryTaskHandle );
  }
  // USB Management Task
  xReturned = xTaskCreate(usb_task, "USBTask", 100, NULL, 0, &usbTaskHandle);
  if( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating usb management task. Error: %d\n", xReturned);
      vTaskDelete( usbTaskHandle );
  }
  // PWM Task
  xReturned = xTaskCreate(pwm_task, "PWMTask", 100, NULL, 0, &pwmTaskHandle);
  if ( xReturned != pdPASS ) {
      /* The task was created.  Use the task's handle to delete the task. */
      printf("Error creating PWM task. Error: %d\n", xReturned);
      vTaskDelete( pwmTaskHandle );
  }
}

/*********************************************************************
*
*       create_queues()
*
*   Creates all of the FreeRTOS Queues
*/
void create_queues() {
  // Main Task Queues
  main_batteryDataQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));       
  if (main_batteryDataQueue == NULL)
    printf("Unable to create main_batteryDataQueue queue\n");
  main_switchQueue = xQueueCreate(QUEUE_SIZE, sizeof(sensor_switches_t));
  if (main_switchQueue == NULL)
    printf("Unable to create main_switchQueue queue\n");

  // Heater Task Queues
  heater_zoneRunQueue = xQueueCreate(QUEUE_SIZE, sizeof(zone_run_req_t));
  if (heater_zoneRunQueue == NULL)
    printf("Unable to create heater_zoneRunQueue queue\n");
  heater_temperatureDataQueue = xQueueCreate(QUEUE_SIZE, sizeof(temperature_data_t));
  if (heater_temperatureDataQueue == NULL)
    printf("Unable to create heater_temperatureDataQueue queue\n");

  // Battery Management Task Queues
  battery_requestPercentQueue = xQueueCreate(QUEUE_SIZE, sizeof(battery_percent_req_t));
  if (battery_requestPercentQueue == NULL)
    printf("Unable to create battery_requestPercentQueue queue\n");
  battery_mainStateQueue = xQueueCreate(QUEUE_SIZE, sizeof(main_state_t));
  if (battery_mainStateQueue == NULL)
    printf("Unable to create battery_mainStateQueue queue\n");

  // USB Management Task Queues
  usb_stateChangeQueue = xQueueCreate(QUEUE_SIZE, sizeof(usb_message_t));
  if (usb_stateChangeQueue == NULL)
    printf("Unable to create usb_stateChangeQueue queue\n");

  // Logger Task Queues
  logger_recvBattPercentQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (logger_recvBattPercentQueue == NULL)
    printf("Unable to create logger_recvBattPercentQueue queue\n");
  logger_logMessageQueue = xQueueCreate(QUEUE_SIZE, sizeof(log_data_message_t));
  if (logger_logMessageQueue == NULL)
    printf("Unable to create logger_logMessageQueue queue\n");
  logger_mainStateChangeQueue = xQueueCreate(QUEUE_SIZE, sizeof(main_state_t));
  if (logger_mainStateChangeQueue == NULL)
    printf("Unable to create logger_mainStateChangeQueue queue\n");
}

/*********************************************************************
*
*       main()
*
*   Application entry point.
*/
int main(void) {
  ret_code_t err_code;

  // Initialize clock driver for better time accuracy in FREERTOS
  err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  // Full Peripheral Initalizations
  init_adc();             // ADC
  init_sensors_gpios();   // Sensor GPIOs
  vInit_TWI_Hardware(i2c_interface_system, I2C0_SDA_PIN, I2C0_SCL_PIN, i2c_speed_400k);   // I2C
  //vInit_TWI_Hardware(i2c_interface_sensors, I2C0_SDA_PIN, I2C0_SCL_PIN, i2c_speed_400k);   // I2C
  init_sd_card();

  // Create Tasks
  create_tasks();

  // Create Queues
  create_queues();

  // Start Tasks
  vTaskStartScheduler();

}

/*************************** End of file ****************************/

