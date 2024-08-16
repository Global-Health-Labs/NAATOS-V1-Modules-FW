/*********************************************************************
*
*       usb_task()
*
*  Function description
*   Application entry point.
*/

#include "usb.h"
#include "naatos_config.h"
#include "nrf_drv_power.h"
#include "nrf_drv_usbd.h"
#include "sd_card.h"
#include "timers.h"
#include "semphr.h"

/* ***** Variables ***** */
// USB connection status
static bool m_usb_connected = false;

// Queue Handles
xQueueHandle usbRxQueue;

xQueueHandle usb_recvUsbWaitAcceptQueue;
xQueueHandle usb_usbWaitOverQueue;

xQueueHandle compositeRxQueue;

// Main Loop
usb_message_t recv_msg;
charge_state_t connection_state;
main_state_t main_state;
usb_command_t command;

// USB Suspended Vars
bool usb_started = false;
bool usb_suspended_tasks = false;
bool usb_detected = false;
bool msc_active = false, cdc_acm_active = false;
bool usb_done_config = false;
bool usb_initalized = false;
bool restarting = false;
bool needs_response = false;
bool usb_conn_updated = false;
bool com_port_open = false;

// Timers
TimerHandle_t usbTimer;
TimerHandle_t compositeUsbTimer;

/* ***** Instances ***** */

// Mass storage class instance
APP_USBD_MSC_GLOBAL_DEF(m_app_msc,
    0,
    msc_user_ev_handler,
    ENDPOINT_LIST(),
    BLOCKDEV_LIST(),
    MSC_WORKBUFFER_SIZE);
// CDC_ACM class instance
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
    cdc_acm_user_ev_handler,
    CDC_ACM_COMM_INTERFACE,
    CDC_ACM_DATA_INTERFACE,
    CDC_ACM_COMM_EPIN,
    CDC_ACM_DATA_EPIN,
    CDC_ACM_DATA_EPOUT,
    APP_USBD_CDC_COMM_PROTOCOL_AT_V250);

void vUSBTimerCallback(TimerHandle_t xTimer) {
  BaseType_t xReturned;
  usbRxMsgType_t msg = {
      .cmd = NULL,
      .msg_type = USB_MSG_CHECK_CONN};

  xReturned = xQueueSend(usbRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("USB_TASK: Unable to send check connection from timer.\n");
  }
}

void startUSBTimer(void) {
  TickType_t sampleRateTicks = USB__TASK_DELAY;

  if (xTimerChangePeriod(usbTimer, sampleRateTicks, 100) != pdPASS) {
    printf("Cannot change period of usb timer. \n");
  }

  if (xTimerStart(usbTimer, 0) != pdPASS) {
    printf("Failed to start usb timer. \n");
  }
}

void stopUSBTimer(void) {
  if (xTimerStop(usbTimer, 100) != pdPASS) {
    printf("Failed to stop usb timer. \n");
  }
}

void vCompositeUSBTimerCallback(TimerHandle_t xTimer) {
  BaseType_t xReturned;
  CompositeUSBRxQueueType_t msg = COMPOSITE_MSG_CONTINUE;

  xReturned = xQueueSend(compositeRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("COMPOSITE_TASK: Unable to send continue from timer.\n");
  }
}

void startCompositeTimer(void) {
  TickType_t sampleRateTicks = USB_TASK_DELAY;

  if (xTimerChangePeriod(compositeUsbTimer, sampleRateTicks, 100) != pdPASS) {
    printf("Cannot change period of composite usb timer. \n");
  }

  if (xTimerStart(compositeUsbTimer, 0) != pdPASS) {
    printf("Failed to start composite usb timer. \n");
  }
}

void stopCompositeTimer(void) {
  if (xTimerStop(compositeUsbTimer, 100) != pdPASS) {
    printf("Failed to stop composite usb timer. \n");
  }
}

void respond_to_usb_change(void) {
  BaseType_t xReturned;
  bool state_changed = true;

  xReturned = xQueueSend(main_usbChangedConfQueue, &state_changed, 0);
  if (xReturned != pdPASS) {
    printf("USB_TASK: Unable to send usb changed response to main_usbChangedConfQueue. \n");
  }
}


static SemaphoreHandle_t uartSemaphore;

void setup_uart_semaphore(void) {
    uartSemaphore = xSemaphoreCreateBinary();// Ensure the semaphore is created before it gets used.
    ASSERT( uartSemaphore );          // LOCK HERE: the semaphore could not be created
    xSemaphoreGive( uartSemaphore );  // 'Give' the peripheral protection semaphore
}

