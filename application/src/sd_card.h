#ifndef SD_CARD_H
#define SD_CARD_H

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

#include "heater/heater.h"
#include "naatos_config.h"

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

/* SDC block device definition */
NRF_BLOCK_DEV_SDC_DEFINE(
    m_block_dev_sdc,
    NRF_BLOCK_DEV_SDC_CONFIG(
        SDC_SECTOR_SIZE,
        APP_SDCARD_CONFIG(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, SPI_SD_SS_PIN)),
    NFR_BLOCK_DEV_INFO_CONFIG("NAATOS", "SDC", "1.00"));

#define BLOCKDEV_LIST() ( \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev))

#ifndef SAMPLE_PREP_BOARD
typedef enum {
  SAMPLE_RATE,
  LOGGING_RATE,
  LOW_POWER_THRESHOLD,
  RECOVERY_POWER_THRESHOLD,
  ALERT_TIMEOUT_TIME,
  SAMPLE_VALID_TIMEOUT,
  MIN_RUN_ZONE_TEMP_C,
  MIN_RUN_ZONE_TEMP_EN,
  MAX_HEATER_PID_PWM,
  SAMPLE_COMPLETE_DELAY,

  MIN_WAIT_TIME_AFTER_VALVE,
  VALVE_ZONE_RUN_TIME,
  AMP_ZONE_RUN_TIME,
  VALVE_SETPOINT_C,
  AMP0_SETPOINT_C,
  AMP1_SETPOINT_C,
  AMP2_SETPOINT_C,
  VALVE_MAX_TEMP_C,
  AMP0_MAX_TEMP_C,
  AMP1_MAX_TEMP_C,
  AMP2_MAX_TEMP_C,
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
  OPTICAL_DISTANCE,
  VALVE_SETPOINT_C_2,
  AMP0_SETPOINT_C_2,
  AMP1_SETPOINT_C_2,
  AMP2_SETPOINT_C_2,
  VALVE_KP_2,
  VALVE_KI_2,
  VALVE_KD_2,
  AMP0_KP_2,
  AMP0_KI_2,
  AMP0_KD_2,
  AMP1_KP_2,
  AMP1_KI_2,
  AMP1_KD_2,
  AMP2_KP_2,
  AMP2_KI_2,
  AMP2_KD_2,
  MMDDYY,
  HHMMSS,
  SET_DATE_TIME,
  NUM_PARAMETERS
} naatos_config_params_t;
#else
typedef enum {
  SAMPLE_RATE,
  LOGGING_RATE,
  LOW_POWER_THRESHOLD,
  RECOVERY_POWER_THRESHOLD,
  ALERT_TIMEOUT_TIME,
  SAMPLE_VALID_TIMEOUT,
  MIN_RUN_ZONE_TEMP_C,
  MIN_RUN_ZONE_TEMP_EN,
  MAX_HEATER_PID_PWM,
  SAMPLE_COMPLETE_DELAY,

  CYCLE_1_RUN_TIME,
  CYCLE_2_RUN_TIME,
  RAMP_TO_TEMP_BEFORE_CYCLE_1_START,
  RAMP_TO_TEMP_BEFORE_CYCLE_2_START,
  RAMP_TO_TEMP_C1_TIMEOUT,
  RAMP_TO_TEMP_C2_TIMEOUT,
  HEATER_SETPOINT_1_C,
  HEATER_SETPOINT_2_C,
  HEATER_MAX_TEMP_C,
  HEATER_KP_1,
  HEATER_KI_1,
  HEATER_KD_1,
  HEATER_KP_2,
  HEATER_KI_2,
  HEATER_KD_2,
  MOTOR_KP_1,
  MOTOR_KI_1,
  MOTOR_KD_1,
  _MOTOR_SETPOINT_1,
  MOTOR_KP_2,
  MOTOR_KI_2,
  MOTOR_KD_2,
  _MOTOR_SETPOINT_2,
  RUN_MOTOR_1,
  RUN_HEATER_1,
  RUN_MOTOR_2,
  RUN_HEATER_2,
  MMDDYY,
  HHMMSS,
  SET_DATE_TIME,
  NUM_PARAMETERS
} naatos_config_params_t;
#endif

typedef struct {
  bool log_dir_needed;
  bool config_dir_needed;
} directories_needed_t;

extern bool sd_card_inited;

void init_sd_card(void);
void uninit_sd_card(void);
void sd_card_list_contents(void);
void create_naatos_directories(void);
FRESULT sd_card_create_log_file(const char *file_name);
FRESULT sd_card_mount(void);
FRESULT sd_card_unmount(void);
FRESULT sd_card_write_log_line(const char *logName, const char *writeBuff, uint32_t writeBuffSize);
FRESULT sd_card_reset_set_time_date(void);
FRESULT get_naatos_configuration_parameters(naatos_config_parameters *parameters);

double parse_double(const char *str, double default_value);
int parse_int(const char *str, int default_value);

#endif