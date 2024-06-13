#include "is31fl3196.h"
#include "i2c_hal_freertos.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define WRITE_BUFFER_SIZE 1
#define READ_BUFFER_SIZE 4
typedef struct {
  uint8_t regSelection;
  uint8_t readLength;
  uint8_t readData[READ_BUFFER_SIZE];
} led_driver_init_op_read_buffer_t;

typedef enum {
  led_driver_init_read_op,
  led_driver_init_write_op,
  led_driver_reg_read_op,
  led_driver_reg_write_op,
  led_driver_reset_op,
  led_driver_set_pwm_op,
  led_driver_update_pwm_op,
  led_driver_en_channel_op,
  led_driver_read_channels_op,
  led_driver_solid_animation_op,
  led_driver_flashing_animation_op,
  led_driver_breathing_animation_op,
} led_driver_internal_op_type_t;

uint8_t PWM_Gamma64[64] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0b, 0x0d, 0x0f, 0x11, 0x13, 0x16,
        0x1a, 0x1c, 0x1d, 0x1f, 0x22, 0x25, 0x28, 0x2e,
        0x34, 0x38, 0x3c, 0x40, 0x44, 0x48, 0x4b, 0x4f,
        0x55, 0x5a, 0x5f, 0x64, 0x69, 0x6d, 0x72, 0x77,
        0x7d, 0x80, 0x88, 0x8d, 0x94, 0x9a, 0xa0, 0xa7,
        0xac, 0xb0, 0xb9, 0xbf, 0xc6, 0xcb, 0xcf, 0xd6,
        0xe1, 0xe9, 0xed, 0xf1, 0xf6, 0xfa, 0xfe, 0xff};

uint8_t led_pos_bms[9] =
    {
        0b00000001,
        0b00000010,
        0b00000100,
        0b00010000,
        0b00100000,
        0b01000000,
        0b00000001,
        0b00000010,
        0b00000100};

static bool driverIsBusy = false;
static led_driver_internal_op_type_t current_op_type = led_driver_init_read_op;

static led_driver_opDoneCallback_t savedUserCallback = NULL;

static uint8_t writeBuffer = 0;
static led_driver_init_op_read_buffer_t readBuffer = {0};
static i2c_status_t initOpOutcome = i2c_success;
static bool conversionStarted = false;
static uint8_t channel_read_buffer;
static uint8_t led_target;
static uint8_t led_ctrl_reg_1_en_channels = 0b00000000;
static uint8_t led_ctrl_reg_2_en_channels = 0b000;

static uint8_t led_ramp_mode_reg_val = 0x00;

static led_driver_errors_t led_driver_readRegister(uint8_t reg, uint8_t *p_data, led_driver_internal_op_type_t led_driver_operation, led_driver_opDoneCallback_t cb);
static led_driver_errors_t led_driver_writeRegister(uint8_t *buf,
    uint8_t len,
    led_driver_internal_op_type_t led_operation,
    led_driver_opDoneCallback_t cb);

static led_driver_errors_t led_driver_writeRegister(uint8_t *buf,
    uint8_t len,
    led_driver_internal_op_type_t led_operation,
    led_driver_opDoneCallback_t cb) {
  if (driverIsBusy) {
    return led_driver_busy;
  }

  driverIsBusy = true;
  uint32_t err_code;
  uint8_t reg = buf[0];

  current_op_type = led_operation;

#ifdef SAMPLE_PREP_BOARD
  err_code = xUtil_TWI_Write_Single(i2c_interface_sensors, IS31FL3199_ADDR_low, reg, buf, 2); //acquire data
#else
  err_code = xUtil_TWI_Write_Single(i2c_interface_sensors, IS31FL3199_ADDR, reg, buf, 2); //acquire data
#endif

  if (err_code != NRF_SUCCESS) {
    driverIsBusy = false;
    if (err_code == NRF_ERROR_BUSY) {
      return led_driver_busy;
    }
    return led_driver_i2c_error;
  }
  driverIsBusy = false;
  return led_driver_success;
}

// Used for init only, do not make public
static led_driver_errors_t led_driver_writeRegisterBlocking(uint8_t *buf, uint8_t len) {
  led_driver_errors_t led_driverError = led_driver_writeRegister(buf, len, led_driver_reg_write_op, NULL);

  if (led_driverError != led_driver_success) {
    return led_driverError;
  }
  return led_driver_success;
}

