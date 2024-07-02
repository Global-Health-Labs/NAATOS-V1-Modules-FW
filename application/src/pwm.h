#ifndef PWM_H
#define PWM_H

#include "FreeRTOS.h"
#include "app_pwm.h"
#include "bsp.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "queue.h"
#include <stdbool.h>

#define VALVE_CHANNEL 0
#define AMP0_CHANNEL 1
#define AMP1_CHANNEL 0
#define AMP2_CHANNEL 1

void init_pwms();
void pwm_task(void *pvParameters);
void updateDutyCycles(temperature_pwm_data_t pwmData);

#endif