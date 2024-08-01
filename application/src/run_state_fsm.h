#pragma once
#include "naatos_config.h"


/* Main States */
typedef enum {
  RUN_VALIDATE_INIT_CONDITIONS,
  RUN_START_CYCLE_1,
  RUN_CYCLE_1_RAMP_TO_TEMP,
  RUN_CYCLE_1_TIMER,
  RUN_START_CYCLE_2,
  RUN_CYCLE_2_RAMP_TO_TEMP,
  RUN_CYCLE_2_TIMER,
  RUN_EXIT
} run_state_t;
