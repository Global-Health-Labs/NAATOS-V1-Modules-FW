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
static diskio_blkdev_t drives[] =
{
    DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev), NULL)
};

void init_nor_flash(void) {
  disk_state = STA_NOINIT;

  memset(&fs, 0, sizeof(FATFS));

  // Register the NOR Flash Driver
  diskio_blockdev_register(drives, ARRAY_SIZE(drives));
  
  // Initalize the disk
  disk_state = disk_initialize(0);
  if (disk_state) {
      printf("Disk initialization failed. State: %d\n", disk_state);
      return;
  }
  printf("NOR Flash Storage initialized\n");

  // Mount the NOR Flash Volume
  ff_result = mount_nor_flash();
  if (ff_result != FR_OK) {
    printf("Unable to mount NOR flash!\n");
    return;
  }

  // Show contents
  nor_flash_list_contents();
}

void uninit_nor_flash(void) {
  disk_state = disk_uninitialize(0);

  app_usbd_ep_disable(ENDPOINT_LIST());

  printf("NOR Flash Storage Uninitialized.\n");
}

FRESULT mount_nor_flash(void) {
  return f_mount(&fs, "", 1);
}

FRESULT unmount_nor_flash(void) {
  return f_mount(0, "", 0);
}

void nor_flash_list_contents(void) {
  printf("\r\n Listing NOR Flash directory: /\n");
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