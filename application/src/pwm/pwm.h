#ifndef PWM_H
#define PWM_H

#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "FreeRTOS.h"
#include "app_pwm.h"
#include "bsp.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "queue.h"
#include <stdbool.h>

#define SAMPLE_HEATER_CHANNEL 0

#ifdef SAMPLE_PREP_BOARD
#define HEATER_CHANNEL  0
#define MOTOR_CHANNEL   0
#else
#define AMP_CHANNEL 0
#define VALVE_CHANNEL 0
#define MOTOR_CHANNEL 0
#endif

void init_pwms(void);
void uninit_pwms(void);
void pwm_task(void *pvParameters);
void updateDutyCycles(temperature_pwm_data_t pwmData);

#endif