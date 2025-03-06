#include "is31fl3196.h"
#include "i2c_hal_freertos.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "logger/logger.h"

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
  led_driver_update_time_op,
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

LED_DRIVER_REGS_t led_drv_regs = {0};
LED_DRIVER_REGS_t led_drv_regs_LAST = {0};
LED_DRIVER_REGS_t * led_drv_regs_public = &led_drv_regs;
typedef struct  {
  LED_DRIVER_REGS_t* now;
  LED_DRIVER_REGS_t* prev;
} cached_registers_t;
cached_registers_t cached_registers = {
  .now = &led_drv_regs,
  .prev = &led_drv_regs_LAST
};

static char tmp[128];
static const uint8_t tmp_sz=128;

static uint8_t led_driver_i2c_addr = IS31FL3199_ADDR_low;   // this works for sampleprep revB/C and for power module revB top leds
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

led_driver_errors_t led_driver_commit_data_regs(led_driver_opDoneCallback_t cb);
led_driver_errors_t led_driver_commit_time_regs(led_driver_opDoneCallback_t cb);
led_driver_errors_t led_driver_update_device_if_needed();
led_driver_errors_t led_driver_send_local_cached_registers_to_device();

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

//#ifdef SAMPLE_PREP_BOARD
//  err_code = xUtil_TWI_Write_Single(i2c_interface_sensors, IS31FL3199_ADDR_low, reg, buf, 2); //acquire data
//#else
//  err_code = xUtil_TWI_Write_Single(i2c_interface_sensors, IS31FL3199_ADDR_low, reg, buf, 2); //acquire data (non-low is smaller leds)
//#endif
  err_code = xUtil_TWI_Write_Single(i2c_interface_sensors, led_driver_i2c_addr, reg, buf, 2); //acquire data (non-low is smaller leds)

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


static led_driver_errors_t led_driver_writeSingleRegisterBlocking(uint8_t regaddr,uint8_t regval) {
  uint8_t buf[2] = {regaddr, regval};
  led_driver_errors_t led_driverError = led_driver_writeRegister(buf, 2, led_driver_reg_write_op, NULL);

  if (led_driverError != led_driver_success) {
    return led_driverError;
  }
  return led_driver_success;
}

static led_driver_errors_t led_driver_writeRegister_if_needed(uint8_t regaddr,uint8_t* ptrInCurrentStruct) {
  // WE EXPECT ptrInCurrentStruct to be a member of cached_registers.now
  //uint32_t inStructureOffset = (ptrInCurrentStruct - &cached_registers.now)
  uint8_t newvalue = *ptrInCurrentStruct;

  // Get the offset of this pointer from cached_register.now, so we can dereference it from cached_registers.prev
  int offset_in_register_structure = ( (uint8_t*) ptrInCurrentStruct - (uint8_t*) cached_registers.now ) ;
  uint8_t oldvalue = (uint8_t) *( ((uint8_t*) cached_registers.prev) + offset_in_register_structure );

  //snprintf(tmp, tmp_sz, "IS31Fxx: writeIfNeed regaddr=0x%02x newval=0x%02x offsetBytesInStruct=% 2d oldval=0x%02x", regaddr, newvalue, offset_in_register_structure, oldvalue);
  //send_debug_log_message(tmp);

  // Did value change? Then actually write the register
  if(newvalue != oldvalue)  {
    #if 0
    snprintf(tmp, tmp_sz, "IS31Fxx: writeIfNeed regaddr=0x%02x newval=0x%02x [ offsetBytesInStruct=%d oldval=0x%02x ]", regaddr, newvalue, offset_in_register_structure, oldvalue);
    send_debug_log_message(tmp);
    #endif
    return led_driver_writeSingleRegisterBlocking(regaddr, newvalue);
  } else{
    return (led_driver_errors_t) led_driver_success_nowrite_needed;
  }
  
  //return led_driver_writeSingleRegisterBlocking(regaddr, newvalue);
}

