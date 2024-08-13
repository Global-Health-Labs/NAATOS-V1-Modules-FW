#pragma once
#include "naatos_config.h"

// Zone Request Constants
extern const HeaterRxQueueMsg_t run_amplification_zone;
extern const HeaterRxQueueMsg_t run_valve_zone;
extern const HeaterRxQueueMsg_t stop_amplification_zone;
extern const HeaterRxQueueMsg_t stop_valve_zone;

// Log Event Constants
extern const log_data_message_t new_log_msg; // starts a new log file
extern const log_event_t start_event;
extern const log_data_message_t start_log_msg;
extern const log_event_t stop_event;
extern const log_data_message_t stop_log_msg;
extern const log_event_t interrupt_hal_event;
extern const log_data_message_t interrupt_hal_log_msg;
extern const log_event_t temps_not_stabalized_event;
extern const log_data_message_t temps_not_stablized_msg;
extern const log_event_t recovery_batt_event;
extern const log_data_message_t recovery_batt_msg;
extern const log_event_t over_temp_event;
extern const log_data_message_t over_temp_msg;
extern const log_event_t interrupt_opt_event;
extern const log_data_message_t interrupt_opt_log_msg;
extern const log_event_t valve_start_event;
extern const log_data_message_t valve_start_log_msg;
extern const log_event_t valve_stop_event;
extern const log_data_message_t valve_stop_log_msg;
extern const log_event_t amplification_start_event;
extern const log_data_message_t amplification_start_log_msg;
extern const log_event_t amplification_stop_event;
extern const log_data_message_t amplification_stop_log_msg;
extern const log_data_message_t ramp_to_temp_complete_log_msg;
extern const log_data_message_t ramp_to_temp_timeout_log_msg;

extern const log_event_t setpoint_timeout_event;
extern const log_data_message_t setpoint_timeout_log_msg;

extern const BatteryRxQueueMsg_t batt_req;
extern const BatteryRxQueueMsg_t batt_req_log;

// USB Main State Update Constants
extern const usb_message_t standby_update;
extern const usb_message_t running_update;