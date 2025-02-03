#include "charger_driver.h"
#include "logger/logger.h"

//uint32_t read_pd_register(uint8_t reg) {
//  uint8_t data[4];

//  ret_code_t ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, reg, data, 4);

//  if(ret) {
//    printf("fail read\r\n");
//  }

//  return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
//}

uint32_t read_pd_register_multi(uint8_t reg, uint8_t* rxdata, uint8_t rxlen) {
  //uint8_t data[4];
  // https://www.ti.com/lit/ug/slvuc05a/slvuc05a.pdf

  // THE Device will respond with the first byte being the number of bytes following
  // thus we will memcpy positions 1 to  nbytes into positions 0 to nbytes-1
  
  // as a reslt, rxdata buffer should always be 1 larger than the rxlen


  // Execute the read transaction
  ret_code_t ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, reg, rxdata, rxlen+1);
  if(ret) {
    printf("fail read pd multi\r\n");
  }

  
  // First byte is the number of bytes in the response
  uint8_t nbytesrx = rxdata[0];
  if(nbytesrx!=rxlen)  {
    // something weird, response was not the length requested
    printf("read_pd_register_multi rxlen=%d nbytes=%d ABNORMAL\r\n",rxlen,nbytesrx);
    return 0;

  } else{
    // all good, nbytesrx matched the requested
    // do the array copy operation
    memcpy(&rxdata[0],&rxdata[1],nbytesrx);
    // place 0 in the rxlen+1 position
    rxdata[rxlen] = 0;
    return nbytesrx;
  }

}


uint32_t read_pd_register32(uint8_t reg) {
  uint8_t data[5];

  //ret_code_t ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, reg, data, 4);

  //if(ret) {
  //  printf("fail read\r\n");
  //}

  ret_code_t ret = read_pd_register_multi( reg, data, 4 );

  return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}



bool pd_eeprom_init_complete() {
  // https://www.ti.com/lit/ug/slvuc05a/slvuc05a.pdf
  // MODE is a 4-byte  4cc code, if "APP " then that is the mode we want here

  //uint32_t mode;
  //char modestr[5];
  //mode = read_pd_register32(MODE_REGISTER);
  //memcpy(modestr,(char*) &mode,4); // copy 4 bytes into modestr
  //modestr[4] = 0;  // terminating null

  uint8_t rxbuf_mode[5];
  uint32_t mode;
  ret_code_t ret = read_pd_register_multi(MODE_REGISTER, rxbuf_mode, 4);
  mode = (rxbuf_mode[0] << 24) | (rxbuf_mode[1] << 16) | (rxbuf_mode[2] << 8) | rxbuf_mode[3];
  printf("PD mode:0x%08x modestr:\"%s\"\n", mode,(const char*) rxbuf_mode);
  return mode == APP_MODE;
  //return strncmp( (const char*) rxbuf_mode,"APP ", 5 );
}

int pd_controller_i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint16_t value) { //TODO currently will only work for 1byte registers
  uint8_t data[8];
  uint8_t rxData[7];

  // Prepare the I2Cw command
  data[0] = DATA1_REGISTER;
  data[1] = 5; //length
  data[2] = slave_addr;
  data[3] = 0x02;

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

int pd_controller_i2c_write16(uint8_t slave_addr, uint8_t reg_addr, uint16_t value) { //TODO currently will only work for 1byte registers
  uint8_t data[8];
  uint8_t rxData[7];

  // Prepare the I2Cw command
  data[0] = DATA1_REGISTER;
  data[1] = 6; //length
  data[2] = slave_addr;
  data[3] = 0x03;

  // Load the target slave address and data
  data[4] = 0x00;
  data[5] = reg_addr;          // Target I2C slave address (battery charger)
  data[6] = (uint8_t)(value >> 8); 
  data[7] = (uint8_t)(value & 0xFF);


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
  pd_controller_i2c_write(BQ25792_ADDR, 0x0F, 0xA2);                // FIX FOR CCG BOARDS, SETS 0x0F TO DEFAULT
  pd_controller_i2c_write(BQ25792_ADDR, REG0E_Timer_Control, 0x05); // Keep all defaults but disable EN_CHG_TMR, EN_TRICHG_TMR, EN_PRECHG_TMR
  pd_controller_i2c_write(BQ25792_ADDR, 0x12, 0x04);                // Disable BATFET LDO mode in precharge state (disables minimum system voltage regulation)
  pd_controller_i2c_write(BQ25792_ADDR, 0x08, 0x03);                // Set BAT LOWV to the lowest % so that charger avoids precharge
  pd_controller_i2c_write16(BQ25792_ADDR, 0x03, 0x01F4);            // Set charge current limit to maximum of 5A (500 * 10mA = 0x01F4), 9-bit register
}

int read_charger_register(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t length) {
  ret_code_t ret;
  uint8_t cmd[7];
  uint8_t rxData[6];

  ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, CMD1_REGISTER, rxData, 4);

  // Prepare the I2Cw command
  cmd[0] = DATA1_REGISTER;
  cmd[1] = 3; // length
  cmd[2] = slave_addr;
  cmd[3] = reg_addr;
  cmd[4] = length;          // size of device register
  ret = xUtil_TWI_Write_Single(i2c_interface_sensors, TPS25750_I2C_ADDRESS, DATA1_REGISTER, cmd, 5);


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
  vTaskDelay(pdMS_TO_TICKS(25));

  ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, CMD1_REGISTER, rxData, 4);

  //TODO need a delay here
  vTaskDelay(pdMS_TO_TICKS(25));

  ret = xUtil_TWI_Read( i2c_interface_sensors, TPS25750_I2C_ADDRESS, DATA1_REGISTER, data, 2 + length);


  if(ret) {
    printf("fail read reg\r\n");
  }

    return 0;
}

int test_read_reg() {
  uint8_t pd_type[11] = {0};
  //uint8_t chgdata[2] = {0};
  uint8_t pd_devinfo[52] = {0};

  
  read_pd_register_multi(0x04, pd_type, 4);  // TYPE register (4 bytes)
  read_pd_register_multi(0x2F, pd_devinfo, 40); // DEVICE_INFO register (40 bytes)



  uint8_t chg_partinfo[3] = {0};
  read_charger_register(BQ25792_ADDR, 0x48, chg_partinfo, 1); // PARTINFO

  uint8_t chg_voltage_limit[6] = {0};
  read_charger_register(BQ25792_ADDR, 0x01, chg_voltage_limit, 2); // REG01_Charge_Voltage_Limit
  // was 840d = 8.40V

  uint8_t chg_adc_vbat[6] = {0};
  read_charger_register(BQ25792_ADDR, 0x3B, chg_adc_vbat, 2); // REG3B_VBAT_ADC
  // was 840d = 8.40V
  
  //read_pd_register_multi(0x14, pddata, 11);
  //read_charger_register(BQ25792_ADDR, 0x3B, chgdata, 2);

  //read_charger_register(BQ25792_ADDR, 0x0F, data, 1);
  //read_charger_register(BQ25792_ADDR, REG0E_Timer_Control, data, 1);
  //read_charger_register(BQ25792_ADDR, 0x12, data, 1);
  //read_charger_register(BQ25792_ADDR, 0x08, data, 1);
  //read_charger_register(BQ25792_ADDR, 0x03, data, 2);

  //read_charger_register(BQ25792_ADDR, 0x00, data, 2);
  //read_charger_register(BQ25792_ADDR, 0x01, data, 2);
  //read_charger_register(BQ25792_ADDR, 0x3B, data, 2);
  return 0;
}