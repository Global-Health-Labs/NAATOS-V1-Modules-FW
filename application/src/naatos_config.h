#pragma once

#include "stdbool.h"

#define LOW_POWER_THRESHOLD     20

#define AMPLIFICATION_ZONE_ON_TIME  30    // Minutes
#define VALVE_ZONE_ON_TIME          3     // Minutes

// Charging Enum
typedef enum {
  CHARGING,
  NOT_CHARGING,
  DISABLED
} charge_state_t;

// Zone Identification Enum
typedef enum {
  AMPLIFICATION,
  VALVE
} zone_t;

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