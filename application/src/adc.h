#ifndef ADC_H
#define ADC_H

#include "naatos_config.h"
#include "nrf_drv_saadc.h"

#define OPTICAL_CHANNEL 0

void init_adc(void);
bool get_optical_triggered(void);

#endif