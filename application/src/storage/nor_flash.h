#pragma once

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
#define EXTERNAL_1G_MEM_CONFIG                                        \
{                                                                       \
    .xip_offset  = NRFX_QSPI_CONFIG_XIP_OFFSET,                         \
    .pins = {                                                           \
       .sck_pin     = QSPI_CLK_PIN,                                \
       .csn_pin     = QSPI_CS_PIN,                                \
       .io0_pin     = QSPI_IO0_PIN,                                \
       .io1_pin     = QSPI_IO1_PIN,                                \
       .io2_pin     = QSPI_IO2_PIN,                                \
       .io3_pin     = QSPI_IO3_PIN,                                \
    },                                                                  \
    .prot_if = {                                                        \
        .readoc     = (nrf_qspi_readoc_t)NRFX_QSPI_CONFIG_READOC,       \
        .writeoc    = (nrf_qspi_writeoc_t)NRFX_QSPI_CONFIG_WRITEOC,     \
        .addrmode   = (nrf_qspi_addrmode_t)NRFX_QSPI_CONFIG_ADDRMODE,   \
        .dpmconfig  = false,                                            \
    },                                                                  \
    .phy_if = {                                                         \
        .sck_delay  = (uint8_t)NRFX_QSPI_CONFIG_SCK_DELAY,              \
        .dpmen      = false,                                            \
        .spi_mode   = (nrf_qspi_spi_mode_t)NRFX_QSPI_CONFIG_MODE,       \
        .sck_freq   = (nrf_qspi_frequency_t)NRFX_QSPI_CONFIG_FREQUENCY, \
    },                                                                  \
    .irq_priority   = (uint8_t)NRFX_QSPI_CONFIG_IRQ_PRIORITY,           \
}
NRF_BLOCK_DEV_QSPI_DEFINE(
    m_block_dev_qspi,
    NRF_BLOCK_DEV_QSPI_CONFIG(
        512,
        NRF_BLOCK_DEV_QSPI_FLAG_CACHE_WRITEBACK,
        EXTERNAL_1G_MEM_CONFIG
     ),
     NFR_BLOCK_DEV_INFO_CONFIG("Nordic", "QSPI", "1.00")
);

extern bool nor_flash_inited;

void init_nor_flash(void);
void uninit_nor_flash(void);
FRESULT mount_nor_flash(void);
FRESULT unmount_nor_flash(void);
void nor_flash_list_contents(void);