void led_driver_init(uint16_t ledtop0frnt1) {
#if !POWER_MODULE_REV_B
  nrf_gpio_cfg_output(LED_DRV_EN);
#else 
  nrf_gpio_cfg_output(TOP_LED_DRV_EN);
#endif

  //snprintf(tmp,tmp_sz,"IS31Fxx: sizeof(register_struct)=%d",sizeof(LED_DRIVER_REGS_t));
  //send_debug_log_message(tmp);

  #if defined(POWER_MODULE_REV_B)
  // Select I2C address based on the config file
  if(config.ledtop0frnt1==1)
    led_driver_i2c_addr = IS31FL3199_ADDR;
  else
    led_driver_i2c_addr = IS31FL3199_ADDR_low;
  #endif
  
  enable_led_driver(false);
  for (int i = 0; i < 100; i++) {
    asm volatile("nop");
  }
  enable_led_driver(true);

  // Send Software reset
  //nrf_gpio_pin_set(TOP_LED_DRV_EN);
  uint8_t buf_reset[2] = {LED_DRIVER_RESET_REG, 0x00};
  led_driver_writeRegisterBlocking(buf_reset, 2);
  for (int i = 0; i < 100; i++) {
    asm volatile("nop");
  }
  enable_led_driver(true);

  // Set LED current to 20mA
  //led_drv_regs.s.CFGREG2_04.bit.CURRENT_SETTING = LED_CURRENT__5MA>>4;
  led_drv_regs.s.CFGREG2_04.bit.CURRENT_SETTING = LED_CURRENT_25MA>>4;

  //Set all LED's ON (but the PWM's will be off so nonhthing will appear yet)
  //uint8_t led_off_buf[3] = {LED_DRIVER_CTRL_REG_1, 0x00};
  //led_driver_writeRegisterBlocking(led_off_buf, 2);
  led_drv_regs.s.LEDCTRL_01.OUT_LED1_VEC = 0b111;
  led_drv_regs.s.LEDCTRL_01.OUT_LED2_VEC = 0b111;

  //PWM data
  led_drv_regs.s.PWMs.led[0].PWM[0] = 0x00;
  led_drv_regs.s.PWMs.led[0].PWM[1] = 0x00;
  led_drv_regs.s.PWMs.led[0].PWM[2] = 0x00;
  led_drv_regs.s.PWMs.led[1].PWM[0] = 0x00;
  led_drv_regs.s.PWMs.led[1].PWM[1] = 0x00;
  led_drv_regs.s.PWMs.led[1].PWM[2] = 0x00;

  /*
  // TESTING AND DEBUG SECTION - FOR LED 1 //
  led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_1 = true;
  led_drv_regs.s.LEDCTRL_01.OUT_LED1_VEC=0b111;
  led_drv_regs.s.CFGREG1_03.bit.oneshot_1 = true;
  ////Set T0 for one-shot mode
  //led_drv_regs.s.T0.led_out[0][0].A=0;
  //led_drv_regs.s.T0.led_out[0][0].B=0;
  //led_drv_regs.s.T0.led_out[0][1].A=0;
  //led_drv_regs.s.T0.led_out[0][1].B=0;
  //led_drv_regs.s.T0.led_out[0][2].A=0;
  //led_drv_regs.s.T0.led_out[0][2].B=0;
  //Set T1 to be "no breathing"
  led_drv_regs.s.T1T3.field[0].A = 1;  //<-- Controls T1 = [5 or 6 is breathing OFF]
  //Set T2 to be shortest-non-zero ramp-time
  led_drv_regs.s.T1T3.field[0].B = 0;  //<-- Controls T2 = [t*2^(B-1) where t is about 260ms]
  //Set T4
  //led_drv_regs.s.T4.led_out[0][0].A = 1;
  //led_drv_regs.s.T4.led_out[0][0].B = 1;
  //led_drv_regs.s.T4.led_out[0][1].A = 1;
  //led_drv_regs.s.T4.led_out[0][1].B = 1;
  //led_drv_regs.s.T4.led_out[0][2].A = 1;
  //led_drv_regs.s.T4.led_out[0][2].B = 1;
  // set RGB
  led_drv_regs.s.PWMs.led[0].PWM[0] = 0xFF;
  led_drv_regs.s.PWMs.led[0].PWM[1] = 0;
  led_drv_regs.s.PWMs.led[0].PWM[2] = 0x33;
  */

  /*
  // TESTING AND DEBUG SECTION - FOR LED 2 //
  led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_2 = true;
  led_drv_regs.s.LEDCTRL_01.OUT_LED2_VEC = 0b111;
  led_drv_regs.s.CFGREG1_03.bit.oneshot_2 = false;
  //Set T1 to be "no breathing"
  led_drv_regs.s.T1T3.field[1].A = 5;  //<-- Controls T1 = [5 or 6 is breathing OFF]
  //Set T2 to be shortest-non-zero ramp-time
  led_drv_regs.s.T1T3.field[1].B = 1;  //<-- Controls T2 = [t*2^(B-1) where t is about 260ms]
  // set RGB
  led_drv_regs.s.PWMs.led[1].PWM[0] = 0;
  led_drv_regs.s.PWMs.led[1].PWM[1] = 0xFF;
  led_drv_regs.s.PWMs.led[2].PWM[2] = 0;
  */

  led_driver_update_device_if_needed();
}

