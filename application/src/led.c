#include "led.h"

led_color led1_current_color = blue;
led_color led2_current_color = blue;

xQueueHandle ledRxQueue;

static LEDFlags_t ledFlags;
bool first_run_through = true;

int powerLevel = 100;
led_power_level_t led_power_level = led_pl_high;

void handleLedState(LEDRxQueueMsg_t ledMsg);

led_power_level_t getCurrentPowerLevel(void) {
  return led_power_level;
}

void led_task(void *pvParameters) {
  BaseType_t xReturned;
  LEDRxQueueMsg_t ledRxMessage;

  for (;;) {
    xReturned = xQueueReceive(ledRxQueue, &ledRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      send_debug_log_message("Unable to Rx data to sensor queue");
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
    if (ledFlags.batteryCharging == ledMsg.active) {
      return;
    }
    ledFlags.batteryCharging = ledMsg.active;
    break;
  case LED_STANDBY:
    first_run_through = true;
    ledFlags.standbyCondition = ledMsg.active;
    led_power_level = ledMsg.powerLevel;
    break;
  case LED_RUN_HEATER:
    ledFlags.runConditionHeater = ledMsg.active;
    break;
  case LED_RUN_MOTOR:
    ledFlags.runConditionMotor = ledMsg.active;
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
    send_debug_log_message("Handling unknown led state");
    break;
  }

  if (!ledFlags.wakeupCondition) {
    enable_led_driver(false);
  } else {
    enable_led_driver(true);
    turn_off_led_channels();
    if (ledFlags.testDecline) {
      set_led1_red_slow_blink();
      set_led2_red_slow_blink();
    } else if (ledFlags.testAbort) {
      turn_off_led2_blue_animation();
      set_led1_red_fast_blink();
      set_led2_red_fast_blink();
    } else if (ledFlags.testComplete) {
      set_led1_green_solid();
      set_led2_green_solid();
    } else if (ledFlags.standbyCondition) {
      //if battery charging
      if (ledFlags.batteryCharging) {
        set_led2_blue_breathe();
      } else {
        turn_off_led2_blue_animation();
        if (led_power_level == led_pl_high) {
          set_led2_white_solid();
        }
        else if (led_power_level == led_pl_medium) {
          set_led2_yellow_solid();
        }
        else if (led_power_level == led_pl_low){
          set_led2_red_solid();
        }
      }
    } else if (ledFlags.runConditionHeater) {
      set_led1_green_breathe();
      //if battery charging
      if (ledFlags.batteryCharging) {
        set_led2_blue_breathe();
      } else {
        turn_off_led2_blue_animation();
      }
    } 
    else if (ledFlags.runConditionMotor) {
      set_led1_green_fast_blink();
      //if battery charging
      if (ledFlags.batteryCharging) {
        set_led2_blue_breathe();
      } else {
        turn_off_led2_blue_animation();
      }
    }
    else if (ledFlags.usbMscStarting) {
      set_led1_blue_breathe();
      set_led2_blue_breathe();
    }
  }
}

/* LED 1 */
void set_led1_green_solid(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_disable_channel(LED1, red, NULL);
  led_driver_disable_channel(LED1, green, NULL);
  led_driver_disable_channel(LED1, blue, NULL);
  led_driver_enable_channel(LED1, green, NULL);
  led_driver_set_channel_animation_solid(LED1, green, true, NULL);
  led1_current_color = green;
#endif
}

void set_led1_green_breathe(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_disable_channel(LED1, red, NULL);
  led_driver_disable_channel(LED1, green, NULL);
  led_driver_disable_channel(LED1, blue, NULL);
  led_driver_enable_channel(LED1, green, NULL);
  led_driver_set_channel_animation_breathing(LED1, green, true, NULL);
  led1_current_color = green;
#endif
}

void set_led1_blue_slow_blink(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_disable_channel(LED1, red, NULL);
  led_driver_disable_channel(LED1, green, NULL);
  led_driver_disable_channel(LED1, blue, NULL);
  led_driver_enable_channel(LED1, blue, NULL);
  led_driver_set_channel_animation_flashing(LED1, blue, true, NULL);
  led1_current_color = blue;
#endif
}

void set_led1_red_slow_blink(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_disable_channel(LED1, red, NULL);
  led_driver_disable_channel(LED1, green, NULL);
  led_driver_disable_channel(LED1, blue, NULL);
  led_driver_enable_channel(LED1, red, NULL);
  led_driver_set_channel_animation_flashing(LED1, red, true, NULL);
  led1_current_color = red;
#endif
}

void set_led1_red_fast_blink(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_disable_channel(LED1, red, NULL);
  led_driver_disable_channel(LED1, green, NULL);
  led_driver_disable_channel(LED1, blue, NULL);
  led_driver_enable_channel(LED1, red, NULL);
  led_driver_set_channel_animation_flashing(LED1, red, true, NULL);
  led1_current_color = red;
#endif
}

void set_led1_green_fast_blink(void) {
  #if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED1, green, NULL);
  led_driver_set_channel_animation_flashing(LED1, green, true, NULL);
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
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_set_channel_animation_flashing(LED2, red, true, NULL);
  led2_current_color = red;
#endif
}

void set_led2_red_fast_blink(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED2, led2_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_set_channel_animation_flashing(LED2, red, true, NULL);
  led2_current_color = red;
#endif
}

void set_led2_green_solid(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED2, led1_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED2, green, NULL);
  led_driver_set_channel_animation_solid(LED2, green, true, NULL);
  led2_current_color = green;
#endif
}

void set_led2_white_solid(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED2, led1_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED2, green, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_enable_channel(LED2, blue, NULL);
  led_driver_set_channel_animation_solid(LED2, blue, true, NULL);
  led2_current_color = green;
#endif
}

