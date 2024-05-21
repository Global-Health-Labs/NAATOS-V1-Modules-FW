#ifndef  ADC_H
#define  ADC_H

#include "nrf_drv_saadc.h"

#define OPTICAL_CHANNEL   0

#define OPTICAL_TRIG_THRES  800

void init_adc(void);
bool get_optical_triggered(void);

#endif