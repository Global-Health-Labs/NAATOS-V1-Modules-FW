#include "charger_driver.h"
#include "logger/logger.h"

uint32_t read_pd_register(uint8_t reg) {
  uint8_t data[4];

  ret_code_t ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, reg, data, 4);

  if(ret) {
    printf("fail read\r\n");
  }

  return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}



bool pd_eeprom_init_complete() {
  uint32_t mode;
  mode = read_pd_register(MODE_REGISTER);
  printf("mode:%u", mode);
  return mode == APP_MODE;
}

int pd_controller_i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint16_t value) { //TODO currently will only work for 1byte registers
  uint8_t data[9];
  uint8_t rxData[7];

  // Prepare the I2Cw command
  data[0] = DATA1_REGISTER;
  data[1] = 5; //length
  data[2] = slave_addr;
  data[3] = 0x03;

  // Load the target slave address and data
  data[4] = 0x00;
  data[5] = reg_addr;          // Target I2C slave address (battery charger)
  data[6] = value;
  data[7] = value;            // Register address to write to


  ret_code_t ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS25750_I2C_ADDRESS, DATA1_REGISTER, data, 8);

  if (ret) {
    printf("fail write\r\n");
    send_debug_log_message("Unable to i2c communicate with pd chip!");
    return -1;
  }

   ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, DATA1_REGISTER, rxData, 6);

  // Prepare data to write
  uint8_t value_data[7];
  value_data[0] = CMD1_REGISTER;
  value_data[1] = 4; // length
  value_data[2] = 0x49;
  value_data[3] = 0x32;
  value_data[4] = 0x43;
  value_data[5] = 0x77;


  ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS25750_I2C_ADDRESS, CMD1_REGISTER, value_data, 6);

  if (ret) {
    send_debug_log_message("Error writing data to Data1 on PD!");
    return -1;
  }

   ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, CMD1_REGISTER, rxData, 4);


  return 0;
}

int setup_charger() {
  pd_controller_i2c_write(BQ25792_ADDR, REG0E_Timer_Control, 0x35); // Keep all defaults but disable EN_CHG_TMR
}

int read_charger_register(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t length) {
  uint8_t cmd[7];
  uint8_t rxData[6];


  // Prepare the I2Cw command
  cmd[0] = DATA1_REGISTER;
  cmd[1] = 6; //length
  cmd[2] = slave_addr;
  cmd[3] = reg_addr;
  cmd[4] = 1;          // size of device register


  ret_code_t ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS25750_I2C_ADDRESS, DATA1_REGISTER, cmd, 5);


    // Prepare the I2Cr command
    cmd[0] = CMD1_REGISTER;
    cmd[1] = 4; // length
    cmd[2] = 0x49; // 'I2Cr' command
    cmd[3] = 0x32;
    cmd[4] = 0x43;
    cmd[5] = 0x72;


   ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS25750_I2C_ADDRESS, CMD1_REGISTER, cmd, 6);

  if (ret) {
    printf("fail write\r\n");
    send_debug_log_message("Unable to i2c communicate with pd chip!");
    return -1;
  }

  //TODO need a delay here

  ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, CMD1_REGISTER, rxData, 4);

  //TODO need a delay here

  ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, DATA1_REGISTER, data, 3);


  if(ret) {
    printf("fail read reg\r\n");
  }

    return 0;
}

int test_read_reg() {
  uint8_t data[1];
  read_charger_register(BQ25792_ADDR, REG0E_Timer_Control, data, 1);

  return 0;
}