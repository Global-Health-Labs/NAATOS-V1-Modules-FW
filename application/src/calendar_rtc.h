#ifndef CALENDAR_RTC_H
#define CALENDAR_RTC_H

#include "i2c_hal_freertos.h"
#include "naatos_config.h"
#include <stdint.h>

#define PCF85_S_ADDR 0x51

#define PCF85_REG_CTRL1_ADDR 0x00
#define PCF85_REG_CTRL2_ADDR 0x01
#define PCF85_REG_TIME_DATE_ADDR 0x04
#define PCF85_REG_ALARM_ADDR 0x0B
#define PCF85_REG_OFFSET_ADDR 0x02
#define PCF85_REG_COUNTDOWN_TIMER_VALUE_ADDR 0x10
#define PCF85_REG_COUNTDOWN_TIMER_MODE_ADDR 0x11

bool calendar_get_time(calendar_time_t *now);
bool calendar_set_time(calendar_time_t *now);
bool calendar_reset(void);
bool calendar_set_time_helper(void);
bool calendar_stop(void);
bool calendar_start(void);
bool calendar_set_32k(void);

#endif