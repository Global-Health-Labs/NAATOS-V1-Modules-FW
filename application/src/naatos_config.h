#pragma once

#include "stdbool.h"

#define LOW_POWER_THRESHOLD     20

#define AMPLIFICATION_ZONE_ON_TIME  30    // Minutes
#define VALVE_ZONE_ON_TIME          3     // Minutes

/* Main States */
typedef enum {
  LOW_POWER,
  STANDBY,
  RUNNING
} main_state_t;

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
  CURRENT_STATE_TYPE
} usb_message_type_t; 

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