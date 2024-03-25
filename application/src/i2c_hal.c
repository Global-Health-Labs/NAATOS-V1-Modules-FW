#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "i2c_hal.h"
#include "nrf_drv_twi.h"
#include "nrfx_twi.h"

#include "app_scheduler.h"
//#include "main.h"


#define NO_FLAGS 0

typedef struct {
    nrf_drv_twi_evt_t evt_data;
    i2c_interface_selection_t interface;
} scheduled_data_t;

static const nrf_drv_twi_t m_i2c[i2c_num_interfaces] = {
    NRF_DRV_TWI_INSTANCE(I2C_INSTANCE_ID),
};
static bool m_i2c_driver_is_busy[i2c_num_interfaces] = {false};
static i2c_transaction_complete_cb_t saved_user_cb[i2c_num_interfaces] = {NULL};

void i2c_scheduled_event_handler(void * p_event_data, uint16_t event_size)
{
    i2c_status_t result;
    uint8_t data_len;
    uint8_t * p_data;
    scheduled_data_t * p_event = (scheduled_data_t*)p_event_data;
    i2c_interface_selection_t interface = p_event->interface;
    
    m_i2c_driver_is_busy[interface] = false;
    
    switch (p_event->evt_data.type) {
        default:
            // Should never reach here
            break;
        case NRF_DRV_TWI_EVT_DONE:         ///< Transfer completed event.
            if (NULL != saved_user_cb[interface]) {
                result = i2c_success;
                
                // Only need to point to data if data was received
                if (NRFX_TWI_XFER_TXRX == p_event->evt_data.xfer_desc.type) {
                    p_data = p_event->evt_data.xfer_desc.p_secondary_buf;
                    data_len = p_event->evt_data.xfer_desc.secondary_length;
                }
                else if (NRFX_TWI_XFER_RX == p_event->evt_data.xfer_desc.type) {
                    p_data = p_event->evt_data.xfer_desc.p_primary_buf;
                    data_len = p_event->evt_data.xfer_desc.primary_length;
                }
                else {
                    p_data = NULL;
                    data_len = 0;
                }
                
                saved_user_cb[interface](result, data_len, p_data);
            }
            break;
        case NRF_DRV_TWI_EVT_ADDRESS_NACK: ///< Error event: NACK received after sending the address.
            if (NULL != saved_user_cb[interface]) {
                result = i2c_addr_nack;
                p_data = NULL;
                data_len = 0;
                saved_user_cb[interface](result, data_len, p_data);
            }
            break;
        case NRF_DRV_TWI_EVT_DATA_NACK:    ///< Error event: NACK received after sending a data byte.
            if (NULL != saved_user_cb[interface]) {
                result = i2c_data_nack;
                p_data = NULL;
                data_len = 0;
                saved_user_cb[interface](result, data_len, p_data);
            }
            break;
    }
}

static void i2c_interrupt_handler_system_bus(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    // Just pass to the scheduler to quickly get this out of interrupt context
    scheduled_data_t scheduled_data;
    memcpy(&scheduled_data.evt_data, p_event, sizeof(nrf_drv_twi_evt_t));
    scheduled_data.interface = i2c_interface_system;
    scheduler_add_event(&scheduled_data, sizeof(scheduled_data_t), i2c_scheduled_event_handler);
}

// unused since we are using a single i2c bus
static void i2c_interrupt_handler_sensor_bus(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    // Just pass to the scheduler to quickly get this out of interrupt context
    scheduled_data_t scheduled_data;
    memcpy(&scheduled_data.evt_data, p_event, sizeof(nrf_drv_twi_evt_t));
    scheduled_data.interface = i2c_interface_sensors;
    scheduler_add_event(&scheduled_data, sizeof(scheduled_data_t), i2c_scheduled_event_handler);
}

void app_i2c_setup(i2c_interface_selection_t interface, uint32_t sda_pin, uint32_t scl_pin, i2c_speed_selection_t frequency)
{
    ret_code_t err_code;
    nrf_drv_twi_frequency_t nrf_freq;
    nrf_drv_twi_evt_handler_t interrupt_handler;
    
    if (interface >= i2c_num_interfaces) {
        return;
    }
    
    if (i2c_speed_100k == frequency) {
        nrf_freq = NRF_DRV_TWI_FREQ_100K;
    }
    else {
        nrf_freq = NRF_DRV_TWI_FREQ_400K;
    }

    const nrf_drv_twi_config_t i2c_bus_config = {
       .scl                = scl_pin,
       .sda                = sda_pin,
       .frequency          = nrf_freq,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false,
       .hold_bus_uninit    = true
    };
    interrupt_handler = i2c_interrupt_handler;
    
    switch (interface) {
        default:
        // Unreached
        break;
        case i2c_interface_fuel:
            interrupt_handler = i2c_interrupt_handler_system_bus;
            break;
        case i2c_interface_audio:
            interrupt_handler = i2c_interrupt_handler_sensor_bus;
            break;
    }
    

    err_code = nrf_drv_twi_init(&m_i2c[interface], &i2c_bus_config, interrupt_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_i2c[interface]);
}

