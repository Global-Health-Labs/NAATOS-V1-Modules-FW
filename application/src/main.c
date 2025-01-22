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
#include "storage/naatos_storage.h"
#include "sdk_errors.h"
#include "sensors.h"
#include "motor_task.h"
#include "spi.h"
#include "states.h"
#include "switch.h"
#include "task.h"
#include "timers.h"
#include "usb.h"
#include "watchdog.h"
#include "charger_driver.h"
#include "tps55288.h"

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
xTaskHandle motorTaskHandle;

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

naatos_config_parameters config = {
    .sample_rate = 0,
    .logging_rate = 0,
    .low_power_threshold = 0,
    .recovery_power_thresh = 0,
    .sample_valid_timeout_s = 0,
    .alert_timeout_time_s = 0,
    .debug_to_com_en = false,
#ifndef SAMPLE_PREP_BOARD
    .optical_distance = 0,
    .max_amp_pid_pwm = 0.0,
    .max_valve_pid_pwm = 0.0,
    .amp_max_temp = 0.0,
    .valve_max_temp = 0.0,
#else
    .max_heater_pid_pwm = 0,
    .heater_max_temp = 0.0,
    .switch_motor_ccw_cw = 0.0,
    .hal_sensor_thresh = 0.0, 
    .motor_stall_percent = 0.0,
    .motor_stall_pwm = 0.0,
    .motor_stall_en = 0.0,
#endif
    .mmddyy = 0,
    .hhmmss = 0,
    .set_date_time = false
};

cycle_config_parameters *cycle_configs; // Will be populated by the storage when the cycle configs are pulled

bool batt_recovering = false;

// Function defs
void sendWdtHeaterInvalid();
void sendWdtMain(bool valid);
void create_tasks(void);
void send_usb_change(usb_command_t cmd);
void reset_and_enter_dfu(void);

char exitBatteryOvrTempString[256];

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
    send_debug_log_message("Warning: configuration file was not able to be read. Using default configuration parameters.");
    use_default_configuration_parameters = true;
  } else {
    use_default_configuration_parameters = false;
  }

  xReturned = get_cycle_configurations(&cycle_configs);
  if (xReturned != FR_OK) {
    send_debug_log_message("Warning: cycle configuration files were not able to be read.");
  }

  xReturned = xQueueSend(heaterRxQueue, &heaterConfigMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send update config request to heaterRxQueue.");
  }

  // Send to Sensors task
  xReturned = xQueueSend(sensorRxQueue, &sensorConfigMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("USB: Unable to send update config request to sensorRxQueue.");
  }
}

void set_startup_enables(void) {
  /* SAMPLE PREP BOARD */
#ifdef SAMPLE_PREP_BOARD
  // Turn off boost for heaters
  nrf_gpio_cfg_output(HEATER_PWR_EN);
  nrf_gpio_pin_clear(HEATER_PWR_EN); 
  // Turn off the motor enable
  nrf_gpio_cfg_output(MOTOR_PWR_EN);
  nrf_gpio_pin_clear(MOTOR_PWR_EN); 
  // LED driver enable
  nrf_gpio_cfg_output(LED_DRV_EN);
  nrf_gpio_pin_set(LED_DRV_EN);
  // Sensors power enable
  nrf_gpio_cfg_output(SENSORS_PWR_EN);
  nrf_gpio_pin_set(SENSORS_PWR_EN);
// Sample Prep Rev. A only enables
#if SAMPLE_PREP_REV_A
  // SD Card Enable
  nrf_gpio_cfg_output(SD_POWER_ENABLE);
  nrf_gpio_pin_set(SD_POWER_ENABLE);
#endif
// Sample prep Rev. B. only enables
#if SAMPLE_PREP_REV_B
  // USB PD Controller Enable
  nrf_gpio_cfg_output(PD_3V3_EN);
  nrf_gpio_pin_set(PD_3V3_EN);
  // NOR Flash Enable
  nrf_gpio_cfg_output(FLASH_PWR_EN);
  nrf_gpio_pin_set(FLASH_PWR_EN);
#endif
#else 

  /* POWER MODULE BOARD */
#if POWER_MODULE_REV_A
  // Turn off boost for heaters
  nrf_gpio_cfg_output(HEATER_PWR_EN);
  nrf_gpio_pin_clear(HEATER_PWR_EN); 
  // LED driver enable
  nrf_gpio_cfg_output(LED_DRV_EN);
  nrf_gpio_pin_set(LED_DRV_EN);
  // Sensors power enable
  nrf_gpio_cfg_output(SENSORS_PWR_EN);
  nrf_gpio_pin_set(SENSORS_PWR_EN);
  // SD Card Enable
  nrf_gpio_cfg_output(SD_POWER_ENABLE);
  nrf_gpio_pin_set(SD_POWER_ENABLE);
#elif POWER_MODULE_REV_B
  // Top LED driver enable
  nrf_gpio_cfg_output(TOP_LED_DRV_EN);
  nrf_gpio_pin_set(TOP_LED_DRV_EN);
  // Front LED driver enable
  nrf_gpio_cfg_output(FRONT_LED_DRV_EN);
  nrf_gpio_pin_set(FRONT_LED_DRV_EN);
  // Turn off boost for Valve heater
  nrf_gpio_cfg_output(VALVE_PWR_EN);
  nrf_gpio_pin_set(VALVE_PWR_EN); 
  // Turn off boost for amp heater
  nrf_gpio_cfg_output(AMP_PWR_EN);
  nrf_gpio_pin_clear(AMP_PWR_EN); 
  // Sensors power enable
  nrf_gpio_cfg_output(SENSORS_PWR_EN);
  nrf_gpio_pin_set(SENSORS_PWR_EN);
  // USB PD Controller Enable
  nrf_gpio_cfg_output(PD_3V3_EN);
  nrf_gpio_pin_set(PD_3V3_EN);
  // NOR Flash Enable
  nrf_gpio_cfg_output(FLASH_PWR_EN);
  nrf_gpio_pin_set(FLASH_PWR_EN);
#endif
#endif
}

