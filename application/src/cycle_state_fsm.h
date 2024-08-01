#pragma once
#include "naatos_config.h"


/* Main States */
typedef enum {
  VALIDATE_INIT_CONDITIONS,
  START_CYCLE_1,
  CYCLE_1_RAMP_TO_TEMP,
  CYCLE_1_TIMER,
  START_CYCLE_2,
  CYCLE_2_RAMP_TO_TEMP,
  CYCLE_2_TIMER,
  EXIT_CYCLE
} cycle_state_t;

typedef enum {
  CYCLE_RUNNING,
  CYCLE_COMPLETE,
  CYCLE_ERROR_POWER_LOW,
  CYCLE_ERROR_SENSOR_BREAK,
  CYCLE_ERROR_BUTTON_EXIT,
  CYCLE_ERROR_OVER_TEMP,
  CYCLE_ERROR_START_TEMP_TOO_HIGH,
  CYCLE_ERROR_UNKNOWN
} cycle_state_exit_t


void run_cycle_state_machine();