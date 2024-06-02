#include "led.h"

led_color led1_current_color = blue;
led_color led2_current_color = blue;

xQueueHandle ledRxQueue;

static LEDFlags_t ledFlags;


void led_task(void * pvParameters) {
  BaseType_t xReturned;
  LEDRxQueueMsg_t ledRxMessage;

  for (;;) {
    xReturned = xQueueReceive(ledRxQueue, &ledRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("Unable to Rx data to sensor queue\n");
    } else {
      updateLedState(ledRxMessage.type, ledRxMessage.active);
    }
  }
}

/* LED 1 */
void set_led1_green_solid(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, green, NULL);
  led_driver_set_channel_animation_solid(LED1, green, true, NULL);
  led1_current_color = green;
#endif
}

void set_led1_green_breathe(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, green, NULL);
  led_driver_set_channel_animation_breathing(LED1, green, true, NULL);
  led1_current_color = green;
#endif
}

void set_led1_blue_slow_blink(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, blue, NULL);
  led_driver_set_channel_animation_flashing(LED1, blue, true, NULL);
  led1_current_color = blue;
#endif
}

void set_led1_red_slow_blink(void) {
#if ENABLE_LEDS
#endif
}

void set_led1_red_fast_blink(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, red, NULL);
  led_driver_set_channel_animation_flashing(LED1, red, true, NULL);
  led1_current_color = red;
#endif
}

void set_led1_red_solid(void) {
#if ENABLE_LEDS
#endif
}

/* LED 2 */
void set_led2_red_slow_blink(void) {
#if ENABLE_LEDS
#endif
}

void set_led2_red_fast_blink(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED2, led2_current_color, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_set_channel_animation_flashing(LED2, red, true, NULL);
  led2_current_color = red;
#endif
}

void set_led2_green_solid(void) {
#if ENABLE_LEDS

#endif
}

void set_led2_blue_breathe(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED2, led2_current_color, NULL);
  led_driver_enable_channel(LED2, blue, NULL);
  led_driver_set_channel_animation_breathing(LED2, blue, true, NULL);
  led2_current_color = blue;
#endif
}

void turn_off_led2(void) {
#if ENABLE_LEDS
  led_driver_set_channel_animation_solid(LED2, led2_current_color, true, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
#endif
}

void updateLedState(LEDEvent_e event, bool active) {
  switch(event) {
    case LED_WAKEUP:
      ledFlags.wakeupCondition = active;


      break;
    case LED_CHARGING:
      ledFlags.batteryCharging = active;

      break;
    case LED_STANDBY:
      ledFlags.standbyCondition = active;
      break;
    case LED_RUN:
      ledFlags.runCondition = active;

      break;
    case LED_DECLINE:
      ledFlags.testDecline = active;

      break;
    case LED_ABORT:
      ledFlags.testAbort = active;

      break;
    case LED_COMPLETE:
      ledFlags.testComplete = active;

      break;
    case LED_INVALID:
      ledFlags.testInvalid = active;

      break;
    case LED_LOW_BATTERY:
      ledFlags.lowBattey = active;

      break;
    default:
      // Handle unknown event
      printf("Handling unknown event\n");
      break;
  }
}