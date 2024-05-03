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

  // Set Start up state to standby
  main_state_t main_state = STANDBY;
  // TODO: Get Configuration Settings

  int i = 0;
  
  // Main State Loop
  for (;;) {
    switch(main_state) {
      // In Standby State
      case STANDBY:
        if (error_during_run) error_during_run = false;
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
          i++;
        } while ( i < 50/*TODO: Compare current time against AMPLIFICATION_ON_TIME*/);
        i = 0;
        
        /* ***** Valve Zone Run **** */
        // Send valve zone start request
        begin_valve_zone();
        // Small delay
        vTaskDelay(10);
        // Send amplification zone stop request
        end_amplification_zone();
        // Check for errors
        if (error_during_run) {
            vTaskDelay(10);
            end_valve_zone();
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
          i++;
        } while (i < 50/*TODO: Compare current time against VALVE_ON_TIME*/);
        i=0;
        // Stop the valve zone
        end_valve_zone();
        // Check for errors
        if (error_during_run) {
            break;
        }

        // TODO: might have to do small delay here for heater task to update
        /* ***** End Sample Preperation ***** */
        // Update sensor state prematuraly to stop sending of temp data
        main_state = STANDBY;
        xReturned = xQueueSend(sensor_mainStateQueue, &main_state, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send main state change to heater_mainStateQueue.\n");
        }
        // Wait for the sample to be removed prior to going back to STANDBY state
        do {
          xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
          hal_triggered = switch_data.hal_triggered;
          optical_triggered = switch_data.optical_tiggered;
        } while(hal_triggered && optical_triggered);
        // Update current main state
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
  
  // Send the state to the logger
  xReturned = xQueueSend(logger_mainStateChangeQueue, &new_state, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state change to logger_mainStateChangeQueue.\n");
  }

  // Send the state to the sensor task
  xReturned = xQueueSend(sensor_mainStateQueue, &new_state, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send main state change to heater_mainStateQueue.\n");
  }

  // TODO: might have to do small delay here for usb task to update
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
  xReturned = xTaskCreate(usb_task, "USBTask", 300, NULL, 2, &usbTaskHandle);
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

  // Sensor Task Queues
  sensor_mainStateQueue = xQueueCreate(QUEUE_SIZE, sizeof(main_state_t));
  if (sensor_mainStateQueue == NULL)
    printf("Unable to create sensor_mainStateQueue queue\n");

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
  logger_logMessageQueue = xQueueCreate(2, sizeof(log_data_message_t));
  if (logger_logMessageQueue == NULL)
    printf("Unable to create logger_logMessageQueue queue\n");
  logger_mainStateChangeQueue = xQueueCreate(QUEUE_SIZE, sizeof(main_state_t));
  if (logger_mainStateChangeQueue == NULL)
    printf("Unable to create logger_mainStateChangeQueue queue\n");
}


#define READ_SIZE 1
#define CDC_ACM_COMM_INTERFACE  0
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN2

#define CDC_ACM_DATA_INTERFACE  1
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT1

#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif



static char m_rx_buffer[READ_SIZE];
static char m_tx_buffer[NRF_DRV_USBD_EPSIZE];
static bool m_send_flag = 0;

static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event);

APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            cdc_acm_user_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);

static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_DRV_SUSPEND:
            //bsp_board_led_off(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_DRV_RESUME:
            //bsp_board_led_on(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            bsp_board_leds_off();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            printf("USB power detected");

            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            printf("USB power removed");
            app_usbd_stop();
            break;
        case APP_USBD_EVT_POWER_READY:
            printf("USB ready");
            app_usbd_start();
            break;
        default:
            break;
    }
}
/*
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            cdc_acm_user_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);
*/

static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event)
{
    app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

    switch (event)
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            //bsp_board_led_on(LED_CDC_ACM_OPEN);

            //Setup first transfer
            ret_code_t ret = app_usbd_cdc_acm_read(&m_app_cdc_acm,
                                                   m_rx_buffer,
                                                   READ_SIZE);
            UNUSED_VARIABLE(ret);
            break;
        }
        case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
            //bsp_board_led_off(LED_CDC_ACM_OPEN);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
           // bsp_board_led_invert(LED_CDC_ACM_TX);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            ret_code_t ret;
            NRF_LOG_INFO("Bytes waiting: %d", app_usbd_cdc_acm_bytes_stored(p_cdc_acm));
            do
            {
                //Get amount of data transfered
                size_t size = app_usbd_cdc_acm_rx_size(p_cdc_acm);
                NRF_LOG_INFO("RX: size: %lu char: %c", size, m_rx_buffer[0]);

                // Fetch data until internal buffer is empty 
                ret = app_usbd_cdc_acm_read(&m_app_cdc_acm,
                                            m_rx_buffer,
                                            READ_SIZE);
            } while (ret == NRF_SUCCESS);

            //bsp_board_led_invert(LED_CDC_ACM_RX);
            break;
        }
        default:
            break;
    }
}

void start_usb() {
   ret_code_t ret;
    static const app_usbd_config_t usbd_config = {
        .ev_state_proc = usbd_user_ev_handler
    };

    app_usbd_serial_num_generate();

    ret = app_usbd_init(&usbd_config);
    APP_ERROR_CHECK(ret);
    printf("USBD CDC ACM example started.");

    app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
    ret = app_usbd_class_append(class_cdc_acm);
    APP_ERROR_CHECK(ret);

    if (USBD_POWER_DETECTION)
    {
        ret = app_usbd_power_events_enable();
        APP_ERROR_CHECK(ret);
    }
    else
    {
        printf("No USB power detection enabled\r\nStarting USB now");

        app_usbd_enable();
        app_usbd_start();
    }

    while (true)
    {
        while (app_usbd_event_queue_process())
        {
            // Nothing to do 
        }
    }
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

  nrf_drv_clock_lfclk_request(NULL);

  while(!nrf_drv_clock_lfclk_is_running())
  {
      // Just waiting
  }

  // Start the USB
  start_usb();
  while(!usb_started);

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

