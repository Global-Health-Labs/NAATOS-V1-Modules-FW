#include "led.h"

led_color led1_current_color = blue;
led_color led2_current_color = blue;

/* LED 1 */
void set_led1_green_solid(void) {
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, green, NULL);
  led_driver_set_channel_animation_solid(LED1, green, true, NULL);
  led1_current_color = green;
}

void set_led1_green_breathe(void) {
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, green, NULL);
  led_driver_set_channel_animation_breathing(LED1, green, true, NULL);
  led1_current_color = green;
}

void set_led1_blue_slow_blink(void) {
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, blue, NULL);
  led_driver_set_channel_animation_flashing(LED1, blue, true, NULL);
  led1_current_color = blue;
}

void set_led1_red_slow_blink(void) {

}

void set_led1_red_fast_blink(void) {
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, red, NULL);
  led_driver_set_channel_animation_flashing(LED1, red, true, NULL);
  led1_current_color = red;
}

void set_led1_red_solid(void) {

}

/* LED 2 */
void set_led2_red_slow_blink(void) {

}

void set_led2_red_fast_blink(void) {
  led_driver_disable_channel(LED2, led2_current_color, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_set_channel_animation_flashing(LED2, red, true, NULL);
  led2_current_color = red;
}

void set_led2_green_solid(void) {

}

void set_led2_blue_breathe(void) {
  led_driver_disable_channel(LED2, led2_current_color, NULL);
  led_driver_enable_channel(LED2, blue, NULL);
  led_driver_set_channel_animation_breathing(LED2, blue, true, NULL);
  led2_current_color = blue;
}

void turn_off_led2(void) {
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_enable_channel(LED2, green, NULL);
  led_driver_enable_channel(LED2, blue, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
}
