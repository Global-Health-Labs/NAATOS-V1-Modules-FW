#include "naatos_config_file.h"
#include "naatos_config.h"

static char tmp[80];
static uint8_t tmp_sz = 80;

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
  { "motor_stall_pwm"                 , NAATOS_KV_DT_FLOAT  , (void *) &(config.motor_stall_pwm),       "65"},
  { "motor_stall_en"                  , NAATOS_KV_DT_BOOLS  , (void *) &(config.motor_stall_en),        "true"},
  
  { "canary"                          , NAATOS_KV_DT_UINT16 , (void *) &(config.canary),                "747"},
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
  { "yellow_grace_period_s"           , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.double_yellow_grace_period_s) },

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
  { "sound_volume_graceperiod"        , NAATOS_KV_DT_UINT16 , (void *) &(config.sound_volume_graceperiod),"50"},
  { "sound_volume_abort"              , NAATOS_KV_DT_UINT16 , (void *) &(config.sound_volume_abort),    "50"},
  { "sample_valid_timeout_s"          , NAATOS_KV_DT_FLOAT  , (void *) &(config.sample_valid_timeout_s),"3600.00"},
  { "alert_timeout_time_s"            , NAATOS_KV_DT_FLOAT  , (void *) &(config.alert_timeout_time_s),  "3.00"},
  { "debug_to_com_en"                 , NAATOS_KV_DT_BOOLS  , (void *) &(config.debug_to_com_en),       "true"},
  { "min_run_zone_temp_en"            , NAATOS_KV_DT_BOOLS  , (void *) &(config.min_run_zone_temp_en),  "true"},
  { "min_run_zone_temp"               , NAATOS_KV_DT_FLOAT  , (void *) &(config.min_run_zone_temp),     "60.00"},
  { "do_automatic_runs"               , NAATOS_KV_DT_BOOLS  , (void *) &(config.do_automatic_runs),     "false"},

  { "optical_distance"                , NAATOS_KV_DT_UINT16 , (void *) &(config.optical_distance),      "800"},
  { "max_amp_pid_pwm"                 , NAATOS_KV_DT_FLOAT  , (void *) &(config.max_amp_pid_pwm),       "100"},
  { "max_valve_pid_pwm"               , NAATOS_KV_DT_FLOAT  , (void *) &(config.max_valve_pid_pwm),     "100"},
  { "valve_max_temp"                  , NAATOS_KV_DT_FLOAT  , (void *) &(config.valve_max_temp),        "130.0"},
  { "amp_max_temp"                    , NAATOS_KV_DT_FLOAT  , (void *) &(config.amp_max_temp),          "120.0"},
  { "ledtop0frnt1"                    , NAATOS_KV_DT_UINT16 , (void *) &(config.ledtop0frnt1),          "0"},
  
  { "canary"                          , NAATOS_KV_DT_UINT16 , (void *) &(config.canary),                "747"},
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
  { "yellow_grace_period_s"           , NAATOS_KV_DT_FLOAT  , (void *) &(cycle_cfg_single.double_yellow_grace_period_s) },

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

