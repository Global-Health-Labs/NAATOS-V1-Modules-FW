#include "calendar_rtc.h"
#include "ff.h"
#include "time.h"

// Decode retreived info
uint8_t calendar_decode(uint8_t reading) {
  return (reading >> 4) * 10 + (reading & 0x0F);
}

// Encode retreived info
uint8_t calendar_encode(uint8_t reading) {
  return ((reading / 10) << 4) | (reading % 10);
}

DWORD get_fattime(void) {
  calendar_time_t now = {
      .second = 0,
      .minute = 0,
      .hour = 0,
      .day = 0,
      .week_day = 0,
      .month = 0,
      .year = 0};

  if (calendar_get_time(&now)) {
    // Assume the 'year' is offset from 2000, e.g., 24 for 2024
    DWORD fattime = ((DWORD)(now.year + 20) << 25) // Year since 1980 (20 + 20 = 2024)
                    | ((DWORD)now.month << 21)     // Month (1–12)
                    | ((DWORD)now.day << 16)       // Day (1–31)
                    | ((DWORD)now.hour << 11)      // Hour (0–23)
                    | ((DWORD)now.minute << 5)     // Minute (0–59)
                    | ((DWORD)now.second >> 1);    // Second (0–59, in 2-second resolution)

    return fattime;
  } else {
    // Return a default timestamp if unable to get the time
    return ((DWORD)(24 + 20) << 25) // Default year: 2024
           | ((DWORD)1 << 21)       // Default month: January
           | ((DWORD)1 << 16)       // Default day: 1st
           | ((DWORD)0 << 11)       // Default hour: 00:00
           | ((DWORD)0 << 5)        // Default minute: 00
           | ((DWORD)0 >> 1);       // Default second: 00
  }
}

// Get the current time on the calendar chip
bool calendar_get_time(calendar_time_t *now) {
  uint8_t buff[7];
  ret_code_t ret;

#if USE_CALENDAR_CHIP
  // Get the Time and Date from the calendar chip
  ret = xUtil_TWI_Read(i2c_interface_sensors, PCF85_S_ADDR, PCF85_REG_TIME_DATE_ADDR, buff, 7);
  if (ret) {
    printf("Unable to i2c communicate with calendar chip!\n");
    return false;
  }

  // Decode the retreived data
  now->second = calendar_decode(buff[0] & ~0x80);
  now->minute = calendar_decode(buff[1] & ~0x80);
  now->hour = calendar_decode(buff[2] & ~0xC0);
  now->day = calendar_decode(buff[3] & ~0xC0);
  now->week_day = calendar_decode(buff[4] & ~0xF8);
  now->month = calendar_decode(buff[5] & ~0xE0);
  now->year = calendar_decode(buff[6]);

  return ~(buff[0] & 0x80);
#else
  now->second = 59;
  now->minute = 59;
  now->hour = 12;
  now->day = 30;
  now->week_day = 6;
  now->month = 12;
  now->year = 24;

  return true;
#endif
}

// Set the time on the calendar chip, this should only be done once.
//      * TODO: Need to figure out how we want to use this to set the time and date when
//        boards are being brought up.
bool calendar_set_time(calendar_time_t *now) {
  uint8_t buff[8];
  ret_code_t ret;
#if I2C_CONNECTED
  buff[0] = PCF85_REG_TIME_DATE_ADDR;
  buff[1] = calendar_encode(now->second);
  buff[2] = calendar_encode(now->minute);
  buff[3] = calendar_encode(now->hour);
  buff[4] = calendar_encode(now->day);
  buff[5] = calendar_encode(now->week_day);
  buff[6] = calendar_encode(now->month);
  buff[7] = calendar_encode(now->year);

  ret = xUtil_TWI_Write_Single(i2c_interface_sensors, PCF85_S_ADDR, PCF85_REG_TIME_DATE_ADDR, buff, 8);
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
  ret = xUtil_TWI_Write(i2c_interface_sensors, PCF85_S_ADDR, PCF85_REG_CTRL1_ADDR, buff, 1);
  if (ret)
    return false;
  return true;
#else
  // Always return false if i2c is not enabled
  return false;
#endif
}

// Stops the RTC from counting
bool calendar_stop(void) {
  uint8_t buff = 0x1 << 5;
  ret_code_t ret;
#if I2C_CONNECTED
  ret = xUtil_TWI_Write(i2c_interface_sensors, PCF85_S_ADDR, PCF85_REG_CTRL1_ADDR, buff, 1);
  if (ret)
    return false;
  return true;
#else
  // Always return false if i2c is not enabled
  return false;
#endif
}

// Start the RTC counting
bool calendar_start(void) {
  uint8_t buff = 0x00;
  ret_code_t ret;
#if I2C_CONNECTED
  ret = xUtil_TWI_Write(i2c_interface_sensors, PCF85_S_ADDR, PCF85_REG_CTRL1_ADDR, buff, 1);
  if (ret)
    return false;
  return true;
#else
  // Always return false if i2c is not enabled
  return false;
#endif
}

// Set RTC frequency to 32khz
bool calendar_set_32k(void) {
  uint8_t buff[2];

  buff[0] = PCF85_REG_CTRL1_ADDR;
  buff[1] = 0x00;

  ret_code_t ret;
#if I2C_CONNECTED
  ret = xUtil_TWI_Write_Single(i2c_interface_sensors, PCF85_S_ADDR, PCF85_REG_CTRL2_ADDR, buff, 2);
  if (ret)
    return false;
  return true;
#else
  // Always return false if i2c is not enabled
  return false;
#endif
}

bool calendar_set_time_helper(void) {
  calendar_time_t now;

  //Check if the time should be set
  calendar_get_time(&now);

  //Debug to hardcode the time
  //now.second = 1;
  //now.minute = 43;
  //now.hour = 12;
  //now.day = 2;
  //now.week_day = 2;
  //now.month = 7;
  //now.year = 24;

  //Loads time from config file
  now.second = config.hhmmss % 100;
  now.minute = (config.hhmmss / 100) % 100;
  now.hour = (config.hhmmss / 10000) % 100;

  now.year = config.mmddyy % 100;
  now.day = (config.mmddyy / 100) % 100;
  now.month = (config.mmddyy / 10000) % 100;

  now.week_day = 1; //Leaving the weekday/day of the week value hardcoded. This isn't reported in the log file.

  //calendar_stop();
  calendar_set_time(&now);

  printf("Requested time  M: %d D: %d Y:%d h: %d m: %d s: %d\r\n",
      now.month,
      now.day,
      now.year,
      now.hour,
      now.minute,
      now.second);

  //calendar_start();
  calendar_get_time(&now);

  printf("Read back time  M: %d D: %d Y:%d h: %d m: %d s: %d\r\n",
      now.month,
      now.day,
      now.year,
      now.hour,
      now.minute,
      now.second);

  return true;
}