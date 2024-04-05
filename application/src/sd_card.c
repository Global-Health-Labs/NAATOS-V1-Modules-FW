#include "sd_card.h"

/* SDC block device definition */
NRF_BLOCK_DEV_SDC_DEFINE(
        m_block_dev_sdc,
        NRF_BLOCK_DEV_SDC_CONFIG(
                SDC_SECTOR_SIZE,
                APP_SDCARD_CONFIG(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, SPI_SD_SS_PIN)
         ),
         NFR_BLOCK_DEV_INFO_CONFIG("NAATOS", "SDC", "1.00")
);

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

// Initialize FATFS disk I/O interface by providing the block device.
static diskio_blkdev_t drives[] = { DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev), NULL) };

// Initalize Function
void init_sd_card(void) {
  // Register the drives we have -- Should only be the one
  diskio_blockdev_register(drives, ARRAY_SIZE(drives));
  
  // Initalize the disk on the SD
  printf("Initializing disk 0 (SDC)...\n");
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
  
  // Mount the SD card volume
  printf("Mounting volume...\n");
  ff_result = f_mount(&fs, "", 1);
  if (ff_result) {
    printf("Mount failed.\n");
    return;
  }
  else {
    printf("Mount successful.\n");
  }

  // Create directories if needed
  create_naatos_directories();
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
}

// Create a new file under the log subdirectory
FRESULT sd_card_create_log_file(const char * file_name) {
  FRESULT res;

  // Change directory into logs
  f_chdir(LOGS_DIR);
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
  f_close(&file);
  // Change back directories
  f_chdir("..");

  return res;
}

// Prints directories seen on the sd card
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