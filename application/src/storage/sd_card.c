#include "sd_card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../logger/logger.h"

/* SD Card Variables */
static FATFS fs;
uint32_t sd_bytes_written;
FRESULT sd_ff_result;
DSTATUS sd_disk_state = STA_NOINIT;
uint32_t sd_blocks_per_mb;
uint32_t sd_capacity;

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

  char buff[50];

  sd_disk_state = STA_NOINIT;

  memset(&fs, 0, sizeof(FATFS));

  // Register the drives we have
  diskio_blockdev_register(drives, ARRAY_SIZE(drives));

  // Initalize the disk on the SD
  for (uint32_t retries = 3; retries && sd_disk_state; --retries) {
    sd_disk_state = disk_initialize(0);
  }
  if (sd_disk_state) {
    send_debug_log_message("SD Card initialization failed.");
    return;
  }

  // Get SD Card Specifications
  sd_blocks_per_mb = (1024uL * 1024uL) / m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_size;
  sd_capacity = m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_count / sd_blocks_per_mb;
  sprintf(buff, "Capactity: %d MB", sd_capacity);
  send_debug_log_message(buff);

  // Mount SD Card
  sd_ff_result = sd_card_mount();
  if (sd_ff_result != FR_OK) {
    send_debug_log_message("Unable to mount SD Card!");
  }

  sd_card_inited = true;
}

void uninit_sd_card(void) {
  sd_disk_state = 0;

  UNUSED_RETURN_VALUE(sd_card_unmount());
  UNUSED_RETURN_VALUE(disk_uninitialize(0));

  sd_card_inited = false;
  send_debug_log_message("SD Card Uninitalized.");
}

// Mount the SD card volume
FRESULT sd_card_mount(void) {
  return f_mount(&fs, "", 1);
}

// Mount the SD card volume ** Dont believe this is working right now
FRESULT sd_card_unmount(void) {
  return f_mount(0, "", 0);
}

// Prints directories seen on the sd card -- Was used during debug of sd card, only prints directories and files at root
/* Depreciated, keeping in here incase we want to be able to list contents of the file system in the future.
void sd_card_list_contents(void) {
  send_debug_log_message("\r\n Listing directory: /");
  // Open Root Directory
  ff_result = f_opendir(&dir, "/");
  if (ff_result) {
    send_debug_log_message("Directory listing failed!");
    return;
  }

  // Get the other directories
  do {
    ff_result = f_readdir(&dir, &fno);
    if (ff_result != FR_OK) {
      send_debug_log_message("Directory read failed.");
      return;
    }

    if (fno.fname[0]) {
      if (fno.fattrib & AM_DIR) {
        send_debug_log_message("   <DIR>   %s", (uint32_t)fno.fname);
      } else {
        send_debug_log_message("%9lu  %s", fno.fsize, (uint32_t)fno.fname);
      }
    }
  } while (fno.fname[0]);
}
*/