#include "switch.h"


void button_init(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(BUTTON_INPUT_PIN, NRF_GPIO_PIN_NOPULL); // tied to 3.3v internally 
  
}

void buttonTask(void * pvParameters) {


  //nrf_gpio_pin_read(BUTTON_INPUT_PIN)
}