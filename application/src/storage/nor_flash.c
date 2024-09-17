#include "nor_flash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Initialize FATFS disk I/O interface by providing the block device.
static diskio_blkdev_t drives[] = {DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev), NULL)};

void init_nor_flash(void) {
  disk_state = STA_NOINIT;

  memset(&fs, 0, sizeof(FATFS));

  // Register the NOR Flash Driver
  diskio_blockdev_register(drives, ARRAY_SIZE(drives));
  
  // Initalize the disk
  disk_state = disk_initialize(0);
  if (disk_state) {
      printf("Disk initialization failed.");
      return;
  }

  // Mount the NOR Flash Volume
  ff_result = mount_nor_flash();
  if (ff_result != FR_OK) {
    printf("Unable to mount NOR flash!\n");
  }
}

void uninit_nor_flash(void) {
  UNUSED_RETURN_VALUE(disk_uninitialize(0));
}

FRESULT mount_nor_flash(void) {
  return f_mount(&fs, "", 1);
}

FRESULT unmount_nor_flash(void) {
  return f_mount(0, "", 0);
}