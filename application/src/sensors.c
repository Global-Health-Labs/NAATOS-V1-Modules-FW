#include "sensors.h"

void sensors_task(void * pvParameters) {
  for (;;) {
    printf("In the sensors task :)\n");
    vTaskDelay(1000);
  }
}