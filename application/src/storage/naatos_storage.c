#include "naatos_storage.h"

/* Storage Variables */
static FIL file;
static DIR dir;
static FILINFO fno;
uint32_t b_written;

bool naatos_storage_initalized = false;

// Initalizes either the SD Card or NOR Flash storage
void init_naatos_storage(void) {
#if USE_SD_CARD
  init_sd_card();
#elif USE_NOR_FLASH
  init_nor_flash();
#endif

  // Create directories if needed
  create_naatos_directories();
  // Set storage initalized to true
  naatos_storage_initalized = true;
}

// Uninitalizes either the SD Card or the NOR Flash storage
void uninit_naatos_storage(void) {
#if USE_SD_CARD
  uninit_sd_card();
#elif USE_NOR_FLASH
  uninit_nor_flash();
#endif
  // Set storage initalized to false
  naatos_storage_initalized = false;
}

// Mount SD Card or NOR Flash Storage
void mount_storage(void) {
#if USE_SD_CARD
  if (sd_card_mount() != FR_OK) {
    send_debug_log_message("Unable to unmount SD Card");
  }
#elif USE_NOR_FLASH
  if (mount_nor_flash() != FR_OK) {
    send_debug_log_message("Unable to mount NOR Flash");
  }
#endif
}

// Unmount SD Card or NOR Flash Storage
void unmount_storage(void) {
#if USE_SD_CARD
  if (sd_card_unmount() != FR_OK) {
    send_debug_log_message("Unable to unmount SD Card");
  }
#elif USE_NOR_FLASH
  if (unmount_nor_flash() != FR_OK) {
    send_debug_log_message("Unable to unmount NOR Flash");
  }
#endif
}

// Creates the needed naatos directories if they dont already exist
void create_naatos_directories() {
  FRESULT res;
  // Create logs directory
  res = f_mkdir(LOGS_DIR);
  if (res == FR_OK) {
    send_debug_log_message("Logs directory created");
  }
  // Create config directory
  res = f_mkdir(CONFIG_DIR);
  if (res == FR_OK) {
    send_debug_log_message("Config directory created");
  }
  res = check_for_config_file();
  if (res == FR_OK) {
    send_debug_log_message("New config.txt config file in config subdirectory created with default parameters.");
  } else if (res != FR_EXIST) {
    send_debug_log_message("Unable to retreive config.txt");
  }
}

