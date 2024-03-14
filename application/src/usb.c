/*********************************************************************
*
*       usb_task()
*
*  Function description
*   Application entry point.
*/

#include "usb.h"

void usb_task(void * pvParameters) {
  for (;;) {
    printf("In the usb management task :)\n");
    vTaskDelay(1000);
  }
}