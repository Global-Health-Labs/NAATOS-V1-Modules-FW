#include "fuel.h"
#include "app_scheduler.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define WRITE_BUFFER_SIZE 3
#define READ_BUFFER_SIZE 3
typedef struct {
  uint8_t regSelection;
  uint8_t readLength;
  uint8_t readData[READ_BUFFER_SIZE];
} fg_init_op_read_buffer_t;

typedef enum {
  fg_init_read_op,
  fg_init_write_op,
  fg_get_soc,
  fg_get_batt_voltage,
  fg_get_remaining_capacity,
  fg_get_battery_current,
  fg_load_params,
  fg_save_params,
} fuel_gauge_internal_op_type_t;

static bool fuelGaugeIsBusy = false;
static fram_fuel_gauge_params_t savedParams = {0};
static fuel_gauge_internal_op_type_t current_op_type = fg_init_read_op;

static uint8_t writeBuffer[WRITE_BUFFER_SIZE] = {0};
static fg_init_op_read_buffer_t readBuffer = {0};
static i2c_status_t initOpOutcome = i2c_success;

static fuelGauge_opDoneCallback_t savedUserCallback = NULL;

static uint32_t currentStepInLoadSaveStateMachine = 0;
static const uint32_t numStepsInLoadSaveStateMachine = 5;

static fuel_gauge_errors_t fuelGauge_writeRegister(uint8_t reg,
    uint16_t value,
    fuel_gauge_internal_op_type_t fg_operation,
    fuelGauge_opDoneCallback_t cb);
static fuel_gauge_errors_t fuelGauge_readRegister(uint8_t reg, fuel_gauge_internal_op_type_t fg_operation, fuelGauge_opDoneCallback_t cb);

void i2cDoneCallback(i2c_status_t outcome, uint8_t data_len, uint8_t *p_data_read) {
  fuel_gauge_callback_data_t userData;
  fuelGaugeIsBusy = false;

  switch (current_op_type) {
  default:
    // Never reached
    break;
  case fg_init_read_op:
    initOpOutcome = outcome;
    // Data was written directly to buffer, just record number of bytes actually transferred
    readBuffer.readLength = data_len;
    break;
  case fg_init_write_op:
    initOpOutcome = outcome;
    break;
  case fg_get_soc: {
    if (savedUserCallback != NULL) {
      userData.data_type = fuel_gauge_get_batt_percent;
      if (outcome != i2c_success) {
        savedUserCallback(fuel_gauge_i2c_error, &userData);
      } else if (data_len != 2) {
        savedUserCallback(fuel_gauge_i2c_error, &userData);
      } else {
        uint8_t battPercent = readBuffer.readData[1];
        if (battPercent < 0) {
          battPercent = 0;
        }
        userData.data.batteryChargePercent = battPercent;
        savedUserCallback(fuel_gauge_success, &userData);
      }
    }
    break;
  }
  case fg_get_batt_voltage: {
    if (savedUserCallback != NULL) {
      userData.data_type = fuel_gauge_get_batt_voltage;
      if (outcome != i2c_success) {
        savedUserCallback(fuel_gauge_i2c_error, &userData);
      } else if (data_len != 2) {
        savedUserCallback(fuel_gauge_i2c_error, &userData);
      } else {
        uint16_t regVal = readBuffer.readData[0];
        regVal += (uint16_t)readBuffer.readData[1] << 8;
        float tempRegVal = regVal;
        tempRegVal *= MAX77658_FUEL_GAUGE_VOLTAGE_LSB;
        userData.data.battVoltage = round(tempRegVal);
        savedUserCallback(fuel_gauge_success, &userData);
      }
    }
    break;
  }
  case fg_get_remaining_capacity: {
    if (savedUserCallback != NULL) {
      userData.data_type = fuel_gauge_get_remaining_capacity;
      if (outcome != i2c_success) {
        savedUserCallback(fuel_gauge_i2c_error, &userData);
      } else if (data_len != 2) {
        savedUserCallback(fuel_gauge_i2c_error, &userData);
      } else {
        uint16_t regVal = readBuffer.readData[0];
        regVal += (uint16_t)readBuffer.readData[1] << 8;
        float tempRegVal = regVal;
        tempRegVal *= MAX77658_FUEL_GAUGE_REPCAP_CONV_FACTOR;
        userData.data.remainingCapacityMilliampHours = round(tempRegVal);
        savedUserCallback(fuel_gauge_success, &userData);
      }
    }
    break;
  }
  case fg_get_battery_current: {
    if (savedUserCallback != NULL) {
      userData.data_type = fuel_gauge_get_battery_current;
      if (outcome != i2c_success) {
        savedUserCallback(fuel_gauge_i2c_error, &userData);
      } else if (data_len != 2) {
        savedUserCallback(fuel_gauge_i2c_error, &userData);
      } else {
        int16_t regVal;
        char *copyPointer = (char *)&regVal;
        copyPointer[0] = readBuffer.readData[0];
        copyPointer[1] = readBuffer.readData[1];
        userData.data.batteryCurrentMilliamps = regVal;
        userData.data.batteryCurrentMilliamps *= (MAX77658_FG_CURRENT_LSB);
        savedUserCallback(fuel_gauge_success, &userData);
      }
    }
    break;
  }
  }
}

