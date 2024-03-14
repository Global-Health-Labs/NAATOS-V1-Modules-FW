#include "heater.h"

void heater_task(void * pvParameters) {
  for (;;) {
    printf("In the heater task :)\n");
    vTaskDelay(1000);
  }
}