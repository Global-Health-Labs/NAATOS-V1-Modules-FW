#pragma once

#include "../calendar_rtc.h"
#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "../storage/naatos_storage.h"
#include "../fuel.h""

void set_log_level();
void send_start_log_message();
bool send_event_log_message_struct(const log_data_message_t *message);
void send_event_log_message(event_t eventType, char *message);
void send_data_log_message();
void send_debug_log_message(char *message);

void logger_task(void *pvParameters);