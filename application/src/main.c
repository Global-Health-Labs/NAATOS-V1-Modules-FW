/*********************************************************************
*                               NAATOS                               *
**********************************************************************
File    : main.c
Purpose : NAATOS Application Start
SDK Version: 17.1
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../pwm/pwm.h"
#include "FreeRTOS.h"
#include "adc.h"
#include "app_error.h"
#include "battery.h"
#include "bsp.h"
#include "cycle_state_fsm.h"
#include "fuel.h"
#include "heater/heater.h"
#include "i2c_hal_freertos.h"
#include "led.h"
#include "logger/logger.h"
#include "motor.h"
#include "naatos_config.h"
#include "naatos_messages.h"
#include "naatos_queues.h"
#include "nordic_common.h"
#include "nrf_drv_clock.h"
#include "pid.h"
#include "sd_card.h"
#include "sdk_errors.h"
#include "sensors.h"
#include "spi.h"
#include "states.h"
#include "switch.h"
#include "task.h"
#include "timers.h"
#include "usb.h"
#include "watchdog.h"

#include "core_cm4.h"

#include "core_cm4.h"
#include "nrf_bootloader_info.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_power.h"
#include "nrf_pwr_mgmt.h"

#include "app_error.h"
#include "app_usbd.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_core.h"
#include "app_usbd_serial_num.h"
#include "app_usbd_string_desc.h"
#include "app_util.h"

// Task Handles
xTaskHandle mainTaskHandle;
xTaskHandle heaterTaskHandle;
xTaskHandle loggerTaskHandle;
xTaskHandle sensorsTaskHandle;
xTaskHandle batteryTaskHandle;
xTaskHandle usbTaskHandle;
xTaskHandle pwmTaskHandle;
xTaskHandle wdtTaskHandle;
xTaskHandle compositeTaskHandle;
xTaskHandle buttonTaskHandle;
xTaskHandle ledTaskHandle;

xQueueHandle main_batteryDataQueue;
xQueueHandle main_switchQueue;
xQueueHandle main_mainStateRespQueue;
xQueueHandle main_runRespQueue;
xQueueHandle main_runErrorQueue;
xQueueHandle main_runConfRespQueue;
xQueueHandle button_mainStateQueue;
xQueueHandle main_usbConnRecvQueue;
xQueueHandle main_usbChangedConfQueue;
xQueueHandle main_wakeupTasksQueue;
xQueueHandle main_setPointReached;

// Configuration Parameters
#ifndef SAMPLE_PREP_BOARD
naatos_config_parameters config = {
    .logging_rate = 0,
    .sample_rate = 0,
    .amplification_zone_run_time_m = 0,
    .valve_zone_run_time_m = 0,
    .sample_valid_timeout_s = 0,
    .sample_complete_delay_s = 0,
    .low_power_threshold = 0,
    .valve_setpoint = 0,
    .amp0_setpoint = 0,
    .amp1_setpoint = 0,
    .amp2_setpoint = 0,
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
    .amp2_kd = 0,
    .valve_setpoint_2 = 0,
    .amp0_setpoint_2 = 0,
    .amp1_setpoint_2 = 0,
    .amp2_setpoint_2 = 0,
    .valve_kp_2 = 0,
    .valve_ki_2 = 0,
    .valve_kp_2 = 0,
    .amp0_kp_2 = 0,
    .amp0_ki_2 = 0,
    .amp0_kd_2 = 0,
    .amp1_kp_2 = 0,
    .amp1_ki_2 = 0,
    .amp1_kd_2 = 0,
    .amp2_kp_2 = 0,
    .amp2_ki_2 = 0,
    .amp2_kd_2 = 0};
#else
naatos_config_parameters config = {
    .logging_rate = 0,
    .sample_rate = 0,
    .cycle_1_run_time_m = 0,
    .cycle_2_run_time_m = 0,
    .sample_valid_timeout_s = 0,
    .sample_complete_delay_s = 0,
    .low_power_threshold = 0,
    .heater_setpoint_1 = 0,
    .heater_setpoint_2 = 0,
    .heater_kp_1 = 0,
    .heater_ki_1 = 0,
    .heater_kd_1 = 0,
    .heater_kp_2 = 0,
    .heater_ki_2 = 0,
    .heater_kd_2 = 0,
    .motor_kp_1 = 0,
    .motor_ki_1 = 0,
    .motor_kd_1 = 0,
    .motor_setpoint_1 = 0,
    .motor_kp_2 = 0,
    .motor_ki_2 = 0,
    .motor_kd_2 = 0,
    .motor_setpoint_2 = 0,
    .run_motor_1 = false,
    .run_heater_1 = false,
    .run_motor_2 = false,
    .run_heater_2 = false};
#endif
bool use_default_configuration_parameters = false;

// Function defs
void sendWdtHeaterInvalid();
void sendWdtMain(bool valid);
bool begin_amplification_zone(void);
void end_amplification_zone(void);
void begin_valve_zone(void);
void end_valve_zone(void);
void create_tasks(void);
void send_usb_change(usb_command_t cmd);
void reset_and_enter_dfu(void);

xTaskHandle get_usb_task_handle(void) {
  return usbTaskHandle;
}

// Function for setting general purpose register for entering DFU
void reset_and_enter_dfu(void) {
  uint32_t volatile *const p_gpreg1 = (uint32_t volatile *const)0x4000051C;
  // Clear then set (matches behavior when softdevice is used)
  *p_gpreg1 = 0;
  *p_gpreg1 = BOOTLOADER_DFU_START;

  // Device will enter bootloader on next reset,
  // use this line of code to perform the reset:
  NVIC_SystemReset();
  asm volatile("nop");
}

void read_sd_and_notify_tasks(void) {
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterConfigMsg = {
      .type = HEATER_MSG_CONFIG_UPDATED,
  };

  SensorRxQueueMsg_t sensorConfigMsg = {
      .type = CONFIG_UPDATED,
  };

  xReturned = get_naatos_configuration_parameters(&config);
  if (xReturned != FR_OK) {
    printf("Warning: configuration file was not able to be read. Using default configuration parameters.");
    use_default_configuration_parameters = true;
  } else {
    use_default_configuration_parameters = false;
  }

  xReturned = xQueueSend(heaterRxQueue, &heaterConfigMsg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send run amplification zone request.\n");
  }

  // Send to Sensors task
  xReturned = xQueueSend(sensorRxQueue, &sensorConfigMsg, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send usb suspend request to sensorRxQueue.\n");
  }
}

/*********************************************************************
*
*       main_task()
*
*   Main Task of NAATOS Application
*/
void main_task(void *pvParameters) {
  BaseType_t xReturned;
  uint8_t queue_size;
  sensor_switches_t switch_data = {.optical_tiggered = false};
  int percent_recv;
  button_update_t buttonData = {.event = NONE};
  bool hal_triggered = false, optical_triggered = false;
  bool error_during_run = false;
  bool over_temp;
  uint32_t start_time = 0, end_time = 0, a_t_start = 0;
  uint32_t alert_timeout_ticks;
  bool usb_conn_status = false;
  bool usb_needs_update = false;
  bool wake_up = false;
  int mainWatchDogKickCount = 0;
  const BatteryRxQueueMsg_t batt_req = {
      .type = BATTERY_SOC_REQUEST,
      .sendTo = BATTERY_MSG_SOC_MAIN};
  BaseType_t xHigherPriorityTaskWoken = pdTRUE;

  // Set Start up state to standby
  main_state_t main_state = MAIN_SLEEP;
  main_state_t next_state = MAIN_STANDBY;
  main_state_t last_state = MAIN_SLEEP;
  int i = 0;

  // Get the alert timeout
  if (!use_default_configuration_parameters) {
    alert_timeout_ticks = (uint32_t)(pdMS_TO_TICKS(config.alert_timeout_time_s * 1000.0));
  } else {
    alert_timeout_ticks = (uint32_t)(pdMS_TO_TICKS(DEFAULT_ALERT_TIMEOUT_S * 1000.0));
  }

  uint32_t main_wdt_start_time = 0;
  uint32_t main_wdt_end_time = pdMS_TO_TICKS(1000);
  uint32_t main_wdt_time_left = 0;

  create_tasks();

  // Main State Loop
  for (;;) {
    last_state = main_state;
    main_state = next_state;

    switch (main_state) {
    // In Standby State
    case MAIN_STANDBY:
      if (last_state != main_state) {
        sendWdtHeaterInvalid(); // Invalidate the heater to stop WDT from watching it

        SensorRxQueueMsg_t msg;
        msg.type = SENSOR_MSG_WAKEUP;

        // Send to Sensors task
        xReturned = xQueueSend(sensorRxQueue, &msg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN: Unable to send sensor wakeup to sensorRxQueue.\n");
        }

        vTaskDelay(300);
        read_sd_and_notify_tasks();
        // Get the alert timeout
        if (!use_default_configuration_parameters) {
          alert_timeout_ticks = (uint32_t)(pdMS_TO_TICKS(config.alert_timeout_time_s * 1000.0));
        } else {
          alert_timeout_ticks = (uint32_t)(pdMS_TO_TICKS(DEFAULT_ALERT_TIMEOUT_S * 1000.0));
        }
        if (config.set_date_time) {
          calendar_set_time_helper();
          sd_card_reset_set_time_date();
        }

        updateLedState(LED_WAKEUP, true);
        updateLedState(LED_RUN, false);
        updateLedState(LED_STANDBY, true);
        updateLedState(LED_COMPLETE, false);
        sendWdtMain(true);
        start_time = xTaskGetTickCount();
      }

      hal_triggered = false;
      optical_triggered = false;

      // Request the battery percentage from the bettery task
      xReturned = xQueueSend(batteryRxQueue, &batt_req, 0);
      if (xReturned != pdPASS) {
        printf("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.\n");
      }

      // Check for Battery Data in Battery Queue
      if (xQueueReceive(main_batteryDataQueue, &percent_recv, pdMS_TO_TICKS(100)) == pdPASS) {
        if ((percent_recv < DEFAULT_LOW_POWER_THRESHOLD && use_default_configuration_parameters) || (!use_default_configuration_parameters && percent_recv < config.low_power_threshold)) {
          /*next_state = MAIN_SLEEP;
             sendUpdatedMainTaskState(next_state);
             break;*/
        }
      }

      /* **** HANDLE USB AND SWITCH **** */
      // Get switch status if it has changed
      if (xQueueReceive(button_mainStateQueue, &buttonData, 0) == pdPASS) {
        usbRxMsgType_t conn_req_msg = {
            .cmd = NULL,
            .msg_type = USB_MSG_CONN_STATUS_REQ};
        // Get The USB Connection Status
        xReturned = xQueueSend(usbRxQueue, &conn_req_msg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send usb connection request to usbRxQueue. \n");
        }
        // Receive the USB Connection Status
        xReturned = xQueueReceive(main_usbConnRecvQueue, &usb_conn_status, portMAX_DELAY);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to receive usb connection status from main_usbConnRecvQueue. \n");
        }
        usb_needs_update = true;
      }
      // Get if USB status has changed
      if (xQueueReceive(main_usbConnRecvQueue, &usb_conn_status, 0) == pdPASS) {
        usb_needs_update = true;
      }

      if (buttonData.event == BOOTLOADER_EVENT) {
        sendWdtMain(false);
        next_state = MAIN_BOOTLOADER;
        break;
      }

#ifdef DIRECT_TO_USB
      next_state = MAIN_FILE;
      updateLedState(LED_RUN, false);
      updateLedState(LED_STANDBY, false);
      updateLedState(LED_USB_MSC_STARTING, true);
      send_usb_change(USB_MSC_CDC_ACM);
      break;
#endif

#ifdef DIRECT_TO_RUN
      next_state = MAIN_RUNNING;
      sendUpdatedMainTaskState(next_state);
      // Delay
      vTaskDelay(100);
      break;
#endif

      if (usb_needs_update) {
        // Switch OFF and USB connected
        if (buttonData.event == OFF_EVENT && usb_conn_status) {
          next_state = MAIN_FILE;
          sendWdtMain(false);
          updateLedState(LED_RUN, false);
          updateLedState(LED_STANDBY, false);
          updateLedState(LED_USB_MSC_STARTING, true);
          send_usb_change(USB_MSC_CDC_ACM);
          break;
        }
        // Switch off and USB not connected
        else if (buttonData.event == OFF_EVENT && !usb_conn_status) {
          sendWdtMain(false);
          next_state = MAIN_SLEEP;
          send_usb_change(USB_DISABLED);
        }
        usb_needs_update = false;
      }

      // Wait for Sensor Switch Data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Error receiving switch data from main_switchQueue\n");
      }

      // Update switches triggered
      hal_triggered = switch_data.hal_triggered;
      optical_triggered = switch_data.optical_tiggered;

#ifdef SAMPLE_PREP_BOARD
      // Check if we can go to RUN state
      if (hal_triggered && buttonData.event == ON_EVENT && !error_during_run) {
        next_state = MAIN_RUNNING;
        // Delay
        vTaskDelay(100);
      }
#else
      // Check if we can go to RUN state
      if (hal_triggered && optical_triggered && !error_during_run) {
        next_state = MAIN_RUNNING;
        // Delay
        vTaskDelay(100);
      }
#endif
      buttonData.event = NONE;

      main_wdt_time_left = pdTICKS_TO_MS(xTaskGetTickCount() - main_wdt_start_time);

      if (main_wdt_time_left >= main_wdt_end_time) {
        sendWdtMain(true);
        main_wdt_start_time = xTaskGetTickCount();
        main_wdt_time_left = 0;
      }

      break;

    // In Running State (Will block task for the duration of the test)
    case MAIN_RUNNING: {
      if (last_state != main_state) {
        reset_cycle_state_machine();
        updateLedState(LED_STANDBY, false);
        updateLedState(LED_RUN, true);
      }

      main_wdt_time_left = pdTICKS_TO_MS(xTaskGetTickCount() - main_wdt_start_time);

      if (main_wdt_time_left >= main_wdt_end_time) {
        sendWdtMain(true);
        main_wdt_start_time = xTaskGetTickCount();
        main_wdt_time_left = 0;
      }

      cycle_state_exit_t cycle_exit_info = run_cycle_state_machine();

      if (cycle_exit_info == CYCLE_COMPLETE) {
        next_state = MAIN_STANDBY;
        break;
      }

      if (cycle_exit_info == CYCLE_RUNNING) {
        next_state = MAIN_RUNNING;
      } else {
        next_state = MAIN_ALERT;
      }

      break;
    }

    case MAIN_ALERT: {
      if (last_state != main_state) {
        updateLedState(LED_WAKEUP, true);
        updateLedState(LED_RUN, false);
        updateLedState(LED_STANDBY, true);
        updateLedState(LED_COMPLETE, false);
        a_t_start = xTaskGetTickCount();
        printf("MAIN_TASK: Alert Timeout - %dms\n", pdTICKS_TO_MS(alert_timeout_ticks));
      }

      main_wdt_time_left = pdTICKS_TO_MS(xTaskGetTickCount() - main_wdt_start_time);

      if (main_wdt_time_left >= main_wdt_end_time) {
        sendWdtMain(true);
        main_wdt_start_time = xTaskGetTickCount();
        main_wdt_time_left = 0;
      }

      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY); // TODO do we do  anything with this?

      if (xTaskGetTickCount() >= (a_t_start + alert_timeout_ticks)) {
        updateLedState(LED_CLEAR_ALL_ERROR, true);
        updateLedState(LED_WAKEUP, true);
        updateLedState(LED_RUN, false);
        updateLedState(LED_STANDBY, true);
        updateLedState(LED_COMPLETE, false);
        next_state = MAIN_STANDBY;
      }

      break;
    }

    case MAIN_FILE:
      if (last_state != main_state) {
        SensorRxQueueMsg_t msg;
        msg.type = SENSOR_MSG_SLEEP;
        xReturned = xQueueSend(sensorRxQueue, &msg, 0);
        if (xReturned != pdPASS) {
          printf("Sensor: Unable to send timer update to sensorRxQueue queue from main.\n");
        }
        mainWatchDogKickCount = 0;
        sendWatchdogKickFromTask(MAIN, true);
      }

      if (mainWatchDogKickCount++ >= 10) {
        mainWatchDogKickCount = 0;
        sendWatchdogKickFromTask(MAIN, true);
      }

      /* **** HANDLE USB AND SWITCH **** */
      // Get switch status if it has changed
      if (xQueueReceive(button_mainStateQueue, &buttonData, 0) == pdPASS) {
        usbRxMsgType_t conn_req_msg = {
            .cmd = NULL,
            .msg_type = USB_MSG_CONN_STATUS_REQ};
        // Get The USB Connection Status
        xReturned = xQueueSend(usbRxQueue, &conn_req_msg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send usb connection request to usbRxQueue. \n");
        }
        // Receive the USB Connection Status
        xReturned = xQueueReceive(main_usbConnRecvQueue, &usb_conn_status, portMAX_DELAY);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to receive usb connection status from main_usbConnRecvQueue. \n");
        }
        usb_needs_update = true;
      }
      // Get if USB status has changed
      if (xQueueReceive(main_usbConnRecvQueue, &usb_conn_status, 0) == pdPASS) {
        usb_needs_update = true;
      }

      if (buttonData.event == BOOTLOADER_EVENT) {
        next_state = MAIN_BOOTLOADER;
        break;
      }

      if (usb_needs_update) {
        // Switch ON and USB connected
        if (buttonData.event == ON_EVENT && usb_conn_status) {
          buttonData.event = NONE;
          updateLedState(LED_USB_MSC_STARTING, false);
          next_state = MAIN_STANDBY;
          send_usb_change(USB_CDC_ACM);
          sendWatchdogKickFromTask(MAIN, false);
        }
        // Switch ON and USB not connected
        else if (buttonData.event == ON_EVENT && !usb_conn_status) {
          buttonData.event = NONE;
          updateLedState(LED_USB_MSC_STARTING, false);
          next_state = MAIN_STANDBY;
          send_usb_change(USB_DISABLED);
          sendWatchdogKickFromTask(MAIN, false);
        }
        // Switch OFF and USB connected
        else if (buttonData.event == OFF_EVENT && usb_conn_status) {
          //next_state = MAIN_FILE;
          //send_usb_change(USB_MSC_CDC_ACM);
        }
        // Switch off and USB not connected
        else if (buttonData.event == OFF_EVENT && !usb_conn_status) {
          updateLedState(LED_USB_MSC_STARTING, false);
          next_state = MAIN_SLEEP;
          send_usb_change(USB_DISABLED);
          sendWatchdogKickFromTask(MAIN, false);
        }
        // Updated USB Connection Status
        else if (buttonData.event == NONE && !usb_conn_status) {
          updateLedState(LED_USB_MSC_STARTING, false);
          next_state = MAIN_STANDBY;
          send_usb_change(USB_DISABLED);
        }
        usb_needs_update = false;
      }
      vTaskDelay(pdMS_TO_TICKS(100));
      break;

    // In Low Power State
    case MAIN_SLEEP: {
      if (last_state != main_state) {
        updateLedState(LED_RUN, false);
        updateLedState(LED_STANDBY, false);
        updateLedState(LED_USB_MSC_STARTING, false);
        updateLedState(LED_WAKEUP, false);
        printf("Going to sleep...\n");
        SensorRxQueueMsg_t msg;
        msg.type = SENSOR_MSG_SLEEP;
        xReturned = xQueueSend(sensorRxQueue, &msg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send sensor sleep to sensorRxQueue queue.\n");
        }

        BatteryRxQueueMsg_t battMsg;
        msg.type = BATTERY_MSG_SLEEP;

        xReturned = xQueueSend(batteryRxQueue, &battMsg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send battery sleep to batteryRxQueue.\n");
        }

        ButtonRxQueueMsg_t buttonMsg;
        buttonMsg.type = BUTTON_MSG_SLEEP;

        xReturned = xQueueSend(buttonRxQueue, &buttonMsg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send button sleep to batteryRxQueue.\n");
        }

        usbRxMsgType_t usbMsg;
        usbMsg.msg_type = USB_MSG_SLEEP;
        xReturned = xQueueSend(usbRxQueue, &usbMsg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send usb sleep to usbRxQueue. \n");
        }

        CompositeUSBRxQueueType_t compositeMsg = COMPOSITE_MSG_SLEEP;
        xReturned = xQueueSend(compositeRxQueue, &compositeMsg, 0);
        if (xReturned != pdPASS) {
          printf("MAIN_TASK: Unable to send composite sleep to usbRxQueue. \n");
        }

        updateLedState(LED_RUN, false);
        updateLedState(LED_STANDBY, false);
        updateLedState(LED_USB_MSC_STARTING, false);

        vTaskDelay(100);
        nrf_gpio_pin_clear(SENSORS_EN);
      }

      // this queue is blocked indefinitly until a switch interrupt or usb  interrupt
      xReturned = xQueueReceive(main_wakeupTasksQueue, &wake_up, portMAX_DELAY);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to receive tasks wakeup from main_wakeupTasksQueue. \n");
      }

      printf("Waking up...\n");

      nrf_gpio_pin_set(SENSORS_EN);
      vTaskDelay(100);

      SensorRxQueueMsg_t msg;
      msg.type = SENSOR_MSG_WAKEUP;
      xReturned = xQueueSend(sensorRxQueue, &msg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send sensor sleep to sensorRxQueue queue.\n");
      }

      BatteryRxQueueMsg_t battMsg;
      msg.type = BATTERY_MSG_WAKEUP;

      xReturned = xQueueSend(batteryRxQueue, &battMsg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send battery sleep to batteryRxQueue.\n");
      }

      ButtonRxQueueMsg_t buttonMsg;
      buttonMsg.type = BUTTON_MSG_WAKE;

      xReturned = xQueueSend(buttonRxQueue, &buttonMsg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send button sleep to batteryRxQueue.\n");
      }

      CompositeUSBRxQueueType_t compositeMsg = COMPOSITE_MSG_WAKEUP;
      xReturned = xQueueSend(compositeRxQueue, &compositeMsg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send composite sleep to usbRxQueue. \n");
      }

      usbRxMsgType_t usbMsg;
      usbMsg.msg_type = USB_MSG_WAKEUP;
      xReturned = xQueueSend(usbRxQueue, &usbMsg, 0);
      if (xReturned != pdPASS) {
        printf("MAIN_TASK: Unable to send usb sleep to usbRxQueue. \n");
      }

      next_state = MAIN_STANDBY;

      break;
    }

    case MAIN_BOOTLOADER: {
      reset_and_enter_dfu();
      break;
    }
    // Shouldnt Get here
    default:
      break;
    }
  }
}

