#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "i2c_hal_freertos.h"
#include "nrf_drv_twi.h"
#include "nrfx_twi.h"
#include "nrf_gpio.h"
#include "nrf_error.h"
#include "sdk_errors.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

static const nrf_drv_twi_t m_i2c[i2c_num_interfaces] = {
    NRF_DRV_TWI_INSTANCE(I2C_SYSTEM_INSTANCE_ID),
    NRF_DRV_TWI_INSTANCE(I2C_SENSOR_INSTANCE_ID),
};

static SemaphoreHandle_t m_i2c_semaphores[i2c_num_interfaces];

/* INIT for TWI hardware for peripheral */
void vInit_TWI_Hardware(i2c_interface_selection_t interface, uint32_t sda_pin, uint32_t scl_pin, i2c_speed_selection_t frequency) {
    //NRF_LOG_INFO( "vInit_TWI_Hardware" );
    ret_code_t err_code;
    nrf_drv_twi_frequency_t nrf_freq;
    
    if (interface >= i2c_num_interfaces) {
        return;
    }
    
    if (i2c_speed_100k == frequency) {
        nrf_freq = NRF_DRV_TWI_FREQ_100K;
    }
    else {
        nrf_freq = NRF_DRV_TWI_FREQ_400K;
    }
    nrf_drv_twi_config_t twi_config = {
            .scl = scl_pin,	// SCL pin number.
            .sda = sda_pin, // SDA pin number.
            .frequency = nrf_freq, // TWI frequency.
            .interrupt_priority = APP_IRQ_PRIORITY_HIGH, // Interrupt priority.
            .clear_bus_init = false, // Clear bus during init.
            .hold_bus_uninit = false // Hold pull up state on gpio pins after uninit.
    };
    err_code = nrf_drv_twi_init( &m_i2c[interface], &twi_config, NULL, NULL );
    APP_ERROR_CHECK( err_code );
    //
    nrf_gpio_cfg( scl_pin, // pin_number
                    NRF_GPIO_PIN_DIR_INPUT, // Input.
                    NRF_GPIO_PIN_INPUT_CONNECT, // Connect input buffer.
                    NRF_GPIO_PIN_PULLUP, // Pin pull-up resistor disabled.
                    NRF_GPIO_PIN_S0S1, // Standard '0', standard '1'.
                    NRF_GPIO_PIN_NOSENSE // Pin sense level disabled.
                  );
    nrf_gpio_cfg( sda_pin,	// pin_number
                    NRF_GPIO_PIN_DIR_INPUT, // Input.
                    NRF_GPIO_PIN_INPUT_CONNECT, // Connect input buffer.
                    NRF_GPIO_PIN_PULLUP, // Pin pull-up resistor disabled.
                    NRF_GPIO_PIN_S0S1,	// Standard '0', standard '1'.
                    NRF_GPIO_PIN_NOSENSE // Pin sense level disabled.
                  );
    // RTOS init
    m_i2c_semaphores[interface] = xSemaphoreCreateBinary();// Ensure the semaphore is created before it gets used.
    ASSERT( m_i2c_semaphores[interface] );          // LOCK HERE: the semaphore could not be created
    xSemaphoreGive( m_i2c_semaphores[interface] );  // 'Give' the peripheral protection semaphore
    //NRF_LOG_INFO( "vInit_TWI_Hardware: DONE" );
    return;
}

ret_code_t xUtil_TWI_Read( i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t *p_buff, uint16_t length ) {
    #ifdef VERB_HW_TWI
    NRF_LOG_INFO( "xUtil_TWI_Read:\tslave = 0x%02X\taddr = 0x%02X\tlenght = %u", slave_addr, start_addr, length );
    #endif
    ret_code_t err_code;
    static uint8_t reg_addr;
    reg_addr = start_addr;

    if( length == 0 ) {	
        err_code = NRF_ERROR_INVALID_LENGTH;

    } else if( xSemaphoreTake( m_i2c_semaphores[interface], portMAX_DELAY ) == pdPASS ) { // execute operation only if resuorce is free
        nrf_drv_twi_enable( &m_i2c[interface] ); // enable I2C
        err_code = nrf_drv_twi_tx( &m_i2c[interface], slave_addr, &reg_addr, 1, true );
        if(err_code == 33281){
          return err_code;
        }
        //APP_ERROR_CHECK( err_code );
        err_code = nrf_drv_twi_rx( &m_i2c[interface], slave_addr, p_buff, length );
        if(err_code == 33281){
          return err_code;
        }
        //APP_ERROR_CHECK( err_code );
        nrf_drv_twi_disable( &m_i2c[interface] );
        xSemaphoreGive( m_i2c_semaphores[interface] ); // 'Give' the semaphore to unblock the blocked task.

    } else { err_code = NRF_ERROR_BUSY; }

    return err_code;
}

/*  */
ret_code_t xUtil_TWI_Write( i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t const *p_buff, uint16_t length ) {
    #ifdef VERB_HW_TWI
    NRF_LOG_INFO( "xUtil_TWI_Write:\tslave = 0x%02X\taddr = 0x%02X\tlenght = %u", slave_addr, start_addr, length );
    #endif
    ret_code_t err_code;
    static uint8_t reg_addr;
    reg_addr = start_addr;

    if( length == 0 ) {					
        err_code = NRF_ERROR_INVALID_LENGTH;

    } else if( xSemaphoreTake( m_i2c_semaphores[interface], portMAX_DELAY ) == pdPASS ) {	// execute operation only if resuorce is free
        nrf_drv_twi_enable( &m_i2c[interface] );// enable I2C
        err_code = nrf_drv_twi_tx( &m_i2c[interface], slave_addr, &reg_addr, 1, true );
        APP_ERROR_CHECK( err_code );
        err_code = nrf_drv_twi_tx( &m_i2c[interface], slave_addr, p_buff, length, false );
        APP_ERROR_CHECK( err_code );
        nrf_drv_twi_disable( &m_i2c[interface] );
        xSemaphoreGive( m_i2c_semaphores[interface] );	// 'Give' the semaphore to unblock the blocked task.

    } else { err_code = NRF_ERROR_BUSY; }

    return err_code;
}

ret_code_t xUtil_TWI_Write_Single( i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t const *p_buff, uint16_t length ) {
    #ifdef VERB_HW_TWI
    NRF_LOG_INFO( "xUtil_TWI_Write:\tslave = 0x%02X\taddr = 0x%02X\tlenght = %u", slave_addr, start_addr, length );
    #endif
    ret_code_t err_code;
    static uint8_t reg_addr;
    reg_addr = start_addr;

    if( length == 0 ) {					
        err_code = NRF_ERROR_INVALID_LENGTH;

    } else if( xSemaphoreTake( m_i2c_semaphores[interface], portMAX_DELAY ) == pdPASS ) {	// execute operation only if resuorce is free
        nrf_drv_twi_enable( &m_i2c[interface] );// enable I2C
        //err_code = nrf_drv_twi_tx( &m_i2c[interface], slave_addr, &reg_addr, 1, true );
        //APP_ERROR_CHECK( err_code );
        err_code = nrf_drv_twi_tx( &m_i2c[interface], slave_addr, p_buff, length, false );
        APP_ERROR_CHECK( err_code );
        nrf_drv_twi_disable( &m_i2c[interface] );
        xSemaphoreGive( m_i2c_semaphores[interface] );	// 'Give' the semaphore to unblock the blocked task.

    } else { err_code = NRF_ERROR_BUSY; }

    return err_code;
}