#pragma once
#define STRLEN(s) ( (sizeof(s)/sizeof(s[0])) - sizeof(s[0]) )

const char sp_default_cycle_config_1[] = 
  "cycle_run_time_s:51.00\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:300\n"
  "heater_setpoint:100.00\n"
  "motor_setpoint:0\n"
  "run_heater_enable:true\n"
  "run_motor_enable:false\n"
  "heater_kp:20.000\n"
  "heater_ki:0.050\n"
  "heater_kd:50.000\n"
  "motor_kp:0.005\n"
  "motor_ki:0.0005\n"
  "motor_kd:0.020";
const int sp_default_cycle_config_1_size = STRLEN(sp_default_cycle_config_1);

const char sp_default_cycle_config_2[] = 
  "cycle_run_time_s:120.00\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:300\n"
  "heater_setpoint:93.50\n"
  "motor_setpoint:0\n"
  "run_heater_enable:true\n"
  "run_motor_enable:false\n"
  "heater_kp:60.000\n"
  "heater_ki:0.050\n"
  "heater_kd:50.000\n"
  "motor_kp:0.005\n"
  "motor_ki:0.0005\n"
  "motor_kd:0.020";
const int sp_default_cycle_config_2_size = STRLEN(sp_default_cycle_config_2);

const char sp_default_cycle_config_3[] = 
  "cycle_run_time_s:19.00\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:300\n"
  "heater_setpoint:93.5\n"
  "motor_setpoint:3900\n"
  "run_heater_enable:true\n"
  "run_motor_enable:true\n"
  "heater_kp:60.000\n"
  "heater_ki:0.050\n"
  "heater_kd:50.000\n"
  "motor_kp:0.005\n"
  "motor_ki:0.0005\n"
  "motor_kd:0.020";
const int sp_default_cycle_config_3_size = STRLEN(sp_default_cycle_config_3);

const char sp_default_cycle_config_4[] = 
  "cycle_run_time_s:170.00\n"
  "cycle_delay_time_s:0\n"
  "ramp_to_temp_before_start_cycle:false\n"
  "ramp_to_temp_timeout:300\n"
  "heater_setpoint:0\n"
  "motor_setpoint:3900\n"
  "run_heater_enable:false\n"
  "run_motor_enable:true\n"
  "heater_kp:20.000\n"
  "heater_ki:0.050\n"
  "heater_kd:50.000\n"
  "motor_kp:0.005\n"
  "motor_ki:0.0005\n"
  "motor_kd:0.020";
  const int sp_default_cycle_config_4_size = STRLEN(sp_default_cycle_config_4);

