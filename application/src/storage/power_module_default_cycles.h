#pragma once
#define STRLEN(s) ( (sizeof(s)/sizeof(s[0])) - sizeof(s[0]) )

// DUMMY CYCLE
const char pm_default_cycle_config_1[] = 
  "cycle_run_time_s:30.0\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:0.0\n"
  "double_yellow_grace_period_s:30.0\n"
  "amp_setpoint:0.00\n"
  "valve_setpoint:0.00\n"
  "run_amp_enable:true\n"
  "run_valve_enable:false\n"
  "amp_kp:4.80\n"
  "amp_ki:0.020\n"
  "amp_kd:0.00\n"
  "valve_kp:4.80\n"
  "valve_ki:0.020\n"
  "valve_kd:0.00";
const int pm_default_cycle_config_1_size = STRLEN(pm_default_cycle_config_1);

// AMPLIFICATION CYCLE
const char pm_default_cycle_config_2[] = 
  "cycle_run_time_s:900.0\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:300.0\n"
  "double_yellow_grace_period_s:0.0\n"
  "amp_setpoint:75.00\n"
  "valve_setpoint:72.00\n"
  "run_amp_enable:true\n"
  "run_valve_enable:true\n"
  "amp_kp:4.80\n"
  "amp_ki:0.020\n"
  "amp_kd:0.00\n"
  "valve_kp:4.80\n"
  "valve_ki:0.020\n"
  "valve_kd:0.00";
const int pm_default_cycle_config_2_size = STRLEN(pm_default_cycle_config_2);

// VALVE MELT CYCLE
const char pm_default_cycle_config_3[] = 
  "cycle_run_time_s:300.0\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:120.0\n"
  "double_yellow_grace_period_s:0.0\n"
  "amp_setpoint:75.00\n"
  "valve_setpoint:110.00\n"
  "run_amp_enable:true\n"
  "run_valve_enable:true\n"
  "amp_kp:4.80\n"
  "amp_ki:0.020\n"
  "amp_kd:0.00\n"
  "valve_kp:4.80\n"
  "valve_ki:0.020\n"
  "valve_kd:0.00";
const int pm_default_cycle_config_3_size = STRLEN(pm_default_cycle_config_3);

// WAIT / NO HEATING
const char pm_default_cycle_config_4[] = 
  "cycle_run_time_s:10\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:0.0\n"
  "double_yellow_grace_period_s:0.0\n"
  "amp_setpoint:0.00\n"
  "valve_setpoint:0.00\n"
  "run_amp_enable:true\n"
  "run_valve_enable:false\n"
  "amp_kp:4.80\n"
  "amp_ki:0.020\n"
  "amp_kd:0.00\n"
  "valve_kp:4.80\n"
  "valve_ki:0.020\n"
  "valve_kd:0.00";
const int pm_default_cycle_config_4_size = STRLEN(pm_default_cycle_config_4);