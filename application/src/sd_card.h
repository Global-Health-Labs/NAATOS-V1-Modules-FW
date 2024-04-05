#ifndef  SD_CARD_H
#define  SD_CARD_H

#include "nrf.h"
#include "bsp.h"
#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "naatos_config.h"

#define LOGS_DIR      "logs"
#define CONFIG_DIR    "config"

#define NAATOS_CONFIG_FILE  "config.txt"

#define CSV_HEADER        "Time,ValveTemp,Amp0Temp,Amp1Temp,Amp2Temp,Batt,Event"
#define CSV_HEADER_SIZE   52

typedef struct {
  bool log_dir_needed;
  bool config_dir_needed;
} directories_needed_t;

void init_sd_card(void);
void sd_card_list_contents(void); 
void create_naatos_directories(void);
FRESULT sd_card_create_log_file(const char * file_name);
FRESULT sd_card_mount(void);
FRESULT sd_card_unmount(void);
FRESULT sd_card_write_log_line(const char * logName, const char * writeBuff, uint32_t writeBuffSize);

#endif