void naatosPrintf(const char * msg, int len) {
#ifdef UART_PRINT_F_ENABLED
  write_to_com(msg, len);
#else
  printf(msg);
#endif
}

void write_to_com(const char * msg, int len) {
  if (!usb_detected || !com_port_open)
    return;

  if (xSemaphoreTake(uartSemaphore, portMAX_DELAY) != pdPASS) {
      return NRF_ERROR_BUSY;
  }

  app_usbd_class_inst_t const *class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
  app_usbd_cdc_acm_write(class_cdc_acm, msg, len);

  xSemaphoreGive(uartSemaphore);
}

void cdc_acm_user_ev_handler(app_usbd_class_inst_t const *p_inst,
    app_usbd_cdc_acm_user_event_t event) {
  app_usbd_cdc_acm_t const *p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

  switch (event) {
  case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN: {
    printf("COM port opened.\n");
    com_port_open = true;
    //Setup first transfer
    ret_code_t ret = app_usbd_cdc_acm_read(&m_app_cdc_acm,
        m_rx_buffer,
        READ_SIZE);
    UNUSED_VARIABLE(ret);
    break;
  }
  case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
    com_port_open = false;
    break;
  case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
    break;
  case APP_USBD_CDC_ACM_USER_EVT_RX_DONE: {
    ret_code_t ret;
    printf("Bytes waiting: %d\n", app_usbd_cdc_acm_bytes_stored(p_cdc_acm));
    do {
      //Get amount of data transfered
      size_t size = app_usbd_cdc_acm_rx_size(p_cdc_acm);
      printf("RX: size: %lu char: %c\n", size, m_rx_buffer[0]);

      // Fetch data until internal buffer is empty
      ret = app_usbd_cdc_acm_read(&m_app_cdc_acm,
          m_rx_buffer,
          READ_SIZE);
    } while (ret == NRF_SUCCESS);
    break;
  }
  default:
    break;
  }
}

void usbd_user_ev_handler(app_usbd_event_type_t event) {
  switch (event) {
  case APP_USBD_EVT_DRV_SUSPEND:
    printf("USB: Suspended\n");
    break;
  case APP_USBD_EVT_DRV_RESUME:
    printf("USB: Resumed\n");
    break;
  case APP_USBD_EVT_STARTED:
    printf("USB: Started\n");
    usb_started = true;
    break;
  case APP_USBD_EVT_STOPPED:
    printf("USB: Stopped\n");
    usb_started = false;
    //app_usbd_disable();
    break;
  case APP_USBD_EVT_POWER_DETECTED:
    printf("USB: Power detected\n");
    usb_detected = true;
    usb_conn_updated = true;
    //set_led2_blue_breathe();
    if (!nrf_drv_usbd_is_enabled()) {
      app_usbd_enable();
    }
    break;
  case APP_USBD_EVT_POWER_REMOVED:
    printf("USB: Power removed\n");
    if (nrf_drv_usbd_is_enabled()) {
      app_usbd_stop();
    }
    usb_done_config = false;
    if (!restarting)
      usb_detected = false;
    else
      restarting = false;
    usb_conn_updated = true;
    break;
  case APP_USBD_EVT_POWER_READY:
    printf("USB: Ready\n");
    app_usbd_start();
    m_usb_connected = true;
    break;
  default:
    break;
  }
}

void msc_user_ev_handler(app_usbd_class_inst_t const *p_inst,
    app_usbd_msc_user_event_t event) {
  UNUSED_PARAMETER(p_inst);
  UNUSED_PARAMETER(event);
}


void usb_suspend_conflicting_tasks(void) {
  BaseType_t xReturned;
  bool batt_acpt = false, heater_acpt = false, pwm_acpt = false, sensor_acpt = false;
  usb_suspend_req_t sus_req = {
      .suspend = true,
  };
  usb_suspend_acpt_t sus_acpt;

  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_USB_SUSPEND;
  msg.usbSuspend = true;

  // Send to Sensors task
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send usb suspend request to sensorRxQueue.\n");
  }

  /* Receive back suspend request acceptances */
  while (!sensor_acpt) {
    if (uxQueueMessagesWaiting(usb_recvUsbWaitAcceptQueue) > 0) {
      xReturned = xQueueReceive(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0);
      if (xReturned != pdPASS) {
        printf("USB: Unable to receive from usb_recvUsbWaitAcceptQueue queue.\n");
        continue;
      } else {
        switch (sus_acpt.task) {
        case BATTERY:
          if (sus_acpt.suspended)
            batt_acpt = true;
          break;
        case HEATER:
          if (sus_acpt.suspended)
            heater_acpt = true;
          break;
        case PWM:
          if (sus_acpt.suspended)
            pwm_acpt = true;
          break;
        case SENSORS:
          if (sus_acpt.suspended)
            sensor_acpt = true;
          break;
        default:
          break;
        }
      }
    }
  }

  // Set suspended tasks to true
  usb_suspended_tasks = true;
}

