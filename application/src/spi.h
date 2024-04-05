#ifndef  SPI_H
#define  SPI_H

#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
 
void vInit_SPI_Hardware(uint32_t miso_pin, uint32_t mosi_pin, uint32_t clk_pin, uint32_t ss_pin);

#endif
