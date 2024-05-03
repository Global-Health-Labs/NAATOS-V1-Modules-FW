#pragma once

#include "naatos_queues.h"
#include "naatos_config.h"
#include "states.h"
#include "task.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "sd_card.h"

APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            cdc_acm_user_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);

extern bool usb_started;

void usb_start(void);
void usb_task(void * pvParameters);