void led_driver_uninit(void) {
  // Disable LED Driver
  enable_led_driver(false);
  // Should be all we have to do. The LED will be shutdown when going into sleep mode
  // by turning the LED_DRV_EN pill low.
}

led_driver_errors_t led_driver_commit_data_regs(led_driver_opDoneCallback_t cb) {
  // register 0x10h must have 0x00 written to it,
  // in order to commit/actually update any of the 0x07~0x0C registers
  led_driver_errors_t err_code;

  //send_debug_log_message("IS31Fxx: commit data regs");

  uint8_t buf[2] = {LED_DRIVER_DATA_UPDATE_REG, 0x00};
  led_driver_writeRegister(buf, 2, led_driver_update_pwm_op, savedUserCallback);
  if (err_code != led_driver_success) {
    return led_driver_i2c_error;
  }
  return led_driver_success;
}

led_driver_errors_t led_driver_commit_time_regs(led_driver_opDoneCallback_t cb) {
  // register 0x26h must have 0x00 written to it,
  // in order to commit/actually update any of the
  // 0x11~0x16 and 0x1A~0x1B and 0x1D~0x22 registers
  led_driver_errors_t err_code;

  //send_debug_log_message("IS31Fxx: commit timing regs");

  uint8_t buf[2] = {LED_DRIVER_TIME_UPDATE_REG, 0x00};
  led_driver_writeRegister(buf, 2, led_driver_update_time_op, savedUserCallback);
  if (err_code != led_driver_success) {
    return led_driver_i2c_error;
  }
  return led_driver_success;
}

led_driver_errors_t led_driver_update_device_if_needed() {
  led_driver_errors_t ret;
  ret = led_driver_send_local_cached_registers_to_device();
  
  *(cached_registers.prev) = *(cached_registers.now); // structure assignment-by-copy
  return ret;
}

