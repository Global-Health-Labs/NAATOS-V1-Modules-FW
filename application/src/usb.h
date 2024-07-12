#pragma once

#include "FreeRTOS.h"
#include "led.h"
#include "naatos_config.h"
#include "naatos_queues.h"
#include "queue.h"
#include "sd_card.h"
#include "states.h"
#include "task.h"

#include "app_error.h"
#include "app_timer.h"
#include "app_usbd.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_core.h"
#include "app_usbd_msc.h"
#include "app_usbd_serial_num.h"
#include "app_usbd_string_desc.h"

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

#define USB_TASK_DELAY pdMS_TO_TICKS(10)
#define USB__TASK_DELAY pdMS_TO_TICKS(100)

// SD card enable/disable
#define USE_SD_CARD 1

// CDC ACM Defines
#define CDC_ACM_COMM_INTERFACE 1
#define CDC_ACM_COMM_EPIN NRF_DRV_USBD_EPIN2

#define CDC_ACM_DATA_INTERFACE 2
#define CDC_ACM_DATA_EPIN NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT NRF_DRV_USBD_EPOUT1

#define READ_SIZE 1

// Endpoint list passed to APP_USBD_MSC_GLOBAL_DEF
#define ENDPOINT_LIST() APP_USBD_MSC_ENDPOINT_LIST(3, 3)

// Mass storage class work buffer size
#define MSC_WORKBUFFER_SIZE (2048)

/* ***** Handlers ***** */
void cdc_acm_user_ev_handler(app_usbd_class_inst_t const *p_inst,
    app_usbd_cdc_acm_user_event_t event);

void usbd_user_ev_handler(app_usbd_event_type_t event);

void msc_user_ev_handler(app_usbd_class_inst_t const *p_inst,
    app_usbd_msc_user_event_t event);

extern bool usb_started;

static char m_rx_buffer[READ_SIZE];
static char m_tx_buffer[NRF_DRV_USBD_EPSIZE];
static bool m_send_flag = 0;

void setup_uart_semaphore(void);
void naatosPrintf(const char * msg, int len);


void usb_task(void *pvParameters);
void composite_usb_task(void *pvParameters);
void write_to_com(const char * msg, int len);
void start_usb(bool cdc_acm, bool msc);
void restart_usb_only_cdc_acm(void);
void usb_suspend_conflicting_tasks(void);