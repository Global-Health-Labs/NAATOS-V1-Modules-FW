#pragma once

#include "FreeRTOS.h"
#include "is31fl3196.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "stdbool.h"
#include "logger/logger.h"
#include <stdint.h>

#define ENABLE_LEDS 1

#define LED_POWER_LEVEL_HIGH_THRESH   75

/*
  When an event triggers an LED change it must eventually clear that change.
  A decision tree on what LEDS should be blinking at any given time will be
  decided by a set of flags. When a flag is updated the led chips are updated.
  If all are false then all LED's are off.
*/

typedef struct {
  bool wakeupCondition;
  bool batteryCharging;
  bool standbyCondition;
  bool runConditionHeater;
  bool runConditionMotor;
  bool testDecline;
  bool testAbort;
  bool testAbortYellow;
  bool testComplete;
  bool testInvalid;     // action LED is supposed to stay RED if error occurs, after alert period, until door opened/optical broken
  bool lowBattey;
  bool usbMscStarting;
} LEDFlags_t;

void led_task(void *pvParameters);

void set_led1_green_solid(void);
void set_led1_green_breathe(void);
void set_led1_blue_slow_blink(void);
void set_led1_red_slow_blink(void);
void set_led1_red_fast_blink(void);
void set_led1_red_solid(void);
void set_led1_blue_breathe(void);
void set_led2_white_solid(void);
void set_led2_yellow_solid(void);
void set_led2_red_slow_blink(void);
void set_led2_red_fast_blink(void);
void set_led2_green_solid(void);
void set_led2_red_solid(void);
void set_led2_blue_breathe(void);
void turn_off_led_channels(void);
void set_led1_green_fast_blink(void);
void updateLedState(LEDEvent_e event, bool active);
void updateLedStatePowerLevel(LEDEvent_e event, bool active, led_power_level_t led_pl);
led_power_level_t getCurrentPowerLevel(void);
void turn_off_led2_blue_animation(void);