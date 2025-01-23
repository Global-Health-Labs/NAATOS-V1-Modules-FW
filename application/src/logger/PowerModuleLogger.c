#include "PowerModuleLogger.h"

void powerModuleGetLogFileName(const char *_logFileName, calendar_time_t time) {
}

uint32_t powerModuleConstructSensorDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent, float battery_v, float battery_temperature) {
#ifndef SAMPLE_PREP_BOARD
  return sprintf(logLineBuffer, "20%02d-%02d-%02d %02d:%02d:%02d,%0.2f,%0.2f,%0.2f,%0.2f,%d,%0.2f,%0.2f, \r\n",
      time.year,
      time.month,
      time.day,
      time.hour,
      time.minute,
      time.second,
      log_message.temperature_data.valve_temp,
      log_message.temperature_data.valve_pwm,
      log_message.temperature_data.amp_temp,
      log_message.temperature_data.amp_pwm,
      battery_percent,
      battery_v,
      battery_temperature);
#endif
}

uint32_t powerModuleConstructEventDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent, float battery_v, float battery_temperature) {
#ifndef SAMPLE_PREP_BOARD
  return sprintf(logLineBuffer, "20%02d-%02d-%02d %02d:%02d:%02d,%0.2f,%0.2f,%0.2f,%0.2f,%d,%0.2f,%0.2f,%s\r\n",
      time.year,
      time.month,
      time.day,
      time.hour,
      time.minute,
      time.second,
      log_message.temperature_data.valve_temp,
      log_message.temperature_data.valve_pwm,
      log_message.temperature_data.amp_temp,
      log_message.temperature_data.amp_pwm,
      battery_percent,
      battery_v,
      battery_temperature,
      log_message.event_data.message);
#endif
}