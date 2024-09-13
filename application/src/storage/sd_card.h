#pragma once

#include <string.h>

#include "bsp.h"
#include "diskio_blkdev.h"
#include "ff.h"
#include "nrf.h"
#include "nrf_block_dev.h"
#include "nrf_block_dev_empty.h"
#include "nrf_block_dev_qspi.h"
#include "nrf_block_dev_ram.h"
#include "nrf_block_dev_sdc.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "../heater/heater.h"
#include "../naatos_config.h"

/* SDC block device definition */
NRF_BLOCK_DEV_SDC_DEFINE(
    m_block_dev_sdc,
    NRF_BLOCK_DEV_SDC_CONFIG(
        SDC_SECTOR_SIZE,
        APP_SDCARD_CONFIG(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, SPI_SD_SS_PIN)),
    NFR_BLOCK_DEV_INFO_CONFIG("NAATOS", "SDC", "1.00"));

extern bool sd_card_inited;

void init_sd_card(void);
void uninit_sd_card(void);
void sd_card_list_contents(void);
FRESULT sd_card_mount(void);
FRESULT sd_card_unmount(void);
