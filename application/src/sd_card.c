#include "sd_card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
static diskio_blkdev_t drives[] = {DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev), NULL)};

// "private" functions
FRESULT remount_goto_logs_dir(void);
FRESULT check_for_config_file(void);
FRESULT remount_goto_config_dir(void);

// Initalize Function
void init_sd_card(void) {
  nrf_gpio_cfg(SPI_SCK_PIN,
      NRF_GPIO_PIN_DIR_OUTPUT,
      NRF_GPIO_PIN_INPUT_DISCONNECT,
      NRF_GPIO_PIN_NOPULL,
      NRF_GPIO_PIN_H0H1, // Require High Drive low/high level
      NRF_GPIO_PIN_NOSENSE);

  nrf_gpio_cfg(SPI_MOSI_PIN,
      NRF_GPIO_PIN_DIR_OUTPUT,
      NRF_GPIO_PIN_INPUT_DISCONNECT,
      NRF_GPIO_PIN_NOPULL,
      NRF_GPIO_PIN_H0H1, // Require High Drive low/high level
      NRF_GPIO_PIN_NOSENSE);

  disk_state = STA_NOINIT;

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
  printf("Capactity: %d MB\n", capacity);

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
  disk_state = 0;

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
  } else if (res != FR_EXIST) {
    printf("Unable to retreive config.txt from sd card.\n");
  }
}

// Create a new file under the log subdirectory ** NO NAMES WITH : ALLOWED **
FRESULT sd_card_create_log_file(const char *file_name) {
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
FRESULT sd_card_write_log_line(const char *logName, const char *writeBuff, uint32_t writeBuffSize) {
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
        printf("   <DIR>   %s\n", (uint32_t)fno.fname);
      } else {
        printf("%9lu  %s\n", fno.fsize, (uint32_t)fno.fname);
      }
    }
  } while (fno.fname[0]);
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
  // Write Alert Timeout (minutes)
  configBufferSize = sprintf(configBuffer, "alert_timeout_time_s:%0.2f\n", DEFAULT_ALERT_TIMEOUT_S);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  configBufferSize = sprintf(configBuffer, "sample_valid_timeout_s:%0.2f\n", DEFAULT_VALID_TIMEOUT_S);
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

  configBufferSize = sprintf(configBuffer, "min_run_zone_temp_en:%s\n", DEFAULT_MIN_RUN_ZONE_TEMP_EN ? "true" : "false");
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  configBufferSize = sprintf(configBuffer, "max_heater_pwm:%d\n", DEFAULT_MAX_HEATER_PID);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  configBufferSize = sprintf(configBuffer, "cycles_complete_delay:%d\n", DEFAULT_CYCLES_COMPLETE_DELAY_S);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

