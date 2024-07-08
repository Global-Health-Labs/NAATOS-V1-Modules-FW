#ifndef _IS31FL3199_DRIVER_H_
#define _IS31FL3199_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "i2c_hal_freertos.h"
#include "nrf_gpio.h"
#include <stdbool.h>
#include <stdint.h>

#define IS31FL3199_ADDR 0b1100111
#define IS31FL3199_ADDR_low 0b1100100

#define LED_DRIVER_SHUTDOWN_REG 0x00
#define LED_DRIVER_CTRL_REG_1 0x01
#define LED_DRIVER_CTRL_REG_2 0x02
#define LED_DRIVER_CONFIG_REG_1 0x03
#define LED_DRIVER_CONFIG_REG_2 0x04
#define LED_DRIVER_RAMP_MODE_REG 0x05
#define LED_DRIVER_BREATHING_MARK_REG 0x06
#define LED1_R 0x07
#define LED1_G 0x08
#define LED1_B 0x09
#define LED2_R 0x0A
#define LED2_G 0x0B
#define LED3_B 0x0C
#define LED_DRIVER_DATA_UPDATE_REG 0x10
#define LED_DRIVER_T0_REG 0x11
#define LED_DRIVER_TIME_UPDATE_REG 0x26
#define LED_DRIVER_RESET_REG 0xFF

#define LED_CURRENT_15MA 0b00010000
#define LED_CURRENT_20MA 0b00000000
#define LED_CURRENT_25MA 0b01110000
#define LED_CURRENT_30MA 0b01100000
#define LED_MAX_BRIGHTNESS 0x4D

typedef enum {
  led_driver_success,
  led_driver_i2c_error,
  led_driver_busy,
  led_driver_timeout
} led_driver_errors_t;

typedef enum {
  enable_channel_op,
  disable_channel_op,
  change_pwm_op,
  reset_op,
  write_reg,
} led_driver_op_type_t;

typedef enum {
  LED1,
  LED2,
} led_driver_led_selection;

typedef enum {
  red,
  green,
  blue,
} led_color;

typedef struct {
  led_driver_op_type_t data_type;
  uint8_t *data;
} led_driver_callback_data_t;

typedef void (*led_driver_opDoneCallback_t)(led_driver_errors_t outcome, led_driver_callback_data_t *p_data);

typedef struct {
  led_driver_op_type_t op_type;
  led_driver_led_selection led_selection;
  led_color color;
  uint8_t pwm;
  uint8_t reg_selection;
  uint8_t reg_value;
  led_driver_opDoneCallback_t cb;
} scheduled_led_evt_data_t;

void enable_led_driver(bool enable);
led_driver_errors_t led_driver_set_rgb(led_driver_led_selection led_selection, led_color color, uint8_t pwm, led_driver_opDoneCallback_t cb);
led_driver_errors_t led_driver_enable_channel(led_driver_led_selection led_selection, led_color color, led_driver_opDoneCallback_t cb);
led_driver_errors_t led_driver_disable_channel(led_driver_led_selection led_selection, led_color color, led_driver_opDoneCallback_t cb);
led_driver_errors_t led_driver_set_leds_off(led_driver_led_selection led_selection, led_driver_opDoneCallback_t cb);
led_driver_errors_t led_driver_set_channel_animation_solid(led_driver_led_selection led_selection, led_color color, bool enable, led_driver_opDoneCallback_t cb);
led_driver_errors_t led_driver_set_channel_animation_flashing(led_driver_led_selection led_selection, led_color color, bool enable, led_driver_opDoneCallback_t cb);
led_driver_errors_t led_driver_set_channel_animation_breathing(led_driver_led_selection led_selection, led_color color, bool enable, led_driver_opDoneCallback_t cb);
void led_driver_init(void);
void led_driver_scheduled_evt_handler(void *p_context, uint16_t size);
led_driver_errors_t led_driver_set_channel_animation_flash_fast(led_driver_led_selection led_selection, led_color color, bool enable, led_driver_opDoneCallback_t cb);

#ifdef __cplusplus
}
#endif
#endif