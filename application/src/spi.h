#ifndef SPI_H
#define SPI_H

#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

void vInit_SPI_Hardware(uint32_t miso_pin, uint32_t mosi_pin, uint32_t clk_pin, uint32_t ss_pin);

#endif