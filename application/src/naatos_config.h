#pragma once

#include "stdbool.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include  "task.h"

#define SAMPLE_PREP_BOARD

#define NAATOS_FW_VERSON  "v0.1.0_c"

#define pdTICKS_TO_MS( xTimeInTicks )    ( ( TickType_t ) ( ( ( uint64_t ) ( xTimeInTicks ) * ( uint64_t ) 1000U ) / ( uint64_t ) configTICK_RATE_HZ ) )

/* Battery Parameters */
#define DEFAULT_LOW_POWER_THRESHOLD     20

/* Heater Zones' Parameters */
#define DEFAULT_AMPLIFICATION_ZONE_ON_TIME  30    // Minutes
#define DEFAULT_VALVE_ZONE_ON_TIME          3     // Minutes

/* I2C Pins */
#define I2C0_SDA_PIN    17
#define I2C0_SCL_PIN    16
#define I2C1_SDA_PIN    24
#define I2C1_SCL_PIN    23

/* SPI Pins */
#define SPI_SCK_PIN     15    /* P0.15 */
#define SPI_MOSI_PIN    13    /* P0.13 */
#define SPI_MISO_PIN    14    /* P0.14 */
#define SPI_SD_SS_PIN   12    /* P0.12 */

/* Default Rates
 * These rates are only used when there is no configuration file seen in the 
   naatos_config.txt file on the sd card. When a new config files is created
   these rates will be used in the system.
 * Rates are in seconds
*/ 
#define DEFAULT_SAMPLE_RATE           0.200  // 0.048 minimum
#define DEFAULT_LOGGING_RATE          5.000
#define DEFAULT_VALVE_MAX_TEMP        95.0
#define DEFAULT_AMP0_MAX_TEMP         80.0
#define DEFAULT_AMP1_MAX_TEMP         80.0
#define DEFAULT_AMP2_MAX_TEMP         80.0
#define DEFAULT_MIN_RUN_ZONE_TEMP     50.0
#define DEFAULT_MIN_RUN_ZONE_TEMP_EN  true
#define DEFAULT_ALERT_TIMEOUT_S       30.0   // Seconds
#define DEFAULT_RECOVERY_THRES        40    // Percent
#define OPTICAL_TRIG_THRES            800
#define DEFAULT_HEATER_SETPOINT       65.0
#define DEFAULT_MOTOR_SPEED_PWM       40
#define DEFAULT_RUN_MOTOR_1           false
#define DEFAULT_RUN_HEATER_1          true
#define DEFAULT_RUN_MOTOR_2           true
#define DEFAULT_RUN_HEATER_2          false

/* Device Debug Parameters */
#define I2C_CONNECTED           1
#define GO_STRAIGHT_TO_RUNNING  0
#define USE_CALENDAR_CHIP       1 
#define VERBOSE_PID             1
#define USE_MOTOR               1

/* Log Event Messages */
#define START_EVENT_MSG           "Sample Preperation Started."
#define STOP_EVENT_MSG            "Sample Preperation Completed."
#define INTERRUPT_HAL_EVENT_MSG   "Sample Preperation Interrupted. Cover Removed."
#define INTERRUPT_OPT_EVENT_MSG   "Sample Preperation Interrupted. Sample Removed."
#define AMP_START_MSG             "Amplification Zone Heating Started."
#define AMP_END_MSG               "Amplification Zone Heating Stopped."
#define VALV_START_MSG            "Valve Zone Heating Started."
#define VALV_STOP_MSG             "Valve Zone Heating Stopped."
#define TEMPS_NOT_STABLE          "Zone Temperatures are no below the minimum run temperature. Aborting run."
#define RECOVERY_BATT             "Battery Percentage lower than the recovery threshold. Charge Battery More."
#define OVER_TEMP_MSG             "A Zone went over its maximum temperature. Run stopped."

#define USB_SUSPEND_TASKS_TIME    15000

/* Main States */
typedef enum {
  MAIN_STANDBY,
  MAIN_RUNNING,
  MAIN_FILE,
  MAIN_SLEEP,
  MAIN_BOOTLOADER
} main_state_t;

typedef enum {
  MAIN,
  BATTERY,
  HEATER,
  LOGGER,
  SENSORS, 
  USB,
  PWM,
  COMPOSITE
} tasks_t;

// Charging Enum
typedef enum {
  CHARGING,
  NOT_CHARGING
} charge_state_t;

// Zone Identification Enum
typedef enum {
  AMPLIFICATION,
  VALVE
} zone_t;

// Type of update message being sent to usb queue
typedef enum {
  USB_CONNECTION_TYPE,
  MAIN_STATE_TYPE
} usb_message_type_t; 

typedef enum {
  TEMPERATURE_DATA,
  EVENT_DATA
} log_data_type_t;

