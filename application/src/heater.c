#include "heater.h"

xQueueHandle heater_zoneRunQueue;
xQueueHandle heater_temperatureDataQueue;

void heater_task(void * pvParameters) {
  for (;;) {
    printf("In the heater task :)\n");
    vTaskDelay(1000);
  }
}