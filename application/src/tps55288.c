#include "tps55288.h"
#include "logger/logger.h"

// Set Boost to output 6v
bool valve_zone_set_6v(void) {
  bool ret = true;

  // Set VOUT_FS to External Resistor
  uint8_t ex_data = 0b00000011;
  i2c_status_t status = tps55288_write_reg8(0x04, &ex_data);
  if (status != i2c_success) {
    ret = false;
    send_debug_log_message("TPS55288 Setting External Resistor Failure.");
    return ret;
  }
  
  // Read and ensure data was written correctly
  uint8_t ex_recv_data;
  status = tps55288_read_reg8(0x04, &ex_recv_data);
  if (status != i2c_success) {
    send_debug_log_message("TPS55288 Readback of External Resistor Failure.");
    ret = false;
    return ret;
  }

  // Confirm the data
  if (ex_data != ex_recv_data) {
    send_debug_log_message("TPS55288 External Resistor Mismatch Failure.");
    ret = false;
    return ret;
  }

  // Set the Internal reference voltage to 1129mV
  uint8_t data[2];
  data[0] = 0b00000110;
  data[1] = 0b00000001;
  status = tps55288_write_reg16(0x00, data);
  if (status != i2c_success) {
    ret = false;
    send_debug_log_message("TPS55288 Setting Internal Voltage Reference Failure.");
    return ret;
  }
  
  // Read and ensure data was written correctly
  uint8_t recv_data[2];
  status = tps55288_read_reg16(0x00, recv_data);
  if (status != i2c_success) {
    send_debug_log_message("TPS55288 Readback of Internal Voltage Reference Failure.");
    ret = false;
    return ret;
  }

  // Confirm the data
  if (data[0] != recv_data[0] || data[1] != recv_data[1]) {
    send_debug_log_message("TPS55288 Internal Voltage Reference Mismatch Failure.");
    ret = false;
    return ret;
  }
 
  return ret;
}


// Enable/Disable the boost
bool enable_valve_boost(void) {
  bool ret = true;
  uint8_t data = 0b10000011;

  // Enable output
  i2c_status_t status = tps55288_write_reg8(0x06, &data);
  if (status != i2c_success) {
    send_debug_log_message("TPS55288 Setting Enable Failure.");
    ret = false;
    return ret;
  }

  // Read and ensure data was written correctly
  uint8_t recv_data;
  status = tps55288_read_reg8(0x06, &recv_data);
  if (status != i2c_success) {
    send_debug_log_message("TPS55288 Readback Enable Failure.");
    ret = false;
    return ret;
  }
  else if (recv_data != data) {
   send_debug_log_message("TPS55288 Enable Mismatch Failure.");
    ret = false;
    return ret;
  }

  return ret;
}

bool disable_valve_boost(void){
  bool ret = true;
  uint8_t data = 0b00000011;

  // Enable output
  i2c_status_t status = tps55288_write_reg8(0x06, &data);
  if (status != i2c_success) {
    send_debug_log_message("TPS55288 Setting Disable Failure.");
    ret = false;
    return ret;
  }

  // Read and ensure data was written correctly
  uint8_t recv_data;
  status = tps55288_read_reg8(0x06, &recv_data);
  if (status != i2c_success) {
    send_debug_log_message("TPS55288 Readback Disable Failure.");
    ret = false;
    return ret;
  }
  else if (recv_data != data) {
    send_debug_log_message("TPS55288 Disable Mismatch Failure.");
    ret = false;
    return ret;
  }

  return ret;
}

uint8_t tps55288_read_status(void) {
  // Read and ensure data was written correctly
  uint8_t recv_data = 0x00;
  i2c_status_t status = tps55288_read_reg8(0x06, &recv_data);
  if (status != i2c_success) {
    send_debug_log_message("TPS55288 Readback Enable Failure.");
  }

  return recv_data;
}


// Write/Read to 8 bit reg
i2c_status_t tps55288_read_reg8(uint8_t addr, uint8_t *rx){
  i2c_status_t i2c_ret = i2c_success;

  ret_code_t ret = xUtil_TWI_Read(i2c_interface_sensors, TPS55288_ADDRESS, addr, rx, 1);
  if (ret) {
    i2c_ret = i2c_failure;
  }
  
  return i2c_ret;
}

i2c_status_t tps55288_write_reg8(uint8_t addr, uint8_t *tx){
  i2c_status_t i2c_ret = i2c_success;
  uint8_t data[2];
  data[0] = addr;
  data[1] = tx[0];

  ret_code_t ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS55288_ADDRESS, addr, data, 2);
  if (ret) {
    i2c_ret = i2c_failure;
  }

  return i2c_ret;
}


// Write/Read to 16 bit reg
i2c_status_t tps55288_read_reg16(uint8_t addr, uint8_t *rx){
  i2c_status_t i2c_ret = i2c_success;

  ret_code_t ret = xUtil_TWI_Read(i2c_interface_sensors, TPS55288_ADDRESS, addr, rx, 2);
  if (ret) {
    i2c_ret = i2c_failure;
  }
  
  return i2c_ret;
}

i2c_status_t tps55288_write_reg16(uint8_t addr, uint8_t *tx){
  i2c_status_t i2c_ret = i2c_success;
  uint8_t data[3];
  data[0] = addr;
  data[1] = tx[0];
  data[2] = tx[1];

  ret_code_t ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS55288_ADDRESS, addr, data, 3);
  if (ret) {
    i2c_ret = i2c_failure;
  }

  return i2c_ret;
}


