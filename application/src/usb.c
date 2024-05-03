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

#include "app_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_msc.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"
#include "app_error.h"
#include "app_timer.h"

#include "nrf_cli.h"
#include "nrf_cli_uart.h"

/* ***** Defines ***** */

#define ENDLINE_STRING "\r\n"

// The maximum delay inside the USB task to wait for an event.
#define USB_THREAD_MAX_BLOCK_TIME portMAX_DELAY

// Enable power USB detection
// Configure if example supports USB port connection
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif

// SD card enable/disable
#define USE_SD_CARD       1

// CDC ACM Defines
#define CDC_ACM_COMM_INTERFACE  0
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN2

#define CDC_ACM_DATA_INTERFACE  1
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT1

#define READ_SIZE 1

// Endpoint list passed to APP_USBD_MSC_GLOBAL_DEF
#define ENDPOINT_LIST() APP_USBD_MSC_ENDPOINT_LIST(3, 3)

// Mass storage class work buffer size
#define MSC_WORKBUFFER_SIZE (1024)

/* ***** Handlers ***** */

// Virtual COM Port class user event handler
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event);
// Mass storage class user event handler
static void msc_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                app_usbd_msc_user_event_t     event);

/* ***** Instances ***** */

// CDC_ACM class instance
/*
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            cdc_acm_user_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);
*/
// Mass storage class instance
APP_USBD_MSC_GLOBAL_DEF(m_app_msc,
                        0,
                        msc_user_ev_handler,
                        ENDPOINT_LIST(),
                        BLOCKDEV_LIST(),
                        MSC_WORKBUFFER_SIZE);


/* ***** Variables ***** */

// CDC ACM Variables
static char m_rx_buffer[READ_SIZE];
static char m_tx_buffer[NRF_DRV_USBD_EPSIZE];
static bool m_send_flag = 0;
// USB connection status
static bool m_usb_connected = false;
// Queue Handles
xQueueHandle usb_stateChangeQueue;
// Main Loop 
usb_message_t recv_msg;
charge_state_t connection_state;
main_state_t main_state;

bool usb_started = false;

// User event handler -- app_usbd_cdc_acm_user_ev_handler_t 
/*
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event)
{
    app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

    switch (event)
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            //bsp_board_led_on(LED_CDC_ACM_OPEN);

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
            //bsp_board_led_invert(LED_CDC_ACM_TX);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            ret_code_t ret;
            printf("Bytes waiting: %d", app_usbd_cdc_acm_bytes_stored(p_cdc_acm));
            do
            {
                //Get amount of data transfered
                size_t size = app_usbd_cdc_acm_rx_size(p_cdc_acm);
                printf("RX: size: %lu char: %c", size, m_rx_buffer[0]);

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
*/

// USBD library specific event handler.
/*
static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_DRV_SUSPEND:
            //bsp_board_led_off(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_DRV_RESUME:
            //bsp_board_led_on(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            printf("USB power detected\n");

            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            printf("USB power removed");
            app_usbd_stop();
            break;
        case APP_USBD_EVT_POWER_READY:
            printf("USB ready\n");
            app_usbd_start();
            usb_started = true;
            break;
        default:
            break;
    }
}
*/

static void msc_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                app_usbd_msc_user_event_t     event)
{
    UNUSED_PARAMETER(p_inst);
    UNUSED_PARAMETER(event);
}


void usb_new_event_isr_handler(app_usbd_internal_evt_t const * const p_event, bool queued) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    UNUSED_PARAMETER(p_event);
    UNUSED_PARAMETER(queued);
    /* Release the semaphore */
    vTaskNotifyGiveFromISR(usbTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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
/*
void usb_start(void) {
  ret_code_t ret;
  static const app_usbd_config_t usbd_config = {
      //.ev_isr_handler = usb_new_event_isr_handler,
      .ev_state_proc = usbd_user_ev_handler
  };

  app_usbd_serial_num_generate();
  
  ret = app_usbd_init(&usbd_config);
  APP_ERROR_CHECK(ret);
  
  app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
  ret = app_usbd_class_append(class_cdc_acm);
  APP_ERROR_CHECK(ret);
  
  //app_usbd_class_inst_t const * class_inst_msc = app_usbd_msc_class_inst_get(&m_app_msc);
  //ret = app_usbd_class_append(class_inst_msc);
  //APP_ERROR_CHECK(ret);

  if (USBD_POWER_DETECTION)
  {
      ret = app_usbd_power_events_enable();
      APP_ERROR_CHECK(ret);
  }
  else
  {
      NRF_LOG_INFO("No USB power detection enabled\r\nStarting USB now");

      app_usbd_enable();
      app_usbd_start();
  }

   while (app_usbd_event_queue_process())
   {
        // Nothing to do 
   }
}
*/

void usb_task(void * pvParameters) {
  BaseType_t xReturned;
  bool conn_state_updated = false, main_state_updated = false;

  // Set Current State to Standby
  main_state = STANDBY;
  // Set connection state to not charging
  connection_state = NOT_CHARGING;

  // Set the first event to make sure that USB queue is processed after it is started
  //UNUSED_RETURN_VALUE(xTaskNotifyGive(xTaskGetCurrentTaskHandle()));
  for (;;) {

    /* Waiting for event */
    //UNUSED_RETURN_VALUE(ulTaskNotifyTake(pdTRUE, USB_THREAD_MAX_BLOCK_TIME));
    while (app_usbd_event_queue_process())
    {
      
    }

    //ret = app_usbd_cdc_acm_write(&m_app_cdc_acm, "HELLO", 6);
    

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