#pragma once
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"

#include "nrf_drv_gpiote.h"

#define MOTOR_INPUT_PIN 4    //TP1= P1.12
#define MOTOR_OUTPUT_PIN 26  //47 //TP4 = P1.15
#define MOTOR_OUTPUT_PIN2 45 //TP2 = P1.13 Only here because PWM.c asks for two pins to cfg a PWM module
#define MOTOR_POWER_ENABLE 28

static const nrf_drv_timer_t m_counter1;
static nrf_ppi_channel_t m_ppi_channel1;

static void gpiote_init(void);
static void empty_gpiote_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
static void ppi_init(void);
nrf_drv_timer_t *motor_tach_init(void);