#pragma once

#include "i2c_hal_freertos.h"
#include "naatos_config.h"

#define TPS55288_ADDRESS  0x74

// Set Boost to output 6v
bool valve_zone_set_6v_and_configure_and_ENABLE(void);
bool valve_zone_set_6v(void);

// Enable/Disable the boost
bool enable_valve_boost(void);
bool disable_valve_boost(void);

// Write/Read to 8 bit reg
i2c_status_t tps55288_read_reg8(uint8_t addr, uint8_t *rx);
i2c_status_t tps55288_write_reg8(uint8_t addr, uint8_t *tx);

// Write/Read to 16 bit reg
i2c_status_t tps55288_read_reg16(uint8_t addr, uint8_t *rx);
i2c_status_t tps55288_write_reg16(uint8_t addr, uint8_t *tx); 

// Multiple Write/Read
i2c_status_t tps55288_write_multi(uint8_t *txbuf, uint8_t txsize);
i2c_status_t tps55288_read_multi(uint8_t regaddr, uint8_t *rxbuf, uint8_t rxsize);

uint8_t tps55288_read_status(void);