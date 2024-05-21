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

#include "motorDebug.h"

//Use one hardware timer (which is set up as a counter)
static const nrf_drv_timer_t m_counter1 = NRF_DRV_TIMER_INSTANCE(1);

static nrf_ppi_channel_t ppi_channel_1;

/* Empty event handler. Not used since tasks/events are handled in hardware (PPI), but needs to be defined for semantical reasons. */
static void empty_gpiote_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    //uint32_t count = nrf_drv_timer_capture_get(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0); //This one you supposedly should use if using PPI? But it returns large negative number?
    uint32_t count = nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0); //Just returns 0
    printf("Value on MOTOR_INPUT_PIN changed. Counter value = %d\r\n", count);
}

/* Empty event handler. Not used since tasks/events are handled in hardware (PPI), but needs to be defined for semantical reasons. */
static void empty_timer_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    printf("Timer event triggered\r\n");

}

static void gpiote_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.pull = NRF_GPIO_PIN_NOPULL;

    err_code = nrf_drv_gpiote_in_init(MOTOR_INPUT_PIN, &in_config, empty_gpiote_event_handler);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(MOTOR_INPUT_PIN, true); //TODO: Second argument should be set to false to prevent function call to the empty event handler...
                                                            //Setting to false doesn't work?
}


/** @brief Function for Counter 1 initialization.
 *  @details Counter 1 will be incremented by toggling state on GPIO pin via PPI.
 */
static void counter1_init(void)
{
    ret_code_t err_code;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32; //Do I need to specify a width? Example did
    timer_cfg.mode = NRF_TIMER_MODE_LOW_POWER_COUNTER;
    err_code = nrf_drv_timer_init(&m_counter1, &timer_cfg, empty_timer_event_handler);
    APP_ERROR_CHECK(err_code);
}

/** @brief Function for initializing the PPI peripheral.
*/
static void ppi_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel_1);
    APP_ERROR_CHECK(err_code);

    uint32_t gpiote_evt_addr_1 = nrf_drv_gpiote_in_event_addr_get(MOTOR_INPUT_PIN);
    uint32_t timer_count_task_addr = nrf_drv_timer_task_address_get(&m_counter1, NRF_TIMER_TASK_COUNT);

    err_code = nrf_drv_ppi_channel_assign(ppi_channel_1, gpiote_evt_addr_1, timer_count_task_addr); // Trigger timer count task when GPIOTE pin detects edge
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_ppi_channel_enable(ppi_channel_1);
    APP_ERROR_CHECK(err_code);

}

void motor_tach_init(void)
{
    gpiote_init();
    counter1_init();
    ppi_init();
}