static fuel_gauge_errors_t fuelGauge_writeRegister(uint8_t reg,
    uint16_t value,
    fuel_gauge_internal_op_type_t fg_operation,
    fuelGauge_opDoneCallback_t cb) {
  if (fuelGaugeIsBusy) {
    return fuel_gauge_busy;
  }

  fuelGaugeIsBusy = true;
  uint32_t err_code;

  writeBuffer[0] = (value & 0x00FF); //LSB of value
  writeBuffer[1] = (value >> 8);     //MSB of value

  current_op_type = fg_operation;
  err_code = xUtil_TWI_Write(i2c_interface_sensors, MAX77658_FUELGAUGE_ADDRESS, reg, writeBuffer, 2);

  if (err_code != NRF_SUCCESS) {
    fuelGaugeIsBusy = false;
    if (err_code == NRF_ERROR_BUSY) {
      return fuel_gauge_busy;
    }
    return fuel_gauge_i2c_error;
  }
  fuelGaugeIsBusy = false;
  return fuel_gauge_success;
}

// Used for init only, do not make public
static fuel_gauge_errors_t fuelGauge_writeRegisterBlocking(uint8_t reg, uint16_t value) {
  fuel_gauge_errors_t fuelError = fuelGauge_writeRegister(reg, value, fg_init_write_op, NULL);

  if (fuelError != fuel_gauge_success) {
    return fuelError;
  }

  return fuel_gauge_success;
}

static fuel_gauge_errors_t fuelGauge_readRegister(uint8_t reg, fuel_gauge_internal_op_type_t fg_operation, fuelGauge_opDoneCallback_t cb) {
  if (fuelGaugeIsBusy) {
    return fuel_gauge_busy;
  }

  fuelGaugeIsBusy = true;
  uint32_t err_code;

  current_op_type = fg_operation;
  err_code = xUtil_TWI_Read(i2c_interface_sensors, MAX77658_FUELGAUGE_ADDRESS, reg, &readBuffer.readData[0], 2); //acquire data

  if (err_code != NRF_SUCCESS) {
    fuelGaugeIsBusy = false;
    if (err_code == NRF_ERROR_BUSY) {
      return fuel_gauge_busy;
    }
    return fuel_gauge_i2c_error;
  }
  fuelGaugeIsBusy = false;
  return fuel_gauge_success;
}

