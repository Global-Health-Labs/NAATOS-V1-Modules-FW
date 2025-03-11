#include "tps55288.h"
#include "logger/logger.h"

// Set Boost to output 6v
bool valve_zone_set_6v_and_configure_and_ENABLE(void) {
  bool ret = true;
  i2c_status_t status;

  // TPS55288 device has 8 registers, but only first 7 will be written, first in this buffer is destination address
  uint8_t rxbuf[8]; //<-- read the final register which is STATUS
  uint8_t txbuf[8] = {
    0x00,   //<-- destination register address for multiple-write
    
    // register values follow

    // 0x00, 0x01 "REF" , Reference Voltage
    // Set the Internal reference voltage to 1129mV
    0b00000110,     // reg 0x00
    0b00000001,     // reg 0x01

    // 0x02 "IOUT" , Current Limit Setting
    // default value in datasheet: 11100100b standing for 50 mV. 1 LSB stands for 0.5 mV
    0b11100100,     // reg 0x02

    // 0x03 "VOUT_SR"  , Slew Rate
    // default is 0b00000001, so it is 128us so immediately limits the output current, and 2.5 mV/us
    0b00000001,     //reg 0x03

    // 0x04 "VOUT_FS"  , Feedback Selection
    // default is 0b00000011 which is what ODIC has used
    0b00000011,     // reg 0x04,     // Set Feedback To Internal and ratio 0.0564

    // 0x05 "CDC"  , Cable Compensation
    // default is 0b11100000 which is what ODIC has used
    0b11100000,     // reg 0x05,

    // 0x06 "MODE"  , Mode control
    // default is 0b00100000 which is what ODIC has used
    // [7] - OE [0 default = output disabled, 1 = output enabled]
    //0b00100000,     // reg 0x06,
    0b10100000,     // reg 0x06,    //<-- default + output enabled!

    //// 0x07 "STATUS"  , Operating Status
    //// default is 0b11100000 which is what ODIC has used
    //// reg 0x04
    //0b11100000,     // reg 0x05,
  };

  // Send the above configuration parameters to the TPS55288
  status = tps55288_write_multi(txbuf,8);
  if (status != i2c_success) {
    ret = false;
    send_debug_log_message("TPS55288 Writing Settings Failed.");
    return ret;
  }

  // Readback all the registers from the TPS55288 to verify
  status = tps55288_read_multi(0x00,rxbuf,8); //<-- include additional
  if (status != i2c_success) {
    ret = false;
    send_debug_log_message("TPS55288 Reading Settings Failed.");
    return ret;
  }

  // Compare and verify the first 7 register values (not the final status)
  if( memcmp(rxbuf,&(txbuf[1]),7) == 0 ) {
    // values match perfectly
    //
  } else{
    // values did not match!
    ret = false;
    send_debug_log_message("TPS55288 Registers Miscompare");
    return ret;
  }

  

  //// Set VOUT_FS to External Resistor
  //uint8_t ex_data = 0b00000011;
  //i2c_status_t status = tps55288_write_reg8(0x04, &ex_data);
  //if (status != i2c_success) {
  //  ret = false;
  //  send_debug_log_message("TPS55288 Setting External Resistor Failure.");
  //  return ret;
  //}
  
  //// Read and ensure data was written correctly
  //uint8_t ex_recv_data;
  //status = tps55288_read_reg8(0x04, &ex_recv_data);
  //if (status != i2c_success) {
  //  send_debug_log_message("TPS55288 Readback of External Resistor Failure.");
  //  ret = false;
  //  return ret;
  //}

  //// Confirm the data
  //if (ex_data != ex_recv_data) {
  //  send_debug_log_message("TPS55288 External Resistor Mismatch Failure.");
  //  ret = false;
  //  return ret;
  //}



  //// Set the Internal reference voltage to 1129mV
  //uint8_t data[2];
  //data[0] = 0b00000110;
  //data[1] = 0b00000001;
  //status = tps55288_write_reg16(0x00, data);
  //if (status != i2c_success) {
  //  ret = false;
  //  send_debug_log_message("TPS55288 Setting Internal Voltage Reference Failure.");
  //  return ret;
  //}
  
  //// Read and ensure data was written correctly
  //uint8_t recv_data[2];
  //status = tps55288_read_reg16(0x00, recv_data);
  //if (status != i2c_success) {
  //  send_debug_log_message("TPS55288 Readback of Internal Voltage Reference Failure.");
  //  ret = false;
  //  return ret;
  //}

  //// Confirm the data
  //if (data[0] != recv_data[0] || data[1] != recv_data[1]) {
  //  send_debug_log_message("TPS55288 Internal Voltage Reference Mismatch Failure.");
  //  ret = false;
  //  return ret;
  //}
 
  return ret;
}

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
  uint8_t data = 0b10000000;

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
  uint8_t data = 0b00000000;

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

i2c_status_t tps55288_write_multi(uint8_t *txbuf, uint8_t txsize){
  i2c_status_t i2c_ret = i2c_success;

  //uint8_t data[3];
  //data[0] = addr;
  //data[1] = tx[0];
  //data[2] = tx[1];

  // first byte in txbuf must be the destination address

  //ret_code_t ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS55288_ADDRESS, txbuf[0] /*unused*/, data, 3);
  ret_code_t ret = xUtil_TWI_Write(i2c_interface_sensors, TPS55288_ADDRESS, txbuf[0] /*GHL thinks unused?*/, txbuf,txsize);
  if (ret) {
    i2c_ret = i2c_failure;
  }

  return i2c_ret;
}

i2c_status_t tps55288_read_multi(uint8_t regaddr, uint8_t *rxbuf, uint8_t rxsize) {
  i2c_status_t i2c_ret = i2c_success;

  ret_code_t ret = xUtil_TWI_Read(i2c_interface_sensors, TPS55288_ADDRESS, regaddr, rxbuf, rxsize);
  if (ret) {
    i2c_ret = i2c_failure;
  }

  return i2c_ret;
}
