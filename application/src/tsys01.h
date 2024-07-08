#ifndef _TSYS01_DRIVER_H_
#define _TSYS01_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define TSYS01_ADDR (0x77U)
#define TSYS01_ADDR_ALT (0x76U)

#define TSYS01_RESET_COMMAND 0x1E
#define TSYS01_START_CONVERSION_COMMAND 0x48
#define TSYS01_READ_ADC_COMMAND 0x00

#define TSYS01_CAL_K0_REG 0xAA
#define TSYS01_CAL_K1_REG 0xA8
#define TSYS01_CAL_K2_REG 0xA6
#define TSYS01_CAL_K3_REG 0xA4
#define TSYS01_CAL_K4_REG 0xA2

#define TSYS01_TEMP_DATA_LEN 3
#define TSYS01_CAL_DATA_LEN 2

typedef enum {
  tsys01_success,
  tsys01_i2c_error,
  tsys01_busy,
  tsys01_timeout,
  tsys01_out_of_range,
} tsys01_errors_t;

typedef enum {
  valve_zone,
  amp_zone_0,
  amp_zone_1,
  amp_zone_2,
  num_sensors
} sensor_selection_t;

typedef enum {
  tsys01_start_conversion,
  tsys01_read_adc,
  tsys01_reset,
  tsys01_cal,
} tsys01_op_type_t;

typedef union {
  double temperature;
  uint8_t crc;
} tsys01_data_t;

typedef struct {
  tsys01_op_type_t data_type;
  tsys01_data_t data;
} tsys01_callback_data_t;

typedef struct {
  uint16_t k0;
  uint16_t k1;
  uint16_t k2;
  uint16_t k3;
  uint16_t k4;
} tsys01_calibration_values_t;

typedef void (*tsys01_opDoneCallback_t)(tsys01_errors_t outcome, tsys01_callback_data_t *p_data);

tsys01_errors_t tsys01_startConversion(sensor_selection_t sensor);
tsys01_errors_t tsys01_getTemp(sensor_selection_t sensor, float *temp_val);
tsys01_errors_t tsys01_getCalibrationValues(sensor_selection_t sensor);

#ifdef __cplusplus
}
#endif
#endif