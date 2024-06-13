#include "switch.h"
#include "naatos_queues.h"
#include "nrf_drv_gpiote.h"
#include "timers.h"

#define BUTTON_TASK_DELAY 80 // msec

xQueueHandle buttonRxQueue;
TimerHandle_t buttonTimer;

bool skip_debounce = false;
int skip_cnt = 0;

void checkButtonState(void);

void samplePrepButtonState(void);

void button_init(void) {
  /* Setup Hal Sensor */
  nrf_gpio_cfg_input(BUTTON_INPUT_PIN, NRF_GPIO_PIN_NOPULL); // tied to 3.3v internally
}

void sendButtonUpdate(button_update_t msg) {
  BaseType_t xReturned = xQueueSend(button_mainStateQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("SWITCH_TASK: Unable to send to button_mainStateQueue.\n");
  }
}

void vButtonTimerCallback(TimerHandle_t xTimer) {
  BaseType_t xReturned;
  ButtonRxQueueMsg_t msg;
  msg.type = BUTTON_MSG_TIMER_EVENT;

  xReturned = xQueueSend(buttonRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("SWITCH_TASK: Unable to send timer update to buttonRxQueue queue.\n");
  }
}

void startButtonTimer(void) {
  TickType_t sampleRateTicks = pdMS_TO_TICKS(BUTTON_TASK_DELAY);

  if (xTimerChangePeriod(buttonTimer, sampleRateTicks, 100) != pdPASS) {
    printf("SWITCH_TASK: Cannot change period of button timer. \n");
  }

  if (xTimerStart(buttonTimer, 0) != pdPASS) {
    printf("SWITCH_TASK: Failed to start button timer. \n");
  }
}

void stopButtonTimer(void) {
  if (xTimerStop(buttonTimer, 100) != pdPASS) {
    printf("Failed to stop button timer. \n");
  }
}

void gpiote_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  BaseType_t xReturned;

  if (skip_debounce) {
    skip_debounce = false;
    return;
  }

  if (skip_cnt > 0 && !skip_debounce) {
    return;
  }

  ButtonRxQueueMsg_t msg;
  msg.type = BUTTON_MSG_WAKE;

  xReturned = xQueueSendFromISR(buttonRxQueue, &msg, &xHigherPriorityTaskWoken);
  if (xReturned != pdPASS) {
    printf("SWITCH_TASK:: Unable to send timer update to buttonRxQueue queue.\n");
  }

  skip_cnt++;
}

static bool previousSwitchState = true;
static int switchCounter = 0;
static TickType_t lastSwitchTime = 0;
static TickType_t buttonPressStartTime = 0;
button_update_t updateMsg;
button_event_e currentEvent = NONE;
button_event_e previousEvent = ON_EVENT;
bool awake = true;
int notify = 0;

void buttonTask(void *pvParameters) {
  BaseType_t xReturned;
  previousSwitchState = nrf_gpio_pin_read(BUTTON_INPUT_PIN);

  ButtonRxQueueMsg_t buttonRxMessage;

  TickType_t sampleRateTicks = pdMS_TO_TICKS(BUTTON_TASK_DELAY);

  buttonTimer = xTimerCreate("ButtonTimer", sampleRateTicks, pdTRUE, (void *)0, vButtonTimerCallback);
  startButtonTimer();

  while (true) {
    xReturned = xQueueReceive(buttonRxQueue, &buttonRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("Unable to Rx data to button queue\n");
    } else {
      switch (buttonRxMessage.type) {
      case BUTTON_MSG_SLEEP:
        if (xTimerIsTimerActive(buttonTimer) == pdTRUE) {
          stopButtonTimer();
        }

        skip_debounce = true;
        skip_cnt = 0;
        notify = 0;

        nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true); //NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI
        nrf_drv_gpiote_in_init(BUTTON_INPUT_PIN, &config, gpiote_event_handler);
        nrf_drv_gpiote_in_event_enable(BUTTON_INPUT_PIN, true);

        //set button as a edge detect event and send that event over to main queue
        //if button is already true here then send event anyway
        //send to main queue that we are asleep
        break;

      case BUTTON_MSG_WAKE:
        nrf_drv_gpiote_in_event_disable(BUTTON_INPUT_PIN);
        if (xTimerIsTimerActive(buttonTimer) == pdFALSE) {
          startButtonTimer();
        }
        // Notify main queue
        if (notify <= 0) {
          xReturned = xQueueSend(main_wakeupTasksQueue, &awake, 0);
          if (xReturned != pdPASS) {
            printf("SWITCH_TASK: Unable to send tasks wake up to main_wakeupTasksQueue. \n");
          }
          notify++;
        }
        break;

      case BUTTON_MSG_TIMER_EVENT:
#ifdef SAMPLE_PREP_BOARD
        samplePrepButtonState();
#else
        checkButtonState();
#endif
        break;

      default:
        break;
      }
    }
  }
}

void checkButtonState(void) {
  bool switchState = nrf_gpio_pin_read(BUTTON_INPUT_PIN);

  // Check for switch rocking back and forth
  if (switchState != previousSwitchState) {
    switchCounter++;
    lastSwitchTime = xTaskGetTickCount();
  }

  // If 5 seconds have passed since the last switch change
  if ((xTaskGetTickCount() - lastSwitchTime) >= pdMS_TO_TICKS(2000)) {
    if (switchCounter >= 6) { // 3 times back and forth = 6 changes
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

  if (currentEvent != previousEvent) {
    updateMsg.event = currentEvent;
    sendButtonUpdate(updateMsg);
    previousEvent = currentEvent;
  }

  // Update previous state
  previousSwitchState = switchState;
}

bool buttonHeld = false;

void samplePrepButtonState(void) {
  bool switchState = nrf_gpio_pin_read(BUTTON_INPUT_PIN);

  TickType_t currentTime = xTaskGetTickCount();

  if (switchState != previousSwitchState) {
    if (switchState == false) { // Button pressed
      switchCounter++;
      lastSwitchTime = currentTime;
      buttonPressStartTime = currentTime;
      buttonHeld = true;
    } else { // Button released
      buttonHeld = false;
      if ((lastSwitchTime - buttonPressStartTime) < pdMS_TO_TICKS(1000) && switchCounter == 1) {
        currentEvent = ON_EVENT;
      }
    }
  }

  // Check for long press
  if (buttonHeld && (currentTime - buttonPressStartTime) >= pdMS_TO_TICKS(2000)) {
    currentEvent = OFF_EVENT;
    buttonHeld = false;
  }

  // Check for triple press
  if ((currentTime - lastSwitchTime) >= pdMS_TO_TICKS(2000)) {
    if (switchCounter >= 3) {
      currentEvent = BOOTLOADER_EVENT;
    }
    switchCounter = 0;
  }

  if (currentEvent != NONE) {
    updateMsg.event = currentEvent;
    sendButtonUpdate(updateMsg);
    currentEvent = NONE;
  }

  // Update previous state
  previousSwitchState = switchState;
}