void send_usb_change(usb_command_t cmd) {
  BaseType_t xReturned;
  bool confirmed = false;

  usbRxMsgType_t usb_msg = {
      .cmd = cmd,
      .msg_type = USB_MSG_COMMAND};

  printf("MAIN_TASK: Sending USB change request.\n");
  //if (cmd != USB_CDC_ACM)
  uninit_sd_card();

  // Send command
  xReturned = xQueueSend(usbRxQueue, &usb_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send usb command to usb_stateChangeQueue. \n");
  }
  // Wait for response
  xReturned = xQueueReceive(main_usbChangedConfQueue, &confirmed, portMAX_DELAY);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to receive usb change confirmation from main_usbChangedConfQueue. \n");
  }

  if (cmd == USB_CDC_ACM)
    init_sd_card();

  printf("MAIN_TASK: USB state successfully changed.\n");
}

bool begin_amplification_zone(void) {
  BaseType_t xReturned;
  bool start_run = false;
  // Send start zone request

  xReturned = xQueueSend(heaterRxQueue, &run_amplification_zone, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send run amplification zone request.\n");
  }
  // Send Start Amplification Event to logging task
  xReturned = xQueueSend(logger_logMessageQueue, &amplification_start_log_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send start amplification zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xReturned = xQueueReceive(main_runConfRespQueue, &start_run, portMAX_DELAY);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
  // Wait for run ok to start response
  xReturned = xQueueReceive(main_runRespQueue, &start_run, portMAX_DELAY);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }

  return start_run;
}

