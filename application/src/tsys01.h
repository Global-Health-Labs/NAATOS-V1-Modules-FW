#ifndef _TSYS01_DRIVER_H_
#define _TSYS01_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define TSYS01_ADDR 0b1110110
#define TSYS01_ADDR_ALT 0b1110111

#define TSYS01_RESET_COMMAND 0x1E
#define TSYS01_START_CONVERSION_COMMAND 0x48
#define TSYS01_READ_ADC_COMMAND 0x00

typedef enum {
    tsys01_success,
    tsys01_i2c_error,
    tsys01_busy,
    tsys01_timeout
} tsys01_errors_t;

typedef enum {
    valve_zone,
    amp_zone_1,
    amp_zone_2,
    amp_zone_3,
    num_sensors
} sensor_selection_t;

typedef enum {
    tsys01_start_conversion,
    tsys01_read_adc,
    tsys01_reset,
} tsys01_op_type_t;

typedef union {
    double temperature;
    uint8_t crc;
} tsys01_data_t;

typedef struct {
    tsys01_op_type_t data_type;
    tsys01_data_t data;
} tsys01_callback_data_t;

typedef void (*tsys01_opDoneCallback_t)(tsys01_errors_t outcome, tsys01_callback_data_t *p_data);

tsys01_errors_t tsys01_startConversion(sensor_selection_t sensor, tsys01_opDoneCallback_t cb);
tsys01_errors_t tsys01_getTemp(sensor_selection_t sensor, tsys01_opDoneCallback_t cb);

#ifdef __cplusplus
}
#endif
#endif