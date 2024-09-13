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
    return;
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
  return f_mount(0, "", 0);
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

