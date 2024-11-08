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

#define HEAT_ZONE_0_CHANNEL 0
#define HEAT_ZONE_1_CHANNEL 1
#define HEAT_ZONE_2_CHANNEL 0
#define HEAT_ZONE_3_CHANNEL 1
#define MOTOR_CHANNEL 0

void init_pwms(void);
void uninit_pwms(void);
void pwm_task(void *pvParameters);
void updateDutyCycles(temperature_pwm_data_t pwmData);

void ghl_pwm_manual_set_channel(uint8_t zone, uint16_t dutycycle);

#endif