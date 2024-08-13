#ifndef I2C_HAL_FREERTOS_H__
#define I2C_HAL_FREERTOS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naatos_config.h"
#include "sdk_errors.h"
#include <stdbool.h>
#include <stdint.h>

#define I2C_SYSTEM_INSTANCE_ID 0
#define I2C_SENSOR_INSTANCE_ID 1

#define I2C_SYSTEM_SDA 16
#define I2C_SYSTEM_SCL 17

#define I2C_SENSOR_SDA 23
#define I2C_SENSOR_SCL 24

typedef enum {
  i2c_interface_system,
  i2c_interface_sensors,
  i2c_num_interfaces
} i2c_interface_selection_t;

typedef enum {
  i2c_success,
  i2c_failure,
  i2c_busy,
  i2c_unknown_error,
  i2c_addr_nack,
  i2c_data_nack,
  i2c_number_of_status_types
} i2c_status_t;

typedef enum {
  i2c_speed_100k,
  i2c_speed_400k,
  i2c_speed_number_of_selections
} i2c_speed_selection_t;

void vInit_TWI_Hardware(i2c_interface_selection_t interface, uint32_t sda_pin, uint32_t scl_pin, i2c_speed_selection_t frequency);
ret_code_t xUtil_TWI_Write(i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t const *p_buff, uint16_t length);
ret_code_t xUtil_TWI_Read(i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t *p_buff, uint16_t length);
ret_code_t xUtil_TWI_Write_Single(i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t const *p_buff, uint16_t length);

#ifdef __cplusplus
}
#endif
#endif // I2C_HAL_FREERTOS_H__