#include <stdint.h>

#include "nrf_delay.h"
#include "app_error.h"

#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define MOTOR_INPUT_PIN 38

static const nrf_drv_timer_t m_counter0;
static nrf_ppi_channel_t m_ppi_channel1;

static void gpiote_init(void);
static void empty_gpiote_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
static void ppi_init(void);
void motor_tach_init(void);