void clear_enables(void) {
  /* SAMPLE PREP BOARD */
#ifdef SAMPLE_PREP_BOARD
  // Turn off boost for heaters
  nrf_gpio_cfg_output(HEATER_PWR_EN);
  nrf_gpio_pin_clear(HEATER_PWR_EN); 
  // Turn off the motor enable
  nrf_gpio_cfg_output(MOTOR_PWR_EN);
  nrf_gpio_pin_clear(MOTOR_PWR_EN); 
  // LED driver disable
  nrf_gpio_cfg_output(LED_DRV_EN);
  nrf_gpio_pin_clear(LED_DRV_EN);
  // Sensors power disable
  nrf_gpio_cfg_output(SENSORS_PWR_EN);
  nrf_gpio_pin_clear(SENSORS_PWR_EN);
  // Sample Prep Rev. A only disables
#if SAMPLE_PREP_REV_A
  // SD Card Disable
  nrf_gpio_cfg_output(SD_POWER_ENABLE);
  nrf_gpio_pin_clear(SD_POWER_ENABLE);
#endif
// Sample prep Rev. B. only disables
#if SAMPLE_PREP_REV_B
  // USB PD Controller Disable
  nrf_gpio_cfg_output(PD_3V3_EN);
  nrf_gpio_pin_clear(PD_3V3_EN);
  // NOR Flash Disable
  nrf_gpio_cfg_output(FLASH_PWR_EN);
  nrf_gpio_pin_clear(FLASH_PWR_EN);
#endif
#else

  /* POWER MODULE BOARD */
#if POWER_MODULE_REV_A
  // Turn off boost for heaters
  nrf_gpio_cfg_output(HEATER_PWR_EN);
  nrf_gpio_pin_clear(HEATER_PWR_EN); 
  // LED driver Disable
  nrf_gpio_cfg_output(LED_DRV_EN);
  nrf_gpio_pin_clear(LED_DRV_EN);
  // Sensors power Disable
  nrf_gpio_cfg_output(SENSORS_PWR_EN);
  nrf_gpio_pin_clear(SENSORS_PWR_EN);
  // SD Card Disable
  nrf_gpio_cfg_output(SD_POWER_ENABLE);
  nrf_gpio_pin_clear(SD_POWER_ENABLE);
#elif POWER_MODULE_REV_B
  // NOR Flash disable
  nrf_gpio_cfg_output(FLASH_PWR_EN);
  nrf_gpio_pin_clear(FLASH_PWR_EN);
  // Top LED driver disable
  nrf_gpio_cfg_output(TOP_LED_DRV_EN);
  nrf_gpio_pin_clear(TOP_LED_DRV_EN);
  // Front LED driver disable
  nrf_gpio_cfg_output(FRONT_LED_DRV_EN);
  nrf_gpio_pin_clear(FRONT_LED_DRV_EN);
  // Turn off boost for Valve heater
  nrf_gpio_cfg_output(VALVE_PWR_EN);
  nrf_gpio_pin_clear(VALVE_PWR_EN); 
  // Turn off boost for amp heater
  nrf_gpio_cfg_output(AMP_PWR_EN);
  nrf_gpio_pin_clear(AMP_PWR_EN); 
#endif
#endif
}

