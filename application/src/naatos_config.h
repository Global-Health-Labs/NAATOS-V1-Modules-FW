#pragma once

#include "FreeRTOS.h"
#include "stdbool.h"
#include "task.h"
#include <stdint.h>

#define VERSION "2.7b"

/*Define this when building sample prep only otherwise comment out*/
//#define SAMPLE_PREP_BOARD
#define POWER_MODULE_BOARD

#ifdef SAMPLE_PREP_BOARD
#define SAMPLE_PREP_REV_A 0
#define SAMPLE_PREP_REV_B 1
#if SAMPLE_PREP_REV_A == 0 && SAMPLE_PREP_REV_B == 0
#define SAMPLE_PREP_REV_B 1
#endif
#endif

#ifdef POWER_MODULE_BOARD
#define POWER_MODULE_REV_A  0
#define POWER_MODULE_REV_B  1
#if POWER_MODULE_REV_A == 1 && POWER_MODULE_REV_B ==1
#define POWER_MODULE_REV_B  1
#endif 
#endif

#ifndef SAMPLE_PREP_BOARD 
#ifndef POWER_MODULE_BOARD
#warning You do not have NAATOS a board selected!
#endif
#endif

/* Device Debug Parameters */
#define I2C_CONNECTED 1
#define USE_CALENDAR_CHIP 1
#define VERBOSE_PID 1
#define USE_MOTOR 1

#define pdTICKS_TO_MS(xTimeInTicks) ((TickType_t)(((uint64_t)(xTimeInTicks) * (uint64_t)1000U) / (uint64_t)configTICK_RATE_HZ))

/* Battery Parameters */
#define ENABLE_LOW_POWER_MODE 0
#define DEFAULT_LOW_POWER_THRESHOLD 46

/*------------COMMON PIN CONFIGS------------*/
/* I2C Pins */
#define I2C0_SDA_PIN 17
#define I2C0_SCL_PIN 16
#define I2C1_SDA_PIN 24
#define I2C1_SCL_PIN 23

/* SPI Pins */
#define SPI_SCK_PIN 15   /* P0.15 */
#define SPI_MOSI_PIN 13  /* P0.13 */
#define SPI_MISO_PIN 14  /* P0.14 */
#define SPI_SD_SS_PIN 12 /* P0.12 */

/* QSPI Pins */
#if SAMPLE_PREP_REV_B || POWER_MODULE_REV_B
#define QSPI_IO0_PIN  5   /* P0.05 REVB*/ 
#define QSPI_IO1_PIN  6   /* P0.06 REVB*/
#define QSPI_IO2_PIN  7   /* P0.07 REVB*/
#define QSPI_IO3_PIN  8   /* P0.08 REVB*/
#define QSPI_CS_PIN   30  /* P0.30 REVB*/
#define QSPI_CLK_PIN  3   /* P0.03 REVB*/
#endif

/* Sample Prep Enables */
#ifdef SAMPLE_PREP_BOARD
#define HEATER_PWR_EN   21    // Boost Enable For Heater
#define MOTOR_PWR_EN    28    // Motor Enable
#define LED_DRV_EN      35    // LED Driver Enable
#define SENSORS_PWR_EN  39    // Sensors Power Enable
#if SAMPLE_PREP_REV_B
#define FLASH_PWR_EN    13    // NOR Flash Storage Enable
#define PD_3V3_EN       34    // USB PD 3v3 Enable
#elif SAMPLE_PREP_REV_A
#define SD_POWER_ENABLE NRF_GPIO_PIN_MAP(1, 8)  // SD Card Enable
#endif
#endif

/* Power Module Enables */
#ifdef POWER_MODULE_BOARD
#if POWER_MODULE_REV_A
#define HEATER_PWR_EN     21
#define LED_DRV_EN        35
#define SENSORS_PWR_EN    39
#define SD_POWER_ENABLE   40
#endif
#if POWER_MODULE_REV_B
#define FLASH_PWR_EN      13
#define TOP_LED_DRV_EN    35
#define FRONT_LED_DRV_EN  37
#define VALVE_PWR_EN      20
#define AMP_PWR_EN        21
#endif
#endif

