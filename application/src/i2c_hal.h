#ifndef I2C_HAL_H__
#define I2C_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define I2C_INSTANCE_ID 1

typedef enum {
    i2c_interface_system,
    i2c_interface_sensors,
    i2c_num_interfaces
} i2c_interface_selection_t;

typedef enum {
    i2c_speed_100k,
    i2c_speed_400k,
    i2c_speed_number_of_selections
} i2c_speed_selection_t;

typedef enum {
    i2c_success,
    i2c_failure,
    i2c_busy,
    i2c_unknown_error,
    i2c_addr_nack,
    i2c_data_nack,
    i2c_number_of_status_types
} i2c_status_t;

typedef void (*i2c_transaction_complete_cb_t)(i2c_status_t outcome, uint8_t data_len, uint8_t * p_data_read);

typedef struct {
    uint8_t * p_write_data;
    uint8_t write_len;
    uint8_t addr;
    bool stop;
    i2c_transaction_complete_cb_t cb;
} i2c_write_transaction_t;

typedef struct {
    uint8_t * p_read_data;
    uint8_t read_len;
    uint8_t addr;
    i2c_transaction_complete_cb_t cb;
} i2c_read_transaction_t;

typedef struct {
    void * p_register_selection;
    uint8_t register_len;
    uint8_t * p_read_data;
    uint8_t read_len;
    uint8_t addr;
    i2c_transaction_complete_cb_t cb;
} i2c_write_read_with_repeated_start_transaction_t;

// Call this function first to set up the i2c hardware
void app_i2c_setup(i2c_interface_selection_t interface, uint32_t sda_pin, uint32_t scl_pin, i2c_speed_selection_t frequency);

bool i2c_is_busy(i2c_interface_selection_t interface);

i2c_status_t i2c_write_data(i2c_interface_selection_t interface, i2c_write_transaction_t * p_data);

i2c_status_t i2c_read_data(i2c_interface_selection_t interface, i2c_read_transaction_t * p_data);

// Some devices require a write to set the register pointer to read from, followed by
// a read with no i2c stop being issued. Use this function for that use case.
i2c_status_t i2c_write_read_with_repeated_start(i2c_interface_selection_t interface, 
                                                i2c_write_read_with_repeated_start_transaction_t * p_data);

#ifdef __cplusplus
}
#endif
#endif // I2C_HAL_H__
