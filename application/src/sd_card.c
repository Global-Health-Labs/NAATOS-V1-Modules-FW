#include "sd_card.h"

/* SD Card Variables */
static FATFS fs;
static DIR dir;
static FILINFO fno;
static FIL file;
uint32_t bytes_written;
FRESULT ff_result;
DSTATUS disk_state = STA_NOINIT;
uint32_t blocks_per_mb;
uint32_t capacity;
uint32_t b_written;

bool sd_card_inited = false;

// Initialize FATFS disk I/O interface by providing the block device.
static diskio_blkdev_t drives[] = { DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev), NULL) };

// "private" functions
FRESULT remount_goto_logs_dir(void);
FRESULT check_for_config_file(void);

// Initalize Function
void init_sd_card(void) {
  memset(&fs, 0, sizeof(FATFS));

  // Register the drives we have 
  diskio_blockdev_register(drives, ARRAY_SIZE(drives));

  // Initalize the disk on the SD
  for (uint32_t retries = 3; retries && disk_state; --retries) {
    disk_state = disk_initialize(0);
  }
  if (disk_state) {
    printf("SD Card initialization failed.\n");
  }
  
  // Get SD Card Specifications
  blocks_per_mb = (1024uL * 1024uL) / m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_size;
  capacity = m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_count / blocks_per_mb;
  printf("SD Card initalized. Capactity: %d MB\n", capacity);
  
  // Mount SD Card
  ff_result = sd_card_mount();
  if (ff_result != FR_OK) {
    printf("Unable to mount SD Card!\n");
  }

  // Create directories if needed
  create_naatos_directories();

  sd_card_inited = true;
}

void uninit_sd_card(void) {

  UNUSED_RETURN_VALUE(sd_card_unmount());
  UNUSED_RETURN_VALUE(disk_uninitialize(0));
  
  sd_card_inited = false;
  printf("SD Card Uninitalized.\n");
}

// Mount the SD card volume
FRESULT sd_card_mount(void) {
  return f_mount(&fs, "", 1);
}

// Mount the SD card volume ** Dont believe this is working right now
FRESULT sd_card_unmount(void) {
  f_mount(0, "", 0);
}

// Creates the needed naatos directories if they dont already exist
void create_naatos_directories() {
  FRESULT res;
  // Create logs directory
  res = f_mkdir(LOGS_DIR);
  if (res == FR_OK) {
    printf("Logs directory created\n");
  }
  // Create config directory
  res = f_mkdir(CONFIG_DIR);
  if (res == FR_OK) {
    printf("Config directory created\n");
  }
  res = check_for_config_file();
  if (res == FR_OK) {
    printf("New config.txt config file in config subdirectory created with default parameters.\n");
  }
  else if (res != FR_EXIST) {
    printf("Unable to retreive config.txt from sd card.\n");
  }
  
}