/* User Interface */
#ifdef SAMPLE_PREP_BOARD
#define HEATER_ZONE_0_PIN 20                    // P0.20
#define HEATER_ZONE_1_PIN 19                     // P0.19
#define HEATER_ZONE_2_PIN 34                     // P1.02
#define HEATER_ZONE_3_PIN NRF_GPIO_PIN_MAP(1, 1) // P1.01
#define SAMPLE_HEATER_PIN NRF_GPIO_PIN_MAP(1, 1) // P1.01
#define HAL_INPUT_PIN NRF_GPIO_PIN_MAP(0, 2)
#define BUTTON_INPUT_PIN  38
#else 
#if POWER_MODULE_REV_A
#define BUTTON_INPUT_PIN  44                     // USES TP1 on REVA
#define HEATER_ZONE_0_PIN 20                     // P1.01 VALVE ZONE
#define HEATER_ZONE_1_PIN 19                     // P1.02
#define HEATER_ZONE_2_PIN 34                     // P0.19 AMP ZONE
#define HEATER_ZONE_3_PIN 33                     // P0.20
#else
#define BUTTON_INPUT_PIN  38
#define HEATER_ZONE_0_PIN 27                     // P1.01 -> Valve Zone
#define HEATER_ZONE_1_PIN 9                      // P1.02
#define HEATER_ZONE_2_PIN 33                     // P0.19 -> Amplification Zone
#define HEATER_ZONE_3_PIN 10                     // P0.20
#endif
#endif


/*--------------Board Specific Pin Configs----------------*/
#ifdef SAMPLE_PREP_BOARD

/* Heater Zones' Parameters */
#define DEFAULT_CYCLE_ONE_ZONE_ON_TIME 120    // Seconds
#define DEFAULT_CYCLE_TWO_ZONE_ON_TIME 120    // Seconds

#else

#define OPTICAL_INPUT_PIN 3

/* Heater Zones' Parameters */
#define DEFAULT_AMPLIFICATION_ZONE_ON_TIME 2400      // 40min in seconds
#define DEFAULT_VALVE_ZONE_ON_TIME 300               // 5min in seconds
#define BUTTON_INPUT_PIN_ALT NRF_GPIO_PIN_MAP(1, 14) // Used for recovering bad gpio pin defect
#define BUTTON_REWORK_DETECT_INPUT NRF_GPIO_PIN_MAP(1, 12)
#define BUTTON_REWORK_DETECT_OUTPUT NRF_GPIO_PIN_MAP(1, 13)

#endif

