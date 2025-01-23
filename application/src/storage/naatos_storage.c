#include "naatos_storage.h"

/* Storage Variables */
static FIL file;
static DIR dir;
static FILINFO fno;
uint32_t b_written;

bool naatos_storage_initalized = false;
int total_cycles = 0;

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
  res = check_for_naatos_config_file();
  if (res == FR_OK) {
    send_debug_log_message("New config.txt config file in config subdirectory created with default parameters.");
  } else if (res != FR_EXIST) {
    send_debug_log_message("Unable to retreive config.txt");
  }
  res = check_for_cycle_config_files();
  if (res == FR_OK) {
    send_debug_log_message("New cycle configuration files in subdirectory created with default parameters");
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
    // Return that it does not exists
    return res;
  }

  // Set the struct
  for (int i = 0; i < NUM_MASTER_CONFIG_PARAMETERS; i++) {
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
    case LOW_POWER_THRESHOLD:
      parameters->low_power_threshold = parse_int(val, DEFAULT_LOW_POWER_THRESHOLD);
      break;
    case RECOVERY_POWER_THRESHOLD:
      parameters->recovery_power_thresh = parse_int(val, DEFAULT_RECOVERY_THRESHOLD);
      break;
    case SAMPLE_VALID_TIMEOUT:
      parameters->sample_valid_timeout_s = parse_double(val, DEFAULT_VALID_TIMEOUT_S);
      break;
    case ALERT_TIMEOUT_TIME:
      parameters->alert_timeout_time_s = parse_double(val, DEFAULT_ALERT_TIMEOUT_S);
      break;
    case DEBUG_TO_COM_EN:
      num = strcmp(val, "true");
      parameters->debug_to_com_en = num ? false : true;
      break;
#ifndef SAMPLE_PREP_BOARD
    case OPTICAL_DISTANCE:
      parameters->optical_distance = atoi(val);
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
#else
    case HEATER_MAX_TEMP_C:
      parameters->heater_max_temp = parse_double(val, DEFAULT_MAX_HEATER_TEMP);
      break;
    case MAX_HEATER_PID_PWM:
      parameters->max_heater_pid_pwm = parse_int(val, DEFAULT_MAX_HEATER_PID);
      break;
    case MOTOR_SWTICH_CCW_CW:
      num = strcmp(val, "true");
      parameters->switch_motor_ccw_cw = num ? false : true;
      break;
    case HAL_SENSOR_THRESH:
      parameters->hal_sensor_thresh = parse_double(val, DEFAULT_HAL_SENSOR_THRESHOLD);
      break;
    case MOTOR_STALL_PERCENT:
      parameters->motor_stall_percent = parse_int(val, DEFAULT_MOTOR_STALL_PERCENT);
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

FRESULT get_cycle_configurations_parameters(cycle_config_parameters *cycle_configurations_arr) {
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

  for (;;) {
    res = f_readdir(&dir, &fno);
    // Break on error or end of dir 
    if (res != FR_OK || fno.fname[0] == 0) break;  
    // Check if it's a file 
    if (!(fno.fattrib & AM_DIR)) {                 
      if (strncmp(fno.fname, "cycle_config_", 13) == 0) {
        // Check if the filename starts with "cycle_config_"
        char *endptr;
        // Attempt to convert the rest to integer
        strtol(&fno.fname[13], &endptr, 10); 
        // If the rest is an integer followed by a dot
        if (*endptr == '.') { 
          total_cycles++;
        }
      }
    }
  }
  
  // Ensure we have a file
  if (total_cycles > 0) {
    // Malloc space for the cycle configurations array
    cycle_configurations_arr = (cycle_config_parameters *)malloc(total_cycles * sizeof(cycle_config_parameters));
    // Loop through each file
    for (int i = 0; i < total_cycles; i++) {
      char buff[128];
      sprintf(buff, "cycle_config_%d.txt", (i+1));
      // Open current cycle config file
      res = f_open(&file, buff, FA_READ);
      if (res != FR_OK) {
        // Return that it does not exists
        return res;
      }

      // Loop throught the file and populate the cycle configuration array
      for (int j = 0; j < NUM_CYCLE_CONFIG_PARAMS; j++) {
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

        switch ((cycle_config_params_t)j) {
        case CYCLE_RUN_TIME:
            cycle_configurations_arr[i].cycle_run_time_s = parse_double(val, DEFAULT_CYCLE_RUNTIME);
            break;
        case MIN_RUN_ZONE_TEMP_C:
            cycle_configurations_arr[i].min_run_zone_temp = parse_double(val, DEFAULT_MIN_RUN_ZONE_TEMP_C);
            break;
        case MIN_RUN_ZONE_TEMP_EN:
            num = strcmp(val, "true");
            cycle_configurations_arr[i].min_run_zone_temp_en = num ? false : true;
            break;
        case CYCLE_DELAY_TIME:
            cycle_configurations_arr[i].cycle_delay_time = parse_int(val, DEFAULT_CYCLE_DELAY_TIME);
            break;
        case RAMP_TO_TEMP_BEFORE_CYCLE_START:
            num = strcmp(val, "true");
            cycle_configurations_arr[i].ramp_to_temp_before_start_cycle = num ? false : true;
            break;
        case RAMP_TO_TEMP_TIMEOUT:
            cycle_configurations_arr[i].ramp_to_temp_timeout = parse_double(val, DEFAULT_RAMP_TO_TEMP_TIMEOUT);
            break;
    #ifndef SAMPLE_PREP_BOARD
        case AMP_SETPOINT:
            cycle_configurations_arr[i].amp_setpoint = parse_double(val, DEFAULT_AMP_SETPOINT);
            break;
        case VALVE_SETPOINT:
            cycle_configurations_arr[i].valve_setpoint = parse_double(val, DEFAULT_VALVE_SETPOINT);
            break;
        case RUN_AMP:
            num = strcmp(val, "true");
            cycle_configurations_arr[i].run_amp = num ? false : true;
            break;
        case RUN_VALVE:
            num = strcmp(val, "true");
            cycle_configurations_arr[i].run_valve = num ? false : true;
            break;
        case AMP_KP:
            cycle_configurations_arr[i].amp_kp = parse_double(val, DEFAULT_AMP_KP);
            break;
        case AMP_KI:
            cycle_configurations_arr[i].amp_ki = parse_double(val, DEFAULT_AMP_KI);
            break;
        case AMP_KD:
            cycle_configurations_arr[i].amp_kd = parse_double(val, DEFAULT_AMP_KD);
            break;
        case VALVE_KP:
            cycle_configurations_arr[i].valve_kp = parse_double(val, DEFAULT_VALVE_KP);
            break;
        case VALVE_KI:
            cycle_configurations_arr[i].valve_ki = parse_double(val, DEFAULT_VALVE_KI);
            break;
        case VALVE_KD:
            cycle_configurations_arr[i].valve_ki = parse_double(val, DEFAULT_VALVE_KD);
            break;
    #else
        case HEATER_SETPOINT:
            cycle_configurations_arr[i].heater_setpoint = parse_double(val, DEFAULT_HEATER_SETPOINT);
            break;
        case MOTOR_SETPOINT:
            cycle_configurations_arr[i].motor_setpoint = parse_double(val, DEFAULT_MOTOR_SETPOINT);
            break;
        case RUN_HEATER:
            num = strcmp(val, "true");
            cycle_configurations_arr[i].run_heater = num ? false : true;
            break;
        case RUN_MOTOR:
            num = strcmp(val, "true");
            cycle_configurations_arr[i].run_motor = num ? false : true;
            break;
        case HEATER_KP:
            cycle_configurations_arr[i].heater_kp = parse_double(val, DEFAULT_HEATER_KP);
            break;
        case HEATER_KI:
            cycle_configurations_arr[i].heater_ki = parse_double(val, DEFAULT_HEATER_KI);
            break;
        case HEATER_KD:
            cycle_configurations_arr[i].heater_kd = parse_double(val, DEFAULT_HEATER_KD);
            break;
        case MOTOR_KP:
            cycle_configurations_arr[i].motor_kp = parse_double(val, DEFAULT_MOTOR_KP);
            break;
        case MOTOR_KI:
            cycle_configurations_arr[i].motor_ki = parse_double(val, DEFAULT_MOTOR_KI);
            break;
        case MOTOR_KD:
            cycle_configurations_arr[i].motor_ki = parse_double(val, DEFAULT_MOTOR_KD);
            break;
    #endif
        }
      }

      // Close the current cycle file
      res = f_close(&file);
      if (res != FR_OK) {
        return res;
      }
    }
  }
  else {
    return FR_NO_FILE;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return FR_OK;
}

FRESULT check_for_naatos_config_file(void) {
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

  // Write Low Power Threshold
  configBufferSize = sprintf(configBuffer, "low_power_threshold:%d\n", DEFAULT_LOW_POWER_THRESHOLD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Recovery Power Threshold
  configBufferSize = sprintf(configBuffer, "recovery_power_threshold:%d\n", DEFAULT_RECOVERY_THRESHOLD);
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

  // Write Alert Timeout (seconds)
  configBufferSize = sprintf(configBuffer, "alert_timeout_time_s:%0.2f\n", DEFAULT_ALERT_TIMEOUT_S);
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
  configBufferSize = sprintf(configBuffer, "optical_distace:%d\n", DEFAULT_OPTICAL_TRIG_THRESHOLD);
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
#else
  // Write Heater Max Temperature
  configBufferSize = sprintf(configBuffer, "heater_max_temp:%0.2f\n", DEFAULT_MAX_HEATER_TEMP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Max Heater PID PWM
  configBufferSize = sprintf(configBuffer, "max_heater_pid_pwm:%d\n", DEFAULT_MAX_HEATER_PID);
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

  // Write hal sensor voltage threshold
  configBufferSize = sprintf(configBuffer, "hal_sensor_thresh:%0.2f\n", DEFAULT_HAL_SENSOR_THRESHOLD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Write Motor Stall Percentage
  configBufferSize = sprintf(configBuffer, "motor_stall_percentage:%d\n", DEFAULT_MOTOR_STALL_PERCENT);
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

FRESULT check_for_cycle_config_files(void) {
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

  // Try to create new cycle_config_1.txt
  res = f_open(&file, FIRST_CYCLE_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  if (res == FR_EXIST) {
    // Return that it already exists
    return res;
  }

  send_debug_log_message("Warning: NO CYCLE FILES FOUND, CREATING DEFAULTS NOT YET IMPLEMENTED! PUT THEM IN YOURSELF");
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