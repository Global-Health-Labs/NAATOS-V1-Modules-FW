#pragma once
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
#include "fuel.h"
#include "heater/heater.h"
#include "i2c_hal_freertos.h"
#include "led.h"
#include "logger/logger.h"
#include "motor.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "nordic_common.h"
#include "nrf_drv_clock.h"
#include "pid.h"
#include "sd_card.h"
#include "sdk_errors.h"
#include "sensors.h"
#include "spi.h"
#include "states.h"
#include "switch.h"
#include "task.h"
#include "timers.h"
#include "usb.h"
#include "watchdog.h"

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

/* Main States */
typedef enum {
  VALIDATE_INIT_CONDITIONS,
  START_CYCLE_1,
  CYCLE_1_RAMP_TO_TEMP,
  CYCLE_1_TIMER,
  START_CYCLE_2,
  CYCLE_2_RAMP_TO_TEMP,
  CYCLE_2_TIMER,
  CYCLE_COMPLETE_DELAY,
  CYCLE_SAMPLE_VALID_HOLD,
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
  CYCLE_ERROR_TIMEOUT_DURING_RAMP,
  CYCLE_SAMPLE_INVALIDATED,
  CYCLE_ERROR_UNKNOWN
} cycle_state_exit_t;

void reset_cycle_state_machine(void);

cycle_state_exit_t run_cycle_state_machine(void);

/* For sample prep this will start heating the sample and or spin the motor.
 * For power module this will start the amplification zone heating.
 */
bool begin_cycle_1(void);

void begin_cycle_2(void);

void end_cycle_1(void);

void end_cycle_2(void);

bool limitSwitchFreed(sensor_switches_t data);