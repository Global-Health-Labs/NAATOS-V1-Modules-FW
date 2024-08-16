#include "SamplePrepLogger.h"
#include <stdio.h>

void samplePrepGetLogFileName(const char *_logFileName, calendar_time_t time) {
}

uint32_t samplePrepConstructSensorDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent) {

    return sprintf(logLineBuffer, "%02d:%02d:%02d,%0.2f,%0.2f,%0.2f,%0.2f,%d, \r\n",
        time.hour,
        time.minute,
        time.second,
        log_message.temperature_data.amp2_zone_temp,
        log_message.temperature_data.amp0_zone_pwm,
        log_message.temperature_data.motorSpeed,
        log_message.temperature_data.amp1_zone_pwm,
        battery_percent);

}

uint32_t samplePrepConstructEventDataLogLine(char *logLineBuffer, calendar_time_t time, log_data_message_t log_message, int battery_percent) {
  uint32_t ret = 0;
  ret = sprintf(logLineBuffer, "%02d:%02d:%02d,%0.2f,%0.2f,%0.2f,%0.2f,%d,%s\r\n",
      time.hour,
      time.minute,
      time.second,
      log_message.temperature_data.amp2_zone_temp,
      log_message.temperature_data.amp0_zone_pwm,
      log_message.temperature_data.motorSpeed,
      log_message.temperature_data.amp1_zone_pwm,
      battery_percent,
      log_message.event_data.message);
  return ret;
}