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

#define logs_directory    "logs"
#define config_directory  "config"

typedef struct {
  bool log_dir_needed;
  bool config_dir_needed;
} directories_needed_t;

void init_sd_card(void);
void sd_card_list_directories(void); 
void create_naatos_directories();

#endif