void led_driver_init(void) {
  nrf_gpio_cfg_output(LED_HARDWARE_DRIVER_ENABLE_PIN);
  enable_led_driver(false);
  for (int i = 0; i < 100; i++) {
    asm volatile("nop");
  }
  enable_led_driver(true);

  // Set LED current to 20mA
  uint8_t led_drv_current_buf[2] = {LED_DRIVER_CONFIG_REG_2, LED_CURRENT_30MA};
  led_driver_writeRegisterBlocking(led_drv_current_buf, 2);

  // Set green power LED on
  //uint8_t led_on_buf[3] = {LED_DRIVER_CTRL_REG_1, 0x20};
  //led_driver_writeRegisterBlocking(led_on_buf, 3);

  //Set all LEDs off
  uint8_t led_off_buf[3] = {LED_DRIVER_CTRL_REG_1, 0x00};
  led_driver_writeRegisterBlocking(led_off_buf, 2);

  // Set one shot mode
  uint8_t one_shot_buf[2] = {LED_DRIVER_CONFIG_REG_1, 0x70};
  led_driver_writeRegisterBlocking(one_shot_buf, 2);

  //PWM data
  uint8_t pwm_buf[2];
  pwm_buf[1] = 0xFF;
  for (int i = 0x07; i <= 0x0C; i++) {
    pwm_buf[0] = i;
    led_driver_writeRegisterBlocking(pwm_buf, 2);
  }

  // update pwm
  uint8_t update_pwm_buf[2] = {LED_DRIVER_DATA_UPDATE_REG, 0x00};
  led_driver_writeRegisterBlocking(update_pwm_buf, 2);

  //T0 data
  uint8_t t0_buf[2];
  t0_buf[1] = 0x00;
  for (int j = 0x11; j <= 0x16; j++) {
    t0_buf[0] = j;
    led_driver_writeRegisterBlocking(t0_buf, 2);
    //t0_buf[1] = t0_buf[1] + 1;
  }

  //T1-T3 data
  uint8_t t13_buf[2];
  t13_buf[1] = 0xa0;
  for (int k = 0x1a; k <= 0x1b; k++) {
    t13_buf[0] = k;
    led_driver_writeRegisterBlocking(t13_buf, 2);
  }

  //T4 data
  uint8_t t4_buf[2];
  t4_buf[1] = 0x02;
  for (int m = 0x1d; m <= 0x22; m++) {
    t4_buf[0] = m;
    led_driver_writeRegisterBlocking(t4_buf, 2);
  }

  uint8_t time_update_buf[2] = {LED_DRIVER_TIME_UPDATE_REG, 0x00};
  led_driver_writeRegisterBlocking(time_update_buf, 2);

  uint8_t ramp_mode_buf[2] = {LED_DRIVER_RAMP_MODE_REG, 0b00110000};
  led_driver_writeRegisterBlocking(ramp_mode_buf, 2);

  //led_driver_enable_channel(LED1, blue, NULL);
  //led_driver_set_channel_animation_flashing(LED1, blue, true, NULL);

  led_driver_writeRegisterBlocking(one_shot_buf, 2);
}

led_driver_errors_t led_driver_enable_channel(led_driver_led_selection led_selection, led_color color, led_driver_opDoneCallback_t cb) {
  led_driver_errors_t err_code;
  uint8_t target_addr = LED_DRIVER_CTRL_REG_1;
  uint8_t en_channels = led_ctrl_reg_1_en_channels;
  led_ctrl_reg_1_en_channels |= led_pos_bms[led_selection * 3 + color];
  en_channels = led_ctrl_reg_1_en_channels;

  uint8_t led_i2c_write_buf[2] = {target_addr, en_channels};
  err_code = led_driver_writeRegister(led_i2c_write_buf, 2, led_driver_en_channel_op, cb);
  if (err_code != led_driver_success) {
    return led_driver_i2c_error;
  }
  uint8_t pwm_update_buf[2] = {LED_DRIVER_DATA_UPDATE_REG, 0x00};
  led_driver_writeRegister(pwm_update_buf, 2, led_driver_update_pwm_op, savedUserCallback);
  if (err_code != led_driver_success) {
    return led_driver_i2c_error;
  }
  return led_driver_success;
}

