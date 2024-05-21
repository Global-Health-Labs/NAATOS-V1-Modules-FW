#ifndef  SD_CARD_H
#define  SD_CARD_H

#include <string.h>

#include "nrf.h"
#include "bsp.h"
#include "ff.h"
#include "diskio_blkdev.h"

#include "nrf_block_dev.h"
#include "nrf_block_dev_sdc.h"
#include "nrf_block_dev_ram.h"
#include "nrf_block_dev_empty.h"
#include "nrf_block_dev_qspi.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "naatos_config.h"
#include "heater.h"

#define LOGS_DIR      "logs"
#define CONFIG_DIR    "config"

#define NAATOS_CONFIG_FILE  "config.txt"

#define CSV_HEADER        "Time,ValveTemp,ValvePWM,Amp0Temp,Amp0PWM,Amp1Temp,Amp1PWM,Amp2Temp,Amp2PWM,Batt,Event\n"
#define CSV_HEADER_SIZE   86

/**
 * @brief  QSPI block device definition
 */
NRF_BLOCK_DEV_QSPI_DEFINE(
    m_block_dev_qspi,
    NRF_BLOCK_DEV_QSPI_CONFIG(
        512,
        NRF_BLOCK_DEV_QSPI_FLAG_CACHE_WRITEBACK,
        NRF_DRV_QSPI_DEFAULT_CONFIG
     ),
     NFR_BLOCK_DEV_INFO_CONFIG("Nordic", "QSPI", "1.00")
);

/* SDC block device definition */
NRF_BLOCK_DEV_SDC_DEFINE(
        m_block_dev_sdc,
        NRF_BLOCK_DEV_SDC_CONFIG(
                SDC_SECTOR_SIZE,
                APP_SDCARD_CONFIG(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, SPI_SD_SS_PIN)
         ),
         NFR_BLOCK_DEV_INFO_CONFIG("NAATOS", "SDC", "1.00")
);

#define BLOCKDEV_LIST() (                                   \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev)      \
)

typedef enum {
  SAMPLE_RATE,
  LOGGING_RATE,
  VALVE_ZONE_RUN_TIME,
  AMP_ZONE_RUN_TIME,
  LOW_POWER_THRESHOLD,
  VALVE_SETPOINT_C,
  AMP0_SETPOINT_C,
  AMP1_SETPOINT_C,
  AMP2_SETPOINT_C,
  VALVE_MAX_TEMP_C,
  AMP0_MAX_TEMP_C,
  AMP1_MAX_TEMP_C,
  AMP2_MAX_TEMP_C,
  MIN_RUN_ZONE_TEMP_C,
  MIN_RUN_ZONE_TEMP_EN,
  ALERT_TIMEOUT_TIME,
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

extern bool sd_card_inited;

void init_sd_card(void);
void uninit_sd_card(void);
void sd_card_list_contents(void); 
void create_naatos_directories(void);
FRESULT sd_card_create_log_file(const char * file_name);
FRESULT sd_card_mount(void);
FRESULT sd_card_unmount(void);
FRESULT sd_card_write_log_line(const char * logName, const char * writeBuff, uint32_t writeBuffSize);
FRESULT get_naatos_configuration_parameters(naatos_config_parameters * parameters);

#endif