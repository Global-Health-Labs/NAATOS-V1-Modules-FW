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

// Main Loop 
usb_message_t recv_msg;
charge_state_t connection_state;
main_state_t main_state;

// USB Suspended Vars
bool usb_started = false;
bool usb_suspended_tasks = false;

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
            //bsp_board_led_off(LED_CDC_ACM_OPEN);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
           // bsp_board_led_invert(LED_CDC_ACM_TX);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            ret_code_t ret;
            NRF_LOG_INFO("Bytes waiting: %d", app_usbd_cdc_acm_bytes_stored(p_cdc_acm));
            do
            {
                //Get amount of data transfered
                size_t size = app_usbd_cdc_acm_rx_size(p_cdc_acm);
                NRF_LOG_INFO("RX: size: %lu char: %c", size, m_rx_buffer[0]);

                // Fetch data until internal buffer is empty 
                ret = app_usbd_cdc_acm_read(&m_app_cdc_acm,
                                            m_rx_buffer,
                                            READ_SIZE);
            } while (ret == NRF_SUCCESS);

            //bsp_board_led_invert(LED_CDC_ACM_RX);
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
            app_usbd_disable();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            printf("USB: Power detected\n");

            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            printf("USB: Power removed\n");
            app_usbd_stop();
            break;
        case APP_USBD_EVT_POWER_READY:
            printf("USB: Ready\n");
            app_usbd_start();
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

static void power_usb_event_handler(nrf_drv_power_usb_evt_t event)
{
    switch(event)
    {
        case NRF_DRV_POWER_USB_EVT_DETECTED:
            printf("USB power detected\r\n");

            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case NRF_DRV_POWER_USB_EVT_REMOVED:
            printf("USB power removed\r\n");
            m_usb_connected = false;
            break;
        case NRF_DRV_POWER_USB_EVT_READY:
            printf("USB ready\r\n");
            m_usb_connected = true;
            break;
        default:
            ASSERT(false);
    }
}

void usb_suspend_conflicting_tasks(void) {
  BaseType_t xReturned;
  bool batt_acpt = false, heater_acpt = false, pwm_acpt = false, sensor_acpt = false;
  usb_suspend_req_t sus_req = {
    .suspend = true,
  };
  usb_suspend_acpt_t sus_acpt;

  /* Send suspend requests to conflicting tasks */
  // Send to Battery
  xReturned = xQueueSend(battery_usbWaitQueue, &sus_req, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send usb suspend request to battery_usbWaitQueue.\n");
  }
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
  // Send to Sensors task
  xReturned = xQueueSend(sensor_usbWaitQueue, &sus_req, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send usb suspend request to sensor_usbWaitQueue.\n");
  }

  /* Receive back suspend request acceptances */
  while(!batt_acpt || !heater_acpt || !pwm_acpt || !sensor_acpt) {
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

void start_usb(void) {
   ret_code_t ret;
    static const app_usbd_config_t usbd_config = {
        .ev_state_proc = usbd_user_ev_handler
    };

    usb_suspend_conflicting_tasks();
    
    if (sd_card_inited) {
      uninit_sd_card();
    }

    app_usbd_serial_num_generate();

    ret = app_usbd_init(&usbd_config);
    APP_ERROR_CHECK(ret);

    app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
    ret = app_usbd_class_append(class_cdc_acm);
    APP_ERROR_CHECK(ret);

    app_usbd_class_inst_t const * class_inst_msc = app_usbd_msc_class_inst_get(&m_app_msc);
    ret = app_usbd_class_append(class_inst_msc);
    APP_ERROR_CHECK(ret);

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

void composite_usb_task(void * pvParameters) {
  BaseType_t xReturned;
  usb_suspend_over_t sus_over;
  bool batt_over = false, heater_over = false, pwm_over = false, sensor_over = false;

  vTaskDelay(pdMS_TO_TICKS(1000));

  // Start the USB
  if (!usb_started) {
    start_usb();
  }

  for (;;) {
    
    while (app_usbd_event_queue_process()) {
        // Nothing to do 
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
      }
    }
    else {
      vTaskDelay(100);
    }
  }
}

void usb_task(void * pvParameters) {
  BaseType_t xReturned;
  bool conn_state_updated = false, main_state_updated = false;

  // Set Current State to Standby
  main_state = STANDBY;
  // Set connection state to not charging
  connection_state = NOT_CHARGING;

  // Set the first event to make sure that USB queue is processed after it is started
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