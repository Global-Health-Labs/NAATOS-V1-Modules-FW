#include "charger_driver.h"
#include "logger/logger.h"

uint32_t read_pd_register(uint8_t reg) {
  uint8_t data[7];

  ret_code_t ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, reg, data, 4);

  return (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
}



bool pd_eeprom_init_complete() {
  uint32_t mode;
  mode = read_pd_register(MODE_REGISTER);
  return mode != APP_MODE;
}

int pd_controller_i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint16_t value) {
  uint8_t data[6];

  // Prepare the I2Cw command
  data[0] = 'I';                 // Command 'I2Cw'
  data[1] = '2';
  data[2] = 'C';
  data[3] = 'w';

  // Load the target slave address and data
  data[4] = slave_addr;          // Target I2C slave address (battery charger)
  data[5] = reg_addr;            // Register address to write to


  ret_code_t ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS25750_I2C_ADDRESS, CMD1_REGISTER, data, 6);

  if (ret) {
    send_debug_log_message("Unable to i2c communicate with pd chip!");
    return -1;
  }

  // Prepare data to write
  uint8_t value_data[2];
  value_data[0] = (value >> 8) & 0xFF; // MSB of the value
  value_data[1] = value & 0xFF;        // LSB of the value

  ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS25750_I2C_ADDRESS, DATA1_REGISTER, value_data, 2);

  if (ret) {
    send_debug_log_message("Error writing data to Data1 on PD!");
    return -1;
  }


  return 0;
}

int setup_charger() {
  pd_controller_i2c_write(BQ25792_ADDR, REG0E_Timer_Control, 0x35); // Keep all defaults but disable EN_CHG_TMR
}