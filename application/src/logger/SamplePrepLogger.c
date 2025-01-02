#include "SamplePrepLogger.h"
#include <stdio.h>

void samplePrepGetLogFileName(const char *_logFileName, calendar_time_t time) {
}

uint32_t samplePrepConstructSensorDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent, float battery_v, float battery_temperature) {

    return sprintf(logLineBuffer, "20%02d-%02d-%02d %02d:%02d:%02d,%0.2f,%0.2f,%0.2f,%0.2f,%d,%0.2f,%0.2f\r\n",
        time.year,
        time.month,
        time.day,
        time.hour,
        time.minute,
        time.second,
        log_message.temperature_data.heat_zone_3_temp,
        log_message.temperature_data.heat_zone_3_pwm,
        log_message.temperature_data.motorSpeed,
        log_message.temperature_data.heat_zone_2_pwm,
        battery_percent,
        battery_v,
        battery_temperature);

}

uint32_t samplePrepConstructEventDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent, float battery_v, float battery_temperature) {
  uint32_t ret = 0;
  ret = sprintf(logLineBuffer, "20%02d-%02d-%02d %02d:%02d:%02d,%0.2f,%0.2f,%0.2f,%0.2f,%d,%0.2f,%0.2f,%s\r\n",
      time.year,
      time.month,
      time.day,
      time.hour,
      time.minute,
      time.second,
      log_message.temperature_data.heat_zone_3_temp,
      log_message.temperature_data.heat_zone_3_pwm,
      log_message.temperature_data.motorSpeed,
      log_message.temperature_data.heat_zone_2_pwm,
      battery_percent,
      battery_v,
      battery_temperature,
      log_message.event_data.message);
  return ret;
}