led_driver_errors_t led_driver_send_local_cached_registers_to_device() {
  //uint8_t buf[7];
  //uint8_t* valnow;
  //uint8_t* vallast;
  led_driver_errors_t ret;
  bool anyUpdatePerformed_DATA = false;
  bool anyUpdatePerformed_TIMING = false;

  // REG 01H - LED Control Register [ 01h ]
  ret = led_driver_writeRegister_if_needed(LED_DRIVER_CTRL_REG_1, &(led_drv_regs.s.LEDCTRL_01.raw));
  if(ret==led_driver_success) {
    anyUpdatePerformed_DATA=true;
  } else if(ret==led_driver_success_nowrite_needed) {
  } else{
    return ret;
  }

  // REG 03H - CONFIG Register 1 [ 03h ]
  ret = led_driver_writeRegister_if_needed(LED_DRIVER_CONFIG_REG_1, &(led_drv_regs.s.CFGREG1_03.raw));
  if(ret==led_driver_success) {
    anyUpdatePerformed_DATA=true;
  } else if(ret==led_driver_success_nowrite_needed) {
  } else{
    return ret;
  }

  // REG 04H - CONFIG Register 2 [ 04h ]
  ret = led_driver_writeRegister_if_needed(LED_DRIVER_CONFIG_REG_2, &(led_drv_regs.s.CFGREG2_04.raw));
  if(ret==led_driver_success) {
    anyUpdatePerformed_DATA=true;
  } else if(ret==led_driver_success_nowrite_needed) {
  } else{
    return ret;
  }

  // REG 05H - Ramping Register [ 05h ]
  ret = led_driver_writeRegister_if_needed(LED_DRIVER_RAMP_MODE_REG, &(led_drv_regs.s.RAMPMODE_05.raw));
  if(ret==led_driver_success) {
    anyUpdatePerformed_DATA=true;
  } else if(ret==led_driver_success_nowrite_needed) {
  } else{
    return ret;
  }

  // skipping Breathing Mark stuff [06h], defaults will be just fine

  // REG 07H ~ 0CH - PWMs (1 byte per output, 6 bytes total)
  for(uint8_t i=0;i<6;i++)  {
    ret = led_driver_writeRegister_if_needed( (uint8_t) (0x07 + i), &(led_drv_regs.s.PWMs.array[i]) );
    if(ret==led_driver_success) {
      anyUpdatePerformed_TIMING=true;
    } else if(ret==led_driver_success_nowrite_needed) {
    } else{
      return ret;
    }
  }

  // REG 11H ~ 16H - T0 registers (1 byte per output, 6 bytes total)
  for(uint8_t i=0;i<6;i++)  {
    ret = led_driver_writeRegister_if_needed( (uint8_t) (0x11 + i), &(led_drv_regs.s.T0.array[i]) );
    if(ret==led_driver_success) {
      anyUpdatePerformed_TIMING=true;
    } else if(ret==led_driver_success_nowrite_needed) {
    } else{
      return ret;
    }
  }

  // REG 1AH ~ 1BH - T1/T3 registers (1 byte per led, 2 bytes total)
  for(uint8_t i=0;i<2;i++)  {
    ret = led_driver_writeRegister_if_needed( (uint8_t) (0x1A + i), &(led_drv_regs.s.T1T3.raw[i]) );
    if(ret==led_driver_success) {
      anyUpdatePerformed_TIMING=true;
    } else if(ret==led_driver_success_nowrite_needed) {
    } else{
      return ret;
    }
  }

  // REG 1DH ~ 22H - T4 registers (1 byte per output, 6 bytes total)
  for(uint8_t i=0;i<6;i++)  {
    ret = led_driver_writeRegister_if_needed( (uint8_t) (0x1D + i), &(led_drv_regs.s.T4.array[i]) );
    if(ret==led_driver_success) {
      anyUpdatePerformed_TIMING=true;
    } else if(ret==led_driver_success_nowrite_needed) {
    } else{
      return ret;
    }
  }

  // all done updating registers that we care about

  // now, if any were updated, the chip requires that we send a special update register command
  //anyUpdatePerformed_DATA = true;
  //anyUpdatePerformed_TIMING = true;
  //if(anyUpdatePerformed_DATA)
  //  ret = led_driver_commit_data_regs(NULL);
  //if(anyUpdatePerformed_TIMING)
  //  ret = led_driver_commit_time_regs(NULL);
  if(anyUpdatePerformed_DATA || anyUpdatePerformed_TIMING)  {
    led_driver_commit_data_regs(NULL);
    led_driver_commit_time_regs(NULL);
    //send_debug_log_message("IS31Fxx: some reg writes were performed");
  }
  
  return ret;
}