void init_peripherals(void) {
  // Full Peripheral Initalizations
  init_adc();
#ifdef SAMPLE_PREP_BOARD
  init_motor_gpio();
#endif
  init_pwms();
  vInit_TWI_Hardware(i2c_interface_system, I2C1_SDA_PIN, I2C1_SCL_PIN, i2c_speed_100k);
  vInit_TWI_Hardware(i2c_interface_sensors, I2C0_SDA_PIN, I2C0_SCL_PIN, i2c_speed_100k);
#if ENABLE_LEDS
  led_driver_init();
#endif
  fuelGauge_init();
  init_naatos_storage();
  button_init();
#ifdef POWER_MODULE_BOARD
  valve_zone_set_6v();
#endif
}

void uninit_peripherals(void) {
  // Full Peripheral Uninitalization
  uninit_naatos_storage();
  // Dont want to turn off the Fuel Guage
  // Dont want to turn off the Button, need it to wake back up
#if ENABLE_LEDS
  led_driver_uninit();
#endif
  vUninit_TWI_Hardware(i2c_interface_system, I2C1_SDA_PIN, I2C1_SCL_PIN);
  vUninit_TWI_Hardware(i2c_interface_sensors, I2C0_SDA_PIN, I2C0_SCL_PIN);
  uninit_pwms();
#ifdef SAMPLE_PREP_BOARD
  uninit_motor_gpio();
#endif
  uninit_adc();
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
  fuel_batt_info_t batt_info_recv;
  button_update_t buttonData = {.event = NONE};
  bool hal_triggered = false, optical_triggered = false;
  bool error_during_run = false;
  bool over_temp = false;
  bool loggedLowPowerOnce = false;
  bool loggedBatteyrOverTempOnce = false;
  uint32_t start_time = 0, end_time = 0, a_t_start = 0;
  uint32_t alert_timeout_ticks;
  bool usb_conn_status = false;
  bool usb_needs_update = false;
  bool wake_up = false;
  bool power_on_log_created = false;
  int mainWatchDogKickCount = 0;
  char w_buff[100];
  const BatteryRxQueueMsg_t batt_req = {
      .type = BATTERY_SOC_REQUEST,
      .sendTo = BATTERY_MSG_SOC_MAIN};
  BaseType_t xHigherPriorityTaskWoken = pdTRUE;

  // Initalize the charger in here after 1 second to avoid 
  // issues with charging when the kill switch is off
  vTaskDelay(pdMS_TO_TICKS(1000));
  while(!pd_eeprom_init_complete());
  setup_charger();

  // See if we are resetting the file system
  bool button_pressed  = !(nrf_gpio_pin_read(BUTTON_INPUT_PIN));
  if (button_pressed && !usb_started) {
    unmount_storage();
    nor_flash_fatfs_mkfs();
    create_naatos_directories();
    get_naatos_configuration_parameters(&config);
  }

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
          send_debug_log_message("MAIN: Unable to send sensor wakeup to sensorRxQueue.");
        }

#ifdef SAMPLE_PREP_BOARD
        MotorRxQueueMsg_t motorMsg;
        motorMsg.type = MOTOR_MSG_WAKEUP;

        xReturned = xQueueSend(motorRxQueue, &motorMsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN: Unable to send sensor wakeup to motorRxQueue.");
        }
