#include "is31fl3196.h"
#include "stdbool.h"
#include <stdint.h>
#include "FreeRTOS.h"

#define ENABLE_LEDS 1

void set_led1_green_solid(void);
void set_led1_green_breathe(void);
void set_led1_blue_slow_blink(void);
void set_led1_red_slow_blink(void);
void set_led1_red_fast_blink(void);
void set_led1_red_solid(void);

void set_led2_red_slow_blink(void);
void set_led2_red_fast_blink(void);
void set_led2_green_solid(void);
void set_led2_blue_breathe(void);
void turn_off_led2(void);
