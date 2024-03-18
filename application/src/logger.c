#include "logger.h"

xQueueHandle logger_recvBattPercentQueue;
xQueueHandle logger_logMessageQueue;

void logger_task(void * pvParameters) {
  for (;;) {
    vTaskDelay(1000);
  }
}