led_driver_errors_t led_driver_GHL_setcolor_solid(led_driver_led_selection led_selection, const uint8_t rgb[3], bool ramp) {
  led_driver_errors_t err_code;
  //// ramp mode reg, 0x70 == hold on T2 ON
  //led_ramp_mode_reg_val |= 1 << led_selection + 4;
  //uint8_t ramp_write_buf[2] = {LED_DRIVER_RAMP_MODE_REG, led_ramp_mode_reg_val};
  //led_driver_writeRegister(ramp_write_buf, 1, write_reg, NULL);

  // Set Ramp Mode (so that LED plays back T1~T2 then holds forever)
  // Ensure LED drivers to be enabled for all 3 channels for this led (we'll use PWM registers to color-mix)
  // Oneshot = true if we want to ramp, false if it will just hold the colors in PWM registers
  if(led_selection==LED1) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_1 = true;
    led_drv_regs.s.RAMPMODE_05.bit.HT_0isT2_1isT1_LED1 = false;
    led_drv_regs.s.LEDCTRL_01.OUT_LED1_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_1 = ramp;
  } else if(led_selection==LED2) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_2 = true;
    led_drv_regs.s.RAMPMODE_05.bit.HT_0isT2_1isT1_LED2 = false;
    led_drv_regs.s.LEDCTRL_01.OUT_LED2_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_2 = ramp;
  }

  //Set T0 for one-shot mode
  led_drv_regs.s.T0.led_out[led_selection][0].A=0;    //red
  led_drv_regs.s.T0.led_out[led_selection][0].B=0;
  led_drv_regs.s.T0.led_out[led_selection][1].A=0;    //green
  led_drv_regs.s.T0.led_out[led_selection][1].B=0;
  led_drv_regs.s.T0.led_out[led_selection][2].A=0;    //blue
  led_drv_regs.s.T0.led_out[led_selection][2].B=0;

  //Set T1 to be "no breathing"
  led_drv_regs.s.T1T3.field[led_selection].A = 0;  //<-- Controls T1 = [0-4 T1=T3=t*2^A where t is about 260ms]

  //Set T2 to be shortest-non-zero ramp-time
  led_drv_regs.s.T1T3.field[led_selection].B = 0;  //<-- Controls T2 = [t*2^(B-1) where t is about 260ms]

  //Set T4
  led_drv_regs.s.T4.led_out[led_selection][0].A = 0;  //red
  led_drv_regs.s.T4.led_out[led_selection][0].B = 0;
  led_drv_regs.s.T4.led_out[led_selection][1].A = 0;  //green
  led_drv_regs.s.T4.led_out[led_selection][1].B = 0;
  led_drv_regs.s.T4.led_out[led_selection][2].A = 0;  //blue
  led_drv_regs.s.T4.led_out[led_selection][2].B = 0;

  // Set Color (PWM's)
  led_drv_regs.s.PWMs.led[led_selection].PWM[0] = rgb[0];
  led_drv_regs.s.PWMs.led[led_selection].PWM[1] = rgb[1];
  led_drv_regs.s.PWMs.led[led_selection].PWM[2] = rgb[2];

  return led_driver_success;
}

led_driver_errors_t led_driver_GHL_turn_off_led(led_driver_led_selection led_selection) {
  led_driver_errors_t err_code;
  //// ramp mode reg, 0x70 == hold on T2 ON
  //led_ramp_mode_reg_val |= 1 << led_selection + 4;
  //uint8_t ramp_write_buf[2] = {LED_DRIVER_RAMP_MODE_REG, led_ramp_mode_reg_val};
  //led_driver_writeRegister(ramp_write_buf, 1, write_reg, NULL);

  // Set Ramp Mode (so that LED plays back T1~T2 then holds forever)
  // Ensure LED drivers to be enabled for all 3 channels for this led (we'll use PWM registers to color-mix)
  // Oneshot = true if we want to ramp, false if it will just hold the colors in PWM registers
  if(led_selection==LED1) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_1 = true;
    led_drv_regs.s.LEDCTRL_01.OUT_LED1_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_1 = true;
  } else if(led_selection==LED2) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_2 = true;
    led_drv_regs.s.LEDCTRL_01.OUT_LED2_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_2 = true;
  }

  //Set T0 for one-shot mode
  led_drv_regs.s.T0.led_out[led_selection][0].A=0;    //red
  led_drv_regs.s.T0.led_out[led_selection][0].B=0;
  led_drv_regs.s.T0.led_out[led_selection][1].A=0;    //green
  led_drv_regs.s.T0.led_out[led_selection][1].B=0;
  led_drv_regs.s.T0.led_out[led_selection][2].A=0;    //blue
  led_drv_regs.s.T0.led_out[led_selection][2].B=0;

  //Set T1 to be "no breathing"
  led_drv_regs.s.T1T3.field[led_selection].A = 5;  //<-- Controls T1 = [0-4 T1=T3=t*2^A where t is about 260ms]

  //Set T2 to be shortest-non-zero ramp-time
  led_drv_regs.s.T1T3.field[led_selection].B = 0;  //<-- Controls T2 = [t*2^(B-1) where t is about 260ms]

  //Set T4
  led_drv_regs.s.T4.led_out[led_selection][0].A = 0;  //red
  led_drv_regs.s.T4.led_out[led_selection][0].B = 0;
  led_drv_regs.s.T4.led_out[led_selection][1].A = 0;  //green
  led_drv_regs.s.T4.led_out[led_selection][1].B = 0;
  led_drv_regs.s.T4.led_out[led_selection][2].A = 0;  //blue
  led_drv_regs.s.T4.led_out[led_selection][2].B = 0;

  // Set Color (PWM's)
  led_drv_regs.s.PWMs.led[led_selection].PWM[0] = 0;
  led_drv_regs.s.PWMs.led[led_selection].PWM[1] = 0;
  led_drv_regs.s.PWMs.led[led_selection].PWM[2] = 0;

  return led_driver_success;
}


