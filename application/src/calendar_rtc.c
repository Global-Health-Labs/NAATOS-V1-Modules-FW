#include "calendar_rtc.h"

// Decode retreived info
uint8_t calendar_decode(uint8_t reading) {
  return (reading >> 4) * 10 + (reading & 0x0F);
}

// Encode retreived info
uint8_t calendar_encode(uint8_t reading) {
  return ((reading / 10) << 4) | (reading % 10);
}

// Get the current time on the calendar chip
bool calendar_get_time(calendar_time_t * now) {
  uint8_t buff[7];
  ret_code_t ret;

#if USE_CALENDAR_CHIP
  // Get the Time and Date from the calendar chip
  ret = xUtil_TWI_Read(i2c_interface_system, PCF85_S_ADDR, PCF85_REG_TIME_DATE_ADDR, buff, 7);
  if (ret) {
    printf("Unable to i2c communicate with calendar chip!\n");
    return false;
  }
  
  // Decode the retreived data
  now->second   = calendar_decode(buff[0] & ~0x80);
  now->minute   = calendar_decode(buff[1] & ~0x80);
  now->hour     = calendar_decode(buff[2] & ~0xC0);
  now->day      = calendar_decode(buff[3] & ~0xC0);
  now->week_day = calendar_decode(buff[4] & ~0xF8);
  now->month    = calendar_decode(buff[5] & ~0xE0);
  now->year     = calendar_decode(buff[6]);
  
  return ~(buff[0] & 0x80);
#else 
  now->second   = 59;
  now->minute   = 59; 
  now->hour     = 12;
  now->day      = 30;
  now->week_day = 6;
  now->month    = 12;
  now->year     = 24;
  
  return true;
#endif
}

// Set the time on the calendar chip, this should only be done once.
//      * TODO: Need to figure out how we want to use this to set the time and date when 
//        boards are being brought up.
bool calendar_set_time(calendar_time_t * now) {
  uint8_t buff[7];
  ret_code_t ret; 
#if I2C_CONNECTED  
  buff[0] = calendar_encode(now->second);
  buff[1] = calendar_encode(now->minute);
  buff[2] = calendar_encode(now->hour);
  buff[3] = calendar_encode(now->day);
  buff[4] = calendar_encode(now->week_day);
  buff[5] = calendar_encode(now->month);
  buff[6] = calendar_encode(now->year);
  
  ret = xUtil_TWI_Write(i2c_interface_system, PCF85_S_ADDR, PCF85_REG_TIME_DATE_ADDR, buff, 7);
  if (ret) {
    return false;
  }
  return true;
#else 
  // Always return false if i2c is not enabled
  return false;
#endif
}

// Resets the calendar chip *** Will get rid of current date set
bool calendar_reset(void) {
  uint8_t buff = 0x58;
  ret_code_t ret;
#if I2C_CONNECTED
  ret = xUtil_TWI_Write(i2c_interface_system, PCF85_S_ADDR, PCF85_REG_CTRL1_ADDR, buff, 1);
  if (ret)
    return false;
  return true;
#else 
  // Always return false if i2c is not enabled
  return false;
#endif  

}