// Create a new file under the log subdirectory ** NO NAMES WITH : ALLOWED **
FRESULT create_log_file(const char *file_name) {
  FRESULT res;

  // Go to the logs directory
  res = remount_goto_logs_dir();
  if (res != FR_OK) {
    return res;
  }

  // Make the log file
  res = f_open(&file, file_name, FA_CREATE_NEW | FA_WRITE);
  if (res != FR_OK) {
    return res;
  }

  // Write the csv header to the file
  res = f_write(&file, CSV_HEADER, CSV_HEADER_SIZE, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return res;
}

// Write a line to the log file
FRESULT write_log_line(const char *logName, const char *writeBuff, uint32_t writeBuffSize) {
  FRESULT res;
  uint32_t b_written;

  // Go to the logs directory
  res = remount_goto_logs_dir();
  if (res != FR_OK) {
    return res;
  }

  // Open the log file
  res = f_open(&file, logName, FA_WRITE | FA_OPEN_APPEND);
  if (res != FR_OK) {
    return res;
  }

  // Write the given line
  res = f_write(&file, writeBuff, writeBuffSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return res;
}

FRESULT remount_goto_logs_dir(void) {
  FRESULT res;

  // Re-Mount Storage
  mount_storage();

  // Open root directory
  res = f_opendir(&dir, "/");
  if (res != FR_OK) {
    return res;
  }

  // Change directory into logs
  res = f_chdir(LOGS_DIR);
  if (res != FR_OK) {
    return res;
  }
}

FRESULT get_naatos_configuration_parameters(naatos_config_parameters *parameters) {
  char configBuffer[50];
  FRESULT res;
  char *pch;
  char val[50];
  int num;

  // Re-Mount Storage
  mount_storage();

  // Open root directory
  res = f_opendir(&dir, "/");
  if (res != FR_OK) {
    return res;
  }

  // Change directory into configs
  res = f_chdir(CONFIG_DIR);
  if (res != FR_OK) {
    return res;
  }
  // Open naatos config file
  res = f_open(&file, NAATOS_CONFIG_FILE, FA_READ);
  if (res != FR_OK) {
    // Return that it already exists
    return res;
  }

  // Set the struct
  for (int i = 0; i < NUM_PARAMETERS; i++) {
    pch = f_gets(configBuffer, 50, &file);
    pch = strtok(configBuffer, ":");
    sprintf(val, "%s", pch);
    pch = strtok(NULL, ":");
    sprintf(val, "%s", pch);
    char *newline = strchr(val, '\n');
    if (newline) {
      // Replace newline character with null terminator
      *newline = '\0';
    }

    switch ((naatos_config_params_t)i) {
    case SAMPLE_RATE:
      parameters->sample_rate = atof(val);
      if (parameters->sample_rate < 0.048) {
        send_debug_log_message("Warning: Sample rate lower than minimum (0.048), setting sample rate to minimum.");
        parameters->sample_rate = 0.048;
        break;
      }
      break;
    case LOGGING_RATE:
      parameters->logging_rate = parse_double(val, DEFAULT_LOGGING_RATE);
      break;
    case CYCLE_ONE_RUN_TIME:
      parameters->cycle_1_run_time_s = atoi(val);
      break;
    case CYCLE_TWO_RUN_TIME:
      parameters->cycle_2_run_time_s = atoi(val);
      break;
    case LOW_POWER_THRESHOLD:
      parameters->low_power_threshold = parse_int(val, DEFAULT_LOW_POWER_THRESHOLD);
      break;
    case RECOVERY_POWER_THRESHOLD:
      parameters->recovery_power_thresh = parse_int(val, DEFAULT_RECOVERY_THRES);
      break;
    case SAMPLE_VALID_TIMEOUT:
      parameters->sample_valid_timeout_s = parse_double(val, DEFAULT_VALID_TIMEOUT_S);
      break;
    case SAMPLE_COMPLETE_DELAY:
      parameters->sample_complete_delay_s = parse_int(val, DEFAULT_CYCLES_COMPLETE_DELAY_S);
      break;
    case ALERT_TIMEOUT_TIME:
      parameters->alert_timeout_time_s = parse_double(val, DEFAULT_ALERT_TIMEOUT_S);
      break;
    case MIN_RUN_ZONE_TEMP_C:
      parameters->min_run_zone_temp = parse_double(val, DEFAULT_MIN_RUN_ZONE_TEMP);
      break;
    case MIN_RUN_ZONE_TEMP_EN:
      num = strcmp(val, "true");
      parameters->min_run_zone_temp_en = num ? false : true;
      break;
    case DEBUG_TO_COM_EN:
      num = strcmp(val, "true");
      parameters->debug_to_com_en = num ? false : true;
      break;
#ifndef SAMPLE_PREP_BOARD
    case OPTICAL_DISTANCE:
      parameters->optical_distance = atoi(val);
      break;
    case MIN_WAIT_TIME_AFTER_CYCLE:
      parameters->min_wait_time_after_cycle_s = atoi(val);
      break;
    case MAX_AMP_PID_PWM:
      parameters->max_amp_pid_pwm = parse_int(val, DEFAULT_MAX_AMP_PID);
      break;
    case MAX_VALVE_PID_PWM:
      parameters->max_valve_pid_pwm = parse_int(val, DEFAULT_MAX_VALVE_PID);
      break;
    case VALVE_MAX_TEMP_C:
      parameters->valve_max_temp = atof(val);
      break;
    case AMP_MAX_TEMP_C:
      parameters->amp_max_temp = atof(val);
      break;
    case VALVE_SETPOINT_C_1:
      parameters->valve_setpoint_1 = atof(val);
      break;
    case AMP_SETPOINT_C_1:
      parameters->amp_setpoint_1 = atof(val);
      break;
    case VALVE_KP_1:
      parameters->valve_kp_1 = atof(val);
      break;
    case VALVE_KI_1:
      parameters->valve_ki_1 = atof(val);
      break;
    case AMP_KP_1:
      parameters->amp_kp_1 = atof(val);
      break;
    case AMP_KI_1:
      parameters->amp_ki_1 = atof(val);
      break;
    case VALVE_KD_1:
      parameters->valve_kd_1 = atof(val);
      break;
    case VALVE_SETPOINT_C_2:
      parameters->valve_setpoint_2 = atof(val);
      break;
    case AMP_SETPOINT_C_2:
      parameters->amp_setpoint_2 = atof(val);
      break;
    case VALVE_KP_2:
      parameters->valve_kp_2 = atof(val);
      break;
    case VALVE_KI_2:
      parameters->valve_ki_2 = atof(val);
      break;
    case VALVE_KD_2:
      parameters->valve_kd_2 = atof(val);
      break;
    case AMP_KP_2:
      parameters->amp_kp_2 = atof(val);
      break;
    case AMP_KI_2:
      parameters->amp_ki_2 = atof(val);
      break;
    case AMP_KD_2:
      parameters->amp_kd_2 = atof(val);
      break;
    case RUN_AMP_CYCLE_1:
      num = strcmp(val, "true");
      parameters->run_amp_cycle_1 = num ? false : true;
      break;
    case RUN_AMP_CYCLE_2:
      num = strcmp(val, "true");  
      parameters->run_amp_cycle_2 = num ? false : true;
      break;
    case RUN_VALVE_CYCLE_1:
      num = strcmp(val, "true");
      parameters->run_valve_cycle_1 = num ? false : true;
      break;
    case RUN_VALVE_CYCLE_2:
      num = strcmp(val, "true");
      parameters->run_valve_cycle_2 = num ? false : true;
      break;
#else
    case MAX_HEATER_PID_PWM:
      parameters->max_heater_pid_pwm = parse_int(val, DEFAULT_MAX_HEATER_PID);
      break;
    case RAMP_TO_TEMP_BEFORE_CYCLE_1_START:
      num = strcmp(val, "true");
      parameters->ramp_to_temp_before_start_cycle_1 = num ? false : true;
      break;
    case RAMP_TO_TEMP_BEFORE_CYCLE_2_START:
      num = strcmp(val, "true");
      parameters->ramp_to_temp_before_start_cycle_2 = num ? false : true;
      break;
    case RAMP_TO_TEMP_C1_TIMEOUT:
      parameters->ramp_to_temp_c1_timeout = parse_double(val, DEFAULT_RAMP_TO_TEMP_TIMEOUT);
      break;
    case RAMP_TO_TEMP_C2_TIMEOUT:
      parameters->ramp_to_temp_c2_timeout = parse_double(val, DEFAULT_RAMP_TO_TEMP_TIMEOUT);
      break;
    case MOTOR_END_WAIT_TIME_S:
      parameters->motor_end_wait_time_s = parse_int(val, DEFAULT_MOTOR_WAIT_TIME_S);
      break;
    case HEATER_SETPOINT_1_C:
      parameters->heater_setpoint_1 = parse_double(val, DEFAULT_HEATER_SETPOINT_1);
      break;
    case HEATER_SETPOINT_2_C:
      parameters->heater_setpoint_2 = parse_double(val, DEFAULT_HEATER_SETPOINT_2);
      break;
    case HEATER_MAX_TEMP_C:
      parameters->heater_max_temp = parse_double(val, DEFAULT_MAX_TEMP);
      break;
    case HEATER_KP_1:
      parameters->heater_kp_1 = parse_double(val, H_KP_1);
      break;
    case HEATER_KI_1:
      parameters->heater_ki_1 = parse_double(val, H_KI_1);
      break;
    case HEATER_KD_1:
      parameters->heater_kd_1 = parse_double(val, H_KD_1);
      break;
    case HEATER_KP_2:
      parameters->heater_kp_2 = parse_double(val, H_KP_2);
      break;
    case HEATER_KI_2:
      parameters->heater_ki_2 = parse_double(val, H_KI_2);
      break;
    case HEATER_KD_2:
      parameters->heater_kd_2 = parse_double(val, H_KD_2);
      break;
    case MOTOR_KP_1:
      parameters->motor_kp_1 = parse_double(val, M_KP);
      break;
    case MOTOR_KI_1:
      parameters->motor_ki_1 = parse_double(val, M_KI);
      break;
    case MOTOR_KD_1:
      parameters->motor_kd_1 = parse_double(val, M_KD);
      break;
    case MOTOR_SWTICH_CCW_CW:
      num = strcmp(val, "true");
      parameters->switch_motor_ccw_cw = num ? false : true;
      break;
    case _MOTOR_SETPOINT_1:
      parameters->motor_setpoint_1 = parse_int(val, MOTOR_SETPOINT_1);
      break;
    case MOTOR_KP_2:
      parameters->motor_kp_2 = parse_double(val, M_KP);
      break;
    case MOTOR_KI_2:
      parameters->motor_ki_2 = parse_double(val, M_KI);
      break;
    case MOTOR_KD_2:
      parameters->motor_kd_2 = parse_double(val, M_KD);
      break;
    case _MOTOR_SETPOINT_2:
      parameters->motor_setpoint_2 = parse_int(val, MOTOR_SETPOINT_2);
      break;
    case RUN_MOTOR_1:
      num = strcmp(val, "true");
      parameters->run_motor_1 = num ? false : true;
      break;
    case RUN_HEATER_1:
      num = strcmp(val, "true");
      parameters->run_heater_1 = num ? false : true;
      break;
    case RUN_MOTOR_2:
      num = strcmp(val, "true");
      parameters->run_motor_2 = num ? false : true;
      break;
    case RUN_HEATER_2:
      num = strcmp(val, "true");
      parameters->run_heater_2 = num ? false : true;
      break;
    case HAL_SENSOR_THRESH:
      parameters->hal_sensor_thresh = parse_double(val, DEFAULT_HAL_SENSOR_THRESHOLD);
      break;
    case MOTOR_STALL_PERCENT:
      parameters->motor_stall_percent = parse_int(val, DEFAULT_MOTOR_STALL_PERCENTAGE);
      break;
    case MOTOR_STALL_PWM:
      parameters->motor_stall_pwm = parse_double(val, DEFAULT_MOTOR_STALL_PWM);
      break;  
    case MOTOR_STALL_ENABLE:
      num = strcmp(val, "true");
      parameters->motor_stall_en = num ? false : true;
      break;
#endif
    case MMDDYY:
      parameters->mmddyy = parse_int(val, DEFAULT_DATE);
      break;
    case HHMMSS:
      parameters->hhmmss = parse_int(val, DEFAULT_TIME);
      break;
    case SET_DATE_TIME:
      num = strcmp(val, "true");
      parameters->set_date_time = num ? false : true;
      break;
    default:
      // Handle default case
      break;
    }
  }

  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return FR_OK;
}

FRESULT check_for_config_file(void) {
  FRESULT res;
  DIR dir;
  char configBuffer[50];
  uint32_t configBufferSize;
  uint32_t b_written;

  // Change directory into configs
  res = f_chdir(CONFIG_DIR);
  if (res != FR_OK) {
    return res;
  }

  // Try to create new naatos_config.txt
  res = f_open(&file, NAATOS_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  if (res == FR_EXIST) {
    // Return that it already exists
    return res;
  }

  // Open the directory for reading
  res = f_opendir(&dir, ".");
  if (res != FR_OK) {
      return res;  // Error opening directory
  }

  // Delete Old Config Files if they are present
  while (1) {
    res = f_readdir(&dir, &fno);  // Read a directory entry
    if (res != FR_OK || fno.fname[0] == 0) {
      break;  // no More files or Error
    }
    
    // Skip "." and ".." entries
    if (fno.fname[0] == '.' && (fno.fname[1] == '\0' || (fno.fname[1] == '.' && fno.fname[2] == '\0'))) {
        continue;
    }

    // If it's a file, delete it
    if (!(fno.fattrib & AM_DIR)) {
      res = f_unlink(fno.fname);  // Delete the file
      if (res != FR_OK) {
          return res;  // Return error if file couldn't be deleted
      }
    }   
    
  }

  f_closedir(&dir);

  // Create new naatos_config.txt
  res = f_open(&file, NAATOS_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  if (res == FR_EXIST) {
    // Return that it already exists
    return res;
  }

  // Populate new config file
  // Write sample rate
  configBufferSize = sprintf(configBuffer, "sample_rate:%0.2f\n", DEFAULT_SAMPLE_RATE);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write logging rate
  configBufferSize = sprintf(configBuffer, "logging_rate:%0.2f\n", DEFAULT_LOGGING_RATE);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write cycle 1 run time 
  configBufferSize = sprintf(configBuffer, "cycle_1_run_time_s:%0.2f\n", DEFAULT_CYCLE_1_RUNTIME);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write cycle 2 run time 
  configBufferSize = sprintf(configBuffer, "cycle_2_run_time_s:%0.2f\n", DEFAULT_CYCLE_2_RUNTIME);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Low Power Threshold
  configBufferSize = sprintf(configBuffer, "low_power_threshold:%d\n", DEFAULT_LOW_POWER_THRESHOLD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Recovery Power Threshold
  configBufferSize = sprintf(configBuffer, "recovery_power_threshold:%d\n", DEFAULT_RECOVERY_THRES);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  
  // Write Sample Valid Timeout (seconds)
  configBufferSize = sprintf(configBuffer, "sample_valid_timeout_s:%0.2f\n", DEFAULT_VALID_TIMEOUT_S);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Sample Complete Delay (seconds)
  configBufferSize = sprintf(configBuffer, "sample_complete_delay_s:%0.2f\n", DEFAULT_CYCLES_COMPLETE_DELAY_S);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Alert Timeout (seconds)
  configBufferSize = sprintf(configBuffer, "alert_timeout_time_s:%0.2f\n", DEFAULT_ALERT_TIMEOUT_S);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Minimum Run Zone Temperature and Enable
  configBufferSize = sprintf(configBuffer, "min_run_zone_temp:%0.2f\n", DEFAULT_MIN_RUN_ZONE_TEMP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  
  // Write Minimum Zone Temperature Enable
  configBufferSize = sprintf(configBuffer, "min_run_zone_temp_en:%s\n", DEFAULT_MIN_RUN_ZONE_TEMP_EN ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  
  // Write Debug Statements to COM Port Enable
  configBufferSize = sprintf(configBuffer, "debug_to_com_en:%s\n", DEFAULT_DEBUG_TO_COM_ENABLE ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
#ifndef SAMPLE_PREP_BOARD
  // Write Optical Switch
  configBufferSize = sprintf(configBuffer, "optical_distace:%d\n", OPTICAL_TRIG_THRES);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Minimum Wait Time After Cycle
  configBufferSize = sprintf(configBuffer, "min_wait_time_after_cycle_s:%d\n", DEFAULT_WAIT_TIME_AFTER_CYCLE_S);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Max Amplification PID PWM
  configBufferSize = sprintf(configBuffer, "max_amp_pid_pwm:%d\n", DEFAULT_MAX_AMP_PID);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Max Valve PID PWM
  configBufferSize = sprintf(configBuffer, "max_valve_pid_pwm:%d\n", DEFAULT_MAX_VALVE_PID);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Max Temperature
  configBufferSize = sprintf(configBuffer, "valve_max_temp:%0.2f\n", DEFAULT_VALVE_MAX_TEMP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  
  // Write Amplification Max Temperature
  configBufferSize = sprintf(configBuffer, "amp_max_temp:%0.2f\n", DEFAULT_AMP_MAX_TEMP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Amplification Zone Setpoint 1
  configBufferSize = sprintf(configBuffer, "amp_setpoint_1:%0.2f\n", AMP_SETPOINT_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Zone Setpoint 1
  configBufferSize = sprintf(configBuffer, "valve_setpoint_1:%0.2f\n", VALVE_SETPOINT_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Kp 1
  configBufferSize = sprintf(configBuffer, "valve_kp_1:%0.3f\n", V_KP_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Ki 1
  configBufferSize = sprintf(configBuffer, "valve_ki_1:%0.3f\n", V_KI_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Kd 1
  configBufferSize = sprintf(configBuffer, "valve_kd_1:%0.3f\n", V_KD_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Amplifcation Kp 1
  configBufferSize = sprintf(configBuffer, "amp_kp_1:%0.3f\n", A_KP_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Amplification Ki 1
  configBufferSize = sprintf(configBuffer, "amp_ki_1:%0.3f\n", A_KI_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Amplification Kd 1
  configBufferSize = sprintf(configBuffer, "amp_kd_1:%0.3f\n", A_KD_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Zone Setpoint 2
  configBufferSize = sprintf(configBuffer, "valve_setpoint_2:%0.2f\n", VALVE_SETPOINT_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amplification Zone Setpoint 2
  configBufferSize = sprintf(configBuffer, "amp_setpoint_2:%0.2f\n", AMP_SETPOINT_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Kp 2
  configBufferSize = sprintf(configBuffer, "valve_kp_2:%0.3f\n", V_KP_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Ki 2
  configBufferSize = sprintf(configBuffer, "valve_ki_2:%0.3f\n", V_KI_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Valve Kd 2
  configBufferSize = sprintf(configBuffer, "valve_kd_2:%0.3f\n", V_KD_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Amplification Kp 2
  configBufferSize = sprintf(configBuffer, "amp_kp_2:%0.3f\n", A_KP_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Amplification Ki 2
  configBufferSize = sprintf(configBuffer, "amp_ki_2:%0.3f\n", A_KI_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Amplification Kd 2
  configBufferSize = sprintf(configBuffer, "amp_kd_2:%0.3f\n", A_KD_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Run Amplification Cycle 1
  configBufferSize = sprintf(configBuffer, "run_amp_cycle_1:%s\n", DEFAULT_RUN_AMP_CYCLE_1 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Run Amplification Cycle 2
  configBufferSize = sprintf(configBuffer, "run_amp_cycle_2:%s\n", DEFAULT_RUN_AMP_CYCLE_2 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Run Valve Cycle 1
  configBufferSize = sprintf(configBuffer, "run_valve_cycle_1:%s\n", DEFAULT_RUN_VALVE_CYCLE_1 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Run Valve Cycle 2
  configBufferSize = sprintf(configBuffer, "run_valve_cycle_2:%s\n", DEFAULT_RUN_VALVE_CYCLE_2 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

#else
  // Write Max Heater PID PWM
  configBufferSize = sprintf(configBuffer, "max_heater_pid_pwm:%d\n", DEFAULT_MAX_HEATER_PID);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  configBufferSize = sprintf(configBuffer, "ramp_to_temp_before_start_cycle_1:%s\n", DEFAULT_RAMP_TO_TEMP_BEFORE_START_1 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  configBufferSize = sprintf(configBuffer, "ramp_to_temp_before_start_cycle_2:%s\n", DEFAULT_RAMP_TO_TEMP_BEFORE_START_2 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  configBufferSize = sprintf(configBuffer, "ramp_to_temp_timeout_c1:%0.2f\n", DEFAULT_RAMP_TO_TEMP_TIMEOUT);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  configBufferSize = sprintf(configBuffer, "ramp_to_temp_timeout_c2:%0.2f\n", DEFAULT_RAMP_TO_TEMP_TIMEOUT);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Motor End Wait Time (Seconds)
  configBufferSize = sprintf(configBuffer, "motor_end_wait_time_s:%d\n", DEFAULT_MOTOR_WAIT_TIME_S);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write heater Zone Setpoint
  configBufferSize = sprintf(configBuffer, "heater_setpoint_1:%0.2f\n", DEFAULT_HEATER_SETPOINT_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "heater_setpoint_2:%0.2f\n", DEFAULT_HEATER_SETPOINT_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater Max Temperature
  configBufferSize = sprintf(configBuffer, "heater_max_temp:%0.2f\n", DEFAULT_MAX_TEMP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Heater 1 Kp
  configBufferSize = sprintf(configBuffer, "heater_kp_1:%0.3f\n", H_KP_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 1 Ki
  configBufferSize = sprintf(configBuffer, "heater_ki_1:%0.3f\n", H_KI_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 1 Kd
  configBufferSize = sprintf(configBuffer, "heater_kd_1:%0.3f\n", H_KD_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 2 Kp
  configBufferSize = sprintf(configBuffer, "heater_kp_2:%0.3f\n", H_KP_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 2 Ki
  configBufferSize = sprintf(configBuffer, "heater_ki_2:%0.3f\n", H_KI_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 2 Kd
  configBufferSize = sprintf(configBuffer, "heater_kd_2:%0.3f\n", H_KD_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor 1 Kp
  configBufferSize = sprintf(configBuffer, "motor_kp_1:%0.3f\n", M_KP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor 1 Ki
  configBufferSize = sprintf(configBuffer, "motor_ki_1:%0.4f\n", M_KI);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor 1 Kd
  configBufferSize = sprintf(configBuffer, "motor_kd_1:%0.3f\n", M_KD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor Switch CCW CW
  configBufferSize = sprintf(configBuffer, "motor_switch_ccw_cw:%s\n", DEFAULT_MOTOR_SWTICH_CCW_CW ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor Setpoint 1
  configBufferSize = sprintf(configBuffer, "motor_setpoint_1:%d\n", MOTOR_SETPOINT_1);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor 2 Kp
  configBufferSize = sprintf(configBuffer, "motor_kp_2:%0.3f\n", M_KP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor 2 Ki
  configBufferSize = sprintf(configBuffer, "motor_ki_2:%0.4f\n", M_KI);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor 2 Kd
  configBufferSize = sprintf(configBuffer, "motor_kd_2:%0.3f\n", M_KD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor Setpoint 1
  configBufferSize = sprintf(configBuffer, "motor_setpoint_2:%d\n", MOTOR_SETPOINT_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Run Motor 1
  configBufferSize = sprintf(configBuffer, "run_motor_1:%s\n", DEFAULT_RUN_MOTOR_1 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Run Heater 1
  configBufferSize = sprintf(configBuffer, "run_heater_1:%s\n", DEFAULT_RUN_HEATER_1 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Run Motor 2
  configBufferSize = sprintf(configBuffer, "run_motor_2:%s\n", DEFAULT_RUN_MOTOR_2 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Run Heater 2
  configBufferSize = sprintf(configBuffer, "run_heater_2:%s\n", DEFAULT_RUN_HEATER_2 ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write hal sensor voltage threshold
  configBufferSize = sprintf(configBuffer, "hal_sensor_thresh:%0.2f\n", DEFAULT_HAL_SENSOR_THRESHOLD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor Stall Percentage
  configBufferSize = sprintf(configBuffer, "motor_stall_percentage:%d\n", DEFAULT_MOTOR_STALL_PERCENTAGE);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor Stall PWM
  configBufferSize = sprintf(configBuffer, "motor_stall_pwm:%0.2f\n", DEFAULT_MOTOR_STALL_PWM);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Motor Stall Enable
  configBufferSize = sprintf(configBuffer, "motor_stall_en:%s\n", DEFAULT_MOTOR_STALL_ENABLE ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
#endif
  // Write calendar date
  configBufferSize = sprintf(configBuffer, "mmddyy:%d\n", DEFAULT_DATE);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write time of day
  configBufferSize = sprintf(configBuffer, "hhmmss:%d\n", DEFAULT_TIME);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write set time flag
  configBufferSize = sprintf(configBuffer, "set_time_date:%s\n", DEFAULT_SET_TIME ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return res;
}

// Sets the set_time_date variable to "false"
FRESULT reset_set_time_date(void) {
  FRESULT res;
  const char *var = "set_time_date";
  UINT br, bw;            // File read/write count
  char buffer[1024];      // Buffer to hold file content
  char temp_buffer[1024]; // Temporary buffer for modified content

  // Re-Mount Storage
  mount_storage();

  // Open root directory
  res = f_opendir(&dir, "/");
  if (res != FR_OK) {
    return res;
  }

  // Change directory into configs
  res = f_chdir(CONFIG_DIR);
  if (res != FR_OK) {
    return res;
  }

  // Open naatos config file
  res = f_open(&file, NAATOS_CONFIG_FILE, FA_READ | FA_WRITE);
  if (res != FR_OK) {
    // Return that it already exists
    return res;
  }

  // Read contents into a buffer
  res = f_read(&file, buffer, sizeof(buffer) - 1, &br);
  if (res != FR_OK) {
    return res;
  }

  buffer[br] = '\0';

  // Search buffer for specified var. Copy lines that do not contain the specified var into temp_buffer
  char *line = strtok(buffer, "\n");
  char *temp_ptr = temp_buffer;
  while (line != NULL) {
    if (strstr(line, var) != NULL) {
      //Specified var found in line. Rewrite line to have it set to false
      temp_ptr += sprintf(temp_ptr, "%s\n", "set_time_date:false");
    } else {
      // If the var is not found in the line, copy the line to the temp buffer
      temp_ptr += sprintf(temp_ptr, "%s\n", line);
    }
    line = strtok(NULL, "\n");
  }

  // Move file pointer back to the beginning
  res = f_lseek(&file, 0);
  if (res != FR_OK) {
    return res;
  }

  // Write the contents of temp_buffer back into the config file (with the specified var removed)
  res = f_write(&file, temp_buffer, strlen(temp_buffer), &bw);
  if (res != FR_OK || bw < strlen(temp_buffer)) {
    return res;
  }

  // Truncate file to new size
  res = f_truncate(&file);
  if (res != FR_OK) {
    return res;
  }

  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }
}

double parse_double(const char *str, double default_value) {
  char *endptr;
  double value = strtod(str, &endptr);

  // Check if the entire string was consumed and a valid number was parsed
  if (endptr == str) {
    // No valid conversion was performed
    return default_value;
  }

  return value;
}

int parse_int(const char *str, int default_value) {
  char *endptr;
  int value = strtol(str, &endptr, 10); // Base 10 for decimal

  // Check if the entire string was consumed and a valid number was parsed
  if (endptr == str) {
    // No valid conversion was performed
    return default_value;
  }

  return value;
}