led_driver_errors_t led_driver_GHL_setcolor_breathing(led_driver_led_selection led_selection, const uint8_t rgb[3], bool enable) {
  led_driver_errors_t err_code;

  // No Ramp Mode, want the T0~T4 pattern repeating
  // Ensure LED drivers to be enabled for all 3 channels for this led (we'll use PWM registers to color-mix)
  // Set Oneshot mode for auto pwm control
  if(led_selection==LED1) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_1 = false;
    led_drv_regs.s.LEDCTRL_01.OUT_LED1_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_1 = true;
  } else if(led_selection==LED2) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_2 = false;
    led_drv_regs.s.LEDCTRL_01.OUT_LED2_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_2 = true;
  }

  //Set T0 governs initial off-time
  led_drv_regs.s.T0.led_out[led_selection][0].A=0;    //red
  led_drv_regs.s.T0.led_out[led_selection][0].B=0;
  led_drv_regs.s.T0.led_out[led_selection][1].A=0;    //green
  led_drv_regs.s.T0.led_out[led_selection][1].B=0;
  led_drv_regs.s.T0.led_out[led_selection][2].A=0;    //blue
  led_drv_regs.s.T0.led_out[led_selection][2].B=0;

  //Set T1 - ramp rate
  led_drv_regs.s.T1T3.field[led_selection].A = 0;  //<-- Controls T1 = [0-4 T1=T3=t*2^A where t is about 260ms]

  //Set T2 - hold time
  led_drv_regs.s.T1T3.field[led_selection].B = 0;  //<-- Controls T2 = [t*2^(B-1) where t is about 260ms]

  //Set T4 - iner-cycle off-time
  led_drv_regs.s.T4.led_out[led_selection][0].A = 0;  //red
  led_drv_regs.s.T4.led_out[led_selection][0].B = 0;
  led_drv_regs.s.T4.led_out[led_selection][1].A = 0;  //green
  led_drv_regs.s.T4.led_out[led_selection][1].B = 0;
  led_drv_regs.s.T4.led_out[led_selection][2].A = 0;  //blue
  led_drv_regs.s.T4.led_out[led_selection][2].B = 0;

  // Set Color (PWM's)
  led_drv_regs.s.PWMs.led[led_selection].PWM[0] = rgb[0];
  led_drv_regs.s.PWMs.led[led_selection].PWM[1] = rgb[1];
  led_drv_regs.s.PWMs.led[led_selection].PWM[2] = rgb[2];

  return led_driver_success;
}

led_driver_errors_t led_driver_GHL_setcolor_flashing_fast(led_driver_led_selection led_selection, const uint8_t rgb[3], bool enable) {
  led_driver_errors_t err_code;

  // No Ramp Mode, want the T0~T4 pattern repeating
  // Ensure LED drivers to be enabled for all 3 channels for this led (we'll use PWM registers to color-mix)
  // Set Oneshot mode for auto pwm control
  if(led_selection==LED1) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_1 = false;
    led_drv_regs.s.LEDCTRL_01.OUT_LED1_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_1 = true;
  } else if(led_selection==LED2) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_2 = false;
    led_drv_regs.s.LEDCTRL_01.OUT_LED2_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_2 = true;
  }

  //Set T0 governs initial off-time
  const uint8_t T0A = 0;
  const uint8_t T0B = 0;
  led_drv_regs.s.T0.led_out[led_selection][0].A=T0A;    //red
  led_drv_regs.s.T0.led_out[led_selection][0].B=T0B;
  led_drv_regs.s.T0.led_out[led_selection][1].A=T0A;    //green
  led_drv_regs.s.T0.led_out[led_selection][1].B=T0B;
  led_drv_regs.s.T0.led_out[led_selection][2].A=T0A;    //blue
  led_drv_regs.s.T0.led_out[led_selection][2].B=T0B;

  //Set T1 - ramp rate (and T3 ?)
  led_drv_regs.s.T1T3.field[led_selection].A = 7;  //<-- Controls T1 = [0-4 T1=T3=t*2^A where t is about 260ms, 7==0.1ms]
  led_drv_regs.s.T1T3.field[led_selection].doubletime = false; //<-- if 0--> T3 =T1    if 1--> T3=2*T1

  //Set T2 - hold time
  led_drv_regs.s.T1T3.field[led_selection].B = 1;  //<-- Controls T2 = [t*2^(B-1) where t is about 260ms]

  //Set T4 - inter-cycle off-time
  const uint8_t T4A = 1;
  const uint8_t T4B = 0;
  led_drv_regs.s.T4.led_out[led_selection][0].A = T4A;  //red
  led_drv_regs.s.T4.led_out[led_selection][0].B = T4B;
  led_drv_regs.s.T4.led_out[led_selection][1].A = T4A;  //green
  led_drv_regs.s.T4.led_out[led_selection][1].B = T4B;
  led_drv_regs.s.T4.led_out[led_selection][2].A = T4A;  //blue
  led_drv_regs.s.T4.led_out[led_selection][2].B = T4B;

  // Set Color (PWM's)
  led_drv_regs.s.PWMs.led[led_selection].PWM[0] = rgb[0];
  led_drv_regs.s.PWMs.led[led_selection].PWM[1] = rgb[1];
  led_drv_regs.s.PWMs.led[led_selection].PWM[2] = rgb[2];

  return led_driver_success;
}

