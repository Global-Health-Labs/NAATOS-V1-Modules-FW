#include "PowerModuleLogger.h"

void powerModuleGetLogFileName(const char *_logFileName, calendar_time_t time) {
}

uint32_t powerModuleConstructSensorDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent, float battery_v) {
  return sprintf(logLineBuffer, "20%02d-%02d-%02d %02d:%02d:%02d,%0.2f,%0.2f,%0.2f,%0.2f,%d, \r\n",
      time.year,
      time.month,
      time.day,
      time.hour,
      time.minute,
      time.second,
      log_message.temperature_data.heat_zone_0_temp,
      log_message.temperature_data.heat_zone_0_pwm,
      log_message.temperature_data.heat_zone_2_temp,
      log_message.temperature_data.heat_zone_2_pwm,
      battery_percent);
}

uint32_t powerModuleConstructEventDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent, float battery_v) {
  return sprintf(logLineBuffer, "20%02d-%02d-%02d %02d:%02d:%02d,%0.2f,%0.2f,%0.2f,%0.2f,%d,%s\n",
      time.year,
      time.month,
      time.day,
      time.hour,
      time.minute,
      time.second,
      log_message.temperature_data.heat_zone_0_temp,
      log_message.temperature_data.heat_zone_0_pwm,
      log_message.temperature_data.heat_zone_2_temp,
      log_message.temperature_data.heat_zone_2_pwm,
      battery_percent,
      log_message.event_data.message);
}