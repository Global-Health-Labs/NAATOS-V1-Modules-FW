#pragma once

#include "stdbool.h"

#define LOW_POWER_THRESHOLD     20

// Holds data for Hal and Optical Switch
typedef struct {
   bool hal_triggered;
   bool optical_tiggered;
} sensor_switches_t;

// Charging Enum
typedef enum {
  CHARGING,
  NOT_CHARGING,
  DISABLED
} charge_state_t;