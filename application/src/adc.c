#include "adc.h"

void saadc_callback_handler(nrf_drv_saadc_evt_t const *p_event) {
  // Empty handler function
}

bool get_optical_triggered(void) {
  nrf_saadc_value_t adc_val;
  // Start adc sample conversion and get value
  nrfx_saadc_sample_convert(OPTICAL_CHANNEL, &adc_val);
  //printf("Optical Value: %d\n", adc_val);
  // Check to see if adc value is below threshold
#ifndef SAMPLE_PREP_BOARD
  if (use_default_configuration_parameters) {
    if (adc_val < OPTICAL_TRIG_THRES)
      return true;
  } else {
    if (adc_val < config.optical_distance)
      return true;
  }
#endif
  return false;
}

void init_adc(void) {
  ret_code_t err;

  // Create channel configuration and assign it defualt values
  // Create the config to be on AIN1 (P0.03) as an input and single ended
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);

  // Initalize saadc
  err = nrf_drv_saadc_init(NULL, saadc_callback_handler);
  APP_ERROR_CHECK(err);

  // Initalize the channel that will be connected to the pin
  err = nrfx_saadc_channel_init(OPTICAL_CHANNEL, &channel_config);
  APP_ERROR_CHECK(err);
}