// Create a new file under the log subdirectory ** NO NAMES WITH : ALLOWED **
FRESULT sd_card_create_log_file(const char * file_name) {
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

// Write a line to the log file on the SD card 
FRESULT sd_card_write_log_line(const char * logName, const char * writeBuff, uint32_t writeBuffSize) {
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

// Prints directories seen on the sd card -- Was used during debug of sd card, only prints directories and files at root
void sd_card_list_contents(void) {
  printf("\r\n Listing directory: /\n");
  // Open Root Directory
  ff_result = f_opendir(&dir, "/");
  if (ff_result) {
      printf("Directory listing failed!\n");
      return;
  }

  // Get the other directories
  do {
      ff_result = f_readdir(&dir, &fno);
      if (ff_result != FR_OK) {
          printf("Directory read failed.\n");
          return;
      }

      if (fno.fname[0]) {
          if (fno.fattrib & AM_DIR) {
              printf("   <DIR>   %s\n",(uint32_t)fno.fname);
          }
          else {
              printf("%9lu  %s\n", fno.fsize, (uint32_t)fno.fname);
          }
      }
  }
  while (fno.fname[0]);
}

FRESULT remount_goto_logs_dir(void) {
  FRESULT res;

  // Re-Mount SD Card
  res = sd_card_mount();
  if (res != FR_OK) {
    return res;
  }

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


FRESULT check_for_config_file(void) {
  FRESULT res;
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
  // Write Amplification Zone Run Time
  configBufferSize = sprintf(configBuffer, "amp_zone_run_time:%d\n", DEFAULT_AMPLIFICATION_ZONE_ON_TIME);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Valve Zone Run Time
  configBufferSize = sprintf(configBuffer, "valve_zone_run_time:%d\n", DEFAULT_VALVE_ZONE_ON_TIME);
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
  // Write Valve Zone Setpoint
  configBufferSize = sprintf(configBuffer, "valve_setpoint:%0.2f\n", VALVE_SETPOINT);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amplification Zone Setpoint
  configBufferSize = sprintf(configBuffer, "amplification_setpoint:%0.2f\n", AMP0_SETPOINT);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Valve Kp
  configBufferSize = sprintf(configBuffer, "valve_kp:%0.3f\n", V_KP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Valve Ki
  configBufferSize = sprintf(configBuffer, "valve_ki:%0.3f\n", V_KI);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Valve Kd
  configBufferSize = sprintf(configBuffer, "valve_kd:%0.3f\n", V_KD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp0 Kp
  configBufferSize = sprintf(configBuffer, "amp0_kp:%0.3f\n", A0_KP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp0 Ki
  configBufferSize = sprintf(configBuffer, "amp0_ki:%0.3f\n", A0_KI);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp0 Kd
  configBufferSize = sprintf(configBuffer, "amp0_kd:%0.3f\n", A0_KD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp1 Kp
  configBufferSize = sprintf(configBuffer, "amp1_kp:%0.3f\n", A1_KP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp1 Ki
  configBufferSize = sprintf(configBuffer, "amp1_ki:%0.3f\n", A1_KI);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp1 Kd
  configBufferSize = sprintf(configBuffer, "amp1_kd:%0.3f\n", A1_KD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp2 Kp
  configBufferSize = sprintf(configBuffer, "amp2_kp:%0.3f\n", A2_KP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp2 Ki
  configBufferSize = sprintf(configBuffer, "amp2_ki:%0.3f\n", A2_KI);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp2 Kd
  configBufferSize = sprintf(configBuffer, "amp2_kd:%0.3f\n", A2_KD);
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

FRESULT get_naatos_configuration_parameters(naatos_config_parameters * parameters) {
  char configBuffer[50];
  FRESULT res;
  char * pch;
  char * val = (char *)malloc(50 * sizeof(char));

  // Re-Mount SD Card
  res = sd_card_mount();
  if (res != FR_OK) {
    return res;
  }

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
  for (int i = 0; i < NUM_PARAMETERS; i ++) {
    pch = f_gets(configBuffer, 50, &file);
    pch = strtok(configBuffer, ":");
    sprintf(val, "%s", pch);
    pch = strtok(NULL, ":");
    sprintf(val, "%s", pch);
    switch((naatos_config_params_t)i) {
      case SAMPLE_RATE:
            parameters->sample_rate = atof(val);
            if (parameters->sample_rate < 0.048) {
              printf("Warning: Sample rate lower than minimum (0.048), setting sample rate to minimum.");
              parameters->sample_rate = 0.048;
              break;
            }
            break;
        case LOGGING_RATE:
            parameters->logging_rate = atof(val);
            break;
        case VALVE_ZONE_RUN_TIME:
            parameters->valve_zone_run_time_m = atoi(val);
            break;
        case AMP_ZONE_RUN_TIME:
            parameters->amplification_zone_run_time_m = atoi(val);
            break;
        case LOW_POWER_THRESHOLD:
            parameters->low_power_threshold = atoi(val);
            break;
        case VALVE_SETPOINT_C:
            parameters->valve_setpoint = atof(val);
            break;
        case AMP_SETPOINT_C:
            parameters->amplification_setpoint = atof(val);
            break;
        case VALVE_KP:
            parameters->valve_kp = atof(val);
            break;
        case VALVE_KI:
            parameters->valve_ki = atof(val);
            break;
        case VALVE_KD:
            parameters->valve_kd = atof(val);
            break;
        case AMP0_KP:
            parameters->amp0_kp = atof(val);
            break;
        case AMP0_KI:
            parameters->amp0_ki = atof(val);
            break;
        case AMP0_KD:
            parameters->amp0_kd = atof(val);
            break;
        case AMP1_KP:
            parameters->amp1_kp = atof(val);
            break;
        case AMP1_KI:
            parameters->amp1_ki = atof(val);
            break;
        case AMP1_KD:
            parameters->amp1_kd = atof(val);
            break;
        case AMP2_KP:
            parameters->amp2_kp = atof(val);
            break;
        case AMP2_KI:
            parameters->amp2_ki = atof(val);
            break;
        case AMP2_KD:
            parameters->amp2_kd = atof(val);
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

  free(val);

  return FR_OK;
}