/* Default Rates
 * These rates are only used when there is no configuration file seen in the 
   naatos_config.txt file on the sd card. When a new config files is created
   these rates will be used in the system.
 * Rates are in seconds
*/
#ifdef SAMPLE_PREP_BOARD
#define DEFAULT_SAMPLE_RATE 0.200 // 0.048 minimum
#define DEFAULT_LOGGING_RATE 1.000
#define DEFAULT_MAX_TEMP 100.0
#define DEFAULT_MIN_RUN_ZONE_TEMP 75.0
#define DEFAULT_MIN_RUN_ZONE_TEMP_EN false
#define DEFAULT_ALERT_TIMEOUT_S 3.0    // Seconds
#define DEFAULT_VALID_TIMEOUT_S 3600.0 // 1 hour
#define DEFAULT_RECOVERY_THRES 47      // Percent
#define OPTICAL_TRIG_THRES 800
#define DEFAULT_HEATER_SETPOINT_1 93.5 // Sample prep only
#define DEFAULT_HEATER_SETPOINT_2 93.5 // Sample prep only
#define DEFAULT_RAMP_SETPOINT 95.0
#define MOTOR_SETPOINT_1 3900
#define MOTOR_SETPOINT_2 3900
#define DEFAULT_RUN_MOTOR_1 false
#define DEFAULT_RUN_HEATER_1 true
#define DEFAULT_RUN_MOTOR_2 true
#define DEFAULT_RUN_HEATER_2 true
#define DEFAULT_MOTOR_SWTICH_CCW_CW  false
#define DEFAULT_CYCLE_1_RUNTIME 240.0
#define DEFAULT_CYCLE_2_RUNTIME 240.0
#define DEFAULT_RAMP_TO_TEMP_BEFORE_START_1 true
#define DEFAULT_RAMP_TO_TEMP_BEFORE_START_2 false
#define DEFAULT_RAMP_TO_TEMP_TIMEOUT 600.0 // 10min
#define DEFAULT_MOTOR_SPEED_PWM 71
#define DEFAULT_MAX_HEATER_PID 100
#define DEFAULT_HAL_SENSOR_THRESHOLD  0.30f
#define DEFAULT_MOTOR_STALL_PERCENTAGE  20 
#define DEFAULT_MOTOR_STALL_ENABLE  true
#define DEFAULT_DEBUG_TO_COM_ENABLE true
#define MAX_MOTOR_PID 100
#define MIN_BATTERY_VOLTAGE 2.90
#define MAX_BATTERY_VOLTAGE 4.20
#define DEFAULT_LOW_POWER_THRESH_V 2.7
#define DEFAULT_MOTOR_WAIT_TIME_S 0

#else
#define DEFAULT_SAMPLE_RATE 0.200 // 0.048 minimum
#define DEFAULT_LOGGING_RATE 5.000
#define DEFAULT_CYCLE_1_RUNTIME 1800.0
#define DEFAULT_CYCLE_2_RUNTIME 180.0
#define DEFAULT_WAIT_TIME_AFTER_CYCLE_S 900.00 // 15 min
#define VALVE_SETPOINT_1 67.6
#define AMP_SETPOINT_1 67.2
#define VALVE_SETPOINT_2 93.0
#define AMP_SETPOINT_2 67.0
#define DEFAULT_VALVE_MAX_TEMP 105.0
#define DEFAULT_AMP_MAX_TEMP 80.0
#define DEFAULT_MIN_RUN_ZONE_TEMP 50.0
#define DEFAULT_MIN_RUN_ZONE_TEMP_EN false
#define DEFAULT_ALERT_TIMEOUT_S 3.0   // seconds
#define DEFAULT_VALID_TIMEOUT_S 3600.0 // 1 hour
#define DEFAULT_RECOVERY_THRES 40      // Percent
#define OPTICAL_TRIG_THRES 800
#define DEFAULT_MAX_AMP_PID 100
#define DEFAULT_MAX_VALVE_PID 100
#define DEFAULT_DEBUG_TO_COM_ENABLE true
#define MIN_BATTERY_VOLTAGE 2.90
#define MAX_BATTERY_VOLTAGE 4.20
#define DEFAULT_RUN_AMP_CYCLE_1 true
#define DEFAULT_RUN_AMP_CYCLE_2 true
#define DEFAULT_RUN_VALVE_CYCLE_1 false
#define DEFAULT_RUN_VALVE_CYCLE_2 true
#endif

#define DEFAULT_DATE 100124                
#define DEFAULT_TIME 120000                
#define DEFAULT_SET_TIME false

#define DEFAULT_CYCLES_COMPLETE_DELAY_S 10

/* Device Debug Parameters */
#define VERBOSE_PID 1