typedef enum {
  SAMPLE_START,
  SAMPLE_END,
  SAMPLE_INTERRUPTED,
  SAMPLE_AMP_STARTED,
  SAMPLE_AMP_ENDED,
  SAMPLE_VALV_STARTED,
  SAMPLE_VALV_ENDED,
  SAMPLE_TEMPS_NOT_STABALIZED,
  SAMPLE_RECOVERY_BATT,
  SAMPLE_OVER_TEMP
  // Add more events here
} event_t;

typedef enum {
  USB_DISABLED = 0,
  USB_CDC_ACM = 1,
  USB_MSC = 2,
  USB_MSC_CDC_ACM = 3
} usb_command_t;

// Holds data for Hal and Optical Switch
typedef struct {
   bool hal_triggered;
   bool optical_tiggered;
} sensor_switches_t;

// Zone Request Struct
typedef struct {
  bool on;
  zone_t zone;
} zone_run_req_t;

// Motor Speed Request Struct
typedef struct {
  bool on;
  double speed;   //RPM
} motor_run_req_t;

// Temperature Data Struct
typedef struct {
  float amp0_zone_temp;
  float valve_zone_pwm;
  float amp1_zone_temp;
  float amp0_zone_pwm;
  float amp1_zone_pwm;
  float amp2_zone_temp;
  float valve_zone_temp;
  float amp2_zone_pwm;
  double motorSpeed;
} temperature_data_t;

// Temperature PWM Data Struct
typedef struct {
  float valve_zone_pwm;
  float amp0_zone_pwm;
  float amp1_zone_pwm;
  float amp2_zone_pwm;
} temperature_pwm_data_t;


typedef enum {
  USB_TO_FILE,
  USB_TO_COM
} usb_requested_state;


// Composite USB Update Message
typedef struct {
  usb_message_type_t message_type;
  main_state_t current_state;
  usb_requested_state request_state;
  charge_state_t charge_state;
} usb_message_t;

typedef struct {
  event_t event;
  char * message;
} log_event_t;

// Log Message
typedef struct {
  log_data_type_t data_type;
  temperature_data_t temperature_data;
  log_event_t event_data;
} log_data_message_t;

// Battery Request with task to send to
typedef struct {
  tasks_t task_req;
} battery_percent_req_t; 


typedef struct {
  tasks_t taskName;
  bool valid; // if the task is meant to not run anymore this should be set to false
} watchdog_time_update_t;

typedef enum {
  ON_EVENT,
  OFF_EVENT,
  BOOTLOADER_EVENT,
  NONE
} button_event_e;

typedef struct {
  button_event_e event;
} button_update_t;

// Calendar time struct
typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t week_day;
  uint8_t month;
  uint8_t year;
} calendar_time_t;

// USB Wait Suspend request
typedef struct {
  bool suspend;
} usb_suspend_req_t;

// USB Wait Suspend Acceptance
typedef struct {
  tasks_t task;
  bool suspended;
} usb_suspend_acpt_t;

// USB Wait Suspend Over
typedef struct {
  tasks_t task;
  bool over;
} usb_suspend_over_t;

// Sensor messages

typedef enum {
  SENSOR_MSG_HEATER_STATE,
  SENSOR_MSG_USB_SUSPEND,
  SENSOR_MSG_PWM_RESPONSE,
  SENSOR_MSG_TIMER_EVENT,
  SENSOR_MSG_SLEEP,
  SENSOR_MSG_WAKEUP,
  CONFIG_UPDATED
} SensorRxQueueType_t;

typedef struct {
  SensorRxQueueType_t type;
  bool heaterRunning;
  bool usbSuspend;
  temperature_pwm_data_t pwmData;
} SensorRxQueueMsg_t;

// Battery messages

typedef enum {
  BATTERY_MSG_USB_SUSPEND,
  BATTERY_MSG_TIMER_EVENT,
  BATTERY_SOC_REQUEST,
  BATTERY_MSG_SLEEP,
  BATTERY_MSG_WAKEUP,
  BATTERY_CONFIG_UPDATED,
  BATTERY_MSG_MAIN_STATE_CHANGE
} BatteryRxQueueType_t;

typedef enum {
  BATTERY_MSG_SOC_MAIN,
  BATTERY_MSG_SOC_LOG
} BatterySOC_Send_to_t;

typedef struct {
  BatteryRxQueueType_t type;
  BatterySOC_Send_to_t sendTo;
  main_state_t mainState;
  bool usbSuspend;
} BatteryRxQueueMsg_t;


// Button/Switch messages
typedef enum {
  BUTTON_MSG_SLEEP,
  BUTTON_MSG_WAKE,
  BUTTON_MSG_TIMER_EVENT,
} ButtonRxQueueType_t;

