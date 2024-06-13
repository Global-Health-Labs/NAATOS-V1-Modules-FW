#pragma once

#include "FreeRTOS.h"
#include "is31fl3196.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "stdbool.h"
#include <stdint.h>

#define ENABLE_LEDS 1

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
  bool runCondition;
  bool testDecline;
  bool testAbort;
  bool testComplete;
  bool testInvalid;
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

void set_led2_red_slow_blink(void);
void set_led2_red_fast_blink(void);
void set_led2_green_solid(void);
void set_led2_blue_breathe(void);
void turn_off_led2(void);
void updateLedState(LEDEvent_e event, bool active);