/* Log Event Messages */
#define START_EVENT_MSG "Sample Preperation Started."
#define STOP_EVENT_MSG "Sample Preperation Completed."
#define INTERRUPT_HAL_EVENT_MSG "Sample Preperation Interrupted. Cover Removed."
#define INTERRUPT_OPT_EVENT_MSG "Sample Preperation Interrupted. Sample Removed."
#define CYCLE_ONE_START_MSG "Cycle 1 Started."
#define CYCLE_ONE_END_MSG "Cycle 1 Stopped."
#define CYCLE_TWO_START_MSG "Cycle 2 Started."
#define CYCLE_TWO_STOP_MSG "Cycle 2 Stopped."
#define TEMPS_NOT_STABLE "Zone Temperatures are not below the minimum run temperature. Aborting run."
#define RECOVERY_BATT "Battery Percentage lower than the recovery threshold. Charge Battery!: "
#define OVER_TEMP_MSG "A Zone went over its maximum temperature. Run stopped."
#define SETPOINT_TIMEOUT_MSG "Setpoint was not reached and configured timeout was hit."
#define SAMPLE_RAMP_TO_TEMP_REACHED_MSG "Ramp to temp complete."
#define SAMPLE_RAMP_TO_TEMP_TIMEOUT_MSG "Ramp to temp timed out. Stopping cycle."
#define SAMPLE_VALID_TIMEOUT_MSG "Sample is no longer valid due to timeout."
#define UNKNOWN_ERROR_MESSAGE "An unknown error has occured."
#define HALL_SENSOR_BRAKE_MSG "HALL sensor interrupted."
#define SAMPLE_I2C_READ_ERROR_MSG "I2C sensor read error"
#define SAMPLE_MOTOR_STALLED_ERROR_MSG "Motor stalled detected."
#define SAMPLE_OVER_TEMPERATURE "Sample over temperature error"
#define SAMPLE_BATTERY_OVER_TEMP "Battery over temperature error:"
#define SAMPLE_LOW_BATTERY_STRING "Battery is too low to start cycle: "
#define POWER_ON_STRING "Sample preperation unit powered on. Starting battery voltage: "
#define USB_SUSPEND_TASKS_TIME 15000

/* Main States */
typedef enum {
  MAIN_INIT_CHARGER,
  MAIN_INIT_CHARGER_BUSY,
  MAIN_STANDBY,
  MAIN_RUNNING,
  MAIN_FILE,
  MAIN_SLEEP,
  MAIN_BOOTLOADER,
  MAIN_ALERT
} main_state_t;

typedef enum {
  MAIN,
  BATTERY,
  HEATER,
  LOGGER,
  SENSORS,
  USB,
  PWM,
  COMPOSITE,
  MOTOR
} tasks_t;

// Charging Enum
typedef enum {
  CHARGING,
  NOT_CHARGING
} charge_state_t;

// Cycle Identification Enum
typedef enum {
  CYCLE_ONE,
  CYCLE_TWO
} cycle_t;

// Type of update message being sent to usb queue
typedef enum {
  USB_CONNECTION_TYPE,
  MAIN_STATE_TYPE
} usb_message_type_t;

