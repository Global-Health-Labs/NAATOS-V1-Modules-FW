/*********************************************************************
*
*       usb_task()
*
*  Function description
*   Application entry point.
*/

#include "usb.h"
#include "sd_card.h"
#include "nrf_drv_usbd.h"
#include "nrf_drv_power.h"
#include "naatos_config.h"

/* ***** Variables ***** */
// USB connection status
static bool m_usb_connected = false;

// Queue Handles
xQueueHandle usb_stateChangeQueue;
xQueueHandle usb_recvUsbWaitAcceptQueue;
xQueueHandle usb_usbWaitOverQueue;
//xQueueHandle usb_mainStateContinueQueue;
xQueueHandle usb_connectionReqQueue;

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
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);

void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event)
{
    app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

    switch (event)
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            printf("COM port opened.\n");

            //Setup first transfer
            ret_code_t ret = app_usbd_cdc_acm_read(&m_app_cdc_acm,
                                                   m_rx_buffer,
                                                   READ_SIZE);
            UNUSED_VARIABLE(ret);
            break;
        }
        case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
            break;
        case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
            break;
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            ret_code_t ret;
            printf("Bytes waiting: %d\n", app_usbd_cdc_acm_bytes_stored(p_cdc_acm));
            do
            {
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

void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
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
            set_led2_blue_breathe();
            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            printf("USB: Power removed\n");
            if (nrf_drv_usbd_is_enabled()) {
              app_usbd_stop();
            }
            usb_done_config = false;  
            //usb_detected = false;
            turn_off_led2();
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

void msc_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                app_usbd_msc_user_event_t     event)
{
    UNUSED_PARAMETER(p_inst);
    UNUSED_PARAMETER(event);
}

const BatteryRxQueueMsg_t usbBattSuspendReq = {
  .type = BATTERY_MSG_USB_SUSPEND,
  .usbSuspend =  true
};

