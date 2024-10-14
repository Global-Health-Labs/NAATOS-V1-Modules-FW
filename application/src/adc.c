#include "adc.h"

void saadc_callback_handler(nrf_drv_saadc_evt_t const *p_event) {
  // Empty handler function
}

bool get_optical_triggered(void) {
#ifndef SAMPLE_PREP_BOARD
  nrf_saadc_value_t adc_val;
  // Start adc sample conversion and get value
  nrfx_saadc_sample_convert(OPTICAL_CHANNEL, &adc_val);
  // Check to see if adc value is below threshold
  if (use_default_configuration_parameters) {
    if (adc_val < OPTICAL_TRIG_THRES)
      return true;
  } else {
    if (adc_val < config.optical_distance)
      return true;
  }
  return false;
#else 
  return true;
#endif
}

bool get_hal_triggered(void) {
#ifdef SAMPLE_PREP_BOARD
  nrf_saadc_value_t adc_val;
  float hal_v;
  // Start adc sample conversion and get value
  nrfx_saadc_sample_convert(HAL_CHANNEL, &adc_val);
  // Convert to a voltage
  hal_v = get_adc_voltage(adc_val);

  // Check thresholds
  // Voltage idles at 1V and changes +/- 45 mV/mT depending on polarity of magnetic feild
  //    0V-2V
  if (use_default_configuration_parameters) {
    if (NOMINAL_HAL + DEFAULT_HAL_SENSOR_THRESHOLD <= hal_v || NOMINAL_HAL - DEFAULT_HAL_SENSOR_THRESHOLD >= hal_v) {
      return true;
    }
  } else {
    if (NOMINAL_HAL + config.hal_sensor_thresh <= hal_v || NOMINAL_HAL - config.hal_sensor_thresh >= hal_v) {
      return true;
    }
  }
  return false;
#else 
  return true;
#endif
}

float get_adc_voltage(nrf_saadc_value_t adc_val) {
  return (float)(adc_val) * ADC_LSB;
}

void init_adc(void) {
  ret_code_t err;

#ifndef SAMPLE_PREP_BOARD
  // Create channel configuration and assign it defualt values
  // Create the config to be on AIN0 (P0.03) as an input and single ended
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);

  // Initalize saadc
  err = nrf_drv_saadc_init(NULL, saadc_callback_handler);
  APP_ERROR_CHECK(err);

  // Initalize the channel that will be connected to the pin
  err = nrfx_saadc_channel_init(OPTICAL_CHANNEL, &channel_config);
  APP_ERROR_CHECK(err);
#else
  // Create channel configuration and assign it defualt values
  // Create the config to be on AIN1 (P0.01) as an input and single ended
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);

  // Initalize saadc
  err = nrf_drv_saadc_init(NULL, saadc_callback_handler);
  APP_ERROR_CHECK(err);

  // Initalize the channel that will be connected to the pin
  err = nrfx_saadc_channel_init(HAL_CHANNEL, &channel_config);
  APP_ERROR_CHECK(err);
#endif
}

void uninit_adc(void) {
  nrfx_saadc_uninit();
}