#ifndef SAMPLE_PREP_BOARD
  configBufferSize = sprintf(configBuffer, "min_wait_time_after_valve_s:%0.2f\n", DEFAULT_WAIT_TIME_AFTER_VALVE_S);
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
  // Write Amplification Zone Run Time
  configBufferSize = sprintf(configBuffer, "amp_zone_run_time:%d\n", DEFAULT_AMPLIFICATION_ZONE_ON_TIME);
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
  configBufferSize = sprintf(configBuffer, "amp0_setpoint:%0.2f\n", AMP0_SETPOINT);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "amp1_setpoint:%0.2f\n", AMP1_SETPOINT);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "amp2_setpoint:%0.2f\n", AMP2_SETPOINT);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Max Temperatures
  configBufferSize = sprintf(configBuffer, "valve_max_temp:%0.2f\n", DEFAULT_VALVE_MAX_TEMP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "amp0_max_temp:%0.2f\n", DEFAULT_AMP0_MAX_TEMP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "amp1_max_temp:%0.2f\n", DEFAULT_AMP1_MAX_TEMP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "amp2_max_temp:%0.2f\n", DEFAULT_AMP2_MAX_TEMP);
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
  // Write Optical Switch
  configBufferSize = sprintf(configBuffer, "optical_distace:%d\n", OPTICAL_TRIG_THRES);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Valve Zone Setpoint
  configBufferSize = sprintf(configBuffer, "valve_setpoint_2:%0.2f\n", VALVE_SETPOINT_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amplification Zone Setpoint
  configBufferSize = sprintf(configBuffer, "amp0_setpoint_2:%0.2f\n", AMP0_SETPOINT_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "amp1_setpoint_2:%0.2f\n", AMP1_SETPOINT_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "amp2_setpoint_2:%0.2f\n", AMP2_SETPOINT_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Valve Kp
  configBufferSize = sprintf(configBuffer, "valve_kp_2:%0.3f\n", V_KP_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Valve Ki
  configBufferSize = sprintf(configBuffer, "valve_ki_2:%0.3f\n", V_KI_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Valve Kd
  configBufferSize = sprintf(configBuffer, "valve_kd_2:%0.3f\n", V_KD_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp0 Kp
  configBufferSize = sprintf(configBuffer, "amp0_kp_2:%0.3f\n", A0_KP_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp0 Ki
  configBufferSize = sprintf(configBuffer, "amp0_ki_2:%0.3f\n", A0_KI_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp0 Kd
  configBufferSize = sprintf(configBuffer, "amp0_kd_2:%0.3f\n", A0_KD_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp1 Kp
  configBufferSize = sprintf(configBuffer, "amp1_kp_2:%0.3f\n", A1_KP_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp1 Ki
  configBufferSize = sprintf(configBuffer, "amp1_ki_2:%0.3f\n", A1_KI_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp1 Kd
  configBufferSize = sprintf(configBuffer, "amp1_kd_2:%0.3f\n", A1_KD_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp2 Kp
  configBufferSize = sprintf(configBuffer, "amp2_kp_2:%0.3f\n", A2_KP_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp2 Ki
  configBufferSize = sprintf(configBuffer, "amp2_ki_2:%0.3f\n", A2_KI_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Amp2 Kd
  configBufferSize = sprintf(configBuffer, "amp2_kd_2:%0.3f\n", A2_KD_2);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
#else
  configBufferSize = sprintf(configBuffer, "cycle_1_run_time_m:%0.2f\n", DEFAULT_CYCLE_1_RUNTIME);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  configBufferSize = sprintf(configBuffer, "cycle_2_run_time_m:%0.2f\n", DEFAULT_CYCLE_2_RUNTIME);
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

  // Write heater Zone Setpoint
  configBufferSize = sprintf(configBuffer, "heater_setpoint_1:%0.2f\n", DEFAULT_HEATER_SETPOINT);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  configBufferSize = sprintf(configBuffer, "heater_setpoint_2:%0.2f\n", DEFAULT_HEATER_SETPOINT);
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
  configBufferSize = sprintf(configBuffer, "heater_kp_1:%0.3f\n", H_KP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 1 Ki
  configBufferSize = sprintf(configBuffer, "heater_ki_1:%0.3f\n", H_KI);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 1 Kd
  configBufferSize = sprintf(configBuffer, "heater_kd_1:%0.3f\n", H_KD);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 2 Kp
  configBufferSize = sprintf(configBuffer, "heater_kp_2:%0.3f\n", H_KP);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 2 Ki
  configBufferSize = sprintf(configBuffer, "heater_ki_2:%0.3f\n", H_KI);
  res = f_write(&file, configBuffer, configBufferSize, &b_written);
  if (res != FR_OK) {
    return res;
  }
  // Write Heater 2 Kd
  configBufferSize = sprintf(configBuffer, "heater_kd_2:%0.3f\n", H_KD);
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
  configBufferSize = sprintf(configBuffer, "motor_ki_1:%0.3f\n", M_KI);
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
  configBufferSize = sprintf(configBuffer, "motor_ki_2:%0.3f\n", M_KI);
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
#endif

  //BOTH
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

FRESULT get_naatos_configuration_parameters(naatos_config_parameters *parameters) {
  char configBuffer[50];
  FRESULT res;
  char *pch;
  char val[50];
  int num;

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
        printf("Warning: Sample rate lower than minimum (0.048), setting sample rate to minimum.");
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
      parameters->recovery_power_thresh = parse_int(val, DEFAULT_RECOVERY_THRES);
      break;
    case MIN_RUN_ZONE_TEMP_C:
      parameters->min_run_zone_temp = parse_double(val, DEFAULT_MIN_RUN_ZONE_TEMP);
      break;
    case MIN_RUN_ZONE_TEMP_EN:
      num = strcmp(val, "true");
      parameters->min_run_zone_temp_en = num ? false : true;
      break;
    case ALERT_TIMEOUT_TIME:
      parameters->alert_timeout_time_s = parse_double(val, DEFAULT_ALERT_TIMEOUT_S);
      break;
    case SAMPLE_VALID_TIMEOUT:
      parameters->sample_valid_timeout_s = parse_double(val, DEFAULT_VALID_TIMEOUT_S);
      break;
    case SAMPLE_COMPLETE_DELAY:
      parameters->sample_complete_delay_s = parse_int(val, DEFAULT_CYCLES_COMPLETE_DELAY_S);
      break;
      break;
    case MAX_HEATER_PID_PWM:
      parameters->max_heater_pid_pwm = parse_int(val, DEFAULT_MAX_HEATER_PID);
      break;
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
#ifndef SAMPLE_PREP_BOARD
    case VALVE_ZONE_RUN_TIME:
      parameters->valve_zone_run_time_m = atoi(val);
      break;
    case AMP_ZONE_RUN_TIME:
      parameters->amplification_zone_run_time_m = atoi(val);
      break;
    case MIN_WAIT_TIME_AFTER_VALVE:
      parameters->min_wait_time_after_valve_s = atoi(val);
      break;
    case VALVE_SETPOINT_C:
      parameters->valve_setpoint = atof(val);
      break;
    case AMP0_SETPOINT_C:
      parameters->amp0_setpoint = atof(val);
      break;
    case AMP1_SETPOINT_C:
      parameters->amp1_setpoint = atof(val);
      break;
    case AMP2_SETPOINT_C:
      parameters->amp2_setpoint = atof(val);
      break;
    case VALVE_MAX_TEMP_C:
      parameters->valve_max_temp = atof(val);
      break;
    case AMP0_MAX_TEMP_C:
      parameters->amp0_max_temp = atof(val);
      break;
    case AMP1_MAX_TEMP_C:
      parameters->amp1_max_temp = atof(val);
      break;
    case AMP2_MAX_TEMP_C:
      parameters->amp2_max_temp = atof(val);
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
    case OPTICAL_DISTANCE:
      parameters->optical_distance = atoi(val);
      break;
    case VALVE_SETPOINT_C_2:
      parameters->valve_setpoint_2 = atof(val);
      break;
    case AMP0_SETPOINT_C_2:
      parameters->amp0_setpoint_2 = atof(val);
      break;
    case AMP1_SETPOINT_C_2:
      parameters->amp1_setpoint_2 = atof(val);
      break;
    case AMP2_SETPOINT_C_2:
      parameters->amp2_setpoint_2 = atof(val);
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
    case AMP0_KP_2:
      parameters->amp0_kp_2 = atof(val);
      break;
    case AMP0_KI_2:
      parameters->amp0_ki_2 = atof(val);
      break;
    case AMP0_KD_2:
      parameters->amp0_kd_2 = atof(val);
      break;
    case AMP1_KP_2:
      parameters->amp1_kp_2 = atof(val);
      break;
    case AMP1_KI_2:
      parameters->amp1_ki_2 = atof(val);
      break;
    case AMP1_KD_2:
      parameters->amp1_kd_2 = atof(val);
      break;
    case AMP2_KP_2:
      parameters->amp2_kp_2 = atof(val);
      break;
    case AMP2_KI_2:
      parameters->amp2_ki_2 = atof(val);
      break;
    case AMP2_KD_2:
      parameters->amp2_kd_2 = atof(val);
      break;
#else
    case CYCLE_1_RUN_TIME:
      parameters->cycle_1_run_time_m = parse_int(val, DEFAULT_CYCLE_1_RUNTIME);
      break;
    case CYCLE_2_RUN_TIME:
      parameters->cycle_2_run_time_m = parse_int(val, DEFAULT_CYCLE_2_RUNTIME);
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
    case HEATER_SETPOINT_1_C:
      parameters->heater_setpoint_1 = parse_double(val, DEFAULT_HEATER_SETPOINT);
      break;
    case HEATER_SETPOINT_2_C:
      parameters->heater_setpoint_2 = parse_double(val, DEFAULT_HEATER_SETPOINT);
      break;
    case HEATER_MAX_TEMP_C:
      parameters->heater_max_temp = parse_double(val, DEFAULT_MAX_TEMP);
      break;
    case HEATER_KP_1:
      parameters->heater_kp_1 = parse_double(val, H_KP);
      break;
    case HEATER_KI_1:
      parameters->heater_ki_1 = parse_double(val, H_KI);
      break;
    case HEATER_KD_1:
      parameters->heater_kd_1 = parse_double(val, H_KD);
      break;
    case HEATER_KP_2:
      parameters->heater_kp_2 = parse_double(val, H_KP);
      break;
    case HEATER_KI_2:
      parameters->heater_ki_2 = parse_double(val, H_KI);
      break;
    case HEATER_KD_2:
      parameters->heater_kd_2 = parse_double(val, H_KD);
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
#endif

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

// Sets the set_time_date variable to "false"
FRESULT sd_card_reset_set_time_date(void) {
  FRESULT res;
  const char *var = "set_time_date";
  UINT br, bw;            // File read/write count
  char buffer[1024];      // Buffer to hold file content
  char temp_buffer[1024]; // Temporary buffer for modified content

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