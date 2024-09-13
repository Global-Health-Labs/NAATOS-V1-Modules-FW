#ifndef NOR_FLASH_H
#define NOR_FLASH_H

#include <string.h>

#include "bsp.h"
#include "diskio_blkdev.h"
#include "ff.h"
#include "nrf.h"

#include "nrf_block_dev.h"
#include "nrf_block_dev_qspi.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "../heater/heater.h"
#include "../naatos_config.h"

/**
 * @brief  QSPI block device definition
 */
NRF_BLOCK_DEV_QSPI_DEFINE(
    m_block_dev_qspi,
    NRF_BLOCK_DEV_QSPI_CONFIG(
        512,
        NRF_BLOCK_DEV_QSPI_FLAG_CACHE_WRITEBACK,
        NRF_DRV_QSPI_DEFAULT_CONFIG),
    NFR_BLOCK_DEV_INFO_CONFIG("Nordic", "QSPI", "1.00"));

extern bool nor_flash_inited;

void init_nor_flash(void);
void uninit_nor_flash(void);
FRESULT mount_nor_flash(void);
FRESULT unmount_nor_flash(void);

#endif