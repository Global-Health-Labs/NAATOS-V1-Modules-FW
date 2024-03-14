/*********************************************************************
*
*       battery_task()
*
*  Function description
*   Application entry point.
*/

#include "battery.h"

void battery_task(void * pvParameters) {
  for (;;) {
    printf("In the battery management task :)\n");
    vTaskDelay(1000);
  }
}