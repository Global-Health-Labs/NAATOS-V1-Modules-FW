#include "led.h"

led_color led1_current_color = blue;
led_color led2_current_color = blue;

const uint8_t COLOR_RGB_OFF[] = {0,0,0};
const uint8_t COLOR_RGB_WHT[] = {255,170,170};  // tone down the blue & green, the white looks too blue on top leds
const uint8_t COLOR_RGB_RED[] = {255,0,0};
const uint8_t COLOR_RGB_GRN[] = {0,255,0};
const uint8_t COLOR_RGB_BLU[] = {0,0,255};
const uint8_t COLOR_RGB_YLW[] = {255,120,0};    // make look less like the green

const uint8_t COLOR_RGB_MAGENTA[] = {0xFF,0,0x10};


xQueueHandle ledRxQueue;
char tmp[64];
const int tmp_sz = 64;
LEDFlags_t ledFlags;
LEDFlags_t ledFlags_previous;

led_power_level_t led_power_level = led_pl_high;
led_power_level_t led_power_level_last = led_pl_UNSET;

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
  ledFlags_previous.vec = ledFlags.vec;
  led_power_level_last = led_power_level;

  switch (ledMsg.type) {
  case LED_WAKEUP:
    ledFlags.bit.wakeupCondition = ledMsg.active;
    break;
  case LED_CHARGING:
    //if (ledFlags.bit.batteryCharging == ledMsg.active) {
    //  return;
    //}
    ledFlags.bit.batteryCharging = ledMsg.active;
    break;
  case LED_STANDBY:
    ledFlags.bit.standbyCondition = ledMsg.active;
    
    // update power-level if it makes sense to do so
    if(ledMsg.active && (ledMsg.powerLevel<=led_pl_low) && ((ledMsg.powerLevel>led_pl_UNSET)))
      led_power_level = ledMsg.powerLevel;
    break;
  case LED_RUN_HEATER:
    ledFlags.bit.runConditionHeater = ledMsg.active;
    break;
  case LED_RUN_MOTOR:
    ledFlags.bit.runConditionMotor = ledMsg.active;
    break;
  case LED_DECLINE:
    ledFlags.bit.testDecline = ledMsg.active;
    break;
  case LED_ABORT:
    ledFlags.bit.testAbort = ledMsg.active;
    break;
  case LED_ABORT_YELLOW:
    ledFlags.bit.testAbortYellow = ledMsg.active;
    break;
  case LED_COMPLETE:
    ledFlags.bit.testComplete = ledMsg.active;
    break;
  case LED_INVALID:
    ledFlags.bit.testInvalid = ledMsg.active;
    break;
  case LED_MACHINE_IN_ERROR:
    ledFlags.bit.machineInError = ledMsg.active;
    break;
  case LED_STANDBY_UNSTARTABLE:
    ledFlags.bit.testUnstartable = ledMsg.active;
    break;
  case LED_LOW_BATTERY:
    ledFlags.bit.lowBattey = ledMsg.active;
    break;
  case LED_USB_MSC_STARTING:
    ledFlags.bit.usbMscStarting = ledMsg.active;
    break;
  case LED_CLEAR_ALL_ERROR:
    ledFlags.bit.testDecline = false;
    ledFlags.bit.testAbort = false;
    ledFlags.bit.testAbortYellow = false;
    break;
  default:
    send_debug_log_message("Handling unknown led state");
    break;
  }

  // DID THE FLAGS CHANGE? ONLY CONTINUE (MAKING DRIVER EFFECT) IF IT DID
  if((ledFlags_previous.vec == ledFlags.vec) && (led_power_level_last==led_power_level))
    return; // exit early
  
  // continuing on to decision-tree
  //snprintf(tmp,tmp_sz,"LED: running decision-tree flags=0x%04x",ledFlags.vec);
  //send_debug_log_message(tmp);
  if (!ledFlags.bit.wakeupCondition) {
    //enable_led_driver(false);
  } else {
    //enable_led_driver(true);
    
    turn_off_led_channels();
    //led_driver_update_device_if_needed();

    if (ledFlags.bit.testDecline) {
      //set_led1_red_slow_blink();
      //set_led2_red_slow_blink();

      // TODO: Remove? condition appears unused

    } else if (ledFlags.bit.testAbort) {
      led_driver_GHL_setcolor_flashing_fast(LED1, COLOR_RGB_RED, true);

    } else if (ledFlags.bit.testAbortYellow) {
      led_driver_GHL_setcolor_flashing_fast(LED1, COLOR_RGB_YLW, true);

    } else if (ledFlags.bit.testComplete) {
      led_driver_GHL_setcolor_solid(LED1, COLOR_RGB_GRN, true);

    } else if (ledFlags.bit.standbyCondition) {
      
      if(ledFlags.bit.testInvalid)  {
        //if sampleInvalid (solid red action)
        led_driver_GHL_setcolor_solid(LED1, COLOR_RGB_RED, false);

      } else if(ledFlags.bit.machineInError)  {
        //if machine is in error (it cannot run due to hardware or config problem)
        led_driver_GHL_set_animation_machine_error(LED1);

      } else if(ledFlags.bit.testUnstartable)  {
        //if run cannot start due to a temporary condition
        led_driver_GHL_setcolor_solid(LED1, COLOR_RGB_YLW, false);

      } else{
        //set_led1_off();
        // keep LED1 off

      }

    } else if (ledFlags.bit.runConditionHeater) {
      led_driver_GHL_setcolor_breathing(LED1, COLOR_RGB_GRN, true);

    } else if (ledFlags.bit.runConditionMotor) {
      //led_driver_GHL_setcolor_breathing(LED1, COLOR_RGB_GRN, true);

    }
    else if (ledFlags.bit.usbMscStarting) {

    }



    //LED2 (if not MSC)
    if(!ledFlags.bit.usbMscStarting) {
      //if battery charging

      if (ledFlags.bit.batteryCharging) {
        led_driver_GHL_setcolor_breathing(LED2, COLOR_RGB_BLU, true);
      } else {
        if (led_power_level == led_pl_high) {
          led_driver_GHL_setcolor_solid(LED2, COLOR_RGB_WHT, false);
        }
        else if (led_power_level == led_pl_medium) {
          led_driver_GHL_setcolor_solid(LED2, COLOR_RGB_YLW, false);
        }
        else if (led_power_level == led_pl_low){
          led_driver_GHL_setcolor_solid(LED2, COLOR_RGB_RED, false);
        } else{
          // Should Never Be Here
          led_driver_GHL_setcolor_solid(LED2, COLOR_RGB_MAGENTA, false);
        }
      }

      //snprintf(tmp,tmp_sz,"LED: ran decision-tree flags=0x%04x and LedPL=%d",ledFlags.vec,(uint8_t) led_power_level);
      //send_debug_log_message(tmp);
    } else{
      // USB Mass Storage Mode
      led_driver_GHL_setcolor_breathing(LED1, COLOR_RGB_BLU, true);
      led_driver_GHL_setcolor_breathing(LED2, COLOR_RGB_BLU, true);
    }

    // Actually send updates to the LED driver chip, if any have occurred
    led_driver_update_device_if_needed();
  }
}

void turn_off_led_channels(void) {
#if ENABLE_LEDS
  //led_driver_disable_channel(LED1, red, NULL);
  //led_driver_disable_channel(LED1, green, NULL);
  //led_driver_disable_channel(LED1, blue, NULL);

  //led_driver_disable_channel(LED2, red, NULL);
  //led_driver_disable_channel(LED2, green, NULL);
  //led_driver_disable_channel(LED2, blue, NULL);

  led_driver_GHL_setcolor_solid(LED1,COLOR_RGB_OFF,true);
  led_driver_GHL_setcolor_solid(LED2,COLOR_RGB_OFF,true);
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