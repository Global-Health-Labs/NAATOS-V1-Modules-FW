#include "naatos_config_file.h"
#include "naatos_config.h"

#if defined(SAMPLE_PREP_BOARD)
/****************************************************************************************************************
 * SAMPLE PREP MODULE
 ****************************************************************************************************************/
// Global Config
naatos_kv_table_entry_t KV_TABLE_GLOBAL[] = {
  // name       ,       dtype,                dataptr (pointer to config struct field),       default value as a string
  { "sample_rate"                     , NAATOS_KV_DT_FLOAT  , (void *) &(config.sample_rate),           "0.20"},
  { "logging_rate"                    , NAATOS_KV_DT_FLOAT  , (void *) &(config.logging_rate),          "1.00"},
  { "low_power_threshold"             , NAATOS_KV_DT_UINT16 , (void *) &(config.low_power_threshold),   "46"},
  { "recovery_power_threshold"        , NAATOS_KV_DT_UINT16 , (void *) &(config.recovery_power_thresh), "47"},
  { "sample_valid_timeout_s"          , NAATOS_KV_DT_FLOAT  , (void *) &(config.sample_valid_timeout_s),"3600.00"},
  { "alert_timeout_time_s"            , NAATOS_KV_DT_FLOAT  , (void *) &(config.alert_timeout_time_s),  "3.00"},
  { "debug_to_com_en"                 , NAATOS_KV_DT_BOOLS  , (void *) &(config.debug_to_com_en),       "true"},
  { "min_run_zone_temp_en"            , NAATOS_KV_DT_BOOLS  , (void *) &(config.min_run_zone_temp_en),  "false"},
  { "min_run_zone_temp"               , NAATOS_KV_DT_FLOAT  , (void *) &(config.min_run_zone_temp),     "80.00"},
  { "do_automatic_runs"               , NAATOS_KV_DT_BOOLS  , (void *) &(config.do_automatic_runs),     "false"},

  { "heater_max_temp"                 , NAATOS_KV_DT_FLOAT  , (void *) &(config.heater_max_temp),       "120.0"},
  { "max_heater_pid_pwm"              , NAATOS_KV_DT_FLOAT  , (void *) &(config.max_heater_pid_pwm),    "100"},
  { "switch_motor_ccw_cw"             , NAATOS_KV_DT_BOOLS  , (void *) &(config.switch_motor_ccw_cw),   "true"},
  { "hal_sensor_thresh"               , NAATOS_KV_DT_FLOAT  , (void *) &(config.hal_sensor_thresh),     "0.30"},
  { "motor_stall_percent"             , NAATOS_KV_DT_INT    , (void *) &(config.motor_stall_percent),   "20"},
  { "motor_stall_pwm"                 , NAATOS_KV_DT_FLOAT  , (void *) &(config.motor_stall_pwm),       "61"},
  { "motor_stall_en"                  , NAATOS_KV_DT_BOOLS  , (void *) &(config.motor_stall_en),        "true"},
  
  { "mmddyy"                          , NAATOS_KV_DT_INT    , (void *) &(config.mmddyy),                "100124"},
  { "hhmmss"                          , NAATOS_KV_DT_INT    , (void *) &(config.hhmmss),                "120000"},
  { "set_time_date"                   , NAATOS_KV_DT_BOOLS  , (void *) &(config.set_date_time),         "false"}

};
const naatos_kv_table_entry_t* KV_TABLE_GLOBAL_PTR = KV_TABLE_GLOBAL;
const uint8_t KV_TABLE_GLOBAL_SIZE = sizeof(KV_TABLE_GLOBAL)/sizeof(KV_TABLE_GLOBAL[0]);

// Cycle Config
naatos_kv_table_entry_cycle_t KV_TABLE_CYCLE[] = {
  // name       ,       dtype,                dataptr (pointer to config struct field) 
  { "cycle_run_time_s"                , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.cycle_run_time_s) },
  { "cycle_delay_time_s"              , NAATOS_KV_DT_UINT16 , (void *) &(cycle_cfg_single.cycle_delay_time) },
  { "ramp_to_temp_before_start_cycle" , NAATOS_KV_DT_BOOLS  , (void *) &(cycle_cfg_single.ramp_to_temp_before_start_cycle) },
  { "ramp_to_temp_timeout"            , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.ramp_to_temp_timeout) },
  { "heater_setpoint"                 , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.heater_setpoint) },
  { "motor_setpoint"                  , NAATOS_KV_DT_UINT16 , (void *) &(cycle_cfg_single.motor_setpoint) },
  { "run_heater_enable"               , NAATOS_KV_DT_BOOLS  , (void *) &(cycle_cfg_single.run_heater) },
  { "run_motor_enable"                , NAATOS_KV_DT_BOOLS  , (void *) &(cycle_cfg_single.run_motor) },
  { "heater_kp"                       , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.heater_kp) },
  { "heater_ki"                       , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.heater_ki) },
  { "heater_kd"                       , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.heater_kd) },
  { "motor_kp"                        , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.motor_kp) },
  { "motor_ki"                        , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.motor_ki) },
  { "motor_kd"                        , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.motor_kd) }
};
const naatos_kv_table_entry_cycle_t* KV_TABLE_CYCLE_PTR = KV_TABLE_CYCLE;
const uint8_t KV_TABLE_CYCLE_SIZE = sizeof(KV_TABLE_CYCLE)/sizeof(KV_TABLE_CYCLE[0]);



