#include "sensors.h"

sensor_switches_t switches;

void sensors_task(void * pvParameters) {
  switches.hal_triggered = false;
  switches.optical_tiggered = false;

  for (;;) {
    
    xQueueSend(main_switchQueue, (void *)&switches, 100);

    vTaskDelay(1000);
  }
}