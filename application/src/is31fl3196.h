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

#define LED_CURRENT__5MA 0b00110000
#define LED_CURRENT_15MA 0b00010000
#define LED_CURRENT_20MA 0b00000000
#define LED_CURRENT_25MA 0b01110000
#define LED_CURRENT_30MA 0b01100000
#define LED_MAX_BRIGHTNESS 0x4D

typedef enum {
  led_driver_success,
  led_driver_success_nowrite_needed,
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


typedef union {
  struct {
    //LSB
    bool OUT1 : 1;
    bool OUT2 : 1;
    bool OUT3 : 1;
    bool _unused1 : 1;

    bool OUT4 : 1;
    bool OUT5 : 1;
    bool OUT6 : 1;
    bool _unused2 : 1;
    //MSB
  } bit;

  struct {
    //LSB
    uint8_t OUT_LED1_VEC : 3;
    bool _unused1 : 1;
    uint8_t OUT_LED2_VEC : 3;
    bool _unused2 : 1;
    //MSB
  };

  uint8_t raw;
} REG_LEDCTRL_01h_t;


typedef union {
  struct {
    //LSB
    bool AGCM : 1;      // AGC Mode Selection 0 = Mode1 (Fast modulation) ; 1 = Mode2 ( Slow modulation )
    bool AGCE : 1;      // AGC Function Enable
    bool AE : 1;        // AUdio Modulate Enable
    bool _unused3 : 1;

    //bool MODE[2] : 1; // per RGB LED, 0=PWM Control mode, 1 = One Shot Programming mode
    //struct {
    //  bool oneshot : 1;
    //} mode[2];
    bool oneshot_1 : 1;
    bool oneshot_2 : 1;
    uint8_t _unused67 : 2;
    //MSB
  } bit;

  uint8_t raw;
} REG_CFGREG1_03h_t;

typedef union {
  struct {
    //LSB
    uint8_t AUDIO_GAIN : 3; // Audio Gain (see lookup table in datasheet)
    bool _unused3 : 1;

    uint8_t CURRENT_SETTING : 3; // Current Setting (see lookup table in datasheet)
    bool CONTROL_MODE : 1; // 0 = Master; 1 = Slave
    //MSB
  } bit;

  uint8_t raw;
} REG_CFGREG2_04h_t;

typedef union {
  struct {
    //LSB
    //bool HOLD_TIME_RGB[2] : 2; // per RGB LED, Hold Time Breathing Hold on (0 == T2)  (1 == T1)
    //struct {
    //  bool HT_0isT2_1isT1 : 1;
    //} HOLD_TIME_RGB[2];
    bool HT_0isT2_1isT1_LED1 : 1;
    bool HT_0isT2_1isT1_LED2 : 1;
    uint8_t _unused32 : 2;

    //bool RAMP_MODE_RGB[2] : 2; // per RGB LED, Ramping Mode Enable
    //struct {
    //  bool rampmode : 1;
    //} RAMP_RGB[2];
    bool RAMP_RGB_1 : 1;
    bool RAMP_RGB_2 : 1;
    uint8_t _unused76 : 2;
    //MSB
  } bit;

  uint8_t raw;
} REG_RAMPMODE_05h_t;

typedef union {
  struct {
    uint8_t PWM[3];
  } led[2];
  uint8_t array[6];
} REG_PWM_t;

typedef struct {
  //LSB
  uint8_t A : 4;    // A time constant for T0
  uint8_t B : 2;    // B time constant for T0
  uint8_t _unused76 : 2;
  //MSB
} REG_T0_def_packed_struct_t;
typedef union {
  REG_T0_def_packed_struct_t led_out[2][3];    // size of 6 for each out (led1 = out1~3 and led2=out4~6)
  uint8_t array[6];     // size of 6 for each out
} REG_T0s_t;

//Time - T1~T3s
typedef struct {
  //LSB
  uint8_t A : 3;    // A time constant for T1~T3
  uint8_t _unused3 : 1;
  uint8_t B : 3;    // B time constant for T1~T3
  bool doubletime : 1;  // DT double-time 0 T3=T1 ; 1 T3=2T1
  //MSB
} REG_T1T3_def_packed_struct_t;
typedef union {
  REG_T1T3_def_packed_struct_t field[2];    // size of 2 for each led
  uint8_t raw[2];     // size of 2 for each led
} REG_T1T3s_t;

//Time - T4s
typedef struct {
  //LSB
  uint8_t A : 4;    // A time constant for T0
  uint8_t B : 2;    // B time constant for T0
  uint8_t _unused76 : 2;
  //MSB
} REG_T4_def_packed_struct_t;
typedef union {
  REG_T4_def_packed_struct_t led_out[2][3];    // size of 6 for each out (led1 = out1~3 and led2=out4~6)
  uint8_t array[6];     // size of 6 for each out
} REG_T4s_t;





typedef union {
  struct {
    REG_LEDCTRL_01h_t LEDCTRL_01;   // REgisters 01h  - 1 bytes
    REG_CFGREG1_03h_t CFGREG1_03;   // REgisters 03h  - 1 bytes
    REG_CFGREG2_04h_t CFGREG2_04;   // REgisters 04h  - 1 bytes
    REG_RAMPMODE_05h_t RAMPMODE_05; // REgisters 05h  - 1 bytes
    REG_PWM_t PWMs;                 // REgisters 07h~0Ch  - 6 bytes

    // updating 01h,07h~0Ch will need to do a data_update

    REG_T0s_t   T0;         // REgisters 11h~16h  - 6 bytes
    REG_T1T3s_t T1T3;       // REgisters 1Ah~1Bh  - 2 bytes
    REG_T4s_t   T4;         // REgisters 1Dh~22h  - 6 bytes

    // updating 11h~16h,1Ah~1Bh,1Dh~22h will need to do a time_update to have the settings take effect

  } s;
  uint8_t vec[24];     // 24 bytes total to hold all of the struct registers
} LED_DRIVER_REGS_t;
extern LED_DRIVER_REGS_t * led_drv_regs_public;


void enable_led_driver(bool enable);
void led_driver_init(uint16_t ledtop0frnt1);
void led_driver_uninit(void);
void led_driver_scheduled_evt_handler(void *p_context, uint16_t size);

led_driver_errors_t led_driver_GHL_setcolor_solid(led_driver_led_selection led_selection, const uint8_t rgb[3], bool ramp);
led_driver_errors_t led_driver_GHL_setcolor_breathing(led_driver_led_selection led_selection, const uint8_t rgb[3], bool enable);
led_driver_errors_t led_driver_GHL_setcolor_flashing_fast(led_driver_led_selection led_selection, const uint8_t rgb[3], bool enable);
led_driver_errors_t led_driver_GHL_set_channel_animation_alternating_yellow_red(led_driver_led_selection led_selection);
led_driver_errors_t led_driver_GHL_turn_off_led(led_driver_led_selection led_selection);
led_driver_errors_t led_driver_GHL_set_animation_machine_error(led_driver_led_selection led_selection);

led_driver_errors_t led_driver_update_device_if_needed();

#ifdef __cplusplus
}
#endif
#endif