typedef struct {
  ButtonRxQueueType_t type;
} ButtonRxQueueMsg_t;

// heater messages
typedef enum {
  HEATER_MSG_SLEEP,
  HEATER_MSG_WAKE,
  HEATER_MSG_ZONE_STATE,
  HEATER_MSG_TEMPERATURE_DATA,
  HEATER_MSG_USB_SUSPEND,
  HEATER_MSG_SENSOR_CONFIRM,
  HEATER_MSG_PWM_REQUEST,
  HEATER_MSG_CONFIG_UPDATED,
  HEATER_MSG_WDT_UPDATE
} HeaterRxQueueType_t;

typedef struct {
  HeaterRxQueueType_t type;
  zone_t zoneSelect;
  temperature_data_t tempData;
  bool zoneEnabled;
  bool usbSuspend;
  bool heaterRunning;
  double motorSpeed;
} HeaterRxQueueMsg_t;

typedef enum {
  PWM_MSG_UPDATE_DUTY,
  PWM_MSG_CALLBACK_EVENT,
  PWM_MSG_DISABLE,
  PWM_MSG_ENABLE
} PWMRxQueueType_t;

typedef struct {
  PWMRxQueueType_t type;

} PwmRxQueueMsg_t;

// LED Handeling
typedef enum {
  LED_WAKEUP,
  LED_CHARGING,
  LED_STANDBY,
  LED_RUN,
  LED_DECLINE,
  LED_ABORT,
  LED_COMPLETE,
  LED_INVALID,
  LED_LOW_BATTERY,
  LED_USB_MSC_STARTING,
  LED_CLEAR_ALL_ERROR
} LEDEvent_e;

typedef struct {
  LEDEvent_e type;
  bool active;
} LEDRxQueueMsg_t;


// Task Handles
extern xTaskHandle mainTaskHandle;
extern xTaskHandle heaterTaskHandle;
extern xTaskHandle loggerTaskHandle;
extern xTaskHandle sensorsTaskHandle;
extern xTaskHandle batteryTaskHandle;
extern xTaskHandle usbTaskHandle;
extern xTaskHandle pwmTaskHandle;
extern xTaskHandle compositeTaskHandle;
extern xTaskHandle buttonTaskHandle;

// Config parameters
#ifndef SAMPLE_PREP_BOARD
typedef struct {
  float sample_rate;
  float logging_rate;
  uint16_t valve_zone_run_time_m;
  uint16_t amplification_zone_run_time_m;
  uint16_t low_power_threshold;
  uint16_t recovery_power_thresh;
  float valve_setpoint;
  float amp0_setpoint;
  float amp1_setpoint;
  float amp2_setpoint;
  float valve_max_temp;
  float amp0_max_temp;
  float amp1_max_temp;
  float amp2_max_temp;
  float min_run_zone_temp;
  bool  min_run_zone_temp_en;
  float alert_timeout_time_m;
  float valve_kp;
  float valve_ki;
  float valve_kd;
  float amp0_kp;
  float amp0_ki;
  float amp0_kd;
  float amp1_kp;
  float amp1_ki;
  float amp1_kd;
  float amp2_kp;
  float amp2_ki;
  float amp2_kd;
  uint16_t optical_distance;
  float valve_setpoint_2;
  float amp0_setpoint_2;
  float amp1_setpoint_2;
  float amp2_setpoint_2;
  float valve_kp_2;
  float valve_ki_2;
  float valve_kd_2;
  float amp0_kp_2;
  float amp0_ki_2;
  float amp0_kd_2;
  float amp1_kp_2;
  float amp1_ki_2;
  float amp1_kd_2;
  float amp2_kp_2;
  float amp2_ki_2;
  float amp2_kd_2;
} naatos_config_parameters;
#else 
typedef struct {
  float sample_rate;
  float logging_rate;
  uint16_t valve_zone_run_time_m;
  uint16_t amplification_zone_run_time_m;
  uint16_t low_power_threshold;
  uint16_t recovery_power_thresh;
  float heater_setpoint_1;
  float heater_setpoint_2;
  float heater_max_temp;
  float min_run_zone_temp;
  bool  min_run_zone_temp_en;
  float alert_timeout_time_s;
  float heater_kp_1;
  float heater_ki_1;
  float heater_kd_1;
  float heater_kp_2;
  float heater_ki_2;
  float heater_kd_2;
  uint16_t motor_speed_pwm_1;
  uint16_t motor_speed_pwm_2;
  bool run_motor_1;
  bool run_heater_1;
  bool run_motor_2;
  bool run_heater_2;
} naatos_config_parameters;
#endif

/* Configuration Parameters Variables */
extern bool use_default_configuration_parameters;
extern naatos_config_parameters config;