#elif defined(POWER_MODULE_BOARD)
/****************************************************************************************************************
 * POWER MODULE
 ****************************************************************************************************************/
// Global Config
naatos_kv_table_entry_t KV_TABLE_GLOBAL[] = {
  // name       ,       dtype,                dataptr (pointer to config struct field),       default value as a string
  { "sample_rate"                     , NAATOS_KV_DT_FLOAT  , (void *) &(config.sample_rate),           "0.20"},
  { "logging_rate"                    , NAATOS_KV_DT_FLOAT  , (void *) &(config.logging_rate),          "1.00"},
  { "low_power_threshold"             , NAATOS_KV_DT_UINT16 , (void *) &(config.low_power_threshold),   "46"},
  { "recovery_power_threshold"        , NAATOS_KV_DT_UINT16 , (void *) &(config.recovery_power_thresh), "47"},
  { "sound_volume_complete"           , NAATOS_KV_DT_UINT16 , (void *) &(config.sound_volume_complete), "50"},
  { "sound_volume_start"              , NAATOS_KV_DT_UINT16 , (void *) &(config.sound_volume_start),    "50"},
  { "sound_volume_abort"              , NAATOS_KV_DT_UINT16 , (void *) &(config.sound_volume_abort),    "50"},
  { "sample_valid_timeout_s"          , NAATOS_KV_DT_FLOAT  , (void *) &(config.sample_valid_timeout_s),"3600.00"},
  { "alert_timeout_time_s"            , NAATOS_KV_DT_FLOAT  , (void *) &(config.alert_timeout_time_s),  "3.00"},
  { "debug_to_com_en"                 , NAATOS_KV_DT_BOOLS  , (void *) &(config.debug_to_com_en),       "true"},
  { "min_run_zone_temp_en"            , NAATOS_KV_DT_BOOLS  , (void *) &(config.min_run_zone_temp_en),  "false"},
  { "min_run_zone_temp"               , NAATOS_KV_DT_FLOAT  , (void *) &(config.min_run_zone_temp),     "80.00"},
  { "do_automatic_runs"               , NAATOS_KV_DT_BOOLS  , (void *) &(config.do_automatic_runs),     "false"},

  { "optical_distace"                 , NAATOS_KV_DT_UINT16 , (void *) &(config.optical_distance),      "800"},
  { "max_amp_pid_pwm"                 , NAATOS_KV_DT_FLOAT  , (void *) &(config.max_amp_pid_pwm),       "100"},
  { "max_valve_pid_pwm"               , NAATOS_KV_DT_FLOAT  , (void *) &(config.max_valve_pid_pwm),     "100"},
  { "valve_max_temp"                  , NAATOS_KV_DT_FLOAT  , (void *) &(config.valve_max_temp),        "130.0"},
  { "amp_max_temp"                    , NAATOS_KV_DT_FLOAT  , (void *) &(config.amp_max_temp),          "120.0"},
  
  { "mmddyy"                          , NAATOS_KV_DT_INT    , (void *) &(config.mmddyy),                "100124"},
  { "hhmmss"                          , NAATOS_KV_DT_INT    , (void *) &(config.hhmmss),                "120000"},
  { "set_time_date"                   , NAATOS_KV_DT_BOOLS  , (void *) &(config.set_date_time),         "false"}
};
const naatos_kv_table_entry_t* KV_TABLE_GLOBAL_PTR = KV_TABLE_GLOBAL;
const uint8_t KV_TABLE_GLOBAL_SIZE = sizeof(KV_TABLE_GLOBAL)/sizeof(KV_TABLE_GLOBAL[0]);

// Cycle Config
naatos_kv_table_entry_cycle_t KV_TABLE_CYCLE[] = {
  // name       ,       dtype,                dataptr (pointer to config struct field) 
  { "cycle_run_time_s"                , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.cycle_run_time_s) },
  { "cycle_delay_time_s"              , NAATOS_KV_DT_UINT16 , (void *) &(cycle_cfg_single.cycle_delay_time) },
  { "ramp_to_temp_before_start_cycle" , NAATOS_KV_DT_BOOLS  , (void *) &(cycle_cfg_single.ramp_to_temp_before_start_cycle) },
  { "ramp_to_temp_timeout"            , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.ramp_to_temp_timeout) },
  { "amp_setpoint"                    , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.amp_setpoint) },
  { "valve_setpoint"                  , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.valve_setpoint) },
  { "run_amp_enable"                  , NAATOS_KV_DT_BOOLS  , (void *) &(cycle_cfg_single.run_amp) },
  { "run_valve_enable"                , NAATOS_KV_DT_BOOLS  , (void *) &(cycle_cfg_single.run_valve) },
  { "amp_kp"                          , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.amp_kp) },
  { "amp_ki"                          , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.amp_ki) },
  { "amp_kd"                          , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.amp_kd) },
  { "valve_kp"                        , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.valve_kp) },
  { "valve_ki"                        , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.valve_ki) },
  { "valve_kd"                        , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.valve_kd) }
};
const naatos_kv_table_entry_cycle_t* KV_TABLE_CYCLE_PTR = KV_TABLE_CYCLE;
const uint8_t KV_TABLE_CYCLE_SIZE = sizeof(KV_TABLE_CYCLE)/sizeof(KV_TABLE_CYCLE[0]);

#endif

