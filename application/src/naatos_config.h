#pragma once

#include "stdbool.h"

/* Battery Parameters */
#define LOW_POWER_THRESHOLD     20

/* Heater Zones' Parameters */
#define AMPLIFICATION_ZONE_ON_TIME  30    // Minutes
#define VALVE_ZONE_ON_TIME          3     // Minutes

/* I2C Pins */
#define I2C0_SDA_PIN    22
#define I2C0_SCL_PIN    23

/* Main States */
typedef enum {
  LOW_POWER,
  STANDBY,
  RUNNING
} main_state_t;

typedef enum {
  MAIN,
  BATTERY,
  HEATER,
  LOGGER,
  SENSORS, 
  USB
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
  SAMPLE_END
  // Add more events here
} event_t;

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

// Temperature Data Struct
typedef struct {
  float amplification_zone_temp;
  float valve_zone_temp;
} temperature_data_t;

// Composite USB Update Message
typedef struct {
  usb_message_type_t message_type;
  main_state_t current_state;
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