#pragma once

#include "../calendar_rtc.h"
#include "../naatos_config.h"
#include "../naatos_queues.h"
#include "../storage/naatos_storage.h"
#include "../fuel.h"

typedef union {
  struct {
    bool file_create_error : 1;
    bool unable_to_write_last_log_line_event : 1;
    bool unable_to_write_last_log_line_data : 1;
    bool _unused3    : 1;

    bool _unused4    : 1;
    bool _unused5    : 1;
    bool _unused6    : 1;
    bool _unused7    : 1;
  } bit;
  uint8_t reg;
} logger_cumulative_errors_t;
extern logger_cumulative_errors_t logger_cumulative_errors;

void set_log_level();
void send_start_log_message();
bool send_event_log_message_struct(const log_data_message_t *message);
void send_event_log_message(event_t eventType, char *message);
void send_data_log_message();
void send_debug_log_message(char *message);

void logger_task(void *pvParameters);