#ifndef  SD_CARD_H
#define  SD_CARD_H

#include <string.h>

#include "nrf.h"
#include "bsp.h"
#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "naatos_config.h"
#include "heater.h"

#define LOGS_DIR      "logs"
#define CONFIG_DIR    "config"

#define NAATOS_CONFIG_FILE  "config.txt"

#define CSV_HEADER        "Time,ValveTemp,Amp0Temp,Amp1Temp,Amp2Temp,Batt,Event\n"
#define CSV_HEADER_SIZE   53

typedef enum {
  SAMPLE_RATE,
  LOGGING_RATE,
  VALVE_ZONE_RUN_TIME,
  AMP_ZONE_RUN_TIME,
  LOW_POWER_THRESHOLD,
  VALVE_SETPOINT_C,
  AMP_SETPOINT_C,
  VALVE_KP,
  VALVE_KI,
  VALVE_KD,
  AMP0_KP,
  AMP0_KI,
  AMP0_KD,
  AMP1_KP,
  AMP1_KI,
  AMP1_KD,
  AMP2_KP,
  AMP2_KI,
  AMP2_KD,
  NUM_PARAMETERS
} naatos_config_params_t;

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
FRESULT get_naatos_configuration_parameters(naatos_config_parameters * parameters);

#endif