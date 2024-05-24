#ifndef PWM_H
#define PWM_H

#include <stdbool.h>
#include "nrf.h"
#include "bsp.h"
#include "app_pwm.h"
#include "nrf_gpio.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "naatos_config.h"
#include "naatos_queues.h"

#define VALVE_ZONE_PIN  20  // P0.20
#define AMP0_ZONE_PIN   19  // P0.19
#define AMP1_ZONE_PIN   34  // P1.03
#define AMP2_ZONE_PIN   33  // P1.04

//#define VALVE_ZONE_PIN  19
//#define AMP0_ZONE_PIN   20
//#define AMP1_ZONE_PIN   33
//#define AMP2_ZONE_PIN   34

#define VALVE_CHANNEL   0
#define AMP0_CHANNEL    1
#define AMP1_CHANNEL    0
#define AMP2_CHANNEL    1

void init_pwms();
void pwm_task(void * pvParameters);
void update_valve_duty(int duty);
void update_amp0_duty(int duty);
void update_amp1_duty(int duty);
void update_amp2_duty(int duty);

#endif