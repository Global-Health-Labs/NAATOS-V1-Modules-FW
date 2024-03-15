#ifndef _TSYS03_DRIVER_H_
#define _TSYS03_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "i2c_hal.h"

#define TSYS03_ADDR 0x40
#define TSYS03_CRC 0x31

#define TSYS03_RESET_COMMAND 0x1E
#define TSYS03_READ_SERIAL_COMMAND 0x0A
#define TSYS03_START_CONVERSION_COMMAND 0x46
#define TSYS03_READ_ADC_COMMAND 0x00

typedef enum {
    tsys03_success,
    tsys03_i2c_error,
    tsys03_busy,
    tsys03_timeout
} tsys03_errors_t;

typedef enum {
    tsys03_get_serial_number,
    tsys03_start_conversion,
    tsys03_read_adc,
    tsys03_reset,
} tsys03_op_type_t;

typedef union {
    double temperature;
    uint8_t crc;
} tsys03_data_t;

typedef struct {
    tsys03_op_type_t data_type;
    tsys03_data_t data;
} tsys03_callback_data_t;

typedef void (*tsys03_opDoneCallback_t)(tsys03_errors_t outcome, tsys03_callback_data_t *p_data);

tsys03_errors_t tsys03_startConversion(tsys03_opDoneCallback_t cb);
tsys03_errors_t tsys03_getTemp(tsys03_opDoneCallback_t cb);

#ifdef __cplusplus
}
#endif
#endif