#include "tsys01.h"
#include "app_scheduler.h"
#include "i2c_hal_freertos.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define WRITE_BUFFER_SIZE 1
#define READ_BUFFER_SIZE 4

typedef enum {
  tsys01_init_read_op,
  tsys01_init_write_op,
  tsys01_reset_op,
  tsys01_read_serial_op,
  tsys01_start_conversion_op,
  tsys01_read_adc_op
} tsys01_internal_op_type_t;

static bool sensorIsBusy = false;
static tsys01_internal_op_type_t current_op_type = tsys01_init_read_op;

static tsys01_calibration_values_t calibration[4] = {0};

static uint8_t writeBuffer = 0;

static tsys01_errors_t tsys01_readRegister(sensor_selection_t sensor, uint8_t reg, tsys01_internal_op_type_t ts_operation, uint8_t *read_buf, uint8_t read_len);

static tsys01_errors_t tsys01_writeRegister(sensor_selection_t sensor, uint8_t reg,
    tsys01_internal_op_type_t ts_operation) {
  if (sensorIsBusy) {
    return tsys01_busy;
  }

  sensorIsBusy = true;

  writeBuffer = reg;
  uint32_t err_code;
  i2c_interface_selection_t interface;
  uint8_t slave_addr;

  current_op_type = ts_operation;

  switch (sensor) {
  case valve_zone:
    interface = i2c_interface_system;
    slave_addr = TSYS01_ADDR_ALT;
    break;
  case amp_zone_0:
    interface = i2c_interface_system;
    slave_addr = TSYS01_ADDR;
    break;
  case amp_zone_1:
    interface = i2c_interface_sensors;
    slave_addr = TSYS01_ADDR_ALT;
    break;
  case amp_zone_2:
    interface = i2c_interface_sensors;
    slave_addr = TSYS01_ADDR;
    break;
  }
  err_code = xUtil_TWI_Write(interface, slave_addr, reg, &writeBuffer, 1);

  if (err_code != NRF_SUCCESS) {
    sensorIsBusy = false;
    if (err_code == NRF_ERROR_BUSY) {
      return tsys01_busy;
    }
    return tsys01_i2c_error;
  }
  sensorIsBusy = false;
  return tsys01_success;
}

static tsys01_errors_t tsys01_readRegister(sensor_selection_t sensor, uint8_t reg, tsys01_internal_op_type_t ts_operation, uint8_t *read_buf, uint8_t read_len) {
  if (sensorIsBusy) {
    return tsys01_busy;
  }

  sensorIsBusy = true;

  uint32_t err_code;
  i2c_interface_selection_t interface;
  uint8_t slave_addr;

  current_op_type = ts_operation;

  switch (sensor) {
  case valve_zone:
    interface = i2c_interface_system;
    slave_addr = TSYS01_ADDR_ALT;
    break;
  case amp_zone_0:
    interface = i2c_interface_system;
    slave_addr = TSYS01_ADDR;
    break;
  case amp_zone_1:
    interface = i2c_interface_sensors;
    slave_addr = TSYS01_ADDR_ALT;
    break;
  case amp_zone_2:
    interface = i2c_interface_sensors;
    slave_addr = TSYS01_ADDR;
    break;
  }

  err_code = xUtil_TWI_Read(interface, slave_addr, reg, read_buf, read_len);

  if (err_code != NRF_SUCCESS) {
    sensorIsBusy = false;
    if (err_code == NRF_ERROR_BUSY) {
      return tsys01_busy;
    }

    printf("I2C Read error hit 3\n");
    return tsys01_i2c_error;
  }
  sensorIsBusy = false;
  return tsys01_success;
}

tsys01_errors_t tsys01_startConversion(sensor_selection_t sensor) {
  return tsys01_writeRegister(sensor, TSYS01_START_CONVERSION_COMMAND, tsys01_start_conversion_op);
}

tsys01_errors_t tsys01_getTemp(sensor_selection_t sensor, float *temp_val) {
  uint8_t buf[3] = {};
  tsys01_errors_t err_code = tsys01_readRegister(sensor, TSYS01_READ_ADC_COMMAND, tsys01_read_adc_op, buf, TSYS01_TEMP_DATA_LEN);
  uint32_t adc24 = (buf[0] << 16) + (buf[1] << 8) + buf[2];
  uint32_t adc16 = adc24 / 256.0;
  float temp_c = (float)((-2) * calibration[sensor].k4 * pow(10, -21) * pow(adc16, 4) + (4) * calibration[sensor].k3 * pow(10, -16) * pow(adc16, 3) + (-2) * calibration[sensor].k2 * pow(10, -11) * pow(adc16, 2) + (1) * calibration[sensor].k1 * pow(10, -6) * adc16 + (-1.5) * calibration[sensor].k0 * pow(10, -2));

  if ((temp_c < 0) || (temp_c > 110)) {
    //err_code = tsys01_out_of_range;
    *temp_val = temp_c;
  } else {
    *temp_val = temp_c;
  }

  return err_code;
}

tsys01_errors_t tsys01_getCalibrationValues(sensor_selection_t sensor) {
  uint8_t temp[2];
  tsys01_errors_t err_code = tsys01_readRegister(sensor, TSYS01_CAL_K0_REG, tsys01_cal, temp, TSYS01_CAL_DATA_LEN);
  if (err_code == tsys01_i2c_error) {
    //sensor not connected
    return err_code;
  }
  calibration[sensor].k0 = (temp[0] << 8) + temp[1];
  err_code = tsys01_readRegister(sensor, TSYS01_CAL_K1_REG, tsys01_cal, temp, TSYS01_CAL_DATA_LEN);
  calibration[sensor].k1 = (temp[0] << 8) + temp[1];
  err_code = tsys01_readRegister(sensor, TSYS01_CAL_K2_REG, tsys01_cal, temp, TSYS01_CAL_DATA_LEN);
  calibration[sensor].k2 = (temp[0] << 8) + temp[1];
  err_code = tsys01_readRegister(sensor, TSYS01_CAL_K3_REG, tsys01_cal, temp, TSYS01_CAL_DATA_LEN);
  calibration[sensor].k3 = (temp[0] << 8) + temp[1];
  err_code = tsys01_readRegister(sensor, TSYS01_CAL_K4_REG, tsys01_cal, temp, TSYS01_CAL_DATA_LEN);
  calibration[sensor].k4 = (temp[0] << 8) + temp[1];
  return err_code;
}