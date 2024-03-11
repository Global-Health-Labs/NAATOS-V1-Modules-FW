#ifndef CUSTOM_BOARD_H__
#define CUSTOM_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// LEDs definitions for custom board
#define LEDS_NUMBER    0

#define LEDS_ACTIVE_STATE 0

#define LEDS_INV_MASK  LEDS_MASK

#define LEDS_LIST {  }

#define BUTTONS_NUMBER 0
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST {  }

#ifdef __cplusplus
}
#endif

#endif
