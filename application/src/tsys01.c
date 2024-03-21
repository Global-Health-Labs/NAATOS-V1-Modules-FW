#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "tsys01.h"
#include "i2c_hal_freertos.h"
#include "app_scheduler.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define WRITE_BUFFER_SIZE 1
#define READ_BUFFER_SIZE 4
typedef struct {
    uint8_t regSelection;
    uint8_t readLength;
    uint8_t readData[READ_BUFFER_SIZE];
} tsys01_init_op_read_buffer_t;

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

static tsys01_opDoneCallback_t savedUserCallback = NULL;

static uint8_t writeBuffer = 0;
static tsys01_init_op_read_buffer_t readBuffer = {0};
static i2c_status_t initOpOutcome = i2c_success;
static bool conversionStarted = false;

static tsys01_errors_t tsys01_readRegister(sensor_selection_t sensor, uint8_t reg, tsys01_internal_op_type_t ts_operation, tsys01_opDoneCallback_t cb);

void tsys01_task(void * pvParameters)
{
    
}


void tsys01i2cDoneCallback(i2c_status_t outcome, uint8_t data_len, uint8_t * p_data_read)
{
    tsys01_callback_data_t userData;
    sensorIsBusy = false;
    
    switch (current_op_type) {
        default:
            // Never reached
            break;
        case tsys01_init_read_op:
            initOpOutcome = outcome;
            // Data was written directly to buffer, just record number of bytes actually transferred
            readBuffer.readLength = data_len;
            break;
        case tsys01_init_write_op:
            initOpOutcome = outcome;
            break;
        case tsys01_reset_op:
            initOpOutcome = outcome;
            break;
        case tsys01_start_conversion_op:
            initOpOutcome = outcome;
            conversionStarted = true;
            //savedUserCallback(tsys01_success, &userData);
            break;
        case tsys01_read_adc_op:
        {
            if (savedUserCallback != NULL) {
                userData.data_type = tsys01_read_adc;
                if (outcome != i2c_success) {
                    savedUserCallback(tsys01_i2c_error, &userData);
                } else if (data_len != 3) {
                    savedUserCallback(tsys01_i2c_error, &userData);
                } else {
                    uint16_t adcRaw = ((uint16_t) readBuffer.readData[0] << 8) + ((uint16_t) readBuffer.readData[1]);
                    double temperature = -40 + adcRaw  * 165 / 65535.0;
                    userData.data.temperature = temperature;
                    userData.data.crc = readBuffer.readData[2];
                    savedUserCallback(tsys01_success, &userData);
                }
            }
            break;
        }
    }
}

static tsys01_errors_t tsys01_writeRegister(sensor_selection_t sensor, uint8_t reg, 
                                                   tsys01_internal_op_type_t ts_operation, 
                                                   tsys01_opDoneCallback_t cb)
{
    if (sensorIsBusy) {
        return tsys01_busy;
    }
    
    sensorIsBusy = true;

    writeBuffer = reg;
    uint32_t err_code;
    i2c_interface_selection_t interface;
    uint8_t slave_addr;

    current_op_type = ts_operation;
    savedUserCallback = cb;

    switch (sensor)
    {
        case valve_zone:
            interface = i2c_interface_system;
            slave_addr = TSYS01_ADDR;
            break;
        case amp_zone_1:
            interface = i2c_interface_system;
            slave_addr = TSYS01_ADDR_ALT;
            break;
        case amp_zone_2:
            interface = i2c_interface_sensors;
            slave_addr = TSYS01_ADDR;
            break;
        case amp_zone_3:
            interface = i2c_interface_sensors;
            slave_addr = TSYS01_ADDR_ALT;
            break;
    }
    err_code = xUtil_TWI_Write( interface, slave_addr, reg, &writeBuffer, 1 );
    
    if (err_code != NRF_SUCCESS) {
        sensorIsBusy = false;
        if (err_code == NRF_ERROR_BUSY) {
            return tsys01_busy;
        }
        return tsys01_i2c_error;
    }
    
    return tsys01_success;
}

static tsys01_errors_t tsys01_readRegister(sensor_selection_t sensor, uint8_t reg, tsys01_internal_op_type_t ts_operation, tsys01_opDoneCallback_t cb)
{
    if (sensorIsBusy) {
        return tsys01_busy;
    }
    
    sensorIsBusy = true;

    uint32_t err_code;
    i2c_interface_selection_t interface;
    uint8_t slave_addr;

    current_op_type = ts_operation;
    savedUserCallback = cb;

    switch (sensor)
    {
        case valve_zone:
            interface = i2c_interface_system;
            slave_addr = TSYS01_ADDR;
            break;
        case amp_zone_1:
            interface = i2c_interface_system;
            slave_addr = TSYS01_ADDR_ALT;
            break;
        case amp_zone_2:
            interface = i2c_interface_sensors;
            slave_addr = TSYS01_ADDR;
            break;
        case amp_zone_3:
            interface = i2c_interface_sensors;
            slave_addr = TSYS01_ADDR_ALT;
            break;
    }

    err_code = xUtil_TWI_Read( interface, slave_addr, readBuffer.regSelection, &readBuffer.readData[0], 3);
    
    if (err_code != NRF_SUCCESS) {
        sensorIsBusy = false;
        if (err_code == NRF_ERROR_BUSY) {
            return tsys01_busy;
        }
        return tsys01_i2c_error;
    }
  
    return tsys01_success;
}

tsys01_errors_t tsys01_startConversion(sensor_selection_t sensor, tsys01_opDoneCallback_t cb)
{
    return tsys01_writeRegister(sensor, TSYS01_START_CONVERSION_COMMAND, tsys01_start_conversion_op, cb);
}

tsys01_errors_t tsys01_getTemp(sensor_selection_t sensor, tsys01_opDoneCallback_t cb)
{
    return tsys01_readRegister(sensor, TSYS01_READ_ADC_COMMAND, tsys01_read_adc_op, cb);
}