#endif

        read_sd_and_notify_tasks();
        // Get the alert timeout
        if (!use_default_configuration_parameters) {
          alert_timeout_ticks = (uint32_t)(pdMS_TO_TICKS(config.alert_timeout_time_s * 1000.0));
        } else {
          alert_timeout_ticks = (uint32_t)(pdMS_TO_TICKS(DEFAULT_ALERT_TIMEOUT_S * 1000.0));
        }
        if (config.set_date_time) {
          calendar_set_time_helper();
          reset_set_time_date();
        }

        updateLedState(LED_WAKEUP, true);
        updateLedState(LED_RUN_MOTOR, false);
        updateLedState(LED_RUN_HEATER, false);
        updateLedState(LED_STANDBY, true);
        updateLedState(LED_COMPLETE, false);
        sendWdtMain(true);
        start_time = xTaskGetTickCount();
        loggedLowPowerOnce = false;
        loggedBatteyrOverTempOnce = false;
      }

      hal_triggered = false;
      optical_triggered = false;

      // Request the battery percentage from the bettery task
      xReturned = xQueueSend(batteryRxQueue, &batt_req, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("LOG_TASK: Unable to send battery percentage request to batteryRxQueue.");
      }

      // Check for Battery Data in Battery Queue
      if (xQueueReceive(main_batteryDataQueue, &batt_info_recv, pdMS_TO_TICKS(100)) == pdPASS) {
        // Create the Power on log if we have not yet
        if (!power_on_log_created && !usb_started) {
          power_on_log_created = true;
          xReturned = xQueueSend(logger_logMessageQueue, &new_log_msg, 10);
          if (xReturned != pdPASS) {
            send_debug_log_message("MAIN_TASK: Unable to send log start\r\n");
          }
          sprintf(exitBatteryOvrTempString, "%s%0.02f", POWER_ON_STRING, batt_info_recv.batt_voltage);
          send_event_log_message(SAMPLE_BATTERY_OVERTEMP, exitBatteryOvrTempString);
        }
        else if (!power_on_log_created && usb_started) {
          power_on_log_created = true;
        }

        // Set the variable LED from the battery percentage
        led_power_level_t l_powerLevel = getCurrentPowerLevel();
        float min_starting_voltage = MIN_BATTERY_VOLTAGE + ((MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE) * ((float)config.low_power_threshold / 100.0));
        float white_starting_voltage = MIN_BATTERY_VOLTAGE + ((MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE) * ((float)LED_POWER_LEVEL_HIGH_THRESH / 100.0));
        float recovery_starting_voltage = MIN_BATTERY_VOLTAGE + ((MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE) * ((float)config.recovery_power_thresh / 100.0));

        if (l_powerLevel != led_pl_high && 
           ((batt_info_recv.batt_voltage >= white_starting_voltage && l_powerLevel != led_pl_medium) ||
            (batt_info_recv.batt_voltage >= white_starting_voltage + (0.01f) && l_powerLevel == led_pl_medium))) {
          batt_recovering = false;
          if (!usb_started)
            updateLedStatePowerLevel(LED_STANDBY, true, led_pl_high);
        }
        else if (l_powerLevel != led_pl_medium && 
                (((batt_info_recv.batt_voltage < white_starting_voltage) && (batt_info_recv.batt_voltage >= min_starting_voltage) && l_powerLevel != led_pl_low ) || 
                 ((batt_info_recv.batt_voltage < white_starting_voltage) && (batt_info_recv.batt_voltage >= recovery_starting_voltage)) && l_powerLevel == led_pl_low )) {
          batt_recovering = false;
          if (!usb_started)
            updateLedStatePowerLevel(LED_STANDBY, true, led_pl_medium);
        }
        else if ((batt_info_recv.batt_voltage < min_starting_voltage) && l_powerLevel != led_pl_low) {
          batt_recovering = true;
          if(!loggedLowPowerOnce) {
            loggedLowPowerOnce = true;
            xReturned = xQueueSend(logger_logMessageQueue, &new_log_msg, 10);
            if (xReturned != pdPASS) {
              send_debug_log_message("MAIN_TASK: Unable to send log start\r\n");
            }
             sprintf(exitBatteryOvrTempString, "%s%f", SAMPLE_LOW_BATTERY_STRING, batt_info_recv.batt_percent);
             send_event_log_message(SAMPLE_BATTERY_LOW, exitBatteryOvrTempString);
          }

          if (!usb_started)
            updateLedStatePowerLevel(LED_STANDBY, true, led_pl_low);
        }


        if(batt_info_recv.batt_temp >= 59.0) {
          if(!loggedBatteyrOverTempOnce) {
            loggedBatteyrOverTempOnce = true;
            xReturned = xQueueSend(logger_logMessageQueue, &new_log_msg, 10);
            if (xReturned != pdPASS) {
              send_debug_log_message("MAIN_TASK: Unable to send log start\r\n");
            }
             sprintf(exitBatteryOvrTempString, "%s%f", SAMPLE_BATTERY_OVER_TEMP, batt_info_recv.batt_temp);
             send_event_log_message(SAMPLE_BATTERY_OVERTEMP, exitBatteryOvrTempString);
          }
        }

#if ENABLE_LOW_POWER_MODE
        if ((percent_recv < DEFAULT_LOW_POWER_THRESHOLD && use_default_configuration_parameters) || (!use_default_configuration_parameters && percent_recv < config.low_power_threshold)) {
             next_state = MAIN_SLEEP;
             break;
        }
#endif
      }

      /* **** HANDLE USB AND SWITCH **** */
      // Get switch status if it has changed
      if (xQueueReceive(button_mainStateQueue, &buttonData, 0) == pdPASS) {
        send_debug_log_message("MAIN_TASK: Button click in idle");
        usbRxMsgType_t conn_req_msg = {
            .cmd = NULL,
            .msg_type = USB_MSG_CONN_STATUS_REQ};
        // Get The USB Connection Status
        xReturned = xQueueSend(usbRxQueue, &conn_req_msg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to send usb connection request to usbRxQueue. ");
        }
        // Receive the USB Connection Status
        xReturned = xQueueReceive(main_usbConnRecvQueue, &usb_conn_status, portMAX_DELAY);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to receive usb connection status from main_usbConnRecvQueue. ");
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
          updateLedState(LED_RUN_MOTOR, false);
          updateLedState(LED_RUN_HEATER, false);
          updateLedState(LED_STANDBY, false);
          updateLedState(LED_USB_MSC_STARTING, true);
          send_usb_change(USB_MSC_CDC_ACM);
          break;
        }
