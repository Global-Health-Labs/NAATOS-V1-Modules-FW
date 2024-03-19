/*********************************************************************
*
*       usb_task()
*
*  Function description
*   Application entry point.
*/

#include "usb.h"

xQueueHandle usb_stateChangeQueue;

usb_message_t recv_msg;
charge_state_t connection_state;
main_state_t main_state;

void usb_task(void * pvParameters) {
  BaseType_t xReturned;
  bool conn_state_updated = false, main_state_updated = false;

  // Set Current State to Standby
  main_state = STANDBY;
  // Set connection state to not charging
  connection_state = NOT_CHARGING;

  for (;;) {
    // Check the USB queue for an update message
    xReturned = xQueueReceive(usb_stateChangeQueue, &recv_msg, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("USB_TASK: Unable to receive usb message from usb_stateChangeQueue.\n");
    }

    // Update from received message
    if (recv_msg.message_type == USB_CONNECTION_TYPE) 
      connection_state = recv_msg.charge_state;
    else if (recv_msg.message_type == MAIN_STATE_TYPE) 
      main_state = recv_msg.current_state;

    // Check USB Connection Status 
    if (connection_state != CHARGING)
      continue;

    // File System Update Based on the main state
    if (main_state == STANDBY) {
      // TODO: Mount the file system
      // TODO: Disable USB
      // TODO: Enable Logging
    }
    else if (main_state == RUNNING) {
      // TODO: Uninit FatFS
      // TODO: Enable UART Stream
      // TODO: Enable Logging
    }
  }
}