led_driver_errors_t led_driver_GHL_led_off(led_driver_led_selection led_selection) {
  if(led_selection==LED1) {
    //led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_1 = false;
    //led_drv_regs.s.LEDCTRL_01.OUT_LED1_VEC=0b000;
    //led_drv_regs.s.CFGREG1_03.bit.oneshot_1 = false;
  } else if(led_selection==LED2) {
    //led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_2 = false;
    //led_drv_regs.s.LEDCTRL_01.OUT_LED2_VEC=0b000;
    //led_drv_regs.s.CFGREG1_03.bit.oneshot_2 = false;
  }

}

led_driver_errors_t led_driver_GHL_set_animation_machine_error(led_driver_led_selection led_selection) {
  led_driver_errors_t err_code;

  // ALTERNATING RED / MAGENTA

  // No Ramp Mode, want the T0~T4 pattern repeating
  // Ensure LED drivers to be enabled for all 3 channels for this led (we'll use PWM registers to color-mix)
  // Set Oneshot mode for auto pwm control
  if(led_selection==LED1) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_1 = false;
    led_drv_regs.s.LEDCTRL_01.OUT_LED1_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_1 = true;
  } else if(led_selection==LED2) {
    led_drv_regs.s.RAMPMODE_05.bit.RAMP_RGB_2 = false;
    led_drv_regs.s.LEDCTRL_01.OUT_LED2_VEC=0b111;
    led_drv_regs.s.CFGREG1_03.bit.oneshot_2 = true;
  }

  //Set T0 governs initial off-time
  const uint8_t T0A = 0;
  const uint8_t T0B = 0;
  led_drv_regs.s.T0.led_out[led_selection][0].A=T0A;    //red
  led_drv_regs.s.T0.led_out[led_selection][0].B=T0B;
  led_drv_regs.s.T0.led_out[led_selection][1].A=T0A;    //green
  led_drv_regs.s.T0.led_out[led_selection][1].B=T0B;
  led_drv_regs.s.T0.led_out[led_selection][2].A=T0A;    //blue
  led_drv_regs.s.T0.led_out[led_selection][2].B=T0B;

  //Set T1 - ramp rate (and T3 ?)
  led_drv_regs.s.T1T3.field[led_selection].A = 7;  //<-- Controls T1 = [0-4 T1=T3=t*2^A where t is about 260ms, 7==0.1ms]
  led_drv_regs.s.T1T3.field[led_selection].doubletime = false; //<-- if 0--> T3 =T1    if 1--> T3=2*T1

  //Set T2 - hold time
  led_drv_regs.s.T1T3.field[led_selection].B = 1;  //<-- Controls T2 = [t*2^(B-1) where t is about 260ms]

  //Set T4 - inter-cycle off-time
  //const uint8_t T4A = 1;
  //const uint8_t T4B = 0;
  led_drv_regs.s.T4.led_out[led_selection][0].A = 0;  //red
  led_drv_regs.s.T4.led_out[led_selection][0].B = 0;
  led_drv_regs.s.T4.led_out[led_selection][1].A = 0;  //green
  led_drv_regs.s.T4.led_out[led_selection][1].B = 0;
  led_drv_regs.s.T4.led_out[led_selection][2].A = 1;  //blue
  led_drv_regs.s.T4.led_out[led_selection][2].B = 0;

  // Set Color (PWM's)
  led_drv_regs.s.PWMs.led[led_selection].PWM[0] = 0xFF;
  led_drv_regs.s.PWMs.led[led_selection].PWM[1] = 0;
  led_drv_regs.s.PWMs.led[led_selection].PWM[2] = 70;

  return led_driver_success;
}