#if ENABLE_LOW_POWER_MODE
        // Switch off and USB not connected
        else if (buttonData.event == OFF_EVENT && !usb_conn_status) {
          sendWdtMain(false);
          next_state = MAIN_SLEEP;
          send_usb_change(USB_DISABLED);
        } 
#endif
        else if (usb_conn_status) {
          updateLedState(LED_CHARGING, true);
        } else if (!usb_conn_status) {
          updateLedState(LED_CHARGING, false);
        }
        usb_needs_update = false;
      }

      // Wait for Sensor Switch Data
      xReturned = xQueueReceive(main_switchQueue, &switch_data, portMAX_DELAY);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Error receiving switch data from main_switchQueue");
      }

      // Update switches triggered
      hal_triggered = switch_data.hal_triggered;
      optical_triggered = switch_data.optical_tiggered;

#ifdef SAMPLE_PREP_BOARD
      // Check if we can go to RUN state
      if (/*hal_triggered && */buttonData.event == ON_EVENT && !error_during_run) {
        next_state = MAIN_RUNNING;
        // Delay
        vTaskDelay(100);
      }
#else
      // Check if we can go to RUN state
      if (optical_triggered && !error_during_run) {
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
      }

      main_wdt_time_left = pdTICKS_TO_MS(xTaskGetTickCount() - main_wdt_start_time);

      if (main_wdt_time_left >= main_wdt_end_time) {
        sendWdtMain(true);
        main_wdt_start_time = xTaskGetTickCount();
        main_wdt_time_left = 0;
      }

      cycle_state_exit_t cycle_exit_info = run_cycle_state_machine();

      // Check if USB State needs to be updated
      if (xQueueReceive(main_usbConnRecvQueue, &usb_conn_status, 0) == pdPASS) {
        usb_needs_update = true;
      }

      if (usb_needs_update) {
        if (usb_conn_status) {
          updateLedState(LED_CHARGING, true);
        } else if (!usb_conn_status) {
          updateLedState(LED_CHARGING, false);
        }
        usb_needs_update = false;
      }

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
        char tmp[50];
        updateLedState(LED_WAKEUP, true);
        updateLedState(LED_RUN_MOTOR, false);
        updateLedState(LED_RUN_HEATER, false);
        updateLedState(LED_STANDBY, true);
        updateLedState(LED_COMPLETE, false);
        a_t_start = xTaskGetTickCount();
        sprintf(tmp, "MAIN_TASK: Alert Timeout - %dms", pdTICKS_TO_MS(alert_timeout_ticks));
        send_debug_log_message(tmp);
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
        updateLedState(LED_RUN_MOTOR, false);
        updateLedState(LED_RUN_HEATER, false);
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
          send_debug_log_message("Sensor: Unable to send timer update to sensorRxQueue queue from main.");
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
          send_debug_log_message("MAIN_TASK: Unable to send usb connection request to usbRxQueue.");
        }
        // Receive the USB Connection Status
        xReturned = xQueueReceive(main_usbConnRecvQueue, &usb_conn_status, portMAX_DELAY);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to receive usb connection status from main_usbConnRecvQueue.");
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
#if ENABLE_LOW_POWER_MODE
        // Switch off and USB not connected
        else if (buttonData.event == OFF_EVENT && !usb_conn_status) {
          updateLedState(LED_USB_MSC_STARTING, false);
          next_state = MAIN_SLEEP;
          send_usb_change(USB_DISABLED);
          sendWatchdogKickFromTask(MAIN, false);
        }
