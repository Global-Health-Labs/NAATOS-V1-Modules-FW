#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "tsys03.h"
#include "i2c_hal.h"
#include "delay_hal.h"
#include "app_scheduler.h"
#include "main.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define WRITE_BUFFER_SIZE 1
#define READ_BUFFER_SIZE 4
typedef struct {
    uint8_t regSelection;
    uint8_t readLength;
    uint8_t readData[READ_BUFFER_SIZE];
} tsys03_init_op_read_buffer_t;

typedef enum {
    tsys03_init_read_op,
    tsys03_init_write_op,
    tsys03_reset_op,
    tsys03_read_serial_op,
    tsys03_start_conversion_op,
    tsys03_read_adc_op
} tsys03_internal_op_type_t;

static bool sensorIsBusy = false;
static tsys03_internal_op_type_t current_op_type = tsys03_init_read_op;

static tsys03_opDoneCallback_t savedUserCallback = NULL;

static uint8_t writeBuffer = 0;
static tsys03_init_op_read_buffer_t readBuffer = {0};
static i2c_status_t initOpOutcome = i2c_success;
static bool conversionStarted = false;

static tsys03_errors_t tsys03_readRegister(uint8_t reg, tsys03_internal_op_type_t ts_operation, tsys03_opDoneCallback_t cb);


void tsys03i2cDoneCallback(i2c_status_t outcome, uint8_t data_len, uint8_t * p_data_read)
{
    tsys03_callback_data_t userData;
    sensorIsBusy = false;
    
    switch (current_op_type) {
        default:
            // Never reached
            break;
        case tsys03_init_read_op:
            initOpOutcome = outcome;
            // Data was written directly to buffer, just record number of bytes actually transferred
            readBuffer.readLength = data_len;
            break;
        case tsys03_init_write_op:
            initOpOutcome = outcome;
            break;
        case tsys03_reset_op:
            initOpOutcome = outcome;
            break;
        case tsys03_start_conversion_op:
            initOpOutcome = outcome;
            conversionStarted = true;
            //savedUserCallback(tsys03_success, &userData);
            break;
        case tsys03_read_adc_op:
        {
            if (savedUserCallback != NULL) {
                userData.data_type = tsys03_read_adc;
                if (outcome != i2c_success) {
                    savedUserCallback(tsys03_i2c_error, &userData);
                } else if (data_len != 3) {
                    savedUserCallback(tsys03_i2c_error, &userData);
                } else {
                    uint16_t adcRaw = ((uint16_t) readBuffer.readData[0] << 8) + ((uint16_t) readBuffer.readData[1]);
                    double temperature = -40 + adcRaw  * 165 / 65535.0;
                    userData.data.temperature = temperature;
                    userData.data.crc = readBuffer.readData[2];
                    savedUserCallback(tsys03_success, &userData);
                }
            }
            break;
        }
    }
}

static tsys03_errors_t tsys03_writeRegister(uint8_t reg, 
                                                   tsys03_internal_op_type_t ts_operation, 
                                                   tsys03_opDoneCallback_t cb)
{
    if (sensorIsBusy) {
        return tsys03_busy;
    }
    
    sensorIsBusy = true;

    writeBuffer = reg;

    i2c_write_transaction_t i2c_trans_info;
    i2c_trans_info.p_write_data = &writeBuffer;
    i2c_trans_info.write_len = 1;
    i2c_trans_info.addr = TSYS03_ADDR;
    i2c_trans_info.cb = tsys03i2cDoneCallback;
    i2c_trans_info.stop = true; //single write then stop

    current_op_type = ts_operation;
    savedUserCallback = cb;
    i2c_status_t i2c_trans_status = i2c_write_data(i2c_interface_master, &i2c_trans_info);//acquire data
    
    if (i2c_trans_status != i2c_success) {
        sensorIsBusy = false;
        if (i2c_trans_status == i2c_busy) {
            return tsys03_busy;
        }
        return tsys03_i2c_error;
    }
    
    return tsys03_success;
}

static tsys03_errors_t tsys03_readRegister(uint8_t reg, tsys03_internal_op_type_t ts_operation, tsys03_opDoneCallback_t cb)
{
    if (sensorIsBusy) {
        return tsys03_busy;
    }
    
    sensorIsBusy = true;
    
    
    i2c_write_read_with_repeated_start_transaction_t i2c_trans_info;

    //setup I2C read
    readBuffer.regSelection = reg;
    i2c_trans_info.addr = TSYS03_ADDR;
    i2c_trans_info.p_register_selection = &readBuffer.regSelection;
    i2c_trans_info.register_len = 1;
    i2c_trans_info.p_read_data = &readBuffer.readData[0];
    if (ts_operation == tsys03_read_adc_op)
    {
        i2c_trans_info.read_len = 3;
    } else if (ts_operation == tsys03_read_serial_op) {
        i2c_trans_info.read_len = 4;
    } 
    //i2c_trans_info.read_len = 3;
    i2c_trans_info.cb = tsys03i2cDoneCallback;

    current_op_type = ts_operation;
    savedUserCallback = cb;
    i2c_status_t i2c_trans_status = i2c_write_read_with_repeated_start(i2c_interface_master, &i2c_trans_info);//acquire data
    
    if (i2c_trans_status != i2c_success) {
        sensorIsBusy = false;
        if (i2c_trans_status == i2c_busy) {
            return tsys03_busy;
        }
        return tsys03_i2c_error;
    }
  
    return tsys03_success;
}

tsys03_errors_t tsys03_startConversion(tsys03_opDoneCallback_t cb)
{
    return tsys03_writeRegister(TSYS03_START_CONVERSION_COMMAND, tsys03_start_conversion_op, cb);
}

tsys03_errors_t tsys03_getTemp(tsys03_opDoneCallback_t cb)
{
    return tsys03_readRegister(TSYS03_READ_ADC_COMMAND, tsys03_read_adc_op, cb);
}