bool i2c_is_busy(i2c_interface_selection_t interface)
{
    if (interface >= i2c_num_interfaces) {
        return true;
    }
    
    bool module_is_busy = false;
    
    if (m_i2c_driver_is_busy[interface] || nrf_drv_twi_is_busy(&m_i2c[interface])) {
        module_is_busy = true;
    }
    
    return (module_is_busy);
}

i2c_status_t i2c_write_data(i2c_interface_selection_t interface, i2c_write_transaction_t * p_data)
{
    ret_code_t err_code;
    i2c_status_t status;
    static uint8_t * p_write_data;
    static uint8_t write_len;
    static uint8_t addr;
    static bool stop;
    
    if (interface >= i2c_num_interfaces) {
        return (i2c_busy);
    }
    
    if (m_i2c_driver_is_busy[interface]) {
        return (i2c_busy);
    }
    
    saved_user_cb[interface] = p_data->cb;
    __DMB();
    __DSB();
    __ISB();
    p_write_data = p_data->p_write_data;
    write_len = p_data->write_len;
    addr = p_data->addr;
    stop = p_data->stop;
    m_i2c_driver_is_busy[interface] = true;
    
    // Note: Final parameter of nrf_drv_twi_tx is "no_stop"
    err_code = nrf_drv_twi_tx(&m_i2c[interface], addr, p_write_data, write_len, !stop);
    
    if (NRF_SUCCESS == err_code) {
        return (i2c_success);
    }
    else if (NRF_ERROR_BUSY == err_code) {
        status = i2c_busy;
    }
    else if (NRF_ERROR_INTERNAL == err_code) {
        status = i2c_unknown_error;
    }
    else {
        status = i2c_failure;
    }
    
    m_i2c_driver_is_busy[interface] = false;
    return (status);
}

i2c_status_t i2c_read_data(i2c_interface_selection_t interface, i2c_read_transaction_t * p_data)
{
    ret_code_t err_code;
    i2c_status_t status;
    static uint8_t * p_read_data;
    static uint8_t read_len;
    static uint8_t addr;
    
    if (interface >= i2c_num_interfaces) {
        return (i2c_busy);
    }
    
    if (m_i2c_driver_is_busy[interface]) {
        return (i2c_busy);
    }
    
    saved_user_cb[interface] = p_data->cb;
    __DMB();
    __DSB();
    __ISB();
    p_read_data = p_data->p_read_data;
    read_len = p_data->read_len;
    addr = p_data->addr;
    m_i2c_driver_is_busy[interface] = true;
    
    err_code = nrf_drv_twi_rx(&m_i2c[interface], addr, p_read_data, read_len);
    
    if (NRF_SUCCESS == err_code) {
        return (i2c_success);
    }
    else if (NRF_ERROR_BUSY == err_code) {
        status = i2c_busy;
    }
    else if (NRF_ERROR_INTERNAL == err_code) {
        status = i2c_unknown_error;
    }
    else {
        status = i2c_failure;
    }
    
    m_i2c_driver_is_busy[interface] = false;
    return (status);
}

i2c_status_t i2c_write_read_with_repeated_start(i2c_interface_selection_t interface, i2c_write_read_with_repeated_start_transaction_t * p_data)
{
    uint32_t flags;
    static nrf_drv_twi_xfer_desc_t i2c_transfer_desc;
    ret_code_t err_code;
    i2c_status_t status;
    
    if (interface >= i2c_num_interfaces) {
        return (i2c_busy);
    }
    
    // This one's unique, you can't provide a NULL callback or the Nordic driver won't accept it
    if (NULL == p_data->cb) {
        return (i2c_failure);
    }
    else if (m_i2c_driver_is_busy[interface]) {
        return (i2c_busy);
    }
    
    saved_user_cb[interface] = p_data->cb;
    __DMB();
    __DSB();
    __ISB();
    m_i2c_driver_is_busy[interface] = true;
    
    i2c_transfer_desc.address = p_data->addr;
    i2c_transfer_desc.p_primary_buf = p_data->p_register_selection;
    i2c_transfer_desc.primary_length = p_data->register_len;
    i2c_transfer_desc.p_secondary_buf = p_data->p_read_data;
    i2c_transfer_desc.secondary_length = p_data->read_len;
    i2c_transfer_desc.type = NRFX_TWI_XFER_TXRX;
    
    // No flags are necessary for this transfer, they are defined in nrfx_twim.h if you're curious what
    // options there are
    err_code = nrf_drv_twi_xfer(&m_i2c[interface], &i2c_transfer_desc, NO_FLAGS);
    
    if (NRF_SUCCESS == err_code) {
        return (i2c_success);
    }
    else if (NRF_ERROR_BUSY == err_code) {
        status = i2c_busy;
    }
    else if (NRF_ERROR_INTERNAL == err_code) {
        status = i2c_unknown_error;
    }
    else {
        status = i2c_failure;
    }
    
    m_i2c_driver_is_busy[interface] = false;
    return (status);
}
