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

#include "nrf_drv_power.h"

#include "app_error.h"
#include "app_util.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"

// Task Handles
xTaskHandle mainTaskHandle;
xTaskHandle heaterTaskHandle;
xTaskHandle loggerTaskHandle;
xTaskHandle sensorsTaskHandle;
xTaskHandle batteryTaskHandle;
xTaskHandle usbTaskHandle;
xTaskHandle pwmTaskHandle;
xTaskHandle compositeTaskHandle;

xQueueHandle main_batteryDataQueue;
xQueueHandle main_switchQueue;
xQueueHandle main_mainStateRespQueue;

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

// Log Event Constants
const log_event_t start_event = {
  .event = SAMPLE_START,
  .message = START_EVENT_MSG
}; 
const log_data_message_t start_log_msg = {
  .data_type = EVENT_DATA,
  .temperature_data = NULL,
  .event_data = start_event
};
const log_event_t stop_event = {
  .event = SAMPLE_END,
  .message = STOP_EVENT_MSG
}; 
const log_data_message_t stop_log_msg = {
  .data_type = EVENT_DATA,
  .temperature_data = NULL,
  .event_data = stop_event
};
const log_event_t interrupt_event = {
  .event = SAMPLE_INTERRUPTED,
  .message = INTERRUPT_EVENT_MSG
}; 
const log_data_message_t interrupt_log_msg = {
  .data_type = EVENT_DATA,
  .temperature_data = NULL,
  .event_data = interrupt_event
};
const log_event_t valve_start_event = {
  .event = SAMPLE_VALV_STARTED,
  .message = VALV_START_MSG
}; 
const log_data_message_t valve_start_log_msg = {
  .data_type = EVENT_DATA,
  .temperature_data = NULL,
  .event_data = valve_start_event
};
const log_event_t valve_stop_event = {
  .event = SAMPLE_VALV_ENDED,
  .message = VALV_STOP_MSG
}; 
const log_data_message_t valve_stop_log_msg = {
  .data_type = EVENT_DATA,
  .temperature_data = NULL,
  .event_data = valve_stop_event
};
const log_event_t amplification_start_event = {
  .event = SAMPLE_AMP_STARTED,
  .message = AMP_START_MSG
}; 
const log_data_message_t amplification_start_log_msg = {
  .data_type = EVENT_DATA,
  .temperature_data = NULL,
  .event_data = amplification_start_event
};
const log_event_t amplification_stop_event = {
  .event = SAMPLE_AMP_ENDED,
  .message = AMP_END_MSG
}; 
const log_data_message_t amplification_stop_log_msg = {
  .data_type = EVENT_DATA,
  .temperature_data = NULL,
  .event_data = amplification_stop_event
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

// Configuration Parameters
naatos_config_parameters config = {
  .logging_rate = 0,
  .sample_rate = 0,
  .amplification_zone_run_time_m = 0,
  .valve_zone_run_time_m = 0,
  .low_power_threshold = 0,
  .valve_setpoint = 0,
  .amplification_setpoint = 0,
  .valve_kp = 0,
  .valve_ki = 0, 
  .valve_kp = 0,
  .amp0_kp = 0,
  .amp0_ki = 0,
  .amp0_kd = 0,
  .amp1_kp = 0,
  .amp1_ki = 0,
  .amp1_kd = 0,
  .amp2_kp = 0,
  .amp2_ki = 0,
  .amp2_kd = 0
};
bool use_default_configuration_parameters;

// Function defs
void sendUpdatedMainTaskState(main_state_t new_state);
void begin_amplification_zone(void);
void end_amplification_zone(void);
void begin_valve_zone(void);
void end_valve_zone(void);

xTaskHandle get_usb_task_handle(void)
{
    return usbTaskHandle;
}

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
  bool error_during_run = false;
  uint32_t start_time = 0, end_time = 0;

  // Set Start up state to standby
  main_state_t main_state = STANDBY;
  int i = 0;

  // Main State Loop
  for (;;) {
    switch(main_state) {
      // In Standby State
      case STANDBY:
        if (error_during_run) error_during_run = false;
        hal_triggered = false;
        optical_triggered = false; 
        // Check for Battery Data in Battery Queue
        if (xQueueReceive(main_batteryDataQueue, &percent_recv, 0) == pdPASS) {
          if ((percent_recv < DEFAULT_LOW_POWER_THRESHOLD && use_default_configuration_parameters) || (!use_default_configuration_parameters && percent_recv < config.low_power_threshold)) {
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
          // Delay
          vTaskDelay(100);
        }
      break;

      // In Running State (Will block task for the duration of the test)
      case RUNNING:
        vTaskDelay(pdMS_TO_TICKS(100));
        /* ***** Start Sample Preperation ***** */
        // TODO: Send Start Test Preperation to log 
#if GO_STRAIGHT_TO_RUNNING
        // Allow for other tasks to get ready to receive main state change
        vTaskDelay(2000);
#endif
        /* ***** Amplification Zone Run ***** */
        // Send start amplification message to heater queue
        begin_amplification_zone();
        // Get the start time and end time
        start_time = xTaskGetTickCount();
        if (use_default_configuration_parameters) {
          end_time = pdMS_TO_TICKS((DEFAULT_AMPLIFICATION_ZONE_ON_TIME * 60) * 1000);
        }
        else {
          end_time = pdMS_TO_TICKS((config.amplification_zone_run_time_m * 60) * 1000);
        }
        // Get sensor switch data ensuring sample is still in position
        do {
          // TODO: Get the current time
          xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
          hal_triggered = switch_data.hal_triggered;
          optical_triggered = switch_data.optical_tiggered;
          if (!hal_triggered || !optical_triggered) {
            // Send Interrupt Event to logging task
            xReturned = xQueueSend(logger_logMessageQueue, &interrupt_log_msg, 0);
            if (xReturned != pdPASS) {
              printf("MAIN_TASK: Unable to send sample interruption event to logging task.\n");
            }
            main_state = STANDBY;
            sendUpdatedMainTaskState(main_state);
            error_during_run = true;
            break;
          }
        } while ( pdTICKS_TO_MS(xTaskGetTickCount() - start_time) < end_time);
        
        // End amplification zone
        end_amplification_zone();
        
        vTaskDelay(100);

        /* ***** Valve Zone Run **** */
        // Send valve zone start request
        if (!error_during_run) {
          begin_valve_zone();
          // Get Start Time and End Time
          start_time = xTaskGetTickCount();
           if (use_default_configuration_parameters) {
            end_time = pdMS_TO_TICKS((DEFAULT_VALVE_ZONE_ON_TIME * 60) * 1000);
          }
          else {
            end_time = pdMS_TO_TICKS((config.valve_zone_run_time_m * 60) * 1000);
          }
        }
        else {
          break;
        }

        // Get sensor switch data ensuring sample is still in position
        do {
          // TODO: Get the current time
          xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
          hal_triggered = switch_data.hal_triggered;
          optical_triggered = switch_data.optical_tiggered;
          if (!hal_triggered || !optical_triggered) {
            // Send interrupt Event to logging task
            xReturned = xQueueSend(logger_logMessageQueue, &interrupt_log_msg, 0);
            if (xReturned != pdPASS) {
              printf("MAIN_TASK: Unable to send sample interruption event to logging task.\n");
            }
            main_state = STANDBY;
            sendUpdatedMainTaskState(main_state);
            error_during_run = true;
            break;
          }
        } while (pdTICKS_TO_MS(xTaskGetTickCount() - start_time) < end_time);
       
        // Stop the valve zone
        end_valve_zone();
        // Check for errors
        if (error_during_run) {
            break;
        }

        vTaskDelay(100);

        // TODO: might have to do small delay here for heater task to update
        /* ***** End Sample Preperation ***** */
        // Update sensor state prematuraly to stop sending of temp data
        //main_state = STANDBY;
       // xReturned = xQueueSend(sensor_mainStateQueue, &main_state, 0);
        //if (xReturned != pdPASS) {
        //  printf("MAIN_TASK: Unable to send main state change to heater_mainStateQueue.\n");
        //}
        // Wait for the sample to be removed prior to going back to STANDBY state
        do {
          xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
          hal_triggered = switch_data.hal_triggered;
          optical_triggered = switch_data.optical_tiggered;
        } while(hal_triggered && optical_triggered);
        // Update current main state
        main_state = STANDBY;
        sendUpdatedMainTaskState(main_state);
        vTaskDelay(250);
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
  bool logger_resp = false, batt_resp = false, usb_resp = false, sensor_resp = false;
  tasks_t task_recv;
  bool main_state_cont = true;
  
  // Get the update to send
  if (new_state == RUNNING)
    msg = running_update;
  else if (new_state == STANDBY)
    msg = standby_update;

  printf("MAIN: Sending Main State Update Messages.\n");

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

  // Send main state update to usb task
  xReturned = xQueueSend(usb_stateChangeQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state change to usb_stateChangeQueue.\n");
  }

  // Send the state to the sensor task
  xReturned = xQueueSend(sensor_mainStateQueue, &new_state, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state change to heater_mainStateQueue.\n");
  }
  /* Get responses from the states to ensure all configurations for the run or standby have been made */
  while(!logger_resp || !batt_resp || !usb_resp || !sensor_resp) {
    if (uxQueueMessagesWaiting(main_mainStateRespQueue) > 0) {
        xReturned = xQueueReceive(main_mainStateRespQueue, &task_recv, 0);
        if (xReturned != pdPASS) {
          printf("USB: Unable to receive main state update response from main_mainStateRespQueue\n");
        }
        switch(task_recv) {
        case LOGGER:
          logger_resp = true;
          printf("MAIN: Logger Task has updated its main state.\n");
          break;
        case BATTERY:
          batt_resp = true;
          printf("MAIN: Battery Task has updated its main state.\n");
          break;
        case USB:
          usb_resp = true;
          printf("MAIN: USB Task has updated its main state.\n");
          break;
        case SENSORS:
          sensor_resp = true;
          printf("MAIN: Sensor Task has updated its main state.\n");
          break;
        default:
          break;
        }
    }
  }

  printf("MAIN: All tasks updated... sending continue requests.\n");
   
  // Send Continues to Tasks that need it
  xReturned = xQueueSend(logger_mainStateContinueQueue, &main_state_cont, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state continue to logger_mainStateContinueQueue.\n");
  }
  xReturned = xQueueSend(battery_mainStateContinueQueue, &main_state_cont, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state continue to battery_mainStateContinueQueue.\n");
  }
  xReturned = xQueueSend(sensor_mainStateContinueQueue, &main_state_cont, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state continue to sensor_mainStateContinueQueue.\n");
  }
  xReturned = xQueueSend(usb_mainStateContinueQueue, &main_state_cont, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state continue to usb_mainStateContinueQueue.\n");
  }

}

void begin_amplification_zone(void) {
  BaseType_t xReturned;
  // Send start zone request
  xReturned = xQueueSend(heater_zoneRunQueue, &run_amplification_zone, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send run amplification zone request.\n");
  }
  // Send Start Amplification Event to logging task
  xReturned = xQueueSend(logger_logMessageQueue, &amplification_start_log_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send start amplification zone event to logging task.\n");
  }
}

void begin_valve_zone(void) {
  BaseType_t xReturned;
  // Send start valve message to heater queue
  xReturned = xQueueSend(heater_zoneRunQueue, &run_valve_zone, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send run valve zone request.\n");
  }
  // Send Start Valve Event to logging task
  xReturned = xQueueSend(logger_logMessageQueue, &valve_start_log_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send start valve zone event to logging task.\n");
  }
}

void end_amplification_zone(void) {
  BaseType_t xReturned;
  // Send stop amplification message to heater queue
  xReturned = xQueueSend(heater_zoneRunQueue, &stop_amplification_zone, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send stop amplification zone request.\n");
  }
  // Send stop amplification Event to logging task
  xReturned = xQueueSend(logger_logMessageQueue, &amplification_stop_log_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send stop amplification zone event to logging task.\n");
  }
}

void end_valve_zone(void) {
  BaseType_t xReturned;
  // Send valve zone stop request
  xReturned = xQueueSend(heater_zoneRunQueue, &stop_valve_zone, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send stop valve zone request.\n");
  }
  // Send stop valve Event to logging task
  xReturned = xQueueSend(logger_logMessageQueue, &valve_stop_log_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send stop valve zone event to logging task.\n");
  }
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
  xReturned = xTaskCreate(main_task, "MainTask", 1024, NULL, 0, &mainTaskHandle);
  if( xReturned != pdPASS ) {
      // The task was created.  Use the task's handle to delete the task. 
      printf("Error creating main task. Error: %d\n", xReturned);
      vTaskDelete( mainTaskHandle );
  }
  // Heater Task
  xReturned = xTaskCreate(heater_task, "HeaterTask", 1024, NULL, 0, &heaterTaskHandle);
  if( xReturned != pdPASS ) {
      // The task was created.  Use the task's handle to delete the task. 
      printf("Error creating heater task. Error: %d\n", xReturned);
      vTaskDelete( heaterTaskHandle );
  }
  // Logger Task
  xReturned = xTaskCreate(logger_task, "LoggerTask", 1024, NULL, 0, &loggerTaskHandle);
  if( xReturned != pdPASS ) {
      // The task was created.  Use the task's handle to delete the task. 
      printf("Error creating logger task. Error: %d\n", xReturned);
      vTaskDelete( loggerTaskHandle );
  }
  // Sensors Task
  xReturned = xTaskCreate(sensors_task, "SensorsTask", 1024, NULL, 0, &sensorsTaskHandle);
  if( xReturned != pdPASS ) {
      // The task was created.  Use the task's handle to delete the task. 
      printf("Error creating sensors task. Error: %d\n", xReturned);
      vTaskDelete( sensorsTaskHandle );
  }
  // Battery Management Task
  xReturned = xTaskCreate(battery_task, "BatteryTask", 1024, NULL, 0, &batteryTaskHandle);
  if( xReturned != pdPASS ) {
      // The task was created.  Use the task's handle to delete the task. 
      printf("Error creating battery management task. Error: %d\n", xReturned);
      vTaskDelete( batteryTaskHandle );
  }
  // USB Management Task
  xReturned = xTaskCreate(usb_task, "USBTask", 1024, NULL, 0, &usbTaskHandle);
  if( xReturned != pdPASS ) {
      // The task was created.  Use the task's handle to delete the task. 
      printf("Error creating usb management task. Error: %d\n", xReturned);
      vTaskDelete( usbTaskHandle );
  }
  // PWM Task
  xReturned = xTaskCreate(pwm_task, "PWMTask", 1024, NULL, 0, &pwmTaskHandle);
  if ( xReturned != pdPASS ) {
      // The task was created.  Use the task's handle to delete the task. 
      printf("Error creating PWM task. Error: %d\n", xReturned);
      vTaskDelete( pwmTaskHandle );
  }
  // USB Composite Task
  xReturned = xTaskCreate(composite_usb_task, "CompositeUSBTask", 1024, NULL, 0, &compositeTaskHandle);
  if ( xReturned != pdPASS ) {
      // The task was created.  Use the task's handle to delete the task. 
      printf("Error creating Composite USB task. Error: %d\n", xReturned);
      vTaskDelete( compositeTaskHandle );
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
  main_mainStateRespQueue = xQueueCreate(4, sizeof(tasks_t));
  if (main_switchQueue == NULL)
    printf("Unable to create main_mainStateRespQueue queue\n");

  // Heater Task Queues
  heater_zoneRunQueue = xQueueCreate(QUEUE_SIZE, sizeof(zone_run_req_t));
  if (heater_zoneRunQueue == NULL)
    printf("Unable to create heater_zoneRunQueue queue\n");
  heater_temperatureDataQueue = xQueueCreate(QUEUE_SIZE, sizeof(temperature_data_t));
  if (heater_temperatureDataQueue == NULL)
    printf("Unable to create heater_temperatureDataQueue queue\n");
  heater_usbWaitQueue = xQueueCreate(QUEUE_SIZE, sizeof(usb_suspend_req_t));
  if (heater_usbWaitQueue == NULL)
    printf("Unable to create heater_usbWaitQueue queue\n");

  // Sensor Task Queues
  sensor_mainStateQueue = xQueueCreate(QUEUE_SIZE, sizeof(main_state_t));
  if (sensor_mainStateQueue == NULL)
    printf("Unable to create sensor_mainStateQueue queue\n");
  sensor_usbWaitQueue = xQueueCreate(QUEUE_SIZE, sizeof(usb_suspend_req_t));
  if (sensor_usbWaitQueue == NULL)
    printf("Unable to create sensor_usbWaitQueue queue\n");
  sensor_mainStateContinueQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (sensor_mainStateContinueQueue == NULL)
    printf("Unable to create sensor_mainStateContinueQueue queue\n");

  // Battery Management Task Queues
  battery_requestPercentQueue = xQueueCreate(QUEUE_SIZE, sizeof(battery_percent_req_t));
  if (battery_requestPercentQueue == NULL)
    printf("Unable to create battery_requestPercentQueue queue\n");
  battery_mainStateQueue = xQueueCreate(QUEUE_SIZE, sizeof(main_state_t));
  if (battery_mainStateQueue == NULL)
    printf("Unable to create battery_mainStateQueue queue\n");
  battery_usbWaitQueue = xQueueCreate(QUEUE_SIZE, sizeof(usb_suspend_req_t));
  if (battery_usbWaitQueue == NULL)
    printf("Unable to create battery_usbWaitQueue queue\n");
  battery_mainStateContinueQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (battery_mainStateContinueQueue == NULL)
    printf("Unable to create battery_mainStateContinueQueue queue\n");

  // USB Management Task Queues
  usb_stateChangeQueue = xQueueCreate(QUEUE_SIZE, sizeof(usb_message_t));
  if (usb_stateChangeQueue == NULL)
    printf("Unable to create usb_stateChangeQueue queue\n");
  usb_recvUsbWaitAcceptQueue = xQueueCreate(4, sizeof(usb_suspend_acpt_t));
  if (usb_recvUsbWaitAcceptQueue == NULL)
    printf("Unable to create usb_recvUsbWaitAcceptQueue queue\n");
  usb_usbWaitOverQueue = xQueueCreate(4, sizeof(usb_suspend_over_t));
  if (usb_usbWaitOverQueue == NULL)
    printf("Unable to create usb_usbWaitOverQueue queue\n");
  usb_mainStateContinueQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (usb_mainStateContinueQueue == NULL)
    printf("Unable to create usb_mainStateContinueQueue queue\n");

  // Logger Task Queues
  logger_recvBattPercentQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (logger_recvBattPercentQueue == NULL)
    printf("Unable to create logger_recvBattPercentQueue queue\n");
  logger_logMessageQueue = xQueueCreate(2, sizeof(log_data_message_t));
  if (logger_logMessageQueue == NULL)
    printf("Unable to create logger_logMessageQueue queue\n");
  logger_mainStateChangeQueue = xQueueCreate(QUEUE_SIZE, sizeof(main_state_t));
  if (logger_mainStateChangeQueue == NULL)
    printf("Unable to create logger_mainStateChangeQueue queue\n");
  logger_mainStateContinueQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (logger_mainStateContinueQueue == NULL)
    printf("Unable to create logger_mainStateContinueQueue queue\n");

  // PWM Task Queues
  pwm_usbWaitQueue = xQueueCreate(QUEUE_SIZE, sizeof(usb_suspend_req_t));
  if (pwm_usbWaitQueue == NULL)
    printf("Unable to create pwm_usbWaitQueue queue\n");

}

// Stack Overflow detection.
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName ){
  printf("FreeRTOS Stack Overflow Has Occured.");
}

/*********************************************************************
*
*       main()
*
*   Application entry point.
*/
 int main(void) {
  ret_code_t err_code;
  FRESULT res;

  // Initialize clock driver for better time accuracy in FREERTOS
  err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  nrf_drv_clock_lfclk_request(NULL);

  while(!nrf_drv_clock_lfclk_is_running())
  {
      // Just waiting
  }

  // Full Peripheral Initalizations
  init_adc();             // ADC
  init_sensors_gpios();   // Sensor GPIOs
  vInit_TWI_Hardware(i2c_interface_system, I2C1_SDA_PIN, I2C1_SCL_PIN, i2c_speed_400k);   // I2C
  vInit_TWI_Hardware(i2c_interface_sensors, I2C0_SDA_PIN, I2C0_SCL_PIN, i2c_speed_400k);   // I2C
  init_sd_card();

  // Get the configuration parameters
  res = get_naatos_configuration_parameters(&config);
  if (res != FR_OK) {
    printf("Warning: configuration file was not able to be read. Using default configuration parameters.");
    use_default_configuration_parameters = true;
  }

  // Uninitalize the SD card
  uninit_sd_card();

  // Create Tasks
  create_tasks();

  // Create Queues
  create_queues();

  // Start Tasks
  vTaskStartScheduler();
}


/*************************** End of file ****************************/