void begin_valve_zone(void) {
  BaseType_t xReturned;
  bool heat_conf = false;
  // Send start valve message to heater queue
  xReturned = xQueueSend(heaterRxQueue, &run_valve_zone, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send run valve zone request.\n");
  }
  // Send Start Valve Event to logging task
  xReturned = xQueueSend(logger_logMessageQueue, &valve_start_log_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send start valve zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xReturned = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
}

void end_amplification_zone(void) {
  BaseType_t xReturned;
  bool heat_conf = false;
  // Send stop amplification message to heater queue
  xReturned = xQueueSend(heaterRxQueue, &stop_amplification_zone, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send stop amplification zone request.\n");
  }
  // Send stop amplification Event to logging task
  xReturned = xQueueSend(logger_logMessageQueue, &amplification_stop_log_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send stop amplification zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xReturned = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
  }
}

void end_valve_zone(void) {
  BaseType_t xReturned;
  bool heat_conf = false;
  // Send valve zone stop request
  xReturned = xQueueSend(heaterRxQueue, &stop_valve_zone, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send stop valve zone request.\n");
  }
  // Send stop valve Event to logging task
  xReturned = xQueueSend(logger_logMessageQueue, &valve_stop_log_msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send stop valve zone event to logging task.\n");
  }
  // Wait for run confirmation response
  xReturned = xQueueReceive(main_runConfRespQueue, &heat_conf, portMAX_DELAY);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to receive the start run response from main_startRunRespQueue queue.\n");
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

  // Heater Task
  xReturned = xTaskCreate(heater_task, "HeaterTask", 1024, NULL, 0, &heaterTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating heater task. Error: %d\n", xReturned);
    vTaskDelete(heaterTaskHandle);
  }
  // Logger Task
  xReturned = xTaskCreate(logger_task, "LoggerTask", 1024, NULL, 0, &loggerTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating logger task. Error: %d\n", xReturned);
    vTaskDelete(loggerTaskHandle);
  }
  // Sensors Task
  xReturned = xTaskCreate(sensors_task, "SensorsTask", 2048, NULL, 0, &sensorsTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating sensors task. Error: %d\n", xReturned);
    vTaskDelete(sensorsTaskHandle);
  }

  // Battery Management Task
  xReturned = xTaskCreate(battery_task, "BatteryTask", 1024, NULL, 0, &batteryTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating battery management task. Error: %d\n", xReturned);
    vTaskDelete(batteryTaskHandle);
  }
  // USB Management Task
  xReturned = xTaskCreate(usb_task, "USBTask", 1024, NULL, 0, &usbTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating usb management task. Error: %d\n", xReturned);
    vTaskDelete(usbTaskHandle);
  }
  // USB Composite Task
  xReturned = xTaskCreate(composite_usb_task, "CompositeUSBTask", 1024, NULL, 0, &compositeTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating Composite USB task. Error: %d\n", xReturned);
    vTaskDelete(compositeTaskHandle);
  }
  // PWM Task
  xReturned = xTaskCreate(pwm_task, "PWMTask", 1024, NULL, 0, &pwmTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating PWM task. Error: %d\n", xReturned);
    vTaskDelete(pwmTaskHandle);
  }
  // WDT Task
  xReturned = xTaskCreate(wdtFeedTask, "WDTTask", 100, NULL, 0, &wdtTaskHandle);
  if (xReturned != pdPASS) {
    /* The task was created.  Use the task's handle to delete the task. */
    printf("Error creating WDT task. Error: %d\n", xReturned);
    vTaskDelete(wdtTaskHandle);
  }
  //Button Task
  xReturned = xTaskCreate(buttonTask, "ButtonTask", 1024, NULL, 0, &buttonTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating Composite Button task. Error: %d\n", xReturned);
    vTaskDelete(buttonTaskHandle);
  }
  //LED Task
  xReturned = xTaskCreate(led_task, "LedTask", 1024, NULL, 0, &ledTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating Composite ledTaskHandle task. Error: %d\n", xReturned);
    vTaskDelete(ledTaskHandle);
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
  main_batteryDataQueue = xQueueCreate(5, sizeof(int));
  if (main_batteryDataQueue == NULL)
    printf("Unable to create main_batteryDataQueue queue\n");

  main_switchQueue = xQueueCreate(QUEUE_SIZE, sizeof(sensor_switches_t));
  if (main_switchQueue == NULL)
    printf("Unable to create main_switchQueue queue\n");

  main_mainStateRespQueue = xQueueCreate(4, sizeof(tasks_t));
  if (main_mainStateRespQueue == NULL)
    printf("Unable to create main_mainStateRespQueue queue\n");

  main_runRespQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_runRespQueue == NULL)
    printf("Unable to create main_runRespQueue queue\n");

  main_runErrorQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_runErrorQueue == NULL)
    printf("Unable to create main_runErrorQueue queue\n");

  main_runConfRespQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_runConfRespQueue == NULL)
    printf("Unable to create main_runConfRespQueue queue\n");

  main_usbConnRecvQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_usbConnRecvQueue == NULL)
    printf("Unable to create main_usbConnRecvQueue queue\n");

  main_usbChangedConfQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_usbChangedConfQueue == NULL) {
    printf("Unable to create main_usbChangedConfQueue queue\n");
  }

  main_wakeupTasksQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_wakeupTasksQueue == NULL) {
    printf("Unable to create main_wakeupTasksQueue queue\n");
  }
  // Heater Task Queues
  heaterRxQueue = xQueueCreate(10, sizeof(HeaterRxQueueMsg_t));
  if (heaterRxQueue == NULL) {
    printf("Unable to create heaterRxQueue queue\n");
  }

  // Sensor Task Queues
  sensorRxQueue = xQueueCreate(20, sizeof(SensorRxQueueMsg_t));
  if (sensorRxQueue == NULL) {
    printf("Unable to create sensorRxQueue queue\n");
  }

  // Battery Management Task Queues
  batteryRxQueue = xQueueCreate(10, sizeof(BatteryRxQueueMsg_t));
  if (batteryRxQueue == NULL) {
    printf("Unable to create batteryRxQueue queue\n");
  }

  // Button Task Queue
  buttonRxQueue = xQueueCreate(10, sizeof(ButtonRxQueueMsg_t));
  if (buttonRxQueue == NULL) {
    printf("Unable to create buttonRxQueue queue\n");
  }

  // USB Management Task Queues
  usbRxQueue = xQueueCreate(10, sizeof(usbRxMsgType_t));
  if (usbRxQueue == NULL) {
    printf("Unable to create usbRxQueue queue\n");
  }

  // Composite USB Task Queues
  compositeRxQueue = xQueueCreate(10, sizeof(CompositeUSBRxQueueType_t));
  if (compositeRxQueue == NULL) {
    printf("Unable to create compositeRxQueue queue\n");
  }

  // Logger Task Queues
  logger_recvBattPercentQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));
  if (logger_recvBattPercentQueue == NULL)
    printf("Unable to create logger_recvBattPercentQueue queue\n");

  logger_logMessageQueue = xQueueCreate(10, sizeof(log_data_message_t));
  if (logger_logMessageQueue == NULL)
    printf("Unable to create logger_logMessageQueue queue\n");

  // Watchdog Task Queues
  watchdog_rxTimesQueue = xQueueCreate(WATCH_DOG_QUEUE_SIZE, sizeof(watchdog_time_update_t));
  if (watchdog_rxTimesQueue == NULL)
    printf("Unable to create watchdog_rxTimesQueue queue\n");

  // Button Task Queues
  button_mainStateQueue = xQueueCreate(QUEUE_SIZE, sizeof(button_update_t));
  if (button_mainStateQueue == NULL)
    printf("Unable to create button_mainStateQueue queue\n");

  // PWM Task Queues
  pwmRxQueue = xQueueCreate(10, sizeof(PwmRxQueueMsg_t));
  if (pwmRxQueue == NULL) {
    printf("Unable to create pwmRxQueue queue\n");
  }

  ledRxQueue = xQueueCreate(10, sizeof(LEDRxQueueMsg_t));
  if (pwmRxQueue == NULL) {
    printf("Unable to create ledRxQueue queue\n");
  }

  main_setPointReached = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_setPointReached == NULL) {
    printf("Unable to create main_setPointReached queue\n");
  }
}