void set_led2_yellow_solid(void) {
  led_driver_disable_channel(LED2, led1_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED2, green, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_set_channel_animation_solid(LED2, green, true, NULL);
  led2_current_color = green;
}

void set_led2_red_solid(void) {
  led_driver_disable_channel(LED2, led1_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED2, red, NULL);
  led_driver_set_channel_animation_solid(LED2, red, true, NULL);
  led2_current_color = red;
}

void set_led1_blue_breathe(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED1, led1_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED1, blue, NULL);
  led_driver_set_channel_animation_breathing(LED1, blue, true, NULL);
  led1_current_color = blue;
#endif
}

void set_led2_blue_breathe(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED2, led2_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_enable_channel(LED2, blue, NULL);
  led_driver_set_channel_animation_breathing(LED2, blue, true, NULL);
  led2_current_color = blue;
#endif
}

void turn_off_led2_blue_animation(void) {
#if ENABLE_LEDS
  led_driver_disable_channel(LED2, led2_current_color, NULL);
  led_driver_disable_channel(LED2, red, NULL);
  led_driver_disable_channel(LED2, green, NULL);
  led_driver_disable_channel(LED2, blue, NULL);
  led_driver_disable_animation(LED2, led2_current_color, NULL);
#endif
}

void turn_off_led_channels(void) {
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
    send_debug_log_message("SENSORS: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue");
  }
}

void updateLedStatePowerLevel(LEDEvent_e event, bool active, led_power_level_t led_pl) {
   BaseType_t xReturned;
  LEDRxQueueMsg_t ledMsg;
  ledMsg.type = event;
  ledMsg.active = active;
  ledMsg.powerLevel = led_pl;
  xReturned = xQueueSend(ledRxQueue, &ledMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("SENSORS: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue");
  }
}

#ifdef POWER_MODULE_BOARD
#if POWER_MODULE_REVB
led_color alt_led1_current_color = blue;
led_color alt_led2_current_color = blue;

/* ALT LED 1 */
void set_alt_led1_green_solid(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED1, alt_led1_current_color, NULL);
  led_alt_driver_enable_channel(LED1, green, NULL);
  led_alt_driver_set_channel_animation_solid(LED1, green, true, NULL);
  alt_led1_current_color = green;
#endif
}

void set_alt_led1_green_breathe(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED1, alt_led1_current_color, NULL);
  led_alt_driver_enable_channel(LED1, green, NULL);
  led_alt_driver_set_channel_animation_breathing(LED1, green, true, NULL);
  alt_led1_current_color = green;
#endif
}

void set_alt_led1_blue_slow_blink(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED1, alt_led1_current_color, NULL);
  led_alt_driver_enable_channel(LED1, blue, NULL);
  led_alt_driver_set_channel_animation_flashing(LED1, blue, true, NULL);
  alt_led1_current_color = blue;
#endif
}

void set_alt_led1_red_slow_blink(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED1, alt_led1_current_color, NULL);
  led_alt_driver_enable_channel(LED1, red, NULL);
  led_alt_driver_set_channel_animation_flashing(LED1, red, true, NULL);
  alt_led1_current_color = red;
#endif
}

void set_alt_led1_red_fast_blink(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED1, alt_led1_current_color, NULL);
  led_alt_driver_enable_channel(LED1, red, NULL);
  led_alt_driver_set_channel_animation_flashing(LED1, red, true, NULL);
  alt_led1_current_color = red;
#endif
}

void set_alt_led1_red_solid(void) {
#if ENABLE_LEDS
#endif
}

/* ALT LED 2 */
void set_alt_led2_red_slow_blink(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED2, alt_led2_current_color, NULL);
  led_alt_driver_enable_channel(LED2, red, NULL);
  led_alt_driver_set_channel_animation_flashing(LED2, red, true, NULL);
  alt_led2_current_color = red;
#endif
}

void set_alt_led2_red_fast_blink(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED2, alt_led2_current_color, NULL);
  led_alt_driver_enable_channel(LED2, red, NULL);
  led_alt_driver_set_channel_animation_flashing(LED2, red, true, NULL);
  alt_led2_current_color = red;
#endif
}

void set_alt_led2_green_solid(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED2, alt_led2_current_color, NULL);
  led_alt_driver_disable_channel(LED2, red, NULL);
  led_alt_driver_disable_channel(LED2, green, NULL);
  led_alt_driver_disable_channel(LED2, blue, NULL);
  led_alt_driver_enable_channel(LED2, green, NULL);
  led_alt_driver_set_channel_animation_solid(LED2, green, true, NULL);
  alt_led2_current_color = green;
#endif
}

void set_alt_led1_blue_breathe(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED1, alt_led1_current_color, NULL);
  led_alt_driver_enable_channel(LED1, blue, NULL);
  led_alt_driver_set_channel_animation_breathing(LED1, blue, true, NULL);
  alt_led1_current_color = blue;
#endif
}

void set_alt_led2_blue_breathe(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED2, alt_led2_current_color, NULL);
  led_alt_driver_enable_channel(LED2, blue, NULL);
  led_alt_driver_set_channel_animation_breathing(LED2, blue, true, NULL);
  alt_led2_current_color = blue;
#endif
}

void turn_off_alt_led2_blue_animation(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED2, alt_led2_current_color, NULL);
  led_alt_driver_disable_animation(LED2, alt_led2_current_color, NULL);
#endif
}

void turn_off_alt_led_channels(void) {
#if ENABLE_LEDS
  led_alt_driver_disable_channel(LED2, alt_led1_current_color, NULL);
  led_alt_driver_disable_channel(LED1, alt_led1_current_color, NULL);
#endif
}

#endif
#endif