typedef enum {
  SAMPLE_START,
  SAMPLE_END,
  SAMPLE_INTERRUPTED,
  SAMPLE_BUTTON_CANCEL,
  SAMPLE_HAL_CANCEL,
  SAMPLE_CYCLE_ONE_STARTED,
  SAMPLE_CYCLE_ONE_ENDED,
  SAMPLE_CYCLE_TWO_STARTED,
  SAMPLE_CYCLE_TWO_ENDED,
  SAMPLE_TEMPS_NOT_STABALIZED,
  SAMPLE_RECOVERY_BATT,
  SAMPLE_OVER_TEMP,
  SAMPLE_SETPOINT_REACHED,
  SAMPLE_SETPOINT_TIMEOUT,
  SAMPLE_RAMP_TO_TEMP_REACHED,
  SAMPLE_RAMP_TO_TEMP_TIMEOUT,
  SAMPLE_INVALID_TIMEOUT,
  SAMPLE_I2C_READ_ERROR,
  SAMPLE_CANT_READ_CONFIG,
  SAMPLE_MOTOR_STALLED,
  SAMPLE_UNKNOWN,
  SAMPLE_BATTERY_LOW,
  SAMPLE_BATTERY_OVERTEMP,
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

// Motor Speed Request Struct
typedef struct {
  bool on;
  double speed; //RPM
} motor_run_req_t;

// Temperature Data Struct
typedef struct {
  float heat_zone_0_temp;
  float heat_zone_0_pwm;
  float heat_zone_1_temp;
  float heat_zone_1_pwm;
  float heat_zone_2_temp;
  float heat_zone_2_pwm;
  float heat_zone_3_temp;
  float heat_zone_3_pwm;
  double motorSpeed;
} temperature_data_t;

// Temperature PWM Data Struct
typedef struct {
  float heat_zone_0_pwm;
  float heat_zone_1_pwm;
  float heat_zone_2_pwm;
  float heat_zone_3_pwm;
  float sample_prep_heater_pwm;
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

// Composite USB

typedef enum {
  COMPOSITE_MSG_CONTINUE,
  COMPOSITE_MSG_SLEEP,
  COMPOSITE_MSG_WAKEUP
} CompositeUSBRxQueueType_t;

// USB

typedef enum {
  USB_MSG_CONN_STATUS_REQ,
  USB_MSG_COMMAND,
  USB_MSG_CHECK_CONN,
  USB_MSG_SLEEP,
  USB_MSG_WAKEUP
} usbRxQueueType_t;

typedef struct {
  usbRxQueueType_t msg_type;
  usb_command_t cmd;
} usbRxMsgType_t;

// Sensor messages

typedef enum {
  SENSOR_MSG_HEATER_STATE,
  SENSOR_MSG_USB_SUSPEND,
  SENSOR_MSG_TIMER_TEMP_EVENT,
  SENSOR_MSG_TIMER_MOTOR_EVENT,
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


typedef enum {
  MOTOR_MSG_USB_SUSPEND,
  MOTOR_MSG_TIMER_MOTOR_EVENT,
  MOTOR_MSG_SLEEP,
  MOTOR_MSG_WAKEUP,
  MOTOR_CONFIG_UPDATED,
  MOTOR_MSG_HEATER_STATE
} MotorRxQueueType_t;

typedef struct {
  MotorRxQueueType_t type;
  bool motorRunning;
  bool usbSuspend;
} MotorRxQueueMsg_t;

// Battery messages

typedef enum {
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
  HEATER_MSG_MOTOR_DATA,
  HEATER_MSG_USB_SUSPEND,
  HEATER_MSG_SENSOR_CONFIRM,
  HEATER_MSG_CONFIG_UPDATED,
  HEATER_MSG_WDT_UPDATE,
  HEATER_MSG_TEMPERATURE_DATA_ERROR,
  HEATER_MSG_MOTOR_STATE
} HeaterRxQueueType_t;

typedef struct {
  HeaterRxQueueType_t type;
  cycle_t cycleSelect;
  temperature_data_t tempData;
  bool cycleEnabled;
  bool usbSuspend;
  bool heaterRunning;
  double motorSpeed;
  bool readTempFailed;
} HeaterRxQueueMsg_t;

typedef struct {
  event_t event;
  char message[256];
} log_event_t;

typedef enum {
  LOGGER_START_CYCLE_LOG,
  TEMPERATURE_DATA,
  EVENT_DATA,
  UART_DATA,
  LOGGER_LOG_DEBUG_EVENT
} log_data_type_t;

// Log Message
typedef struct {
  log_data_type_t data_type;
  temperature_data_t temperature_data;
  log_event_t event_data;
} log_data_message_t;

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
  LED_RUN_HEATER,
  LED_RUN_MOTOR,
  LED_DECLINE,
  LED_ABORT,
  LED_COMPLETE,
  LED_INVALID,
  LED_LOW_BATTERY,
  LED_USB_MSC_STARTING,
  LED_CLEAR_ALL_ERROR
} LEDEvent_e;

typedef enum {
  led_pl_high,
  led_pl_medium,
  led_pl_low
} led_power_level_t;

typedef struct {
  LEDEvent_e type;
  led_power_level_t powerLevel;
  bool active;
} LEDRxQueueMsg_t;

typedef enum {
  MAIN_MSG_BATTERY_PERCENT,
  MAIN_MSG_BUTTON_EVENT,
  MAIN_MSG_USB_STATUS,
  MAIN_MSG_SWITCH_DATA,
  MAIN_MSG_OVERTEMP_EVENT,
  MAIN_MSG_WAKE_EVENT,
  MAIN_MSG_HEATER_CONFIRM_EVENT,
  MAIN_MSG_TIMER_EVENT
} MainStateEvent_e;

typedef struct {
  MainStateEvent_e type;
  int batteryPercent;
  button_update_t buttonEvent;
  usbRxMsgType_t usbMsg;
  sensor_switches_t sensorSwitch;
  bool overTempError;
  bool wakeupEvent;
} MainStateRxQueueMsg_t;

typedef enum {
  ERR_TEMP_SENSOR_READ,
  ERR_OVERTEMP_EVENT,
  ERR_MOTOR_STALLED
} MainStateErrorNotification_t;

typedef struct {
  MainStateErrorNotification_t errType;
  temperature_data_t overTempData;
} MainStateErrorQueueMsg_t;

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

typedef struct {
  float sample_rate;
  float logging_rate;
  float cycle_1_run_time_s;
  float cycle_2_run_time_s;
  uint16_t low_power_threshold;
  uint16_t recovery_power_thresh;
  float sample_valid_timeout_s;
  int sample_complete_delay_s;
  float alert_timeout_time_s;
  float min_run_zone_temp;
  bool min_run_zone_temp_en;
  bool debug_to_com_en;
#ifndef SAMPLE_PREP_BOARD  
  uint16_t optical_distance;
  uint16_t min_wait_time_after_cycle_s;
  float max_amp_pid_pwm;
  float max_valve_pid_pwm;
  float amp_max_temp;
  float valve_max_temp;
  float valve_setpoint_1;
  float amp_setpoint_1;
  float valve_kp_1;
  float valve_ki_1;
  float valve_kd_1;
  float amp_kp_1;
  float amp_ki_1;
  float amp_kd_1;
  float valve_setpoint_2;
  float amp_setpoint_2;
  float valve_kp_2;
  float valve_ki_2;
  float valve_kd_2;
  float amp_kp_2;
  float amp_ki_2;
  float amp_kd_2;
  bool run_amp_cycle_1;
  bool run_amp_cycle_2;
  bool run_valve_cycle_1;
  bool run_valve_cycle_2;
  int mmddyy;
  int hhmmss;
  bool set_date_time;
#else
  float max_heater_pid_pwm;
  bool ramp_to_temp_before_start_cycle_1;
  bool ramp_to_temp_before_start_cycle_2;
  float ramp_to_temp_c1_timeout;
  float ramp_to_temp_c2_timeout;
  uint16_t motor_end_wait_time_s;
  float heater_setpoint_1;
  float heater_setpoint_2;
  float heater_max_temp;
  float heater_kp_1;
  float heater_ki_1;
  float heater_kd_1;
  float heater_kp_2;
  float heater_ki_2;
  float heater_kd_2;
  float motor_kp_1;
  float motor_ki_1;
  float motor_kd_1;
  bool switch_motor_ccw_cw;
  uint16_t motor_setpoint_1;
  float motor_kp_2;
  float motor_ki_2;
  float motor_kd_2;
  uint16_t motor_setpoint_2;
  bool run_motor_1;
  bool run_heater_1;
  bool run_motor_2;
  bool run_heater_2;
  float hal_sensor_thresh;
  int motor_stall_percent;
  bool motor_stall_en;
  int mmddyy;
  int hhmmss;
  bool set_date_time;
#endif
} naatos_config_parameters;

/* Configuration Parameters Variables */
extern bool use_default_configuration_parameters;
extern naatos_config_parameters config;
extern bool batt_recovering;