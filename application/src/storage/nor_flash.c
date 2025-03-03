#include "nor_flash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if SAMPLE_PREP_REV_B || POWER_MODULE_REV_B

/* NOR Flash Variables */
static FATFS fs;
static DIR dir;
static FILINFO fno;
uint32_t bytes_written;
FRESULT ff_result;
DSTATUS disk_state = STA_NOINIT;
uint32_t blocks_per_mb;
uint32_t capacity;

bool nor_flash_inited = false;
bool nor_flash_could_mount_filesystem = true;

// Initialize FATFS disk I/O interface by providing the block device.
static diskio_blkdev_t drives[] =
{
    DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev), NULL)
};

FRESULT init_nor_flash(void) {
  char buff[100];
  disk_state = STA_NOINIT;
  nor_flash_could_mount_filesystem = false;

  memset(&fs, 0, sizeof(FATFS));

  // Register the NOR Flash Driver
  diskio_blockdev_register(drives, ARRAY_SIZE(drives));
  
  // Initalize the disk
  disk_state = disk_initialize(0);
  if (disk_state) {
      sprintf(buff, "Disk initialization failed. State: %d", disk_state);
      send_debug_log_message(buff);
      return disk_state;
  }
  send_debug_log_message("NOR Flash Storage initialized");

  // Mount the NOR Flash Volume
  ff_result = mount_nor_flash();
  if (ff_result == FR_NO_FILESYSTEM) {
    ff_result = nor_flash_fatfs_mkfs();
    if (ff_result != FR_OK) {
      send_debug_log_message("init_nor_flash() Unable to mkfs!");
      return ff_result;
    }
  }
  else if (ff_result != FR_OK) {
    send_debug_log_message("Unable to mount NOR flash!");
    nor_flash_could_mount_filesystem = false;
    return ff_result;
  }
  
  nor_flash_could_mount_filesystem = true;

  // Show contents
  nor_flash_list_contents();
}

void uninit_nor_flash(void) {
  disk_state = disk_uninitialize(0);

  app_usbd_ep_disable(ENDPOINT_LIST());

  send_debug_log_message("NOR Flash Storage Uninitialized.");
}

FRESULT nor_flash_fatfs_mkfs(void) {
  FRESULT ff_result;
  
  send_debug_log_message("Creating filesystem...");
  static uint8_t buf[512];
  ff_result = f_mkfs("", FM_ANY, 0, buf, sizeof(buf));
  if (ff_result != FR_OK) {
      send_debug_log_message("Mkfs failed.");
      return ff_result;
  }

  // Mount the NOR Flash Volume
  ff_result = mount_nor_flash();
  if (ff_result != FR_OK) {
    send_debug_log_message("Unable to mount NOR flash after formatting!");
    nor_flash_could_mount_filesystem = false;
    return ff_result;
  } else{
    nor_flash_could_mount_filesystem = true;
  }

}

FRESULT mount_nor_flash(void) {
  return f_mount(&fs, "", 1);
}

FRESULT unmount_nor_flash(void) {
  return f_mount(0, "", 0);
}

void nor_flash_list_contents(void) {
  char buff[50];

  send_debug_log_message("\r\n Listing NOR Flash directory: /");
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
        sprintf(buff, "   <DIR>   %s", (uint32_t)fno.fname);
        send_debug_log_message(buff);
      } else {
        sprintf("%9lu  %s", fno.fsize, (uint32_t)fno.fname);
        send_debug_log_message(buff);
      }
    }
  } while (fno.fname[0]);
}

#endif