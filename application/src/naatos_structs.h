#pragma once

#include "stdbool.h"

// Holds data for Hal and Optical Switch
typedef struct {
   bool hal_triggered;
   bool optical_tiggered;
} sensor_switches_t;