#endif
        // Updated USB Connection Status
        else if (buttonData.event == NONE && !usb_conn_status) {
          updateLedState(LED_USB_MSC_STARTING, false);
          next_state = MAIN_STANDBY;
          send_usb_change(USB_DISABLED);
        }
        usb_needs_update = false;
      }
      else if (!usb_started) {
        buttonData.event = NONE;
        updateLedState(LED_USB_MSC_STARTING, false);
        next_state = MAIN_STANDBY;
        send_usb_change(USB_CDC_ACM);
        sendWatchdogKickFromTask(MAIN, false);
      }

      vTaskDelay(pdMS_TO_TICKS(100));
      break;

    // In Low Power State
    case MAIN_SLEEP: {
#if ENABLE_LOW_POWER_MODE
      if (last_state != main_state) {
        updateLedState(LED_RUN, false);
        updateLedState(LED_STANDBY, false);
        updateLedState(LED_USB_MSC_STARTING, false);
        updateLedState(LED_WAKEUP, false);
        send_debug_log_message("Going to sleep...");
        SensorRxQueueMsg_t msg;
        msg.type = SENSOR_MSG_SLEEP;
        xReturned = xQueueSend(sensorRxQueue, &msg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to send sensor sleep to sensorRxQueue queue.");
        }

        MotorRxQueueMsg_t motorMsg;
        motorMsg.type = MOTOR_MSG_SLEEP;
        xReturned = xQueueSend(motorRxQueue, &motorMsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to send motor sleep to motorRxQueue queue.");
        }

        BatteryRxQueueMsg_t battMsg;
        battMsg.type = BATTERY_MSG_SLEEP;

        xReturned = xQueueSend(batteryRxQueue, &battMsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to send battery sleep to batteryRxQueue.");
        }

        ButtonRxQueueMsg_t buttonMsg;
        buttonMsg.type = BUTTON_MSG_SLEEP;

        xReturned = xQueueSend(buttonRxQueue, &buttonMsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to send button sleep to batteryRxQueue.");
        }

        usbRxMsgType_t usbMsg;
        usbMsg.msg_type = USB_MSG_SLEEP;
        xReturned = xQueueSend(usbRxQueue, &usbMsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to send usb sleep to usbRxQueue.");
        }

        CompositeUSBRxQueueType_t compositeMsg = COMPOSITE_MSG_SLEEP;
        xReturned = xQueueSend(compositeRxQueue, &compositeMsg, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("MAIN_TASK: Unable to send composite sleep to usbRxQueue.");
        }

        updateLedState(LED_RUN, false);
        updateLedState(LED_STANDBY, false);
        updateLedState(LED_USB_MSC_STARTING, false);

        vTaskDelay(100);

        // Turn off peripherals and enables
        uninit_peripherals();
        clear_enables();
      }

      // this queue is blocked indefinitly until a switch interrupt or usb  interrupt
      xReturned = xQueueReceive(main_wakeupTasksQueue, &wake_up, portMAX_DELAY);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to receive tasks wakeup from main_wakeupTasksQueue.");
      }

      // Dont wake up if the battery percentage is still too low
      if ((percent_recv < DEFAULT_LOW_POWER_THRESHOLD && use_default_configuration_parameters) || (!use_default_configuration_parameters && percent_recv < config.low_power_threshold)) {
           next_state = MAIN_SLEEP;
           break;
      }

      send_debug_log_message("Waking up...");
      
      // Set the enables high then start up the peripherals
      set_startup_enables();
      init_peripherals();

      vTaskDelay(100);

      SensorRxQueueMsg_t msg;
      msg.type = SENSOR_MSG_WAKEUP;
      xReturned = xQueueSend(sensorRxQueue, &msg, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send sensor sleep to sensorRxQueue queue.");
      }

      MotorRxQueueMsg_t motorMsg;
      motorMsg.type = MOTOR_MSG_WAKEUP;
      xReturned = xQueueSend(motorRxQueue, &motorMsg, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send motor wake to motorRxQueue queue.");
      }

      BatteryRxQueueMsg_t battMsg;
      battMsg.type = BATTERY_MSG_WAKEUP;

      xReturned = xQueueSend(batteryRxQueue, &battMsg, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send battery sleep to batteryRxQueue.");
      }

      ButtonRxQueueMsg_t buttonMsg;
      buttonMsg.type = BUTTON_MSG_WAKE;

      xReturned = xQueueSend(buttonRxQueue, &buttonMsg, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send button sleep to batteryRxQueue.");
      }

      CompositeUSBRxQueueType_t compositeMsg = COMPOSITE_MSG_WAKEUP;
      xReturned = xQueueSend(compositeRxQueue, &compositeMsg, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send composite sleep to usbRxQueue.");
      }

      usbRxMsgType_t usbMsg;
      usbMsg.msg_type = USB_MSG_WAKEUP;
      xReturned = xQueueSend(usbRxQueue, &usbMsg, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send usb sleep to usbRxQueue.");
      }

      next_state = MAIN_STANDBY;
#endif
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

  send_debug_log_message("MAIN_TASK: Sending USB change request.");
  if (cmd != USB_CDC_ACM) {
    unmount_storage();
    uninit_naatos_storage();
  }

  // Send command
  xReturned = xQueueSend(usbRxQueue, &usb_msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to send usb command to usb_stateChangeQueue.");
  }
  // Wait for response
  xReturned = xQueueReceive(main_usbChangedConfQueue, &confirmed, portMAX_DELAY);
  if (xReturned != pdPASS) {
    send_debug_log_message("MAIN_TASK: Unable to receive usb change confirmation from main_usbChangedConfQueue.");
  }

  if (cmd == USB_CDC_ACM) {
    //init_naatos_storage();
    NVIC_SystemReset();
  }

  send_debug_log_message("MAIN_TASK: USB state successfully changed.");
}