led_driver_errors_t led_driver_GHL_set_channel_animation_alternating_yellow_red(led_driver_led_selection led_selection) {
  led_driver_errors_t err_code;

  // A = 7 for 0.1ms rise/fall time, B = 2, ~0.5 second flash
  // T13 register
  // 7   - DT (doubletime
  // 6:4 - B value
  // 3   - don't care
  // 2:0 - A value
  uint8_t t13_update_addr = 0x1A + led_selection;
  uint8_t t13_update_value = 0b00010111;
  uint8_t t13_write_buf[2] = {t13_update_addr, t13_update_value};
  err_code = led_driver_writeRegister(t13_write_buf, 1, write_reg, NULL);
  if (err_code != led_driver_success) return err_code;

  // Shorten off time for flashing
  uint8_t t4_update_addr = 0x1D + led_selection * 3 + red;
  uint8_t t4_update_value = 0x00;
  uint8_t t4_write_buf[2] = {t4_update_addr, t4_update_value};
  err_code = led_driver_writeRegister(t4_write_buf, 1, write_reg, NULL);
  if (err_code != led_driver_success) return err_code;

  t4_update_addr = 0x1D + led_selection * 3 + green;
  t4_update_value = 0x01;
  t4_write_buf[0] = t4_update_addr;
  t4_write_buf[1] = t4_update_value;
  err_code = led_driver_writeRegister(t4_write_buf, 1, write_reg, NULL);
  if (err_code != led_driver_success) return err_code;

  //t4_update_addr = 0x1D + led_selection * 3 + blue;
  //t4_update_value = 0x00;
  //t4_write_buf[0] = t4_update_addr;
  //t4_write_buf[1] = t4_update_value;
  //err_code = led_driver_writeRegister(t4_write_buf, 1, write_reg, NULL);
  //if (err_code != led_driver_success) return err_code;

  // time update reg
  uint8_t time_update_write_buf[2] = {LED_DRIVER_TIME_UPDATE_REG, 0x00};
  err_code = led_driver_writeRegister(time_update_write_buf, 1, write_reg, NULL);
  if (err_code != led_driver_success) return err_code;

  // ramp mode reg turn holds off
  led_ramp_mode_reg_val &= ~(1 << led_selection + 4);
  uint8_t ramp_write_buf[2] = {LED_DRIVER_RAMP_MODE_REG, led_ramp_mode_reg_val};
  err_code = led_driver_writeRegister(ramp_write_buf, 1, write_reg, NULL);
  if (err_code != led_driver_success) return err_code;
}

void enable_led_driver(bool enable) {
  if (enable) {
#if !POWER_MODULE_REV_B
    nrf_gpio_pin_set(LED_DRV_EN);
    uint8_t shutdown_buf[2] = {LED_DRIVER_SHUTDOWN_REG, 0x01};
    led_driver_writeRegisterBlocking(shutdown_buf, 2);
#else 
    nrf_gpio_pin_set(TOP_LED_DRV_EN);
    uint8_t shutdown_buf[2] = {LED_DRIVER_SHUTDOWN_REG, 0x01};
    led_driver_writeRegisterBlocking(shutdown_buf, 2);
#endif
  } else {
#if !POWER_MODULE_REV_B
    uint8_t shutdown_buf[2] = {LED_DRIVER_SHUTDOWN_REG, 0x00};
    led_driver_writeRegisterBlocking(shutdown_buf, 2);
    nrf_gpio_pin_clear(LED_DRV_EN);
#else
    uint8_t shutdown_buf[2] = {LED_DRIVER_SHUTDOWN_REG, 0x00};
    led_driver_writeRegisterBlocking(shutdown_buf, 2);
    nrf_gpio_pin_clear(TOP_LED_DRV_EN);
#endif
  }
}

#ifdef POWER_MODULE_BOARD
#endif