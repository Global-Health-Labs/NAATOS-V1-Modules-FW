#include "sensors.h"


void ghl_read_all_temperature_sensors_in_loop_and_printf()  {
  const uint16_t nsamples = 128;
  float temperature1[nsamples];
  float temperature2[nsamples];
  //float runtotal = 0;
  bool readTempSuccess = false;
  uint16_t readTempFailureCount1 = 0;
  uint16_t readTempFailureCount2 = 0;

  float runtotal1 = 0;
  float runtotal2 = 0;
  float s1min;
  float s1max;
  float s2min;
  float s2max;
  float s1avg;
  float s2avg;

  for(uint16_t i=0; i<nsamples; i++) {
    // I2C Read for Heater Zone 0
    readTempSuccess = readTemp(heat_zone_0, &(temperature1[i]));
    if (!readTempSuccess) {
      readTempFailureCount1 += 1;
    } else{
      runtotal1 += temperature1[i];
    }

    // I2C Read for Heater Zone 2
    readTempSuccess = readTemp(heat_zone_2, &(temperature2[i]));
    if (!readTempSuccess) {
      readTempFailureCount2 += 1;
    } else{
      runtotal2 += temperature2[i];
    }
  }

  // calculate avg, max, min
  s1avg = runtotal1/(nsamples-readTempFailureCount1);
  s2avg = runtotal2/(nsamples-readTempFailureCount2);
  s1min = temperature1[0];
  s1min = temperature1[0];
  s2min = temperature2[0];
  s2max = temperature2[0];
  for(uint16_t i=0; i<nsamples; i++)  {
    if(temperature1[i] < s1min) {
      s1min = temperature1[i];
    }
    if(temperature1[i] > s1max) {
      s1max = temperature1[i];
    }
    if(temperature2[i] < s2min) {
      s2min = temperature2[i];
    }
    if(temperature2[i] > s2max) {
      s2max = temperature2[i];
    }
  }

  printf("GHL Looped Temp1 Fails=%lu avg,min,max=%.01f,%.01f,%.01f \n",readTempFailureCount1, s1avg, s1min, s1max);
  printf("GHL Looped Temp2 Fails=%lu avg,min,max=%.01f,%.01f,%.01f \n",readTempFailureCount2, s2avg, s2min, s2max);
}