// Used for init only, do not make public
static fuel_gauge_errors_t fuelGauge_readRegisterBlocking(uint8_t reg, uint16_t *p_data) {
  fuel_gauge_errors_t fuelError = fuelGauge_readRegister(reg, fg_init_read_op, NULL);

  if (fuelError != fuel_gauge_success) {
    return fuelError;
  }

  uint16_t result = readBuffer.readData[0]; //LSB is reported first
  result += ((uint16_t)readBuffer.readData[1]) << 8;
  *p_data = result;

  return fuel_gauge_success;
}

fuel_gauge_errors_t fuelGauge_init(void) {
  /******************************************************************************************************
    Fuel Gauge Init
    ******************************************************************************************************/
  fuel_gauge_errors_t fuelGaugeError = fuel_gauge_success;
  uint16_t hibcfg_original = 0; //used to store original value of hibernation config register
  uint16_t rxBuf = 0;
  uint16_t count = 0;

  fuel_gauge_errors_t opError = fuelGauge_readRegisterBlocking(0x00, &rxBuf); //only get the POR bit
  if (opError != fuel_gauge_success) {
    return opError;
  }

  if (1) //rxBuf & 0x02)
  {
    //    return fuel_gauge_success;

    opError = fuelGauge_readRegisterBlocking(0x3D, &rxBuf);
    if (opError != fuel_gauge_success) {
      return opError;
    }
    uint8_t count2 = 0;
    while (rxBuf & 0x01) {
      vTaskDelay(10); //wait until FSTAT.DNR == 0
      opError = fuelGauge_readRegisterBlocking(0x3D, &rxBuf);
      if (opError != fuel_gauge_success) {
        return opError;
      }
      count2++;
      if (count2 > 150) //timeout
      {
        fuelGaugeError = fuel_gauge_timeout;
        break;
      }
    }

    //store original hibcfg value before waking up
    opError = fuelGauge_readRegisterBlocking(0xBA, &hibcfg_original);
    if (opError != fuel_gauge_success) {
      return opError;
    }

    opError = fuelGauge_writeRegisterBlocking(0x60, 0x90); //exit hibernation step one
    if (opError != fuel_gauge_success) {
      return opError;
    }
    opError = fuelGauge_writeRegisterBlocking(0xBA, 0x00); //exit hibernation step two
    if (opError != fuel_gauge_success) {
      return opError;
    }
    opError = fuelGauge_writeRegisterBlocking(0x60, 0x00); //exit hibernation step three
    if (opError != fuel_gauge_success) {
      return opError;
    }

    //ez config register setup
    //opError = fuelGauge_writeRegisterBlocking(0xBA, 0x00); //exit hibernation step two
    //if (opError != fuel_gauge_success) {
    //    return opError;
    //}

    opError = fuelGauge_writeRegisterBlocking(0x18, BATTERY_MAH * MAX77658_FUEL_GAUGE_DESIGNCAP_MULTI_FACTOR); // 2600mAh design cap
    if (opError != fuel_gauge_success) {
      return opError;
    }
    opError = fuelGauge_writeRegisterBlocking(0x1E, 0x0168); //11.25mA termination current
    if (opError != fuel_gauge_success) {
      return opError;
    }
    opError = fuelGauge_writeRegisterBlocking(0x3A, 0x9661); //empty cell voltage = 3.0v, recovery = 3.88
    if (opError != fuel_gauge_success) {
      return opError;
    }

    opError = fuelGauge_writeRegisterBlocking(0x1D, 0x2200); //Disable external thermistor
    if (opError != fuel_gauge_success) {
      return opError;
    }

    opError = fuelGauge_writeRegisterBlocking(0xDB, 0x8000); //write modelcfg
    if (opError != fuel_gauge_success) {
      return opError;
    }

    opError = fuelGauge_readRegisterBlocking(0xDB, &rxBuf);
    if (opError != fuel_gauge_success) {
      return opError;
    }
    count = 0;
    while (rxBuf & 0x8000) {
      vTaskDelay(10); //wait until modelCFG.Refresh = 0
      opError = fuelGauge_readRegisterBlocking(0xDB, &rxBuf);
      if (opError != fuel_gauge_success) {
        return opError;
      }
      count++;
      if (count > 150) //timeout
      {
        fuelGaugeError = fuel_gauge_timeout;
        break;
      }
    }

    opError = fuelGauge_writeRegisterBlocking(0xBA, hibcfg_original); //restore hibcfg
    if (opError != fuel_gauge_success) {
      return opError;
    }

    opError = fuelGauge_readRegisterBlocking(0x00, &rxBuf); //read status
    if (opError != fuel_gauge_success) {
      return opError;
    }
    opError = fuelGauge_writeRegisterBlocking(0x00, (rxBuf & 0xFD)); //clear POR flag
    if (opError != fuel_gauge_success) {
      return opError;
    }

    //setLearnedParams();
  }

  //set cycles bit value
  //opError = fuelGauge_readRegisterBlocking(0x17, &rxBuf);
  //if (opError != fuel_gauge_success) {
  //    return opError;
  //}
  //cycleBitStatus = (rxBuf & 0x02) >> 1;

  return fuelGaugeError;
}