led_driver_errors_t led_driver_disable_channel(led_driver_led_selection led_selection, led_color color, led_driver_opDoneCallback_t cb) {
  led_driver_errors_t err_code;
  uint8_t target_addr = LED_DRIVER_CTRL_REG_1;
  uint8_t en_channels = led_ctrl_reg_1_en_channels;
  led_ctrl_reg_1_en_channels &= ~led_pos_bms[led_selection * 3 + color];
  en_channels = led_ctrl_reg_1_en_channels;

  uint8_t led_i2c_write_buf[2] = {target_addr, en_channels};
  err_code = led_driver_writeRegister(led_i2c_write_buf, 2, led_driver_en_channel_op, cb);
  if (err_code != led_driver_success) {
    return led_driver_i2c_error;
  }
  if (err_code != led_driver_success) {
    return led_driver_i2c_error;
  }
  return led_driver_success;
}

led_driver_errors_t led_driver_set_rgb(led_driver_led_selection led_selection, led_color color, uint8_t pwm, led_driver_opDoneCallback_t cb) {
  led_driver_errors_t err_code;
  err_code = led_driver_enable_channel(led_selection, color, cb);
  if (err_code != led_driver_success) {
    return led_driver_busy;
  }

  uint8_t target_addr = 0x07 + led_selection * 3 + color;
  uint8_t led_i2c_write_buf[2] = {target_addr, pwm};
  led_driver_writeRegister(led_i2c_write_buf, 2, led_driver_set_pwm_op, cb);

  uint8_t pwm_update_buf[2] = {LED_DRIVER_DATA_UPDATE_REG, 0x00};
  led_driver_writeRegister(pwm_update_buf, 2, led_driver_update_pwm_op, savedUserCallback);
  if (err_code != led_driver_success) {
    return led_driver_i2c_error;
  }
}

led_driver_errors_t led_driver_set_leds_off(led_driver_led_selection led_selection, led_driver_opDoneCallback_t cb) {

  // ramp mode reg, 0x70 == hold on T2 ON
  led_ramp_mode_reg_val |= 1 << led_selection + 4;
  uint8_t ramp_write_buf[2] = {LED_DRIVER_RAMP_MODE_REG, led_ramp_mode_reg_val};
  led_driver_writeRegister(ramp_write_buf, 1, write_reg, NULL);

  // uint8_t led_off_buf[3] = {LED_DRIVER_CTRL_REG_1, 0x00};
  //  led_driver_writeRegister(led_off_buf, 2, write_reg, cb);

  return led_driver_i2c_error;
}

// Just need to set T2 hold in ramp mode register
led_driver_errors_t led_driver_set_channel_animation_solid(led_driver_led_selection led_selection, led_color color, bool enable, led_driver_opDoneCallback_t cb) {
  led_driver_errors_t err_code;
  // ramp mode reg, 0x70 == hold on T2 ON
  led_ramp_mode_reg_val |= 1 << led_selection + 4;
  uint8_t ramp_write_buf[2] = {LED_DRIVER_RAMP_MODE_REG, led_ramp_mode_reg_val};
  led_driver_writeRegister(ramp_write_buf, 1, write_reg, NULL);

  return led_driver_success;
}

led_driver_errors_t led_driver_set_channel_animation_flashing(led_driver_led_selection led_selection, led_color color, bool enable, led_driver_opDoneCallback_t cb) {
  led_driver_errors_t err_code;

  // A = 7 for 0.1ms rise/fall time, B = 2, ~0.5 second flash
  uint8_t t13_update_addr = 0x1A + led_selection;
  uint8_t t13_update_value = 0b00100111;
  uint8_t t13_write_buf[2] = {t13_update_addr, t13_update_value};
  led_driver_writeRegister(t13_write_buf, 1, write_reg, NULL);

  // Shorten off time for flashing
  uint8_t t4_update_addr = 0x1D + led_selection * 3 + color;
  uint8_t t4_update_value = 0x02;
  uint8_t t4_write_buf[2] = {t4_update_addr, t4_update_value};
  led_driver_writeRegister(t4_write_buf, 1, write_reg, NULL);

  // time update reg
  uint8_t time_update_write_buf[2] = {LED_DRIVER_TIME_UPDATE_REG, 0x00};
  led_driver_writeRegister(time_update_write_buf, 1, write_reg, NULL);

  // ramp mode reg turn holds off
  led_ramp_mode_reg_val &= ~(1 << led_selection + 4);
  uint8_t ramp_write_buf[2] = {LED_DRIVER_RAMP_MODE_REG, led_ramp_mode_reg_val};
  led_driver_writeRegister(ramp_write_buf, 1, write_reg, NULL);
}

