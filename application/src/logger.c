#include "logger.h"

xQueueHandle logger_recvBattPercentQueue;
xQueueHandle logger_logMessageQueue;

void logger_task(void * pvParameters) {
  for (;;) {
    printf("In the logger task :)\n");
    vTaskDelay(1000);
  }
}