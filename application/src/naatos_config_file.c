#include "naatos_config_file.h"
#include <string.h>
#include "naatos_config.h"

#ifndef SAMPLE_PREP_BOARD
const uint8_t KV_TABLE_GLOBAL_SIZE=19;
const naatos_kv_table_entry_t KV_TABLE_GLOBAL[KV_TABLE_GLOBAL_SIZE] = {
  // name       ,       dtype,                dataptr (pointer to config struct field)
  { "sample_rate"                     , NAATOS_KV_DT_FLOAT  , (void *) &(config.sample_rate) },
  { "logging_rate"                    , NAATOS_KV_DT_FLOAT  , (void *) &(config.logging_rate) },
  { "low_power_threshold"             , NAATOS_KV_DT_UINT16 , (void *) &(config.low_power_threshold) },
  { "recovery_power_thresh"           , NAATOS_KV_DT_UINT16 , (void *) &(config.recovery_power_thresh) },
  { "sample_valid_timeout_s"          , NAATOS_KV_DT_FLOAT  , (void *) &(config.sample_valid_timeout_s) },
  { "alert_timeout_time_s"            , NAATOS_KV_DT_FLOAT  , (void *) &(config.alert_timeout_time_s) },
  { "debug_to_com_en"                 , NAATOS_KV_DT_BOOLS  , (void *) &(config.debug_to_com_en) },
  { "min_run_zone_temp"               , NAATOS_KV_DT_FLOAT  , (void *) &(config.min_run_zone_temp) },
  { "min_run_zone_temp_en"            , NAATOS_KV_DT_BOOLS  , (void *) &(config.min_run_zone_temp_en) },

  { "heater_max_temp"                 , NAATOS_KV_DT_FLOAT  , (void *) &(config.heater_max_temp) },
  { "max_heater_pid_pwm"              , NAATOS_KV_DT_FLOAT  , (void *) &(config.max_heater_pid_pwm) },
  { "switch_motor_ccw_cw"             , NAATOS_KV_DT_BOOLS  , (void *) &(config.switch_motor_ccw_cw) },
  { "hal_sensor_thresh"               , NAATOS_KV_DT_FLOAT  , (void *) &(config.hal_sensor_thresh) },
  { "motor_stall_percent"             , NAATOS_KV_DT_INT    , (void *) &(config.motor_stall_percent) },
  { "motor_stall_pwm"                 , NAATOS_KV_DT_FLOAT  , (void *) &(config.motor_stall_pwm) },
  { "motor_stall_en"                  , NAATOS_KV_DT_BOOLS  , (void *) &(config.motor_stall_en) },
  
  { "mmddyy"                          , NAATOS_KV_DT_INT    , (void *) &(config.mmddyy) },
  { "hhmmss"                          , NAATOS_KV_DT_INT    , (void *) &(config.hhmmss) },
  { "set_date_time"                   , NAATOS_KV_DT_BOOLS  , (void *) &(config.set_date_time) }
};
//const naatos_kv_table_entry_t KV_TABLE_CYCLE[KV_TABLE_GLOBAL_SIZE] = {
//  // name   , dtype,    dataptr (pointer to config struct field)
//  { "cycle_run_time_s"                , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_delay_time"                , NAATOS_KV_DT_UINT16 , (void *) &(config.cycle_delay_time) },
//  { "ramp_to_temp_before_start_cycle" , NAATOS_KV_DT_FLOAT  , (void *) &(config.ramp_to_temp_before_start_cycle) },
//  { "ramp_to_temp_timeout"            , NAATOS_KV_DT_FLOAT  , (void *) &(config.ramp_to_temp_timeout) },

//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//  { "cycle_run_time_s"       , NAATOS_KV_DT_FLOAT  , (void *) &(config.cycle_run_time_s) },
//}

#else
#endif

float get_config_value_by_key_float(const char* key) {
  for(uint8_t i; i<KV_TABLE_GLOBAL_SIZE; i++) {
    if( strcmp(key, KV_TABLE_GLOBAL[i].name)==0 ) {
      //found;
      if( KV_TABLE_GLOBAL[i].dtype == NAATOS_KV_DT_FLOAT )  {
        // matches dtype
        return *((float *) (KV_TABLE_GLOBAL[i].dataptr));
      } else{
        return -666.0;
      }
    }
  }
  return -666.0;
}

//float get_config_value_by_key_float(const char* key) {
//  for(uint8_t i; i<KV_TABLE_GLOBAL_SIZE; i++) {
//    if( stricmp(key, KV_TABLE_GLOBAL[i].name)==0 ) {
//      //found;
//      if( KV_TABLE_GLOBAL[i].dtype == NAATOS_KV_DT_FLOAT )  {
//        // matches dtype
//        return *((float *) (KV_TABLE_GLOBAL[i].dataptr));
//      } else{
//        return -666.0;
//      }
//    }
//  }
//  return -666.0;
//}