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

#define LOGS_DIR "logs"
#define CONFIG_DIR "config"

#define NAATOS_CONFIG_FILE "config_v2.0a.txt"

#ifdef SAMPLE_PREP_BOARD
#define CSV_HEADER "Time,HeaterTemp,HeaterPWM,MotorSpeed,MotorPWM,Battery,Event\r\n"
#define CSV_HEADER_SIZE 61
#else
#define CSV_HEADER "Time,ValveTemp,ValvePWM,Amp0Temp,Amp0PWM,Amp1Temp,Amp1PWM,Amp2Temp,Amp2PWM,Batt,Event\r\n"
#define CSV_HEADER_SIZE 86
#endif

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

#endif