bool naatos_config_assignParameterUsingKeyValueTable_given_key_and_value_strings(const naatos_kv_table_entry_t * KVTABLE, const uint8_t TABLE_SIZE, const char* keystr, const char* valstr) {
  // Search the key-value table for a matching key
  const bool _printdebugs = false;
  bool matched;

  if(_printdebugs)  {
    sprintf(tmp,"key = \"%s\" in file, checking key-value table...",keystr);
    send_debug_log_message(tmp);
  }
  matched = false;

  // Loop through the key-value table for a matching key
  for(uint8_t i=0; i<TABLE_SIZE; i++){
    // pick item from table
    const naatos_kv_table_entry_t * item = &(KVTABLE[i]);

    // check for match of the key in the table
    if(strcasecmp(keystr,item->name) == 0) {
      if(_printdebugs)  {
        sprintf(tmp,"  matched key = %s -> file value str = \"%s\"",item->name,valstr);
        send_debug_log_message(tmp);
      }

      // we have a match! now
      // parse the value string
      switch(item->dtype) {
        case NAATOS_KV_DT_FLOAT:
          *((float*) item->dataptr) = parse_double(valstr,parse_double(item->defaultcfgstr,0));
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_FLOAT. parsed = %g", *((float*) item->dataptr) );
            send_debug_log_message(tmp);
          }

          matched = true;
          break;
        case NAATOS_KV_DT_INT:
          *((int*) item->dataptr) = parse_int(valstr,parse_int(item->defaultcfgstr,0));
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_INT. parsed = %d", *((int*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        case NAATOS_KV_DT_UINT16:
          *((uint16_t*) item->dataptr) = (uint16_t) parse_int(valstr,parse_int(item->defaultcfgstr,0));
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_UINT16. parsed = %d", *((uint16_t*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        case NAATOS_KV_DT_BOOLS:
          *((bool*) item->dataptr) = strcmp(valstr, "true") ? false : true;
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_BOOLS. parsed = %d", *((bool*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        default:
          sprintf(tmp,"couldn't parse key = \"%s\" -> valstr = \"%s\" because couldn't handle the datatype",item->name,valstr);
          send_debug_log_message(tmp);
          break;
      }
      if(matched)
        break;
    }
  }
  if(!matched)  {
    sprintf(tmp,"couldn't find key = \"%s\" -> valstr = \"%s\"",keystr,valstr);
    send_debug_log_message(tmp);
  }
  return matched;
}


const naatos_kv_table_entry_t* naatos_config_global_get_itemptr_by_key(const char* keystr)  {
  naatos_kv_table_entry_t * tableitem;
  for(uint8_t i = 0; i<KV_TABLE_GLOBAL_SIZE; i++) {
    tableitem = &(KV_TABLE_GLOBAL_PTR[i]);
    if(strcmp(tableitem->name,keystr)==0) {
      return tableitem;
    }
  }
  return NULL;
}

char * naatos_config_global_getval_as_string_from_item(const naatos_kv_table_entry_t * item)  {
  switch(item->dtype) {
    case NAATOS_KV_DT_FLOAT:
      snprintf(tmp,tmp_sz,"%g",*((float*) item->dataptr)); break;
    case NAATOS_KV_DT_INT:
      snprintf(tmp,tmp_sz,"%d",*((int*) item->dataptr)); break;
    case NAATOS_KV_DT_UINT16:
      snprintf(tmp,tmp_sz,"%d",*((uint16_t*) item->dataptr)); break;
    case NAATOS_KV_DT_BOOLS:
      if(*((bool*) item->dataptr))
        snprintf(tmp,tmp_sz,"true");
      else
        snprintf(tmp,tmp_sz,"false");
      break;
    default:
      //unknown/unimplement/error?
      snprintf(tmp,tmp_sz,"");
      break;
  };

  return tmp;
}


// CYCLE TABLE FUNCTIONs
//const naatos_kv_table_entry_t* naatos_config_cycle_get_itemptr_by_key(const char* keystr)  {
//  naatos_kv_table_entry_t * tableitem;
//  for(uint8_t i = 0; i<KV_TABLE_GLOBAL_SIZE; i++) {
//    tableitem = &(KV_TABLE_GLOBAL_PTR[i]);
//    if(strcmp(tableitem->name,keystr)==0) {
//      return tableitem;
//    }
//  }
//  return NULL;
//}

//char * naatos_config_cycle_getval_as_string_from_item(const naatos_kv_table_entry_cycle_t * item)  {
//  switch(item->dtype) {
//    case NAATOS_KV_DT_FLOAT:
//      snprintf(tmp,tmp_sz,"%g",*((float*) item->dataptr)); break;
//    case NAATOS_KV_DT_INT:
//      snprintf(tmp,tmp_sz,"%d",*((int*) item->dataptr)); break;
//    case NAATOS_KV_DT_UINT16:
//      snprintf(tmp,tmp_sz,"%d",*((uint16_t*) item->dataptr)); break;
//    case NAATOS_KV_DT_BOOLS:
//      if(*((bool*) item->dataptr))
//        snprintf(tmp,tmp_sz,"true");
//      else
//        snprintf(tmp,tmp_sz,"true");
//      break;
//    default:
//      //unknown/unimplement/error?
//      snprintf(tmp,tmp_sz,"ERROR");
//      break;
//  };

//  return tmp;
//}