void start_usb(bool cdc_acm, bool msc) {
  ret_code_t ret;

  if (sd_card_inited && msc) {
    uninit_sd_card();
  }

  static const app_usbd_config_t usbd_config = {
      .ev_state_proc = usbd_user_ev_handler};

  app_usbd_serial_num_generate();

  ret = app_usbd_init(&usbd_config);
  APP_ERROR_CHECK(ret);

  usb_initalized = true;

  if (cdc_acm) {
    cdc_acm_active = true;
    app_usbd_class_inst_t const *class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
    ret = app_usbd_class_append(class_cdc_acm);
    APP_ERROR_CHECK(ret);
  } else {
    cdc_acm_active = false;
  }

  if (msc) {
    msc_active = true;
    app_usbd_class_inst_t const *class_inst_msc = app_usbd_msc_class_inst_get(&m_app_msc);
    ret = app_usbd_class_append(class_inst_msc);
    APP_ERROR_CHECK(ret);
  } else {
    msc_active = false;
  }

  if (USBD_POWER_DETECTION) {
    ret = app_usbd_power_events_enable();
    APP_ERROR_CHECK(ret);
  } else {
    printf("No USB power detection enabled\r\nStarting USB now");

    app_usbd_enable();
    app_usbd_start();

    m_usb_connected = true;
  }

  if (command != USB_MSC && command != USB_MSC_CDC_ACM && needs_response) {
    respond_to_usb_change();
    needs_response = false;
  }
}

void restart_usb_only_cdc_acm(void) {
  printf("USB: Restarting USB to only have Virtual COM Port.\n");
  app_usbd_stop();
  //app_usbd_ep_disable(ENDPOINT_LIST());
  init_sd_card();

  //usb_done_config = false;
  usb_started = false;
  m_usb_connected = false;
}

void composite_usb_task(void *pvParameters) {
  BaseType_t xReturned;
  usb_suspend_over_t sus_over;
  bool batt_over = false, heater_over = false, pwm_over = false, sensor_over = false;
  CompositeUSBRxQueueType_t msg;

  compositeUsbTimer = xTimerCreate("CompositeUSBTimer", USB_TASK_DELAY, pdTRUE, (void *)0, vCompositeUSBTimerCallback);
  startCompositeTimer();

  // Start the USB
  if (!usb_started) {
    start_usb(true, false);
    command = USB_CDC_ACM;
  }

  CompositeUSBRxQueueType_t temp_msg = COMPOSITE_MSG_CONTINUE;

  xReturned = xQueueSend(compositeRxQueue, &temp_msg, 0);
  if (xReturned != pdPASS) {
    printf("COMPOSITE_TASK: Unable to send continue from timer.\n");
  }

  for (;;) {

    xReturned = xQueueReceive(compositeRxQueue, &msg, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("COMPOSITE_TASK: Unable to receive continue from composite timer");
    }

    switch (msg) {
    case COMPOSITE_MSG_CONTINUE: {
      while (app_usbd_event_queue_process()) {
        // Nothing to do
      }

      if (msc_active && usb_detected && !usb_suspended_tasks && !usb_done_config && (command == USB_MSC || command == USB_MSC_CDC_ACM)) {
        usb_suspend_conflicting_tasks();
        //set_led1_blue_slow_blink();
        batt_over = false;
        heater_over = false;
        pwm_over = false;
        sensor_over = false;
      }

      if (usb_suspended_tasks) {
        if (uxQueueMessagesWaiting(usb_usbWaitOverQueue) > 0) {
          xReturned = xQueueReceive(usb_usbWaitOverQueue, &sus_over, 0);
          if (xReturned != pdPASS) {
            printf("USB: Unable to receive usb wait over from usb_usbWaitOverQueue\n");
          }
          switch (sus_over.task) {
          case BATTERY:
            if (sus_over.over)
              batt_over = true;
            printf("USB: battery task suspension over.\n");
            break;
          case HEATER:
            if (sus_over.over)
              heater_over = true;
            printf("USB: heater task suspension over.\n");
            break;
          case PWM:
            if (sus_over.over)
              pwm_over = true;
            printf("USB: pwm task suspension over.\n");
            break;
          case SENSORS:
            if (sus_over.over)
              sensor_over = true;
            printf("USB: sensor task suspension over.\n");
            break;
          default:
            break;
          }
        }

        if (/*batt_over &&*/ /*heater_over && pwm_over &&*/ sensor_over) {
          usb_suspended_tasks = false;
          usb_done_config = true;
          //set_led1_green_breathe();
          respond_to_usb_change();
          needs_response = false;
        }
      }
    } break;

    case COMPOSITE_MSG_SLEEP:
      // Go to Sleep
      if (xTimerIsTimerActive(compositeUsbTimer) == pdTRUE) {
        stopCompositeTimer();
      }
      break;

    case COMPOSITE_MSG_WAKEUP:
      // Wakeup
      if (xTimerIsTimerActive(compositeUsbTimer) == pdFALSE) {
        startCompositeTimer();
      }
      break;
    }
  }
}

