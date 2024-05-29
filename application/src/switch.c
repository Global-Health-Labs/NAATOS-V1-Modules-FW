#include "switch.h"
#include "naatos_queues.h"
#define BUTTON_TASK_DELAY 150 // msec

void button_init(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(BUTTON_INPUT_PIN, NRF_GPIO_PIN_NOPULL); // tied to 3.3v internally 
  
}

void sendButtonUpdate(button_update_t msg) {
  BaseType_t xReturned = xQueueSend(button_mainStateQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("MAIN_TASK: Unable to send to button_mainStateQueue.\n");
  }
}

void buttonTask(void * pvParameters) {
  bool previousSwitchState = nrf_gpio_pin_read(BUTTON_INPUT_PIN);
  int switchCounter = 0;
  static TickType_t lastSwitchTime = 0;

  button_update_t updateMsg;
  button_event_e currentEvent = ON_EVENT;
  button_event_e previousEvent = ON_EVENT;

  while(true) {
    vTaskDelay(BUTTON_TASK_DELAY); 
        bool switchState = nrf_gpio_pin_read(BUTTON_INPUT_PIN);
        
        // Check for switch rocking back and forth
        if (switchState != previousSwitchState) {
            switchCounter++;
            lastSwitchTime = xTaskGetTickCount();
        }
        
        // If 5 seconds have passed since the last switch change
        if ((xTaskGetTickCount() - lastSwitchTime) >= pdMS_TO_TICKS(2000)) {
            if (switchCounter >= 6) {  // 3 times back and forth = 6 changes
              currentEvent = BOOTLOADER_EVENT;
            } else {
              if (switchState) {
                currentEvent = ON_EVENT;
              } else {
                currentEvent = OFF_EVENT;
              }
            }
            switchCounter = 0;
        }

        if(currentEvent != previousEvent) {
          updateMsg.event = currentEvent;
          sendButtonUpdate(updateMsg);
          previousEvent = currentEvent;
        }
        
        // Update previous state
        previousSwitchState = switchState;
  }
}