uint8_t fuelGauge_getSOC(fuelGauge_opDoneCallback_t cb) {
  fuelGauge_readRegister(MAX77658_FG_REG_REPSOC, fg_get_soc, cb);
  uint8_t battPercent = readBuffer.readData[1];
  if (battPercent < 0) {
    battPercent = 0;
  }
  return battPercent;
}

double fuelGauge_getBattVoltage(fuelGauge_opDoneCallback_t cb) {
  fuelGauge_readRegister(MAX77658_FG_REG_VCELL, fg_get_batt_voltage, cb);
  uint16_t regVal = readBuffer.readData[0];
  regVal += (uint16_t)readBuffer.readData[1] << 8;
  float tempRegVal = regVal;
  tempRegVal *= MAX77658_FUEL_GAUGE_VOLTAGE_LSB;
  return round(tempRegVal);
}

double fuelGauge_getRepCap(fuelGauge_opDoneCallback_t cb) //capacity remaining in mAh
{
  fuelGauge_readRegister(MAX77658_FG_REG_REPCAP, fg_get_remaining_capacity, cb);
  uint16_t regVal = readBuffer.readData[0];
  regVal += (uint16_t)readBuffer.readData[1] << 8;
  float tempRegVal = regVal;
  tempRegVal *= MAX77658_FUEL_GAUGE_REPCAP_CONV_FACTOR;
  return round(tempRegVal);
}

//return the battery current in mA
double fuelGauge_getBattCurrent(fuelGauge_opDoneCallback_t cb) {
  fuelGauge_readRegister(MAX77658_FG_REG_AVGCURRENT, fg_get_battery_current, cb);
  int16_t regVal;
  char *copyPointer = (char *)&regVal;
  copyPointer[0] = readBuffer.readData[0];
  copyPointer[1] = readBuffer.readData[1];
  regVal *= (MAX77658_FG_CURRENT_LSB);
  return regVal;
}

// Load the parameters into this file.
fuel_gauge_errors_t fuelGauge_loadParams(fram_fuel_gauge_params_t *p_params, fuelGauge_opDoneCallback_t cb) {
  if (fuelGaugeIsBusy) {
    return fuel_gauge_busy;
  }

  currentStepInLoadSaveStateMachine = 0;
  savedUserCallback = cb;

  memcpy(&savedParams, p_params, sizeof(fram_fuel_gauge_params_t));

  return fuelGauge_writeRegister(0x38, savedParams.Saved_RCOMP0, fg_load_params, NULL);
}

// Save the parameters from this file.
fuel_gauge_errors_t fuelGauge_saveParams(fuelGauge_opDoneCallback_t cb) {
  if (fuelGaugeIsBusy) {
    return fuel_gauge_busy;
  }

  currentStepInLoadSaveStateMachine = 0;
  savedUserCallback = cb;

  return fuelGauge_readRegister(0x38, fg_save_params, NULL); //Read RCOMP0
}