/*********************************************************************
*
*       create_tasks()
*
*   Creates all of the FreeRTOS Tasks
*/
void create_tasks() {
  BaseType_t xReturned;
  char buff[100];
  
  // Heater Task
  xReturned = xTaskCreate(heater_task, "HeaterTask", 1024, NULL, 0, &heaterTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating heater task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(heaterTaskHandle);
  }
  // Logger Task
  xReturned = xTaskCreate(logger_task, "LoggerTask", 1024, NULL, 0, &loggerTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating logger task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(loggerTaskHandle);
  }
  // Sensors Task
  xReturned = xTaskCreate(sensors_task, "SensorsTask", 2048, NULL, 0, &sensorsTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating sensors task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(sensorsTaskHandle);
  }
  // Battery Management Task
  xReturned = xTaskCreate(battery_task, "BatteryTask", 1024, NULL, 0, &batteryTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating battery management task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(batteryTaskHandle);
  }
  // USB Management Task
  xReturned = xTaskCreate(usb_task, "USBTask", 1024, NULL, 0, &usbTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating usb management task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(usbTaskHandle);
  }
  // USB Composite Task
  xReturned = xTaskCreate(composite_usb_task, "CompositeUSBTask", 1024, NULL, 0, &compositeTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating Composite USB task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(compositeTaskHandle);
  }
  
  // PWM Task  
  xReturned = xTaskCreate(pwm_task, "PWMTask", 1024, NULL, 0, &pwmTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating PWM task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(pwmTaskHandle);
  }
  // WDT Task
  xReturned = xTaskCreate(wdtFeedTask, "WDTTask", 100, NULL, 0, &wdtTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating WDT task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(wdtTaskHandle);
  }
  //Button Task
  xReturned = xTaskCreate(buttonTask, "ButtonTask", 1024, NULL, 0, &buttonTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating Composite Button task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(buttonTaskHandle);
  }
  //LED Task
  xReturned = xTaskCreate(led_task, "LedTask", 1024, NULL, 0, &ledTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating Composite ledTaskHandle task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(ledTaskHandle);
  }
  #ifdef SAMPLE_PREP_BOARD
  // Motor Task
  xReturned = xTaskCreate(motorTask, "MotorTask", 1024, NULL, 0, &motorTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating MotorTaskHandle task. Error: %d", xReturned);
    send_debug_log_message(buff);
    vTaskDelete(ledTaskHandle);
  }
  #endif
}

/*********************************************************************
*
*       create_queues()
*
*   Creates all of the FreeRTOS Queues
*/
void create_queues() {
  // Main Task Queues
  main_batteryDataQueue = xQueueCreate(5, sizeof(fuel_batt_info_t));
  if (main_batteryDataQueue == NULL)
    send_debug_log_message("Unable to create main_batteryDataQueue queue");

  main_switchQueue = xQueueCreate(QUEUE_SIZE, sizeof(sensor_switches_t));
  if (main_switchQueue == NULL)
    send_debug_log_message("Unable to create main_switchQueue queue");

  main_mainStateRespQueue = xQueueCreate(4, sizeof(tasks_t));
  if (main_mainStateRespQueue == NULL)
    send_debug_log_message("Unable to create main_mainStateRespQueue queue");

  main_runRespQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_runRespQueue == NULL)
    send_debug_log_message("Unable to create main_runRespQueue queue");

  main_runErrorQueue = xQueueCreate(5, sizeof(MainStateErrorQueueMsg_t));
  if (main_runErrorQueue == NULL)
    send_debug_log_message("Unable to create main_runErrorQueue queue");

  main_runConfRespQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_runConfRespQueue == NULL)
    send_debug_log_message("Unable to create main_runConfRespQueue queue");

  main_usbConnRecvQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_usbConnRecvQueue == NULL)
    send_debug_log_message("Unable to create main_usbConnRecvQueue queue");

  main_usbChangedConfQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_usbChangedConfQueue == NULL) {
    send_debug_log_message("Unable to create main_usbChangedConfQueue queue");
  }

  main_wakeupTasksQueue = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_wakeupTasksQueue == NULL) {
    send_debug_log_message("Unable to create main_wakeupTasksQueue queue");
  }
  // Heater Task Queues
  heaterRxQueue = xQueueCreate(10, sizeof(HeaterRxQueueMsg_t));
  if (heaterRxQueue == NULL) {
    send_debug_log_message("Unable to create heaterRxQueue queue");
  }

  // Sensor Task Queues
  sensorRxQueue = xQueueCreate(20, sizeof(SensorRxQueueMsg_t));
  if (sensorRxQueue == NULL) {
    send_debug_log_message("Unable to create sensorRxQueue queue");
  }

  // Battery Management Task Queues
  batteryRxQueue = xQueueCreate(10, sizeof(BatteryRxQueueMsg_t));
  if (batteryRxQueue == NULL) {
    send_debug_log_message("Unable to create batteryRxQueue queue");
  }

  // Button Task Queue
  buttonRxQueue = xQueueCreate(10, sizeof(ButtonRxQueueMsg_t));
  if (buttonRxQueue == NULL) {
    send_debug_log_message("Unable to create buttonRxQueue queue");
  }

  // USB Management Task Queues
  usbRxQueue = xQueueCreate(10, sizeof(usbRxMsgType_t));
  if (usbRxQueue == NULL) {
    send_debug_log_message("Unable to create usbRxQueue queue");
  }

  // Composite USB Task Queues
  compositeRxQueue = xQueueCreate(10, sizeof(CompositeUSBRxQueueType_t));
  if (compositeRxQueue == NULL) {
    send_debug_log_message("Unable to create compositeRxQueue queue");
  }

  // Logger Task Queues
  logger_recvBattPercentQueue = xQueueCreate(QUEUE_SIZE, sizeof(fuel_batt_info_t));
  if (logger_recvBattPercentQueue == NULL)
    send_debug_log_message("Unable to create logger_recvBattPercentQueue queue");

  logger_logMessageQueue = xQueueCreate(10, sizeof(log_data_message_t));
  if (logger_logMessageQueue == NULL)
    send_debug_log_message("Unable to create logger_logMessageQueue queue");

  // Watchdog Task Queues
  watchdog_rxTimesQueue = xQueueCreate(WATCH_DOG_QUEUE_SIZE, sizeof(watchdog_time_update_t));
  if (watchdog_rxTimesQueue == NULL)
    send_debug_log_message("Unable to create watchdog_rxTimesQueue queue");

  // Button Task Queues
  button_mainStateQueue = xQueueCreate(QUEUE_SIZE, sizeof(button_update_t));
  if (button_mainStateQueue == NULL)
    send_debug_log_message("Unable to create button_mainStateQueue queue");

  // PWM Task Queues
  pwmRxQueue = xQueueCreate(10, sizeof(PwmRxQueueMsg_t));
  if (pwmRxQueue == NULL) {
    send_debug_log_message("Unable to create pwmRxQueue queue");
  }

  ledRxQueue = xQueueCreate(10, sizeof(LEDRxQueueMsg_t));
  if (pwmRxQueue == NULL) {
    send_debug_log_message("Unable to create ledRxQueue queue");
  }

  main_setPointReached = xQueueCreate(QUEUE_SIZE, sizeof(bool));
  if (main_setPointReached == NULL) {
    send_debug_log_message("Unable to create main_setPointReached queue");
  }