void usb_task(void *pvParameters) {
  BaseType_t xReturned;
  bool conn_state_req = false;
  tasks_t usb_task = USB;
  bool cont = false;
  volatile int hi;
  bool usb_updated = false;
  usb_command_t last_command;
  usbRxMsgType_t rx_msg;

  // Set connection state to not charging
  connection_state = NOT_CHARGING;

  usbTimer = xTimerCreate("USBTimer", USB__TASK_DELAY, pdTRUE, (void *)0, vUSBTimerCallback);
  startUSBTimer();

  // Set the first event to make sure that USB queue is processed after it is started
  for (;;) {

    // Receive from usbRxQueue
    xReturned = xQueueReceive(usbRxQueue, &rx_msg, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("USB_TASK: Unable to recieve usb message from usbRxQueue. \n");
    }

    switch (rx_msg.msg_type) {
    case USB_MSG_CONN_STATUS_REQ: {
      // Respond to connection status request
      xReturned = xQueueSend(main_usbConnRecvQueue, &usb_detected, 0);
      if (xReturned != pdPASS) {
        printf("USB_TASK: Unable to send usb connection status to main_usbConnRecvQueue. \n");
      }
      break;
    }
    case USB_MSG_COMMAND: {
      needs_response = true;
      // Handle the new command if there is one
      switch (rx_msg.cmd) {
      case USB_DISABLED: {
        if (usb_detected) {
          usbd_user_ev_handler(APP_USBD_EVT_POWER_REMOVED);
          app_usbd_disable();
          app_usbd_uninit();
          disk_uninitialize(0);
          // Restart the USB
          start_usb(true, false);
        }
        respond_to_usb_change();
        break;
      }
      case USB_CDC_ACM: {
        if (last_command == USB_CDC_ACM) {
          break;
        }
        // Stop The USB
        usbd_user_ev_handler(APP_USBD_EVT_POWER_REMOVED);
        app_usbd_disable();
        app_usbd_uninit();
        disk_uninitialize(0);
        // Restart the USB
        start_usb(true, false);
        usbd_user_ev_handler(APP_USBD_EVT_POWER_DETECTED);
        break;
      }
      case USB_MSC: {
        if (last_command == USB_MSC) {
          break;
        }
        // Stop The USB
        usbd_user_ev_handler(APP_USBD_EVT_POWER_REMOVED);
        app_usbd_disable();
        app_usbd_uninit();
        // Restart the USB
        start_usb(false, true);
        usbd_user_ev_handler(APP_USBD_EVT_POWER_DETECTED);
        break;
      }
      case USB_MSC_CDC_ACM: {
        if (last_command == USB_MSC_CDC_ACM) {
          break;
        }
        restarting = true;
        // Stop The USB
        usbd_user_ev_handler(APP_USBD_EVT_POWER_REMOVED);
        app_usbd_disable();
        app_usbd_uninit();
 
        // Restart the USB
        start_usb(true, true);
        usbd_user_ev_handler(APP_USBD_EVT_POWER_DETECTED);
        break;
      }
      default: {
        app_usbd_stop();
        break;
      }
      }
      break;
    }
    case USB_MSG_CHECK_CONN:
      if (usb_conn_updated) {
        xReturned = xQueueSend(main_usbConnRecvQueue, &usb_detected, 0);
        if (xReturned != pdPASS) {
          printf("USB_TASK: Unable to send updated usb connection state to main_usbConnRecvQueue");
        }
        usb_conn_updated = false;
      }
      break;

    case USB_MSG_SLEEP:
      // Go to Sleep
      if (xTimerIsTimerActive(usbTimer) == pdTRUE) {
        stopUSBTimer();
      }
      break;

    case USB_MSG_WAKEUP:
      // Wakeup
      if (xTimerIsTimerActive(usbTimer) == pdFALSE) {
        startUSBTimer();
      }
      break;
    }
  }
}