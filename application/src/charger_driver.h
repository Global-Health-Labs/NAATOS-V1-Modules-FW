#include "i2c_hal_freertos.h"
#include "naatos_config.h"
#include <stdbool.h>
#include <stdint.h>

#define TPS25750_I2C_ADDRESS 0x23
#define BOOT_STATUS_REGISTER 0x2D
#define BQ25792_ADDR 0x6B
//#define APP_MODE 0x04415050 // ASCII for 'APP '
#define APP_MODE 0x41505020 // ASCII for 'APP '
#define MODE_REGISTER 0x03

#define CMD1_REGISTER 0x08        // Command register
#define DATA1_REGISTER 0x09       // Data register

#define REG0F_Charger_Control_0 0x0F
#define REG0E_Timer_Control 0x0E


uint32_t read_pd_register(uint8_t reg);

bool pd_eeprom_init_complete();

int pd_controller_i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint16_t value); //TODO currently will only work for 1byte registers

int pd_controller_i2c_write16(uint8_t slave_addr, uint8_t reg_addr, uint16_t value);

int setup_charger();

int read_charger_register(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t length);


int test_read_reg();