// Stack Overflow detection.
void vApplicationStackOverflowHook(TaskHandle_t xTask,
    signed char *pcTaskName) {
  printf("FreeRTOS Stack Overflow Has Occured.");
}

/*********************************************************************
*
*       main()
*
*   Application entry point.
*/
int main(void) {
  BaseType_t xReturned;
  ret_code_t err_code;
  FRESULT res;

  // Initialize clock driver for better time accuracy in FREERTOS
  err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  nrf_drv_clock_lfclk_request(NULL);

  while (!nrf_drv_clock_lfclk_is_running()) {
    // Just waiting
  }

  // Full Peripheral Initalizations
  init_adc();           // ADC
  init_sensors_gpios(); // Sensor GPIOs
  init_pwms();

  nrf_gpio_cfg_output(BOOST_CONTROL_ENABLE_PIN);
  nrf_gpio_pin_clear(BOOST_CONTROL_ENABLE_PIN); // turn off boost for heaters

  nrf_gpio_cfg_output(MOTOR_POWER_ENABLE);
  nrf_gpio_pin_clear(MOTOR_POWER_ENABLE);

#ifdef SAMPLE_PREP_BOARD
  vInit_TWI_Hardware(i2c_interface_sensors, I2C0_SDA_PIN, I2C0_SCL_PIN, i2c_speed_100k); // I2C
#else
  vInit_TWI_Hardware(i2c_interface_system, I2C1_SDA_PIN, I2C1_SCL_PIN, i2c_speed_100k);  // I2C
  vInit_TWI_Hardware(i2c_interface_sensors, I2C0_SDA_PIN, I2C0_SCL_PIN, i2c_speed_100k); // I2C
#endif
#if ENABLE_LEDS
  led_driver_init();
#endif
  fuelGauge_init();
  init_sd_card();
  button_init();
  nrf_drv_gpiote_init();
  setup_uart_semaphore();

  // Get the configuration parameters
  res = get_naatos_configuration_parameters(&config);
  if (res != FR_OK) {
    printf("Warning: configuration file was not able to be read. Using default configuration parameters.");
    use_default_configuration_parameters = true;
  } else {
    use_default_configuration_parameters = false;
  }

  // Create Queues
  create_queues();

  // Main Task
  xReturned = xTaskCreate(main_task, "MainTask", 1024, NULL, 0, &mainTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    printf("Error creating main task. Error: %d\n", xReturned);
    vTaskDelete(mainTaskHandle);
  }

  // Start Tasks
  vTaskStartScheduler();
}

void sendWdtHeaterInvalid() {
  BaseType_t xReturned;
  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
  }
}

void sendWdtMain(bool valid) {
  BaseType_t xReturned;
  watchdog_time_update_t wdtUpdate = {};
  wdtUpdate.taskName = MAIN;
  wdtUpdate.valid = valid;

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task \n");
  }
}

/*************************** End of file ****************************/