#ifdef SAMPLE_PREP_BOARD
  motorRxQueue = xQueueCreate(10, sizeof(MotorRxQueueMsg_t));
  if (motorRxQueue == NULL) {
    send_debug_log_message("Unable to create motorRxQueue queue");
  }
#endif
}

// Stack Overflow detection.
void vApplicationStackOverflowHook(TaskHandle_t xTask,
    signed char *pcTaskName) {
  send_debug_log_message("FreeRTOS Stack Overflow Has Occured.");
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
  char buff[100];

  // Initialize clock driver for better time accuracy in FREERTOS
  err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

  nrf_drv_clock_lfclk_request(NULL);

  while (!nrf_drv_clock_lfclk_is_running()) {
    // Just waiting
  }

  // Enable Lines 
  set_startup_enables();  
  // Init Peripherals
  init_peripherals();
  // Other Initalizations
  nrf_drv_gpiote_init();
  setup_uart_semaphore();
  
  // Get the configuration parameters
  res = get_naatos_configuration_parameters(&config);
  if (res != FR_OK) {
    send_debug_log_message("Warning: configuration file was not able to be read. Using default configuration parameters.");
    use_default_configuration_parameters = true;
    // Report that the confirguation file cannot be read to a log if sd card is ok
    if (sd_card_inited) {
      log_event_t exit_event_info = {
        .event = SAMPLE_CANT_READ_CONFIG,
        .message = "Configuration file was not able to be read. Using default configuration parameters."};
      log_data_message_t exit_log_message = {
        .data_type = EVENT_DATA,
        .temperature_data = NULL,
        .event_data = exit_event_info};

      xReturned = xQueueSend(logger_logMessageQueue, &new_log_msg, 10);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send configuration error to logging task.");
      }
      xReturned = xQueueSend(logger_logMessageQueue, &exit_log_message, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("MAIN_TASK: Unable to send recovery battery percentage event to logging task.");
      }
   }
  } else {
    use_default_configuration_parameters = false;
  }

  // Create Queues
  create_queues();

  // Main Task
  xReturned = xTaskCreate(main_task, "MainTask", 1024, NULL, 0, &mainTaskHandle);
  if (xReturned != pdPASS) {
    // The task was created.  Use the task's handle to delete the task.
    sprintf(buff, "Error creating main task. Error: %d", xReturned);
    send_debug_log_message(buff);
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
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }
}

void sendWdtMain(bool valid) {
  BaseType_t xReturned;
  watchdog_time_update_t wdtUpdate = {};
  wdtUpdate.taskName = MAIN;
  wdtUpdate.valid = valid;

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }
}

/*************************** End of file ****************************/