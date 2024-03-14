#include "logger.h"

void logger_task(void * pvParameters) {
  for (;;) {
    printf("In the logger task :)\n");
    vTaskDelay(1000);
  }
}