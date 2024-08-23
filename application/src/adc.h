#ifndef ADC_H
#define ADC_H

#include "naatos_config.h"
#include "nrf_drv_saadc.h"

#define OPTICAL_CHANNEL 0
#define HAL_CHANNEL     1

#define ADC_LSB         3.515625f
#define NOMINAL_HAL     1.0f

void init_adc(void);
bool get_optical_triggered(void);
bool get_hal_triggered(void);
float get_adc_voltage(nrf_saadc_value_t adc_val);

#endif