led_driver_errors_t led_driver_set_channel_animation_breathing(led_driver_led_selection led_selection, led_color color, bool enable, led_driver_opDoneCallback_t cb) {
  led_driver_errors_t err_code;
  // A = 0 for 260ms rise/fall time, B = 2, DT = 1
  uint8_t t13_update_addr = 0x1A + led_selection;
  uint8_t t13_update_value = 0b00110010;
  uint8_t t13_write_buf[2] = {t13_update_addr, t13_update_value};
  led_driver_writeRegister(t13_write_buf, 1, write_reg, NULL);

  // Extend off time for breathing
  uint8_t t4_update_addr = 0x1D + led_selection * 3 + color;
  uint8_t t4_update_value = 0x04;
  uint8_t t4_write_buf[2] = {t4_update_addr, t4_update_value};
  led_driver_writeRegister(t4_write_buf, 1, write_reg, NULL);

  // time update reg
  uint8_t time_update_write_buf[2] = {LED_DRIVER_TIME_UPDATE_REG, 0x00};
  led_driver_writeRegister(time_update_write_buf, 1, write_reg, NULL);

  // ramp mode reg turn holds off
  led_ramp_mode_reg_val &= ~(1 << led_selection + 4);
  uint8_t ramp_write_buf[2] = {LED_DRIVER_RAMP_MODE_REG, led_ramp_mode_reg_val};
  led_driver_writeRegister(ramp_write_buf, 1, write_reg, NULL);
}

led_driver_errors_t led_driver_set_channel_animation_flash_fast(led_driver_led_selection led_selection, led_color color, bool enable, led_driver_opDoneCallback_t cb) {
  led_driver_errors_t err_code;

  // A = 7 for 0.1ms rise/fall time, B = 2, ~0.5 second flash
  uint8_t t13_update_addr = 0x1A + led_selection;
  uint8_t t13_update_value = 0b00010111;
  uint8_t t13_write_buf[2] = {t13_update_addr, t13_update_value};
  led_driver_writeRegister(t13_write_buf, 1, write_reg, NULL);

  // Shorten off time for flashing
  uint8_t t4_update_addr = 0x1D + led_selection * 3 + color;
  uint8_t t4_update_value = 0x01;
  uint8_t t4_write_buf[2] = {t4_update_addr, t4_update_value};
  led_driver_writeRegister(t4_write_buf, 1, write_reg, NULL);

  // time update reg
  uint8_t time_update_write_buf[2] = {LED_DRIVER_TIME_UPDATE_REG, 0x00};
  led_driver_writeRegister(time_update_write_buf, 1, write_reg, NULL);

  // ramp mode reg turn holds off
  led_ramp_mode_reg_val &= ~(1 << led_selection + 4);
  uint8_t ramp_write_buf[2] = {LED_DRIVER_RAMP_MODE_REG, led_ramp_mode_reg_val};
  led_driver_writeRegister(ramp_write_buf, 1, write_reg, NULL);
}

void enable_led_driver(bool enable) {
  if (enable) {
    nrf_gpio_pin_set(LED_HARDWARE_DRIVER_ENABLE_PIN);
    uint8_t shutdown_buf[2] = {LED_DRIVER_SHUTDOWN_REG, 0x01};
    led_driver_writeRegisterBlocking(shutdown_buf, 2);
  } else {
    uint8_t shutdown_buf[2] = {LED_DRIVER_SHUTDOWN_REG, 0x00};
    led_driver_writeRegisterBlocking(shutdown_buf, 2);
    nrf_gpio_pin_clear(LED_HARDWARE_DRIVER_ENABLE_PIN);
  }
}