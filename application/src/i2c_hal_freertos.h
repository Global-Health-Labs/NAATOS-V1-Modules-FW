#ifndef I2C_HAL_FREERTOS_H__
#define I2C_HAL_FREERTOS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define I2C_SYSTEM_INSTANCE_ID 0
#define I2C_SENSOR_INSTANCE_ID 1

#define I2C_SYSTEM_SDA 16
#define I2C_SYSTEM_SCL 17

#define I2C_SENSOR_SDA 23
#define I2C_SENSOR_SCL 24

#define I2C_

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

#define I2C_INSTANCE_ID 1
#ifdef __cplusplus
}
#endif
#endif // I2C_HAL_FREERTOS_H__