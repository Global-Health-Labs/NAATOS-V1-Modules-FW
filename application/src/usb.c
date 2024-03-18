/*********************************************************************
*
*       usb_task()
*
*  Function description
*   Application entry point.
*/

#include "usb.h"

xQueueHandle usb_stateChangeQueue;

void usb_task(void * pvParameters) {
  for (;;) {
    vTaskDelay(1000);
  }
}