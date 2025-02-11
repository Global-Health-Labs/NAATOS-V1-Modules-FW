#pragma once
#define STRLEN(s) ( (sizeof(s)/sizeof(s[0])) - sizeof(s[0]) )

const char pm_default_cycle_config_1[] = 
  "cycle_run_time_s:900\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:300\n"
  "amp_setpoint:72.00\n"
  "valve_setpoint:72.00\n"
  "run_amp_enable:true\n"
  "run_valve_enable:true\n"
  "amp_kp:2.250\n"
  "amp_ki:0.050\n"
  "amp_kd:2.250\n"
  "valve_kp:2.250\n"
  "valve_ki:0.050\n"
  "valve_kd:2.250";
const int pm_default_cycle_config_1_size = STRLEN(pm_default_cycle_config_1);

const char pm_default_cycle_config_2[] = 
  "cycle_run_time_s:300\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:300\n"
  "amp_setpoint:72.00\n"
  "valve_setpoint:110.00\n"
  "run_amp_enable:true\n"
  "run_valve_enable:true\n"
  "amp_kp:2.250\n"
  "amp_ki:0.050\n"
  "amp_kd:2.250\n"
  "valve_kp:2.250\n"
  "valve_ki:0.050\n"
  "valve_kd:2.250";
const int pm_default_cycle_config_2_size = STRLEN(pm_default_cycle_config_2);