void usb_suspend_conflicting_tasks(void) {
  BaseType_t xReturned;
  bool batt_acpt = false, heater_acpt = false, pwm_acpt = false, sensor_acpt = false;
  usb_suspend_req_t sus_req = {
    .suspend = true,
  };
  usb_suspend_acpt_t sus_acpt;

  /* Send suspend requests to conflicting tasks */
  // Send to Battery
  /*xReturned = xQueueSend(batteryRxQueue, &usbBattSuspendReq, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send usb suspend request to batteryRxQueue.\n");
  }*/
  // Send to heater 
  xReturned = xQueueSend(heater_usbWaitQueue, &sus_req, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send usb suspend request to heater_usbWaitQueue.\n");
  }
  // Send to PWM task
  xReturned = xQueueSend(pwm_usbWaitQueue, &sus_req, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send usb suspend request to pwm_usbWaitQueue.\n");
  }

  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_USB_SUSPEND;
  msg.usbSuspend = true;


  // Send to Sensors task
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send usb suspend request to sensorRxQueue.\n");
  }

  /* Receive back suspend request acceptances */
  while(/*!batt_acpt ||*/ !heater_acpt || !pwm_acpt || !sensor_acpt) {
    if (uxQueueMessagesWaiting(usb_recvUsbWaitAcceptQueue) > 0) {
      xReturned = xQueueReceive(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0);
      if (xReturned != pdPASS) {
        printf("USB: Unable to receive from usb_recvUsbWaitAcceptQueue queue.\n");
        continue;
      }
      else {
        switch(sus_acpt.task) {
        case BATTERY:
          if (sus_acpt.suspended) batt_acpt = true;
          break;
        case HEATER:
          if (sus_acpt.suspended) heater_acpt = true;
          break;
        case PWM:
          if (sus_acpt.suspended) pwm_acpt = true;
          break;
        case SENSORS:
          if (sus_acpt.suspended) sensor_acpt = true;
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

  if (sd_card_inited) {
    uninit_sd_card();
  }
  
  static const app_usbd_config_t usbd_config = {
      .ev_state_proc = usbd_user_ev_handler
  };

  app_usbd_serial_num_generate();

  ret = app_usbd_init(&usbd_config);
  APP_ERROR_CHECK(ret);

  usb_initalized = true;

  if (cdc_acm) {
    cdc_acm_active = true;
    app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
    ret = app_usbd_class_append(class_cdc_acm);
    APP_ERROR_CHECK(ret);
  }
  else {
    cdc_acm_active = false;
  }
  
  if (msc) {
    msc_active = true;
    app_usbd_class_inst_t const * class_inst_msc = app_usbd_msc_class_inst_get(&m_app_msc);
    ret = app_usbd_class_append(class_inst_msc);
    APP_ERROR_CHECK(ret);
  }
  else {
    msc_active = false;
  }

  if (USBD_POWER_DETECTION)
  {
      ret = app_usbd_power_events_enable();
      APP_ERROR_CHECK(ret);
  }
  else
  {
      printf("No USB power detection enabled\r\nStarting USB now");

      app_usbd_enable();
      app_usbd_start();

      m_usb_connected = true;
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

void composite_usb_task(void * pvParameters) {
  BaseType_t xReturned;
  usb_suspend_over_t sus_over;
  bool batt_over = false, heater_over = false, pwm_over = false, sensor_over = false;

  vTaskDelay(pdMS_TO_TICKS(100));

  // Start the USB
  if (!usb_started) {
    start_usb(true, false);
    command = USB_CDC_ACM;
  }

  for (;;) {
    
    while (app_usbd_event_queue_process()) {
        // Nothing to do 
    }

    if (msc_active && usb_detected && !usb_suspended_tasks && !usb_done_config && (command == USB_MSC || command == USB_MSC_CDC_ACM)) {
      usb_suspend_conflicting_tasks();
      set_led1_blue_slow_blink();
      batt_over = false; heater_over = false; pwm_over = false; sensor_over = false;
    }
    
    if (usb_suspended_tasks) {
      if (uxQueueMessagesWaiting(usb_usbWaitOverQueue) > 0) {
        xReturned = xQueueReceive(usb_usbWaitOverQueue, &sus_over, 0);
        if (xReturned != pdPASS) {
          printf("USB: Unable to receive usb wait over from usb_usbWaitOverQueue\n");
        }
        switch(sus_over.task) {
        case BATTERY:
          if (sus_over.over) batt_over = true;
          printf("USB: battery task suspension over.\n");
          break;
        case HEATER:
          if (sus_over.over) heater_over = true;
          printf("USB: heater task suspension over.\n");
          break;
        case PWM:
          if (sus_over.over) pwm_over = true;
          printf("USB: pwm task suspension over.\n");
          break;
        case SENSORS:
          if (sus_over.over) sensor_over = true;
          printf("USB: sensor task suspension over.\n");
          break;
        default:
          break;
        }
      }

      if (batt_over && heater_over && pwm_over && sensor_over) {
        usb_suspended_tasks = false;
        usb_done_config = true;
        set_led1_green_breathe();
      }
    }
    else {
      vTaskDelay(50);
    }
  }
}

void usb_task(void * pvParameters) {
  BaseType_t xReturned;
  bool conn_state_req = false;
  tasks_t usb_task = USB;
  bool cont = false;
  volatile int hi;
  bool usb_updated = false;
  usb_command_t last_command;

  // Set connection state to not charging
  connection_state = NOT_CHARGING;

  // Set the first event to make sure that USB queue is processed after it is started
  for (;;) {
    // Check for usb connection status request
    if (xQueueReceive(usb_connectionReqQueue, &conn_state_req, 0) == pdPASS) {
      // Respond to connection status request
      xReturned = xQueueSend(main_usbConnRecvQueue, &usb_detected, 0);
      if (xReturned != pdPASS) {
        printf("USB_TASK: Unable to send usb connection status to main_usbConnRecvQueue. \n");
      }
    }
    // Save Last Command
    last_command = command;
    // Check the USB queue for an update message
    if (xReturned = xQueueReceive(usb_stateChangeQueue, &command, 0) != pdPASS) {
      vTaskDelay(USB_TASK_DELAY);
      continue;
    }

    // Handle the new command if there is one
    switch(command) {
      case USB_DISABLED: 
      {
        // Do Nothing
        break;
      }
      case USB_CDC_ACM:
      {
        if (last_command == USB_CDC_ACM) {
          break;
        }

        break;
      }
      case USB_MSC: 
      {
        if (last_command == USB_MSC) {
          break;
        }

        break;
      }
      case USB_MSC_CDC_ACM: 
      {
        if (last_command == USB_MSC_CDC_ACM) 
        {
          break;
        }
        // Stop The USB
        usbd_user_ev_handler(APP_USBD_EVT_POWER_REMOVED);
        app_usbd_disable();
        //app_usbd_class_remove_all();
        app_usbd_uninit();
        //usb_detected = true;
        start_usb(true, true);
        usbd_user_ev_handler(APP_USBD_EVT_POWER_DETECTED);
        break;
      }
      default:
      {
        app_usbd_stop();
        break;
      }
    }
  }
}