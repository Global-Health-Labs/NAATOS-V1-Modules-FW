#include "led.h"

led_color led1_current_color = blue;
led_color led2_current_color = blue;

xQueueHandle ledRxQueue;

static LEDFlags_t ledFlags;

void handleLedState(LEDRxQueueMsg_t ledMsg);

void led_task(void *pvParameters) {
  BaseType_t xReturned;
  LEDRxQueueMsg_t ledRxMessage;

  for (;;) {
    xReturned = xQueueReceive(ledRxQueue, &ledRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("Unable to Rx data to sensor queue\n");
    } else {
      handleLedState(ledRxMessage);
    }
  }
}

void handleLedState(LEDRxQueueMsg_t ledMsg) {
  switch (ledMsg.type) {
  case LED_WAKEUP:
    ledFlags.wakeupCondition = ledMsg.active;
    break;
  case LED_CHARGING:
    ledFlags.batteryCharging = ledMsg.active;
    break;
  case LED_STANDBY:
    ledFlags.standbyCondition = ledMsg.active;
    break;
  case LED_RUN:
    ledFlags.runCondition = ledMsg.active;
    break;
  case LED_DECLINE:
    ledFlags.testDecline = ledMsg.active;
    break;
  case LED_ABORT:
    ledFlags.testAbort = ledMsg.active;
    break;
  case LED_COMPLETE:
    ledFlags.testComplete = ledMsg.active;
    break;
  case LED_INVALID:
    ledFlags.testInvalid = ledMsg.active;
    break;
  case LED_LOW_BATTERY:
    ledFlags.lowBattey = ledMsg.active;
    break;
  case LED_USB_MSC_STARTING:
    ledFlags.usbMscStarting = ledMsg.active;
    break;
  case LED_CLEAR_ALL_ERROR:
    ledFlags.testDecline = false;
    ledFlags.testAbort = false;
    break;
  default:
    printf("Handling unknown led state\n");
    break;
  }

  if (!ledFlags.wakeupCondition) {
    enable_led_driver(false);
  } else {
    enable_led_driver(true);
    turn_off_led2();
    if (ledFlags.testDecline) {
      set_led1_red_slow_blink();
      set_led2_red_slow_blink();
    } else if (ledFlags.testAbort) {
      set_led1_red_fast_blink();
      set_led2_red_fast_blink();
    } else if (ledFlags.testComplete) {
      set_led1_green_solid();
      set_led2_green_solid();
    } else if (ledFlags.standbyCondition) {
      set_led1_green_breathe();
      //if battery charging
      if (ledFlags.batteryCharging) {
        set_led2_blue_breathe();
      } else {
        //turn_off_led2();
      }
    } else if (ledFlags.runCondition) {
      set_led1_green_solid();
      //if battery charging
      if (ledFlags.batteryCharging) {
        set_led2_blue_breathe();
      } else {
        //turn_off_led2();
      }
    } else if (ledFlags.usbMscStarting) {
      set_led1_blue_breathe();
      set_led2_blue_breathe();
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
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, red, NULL);
  led_driver_set_channel_animation_flashing(LED1, red, true, NULL);
  led1_current_color = red;
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
  led_driver_disable_channel(LED2, led1_current_color, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_set_channel_animation_flashing(LED2, red, true, NULL);
  led2_current_color = red;
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
  led_driver_disable_channel(LED2, led1_current_color, NULL);
  led_driver_enable_channel(LED2, green, NULL);
  led_driver_set_channel_animation_solid(LED2, green, true, NULL);
  led2_current_color = green;
#endif
}

void set_led1_blue_breathe(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_enable_channel(LED1, blue, NULL);
  led_driver_set_channel_animation_breathing(LED1, blue, true, NULL);
  led1_current_color = blue;
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
  led_driver_disable_channel(LED2, led1_current_color, NULL);
  led_driver_disable_channel(LED1, led1_current_color, NULL);

#endif
}

void updateLedState(LEDEvent_e event, bool active) {
  BaseType_t xReturned;
  LEDRxQueueMsg_t ledMsg;
  ledMsg.type = event;
  ledMsg.active = active;
  xReturned = xQueueSend(ledRxQueue, &ledMsg, 0);
  if (xReturned != pdPASS) {
    printf("SENSORS: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue\n");
  }
}