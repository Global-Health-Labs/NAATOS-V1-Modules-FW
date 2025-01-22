#pragma once

/* This file will contain the shared storage elements between the SD card
 * and the nor flash. It can also be used to set which interface should be
 * used for storage.
*/

#include "sd_card.h"
#include "nor_flash.h"
#include "../naatos_config.h"

#ifdef SAMPLE_PREP_BOARD
#if SAMPLE_PREP_REV_A
#define USE_SD_CARD     1
#define USE_NOR_FLASH   0
#else
#define USE_SD_CARD     0
#define USE_NOR_FLASH   1
#endif
#else
#if POWER_MODULE_REV_A
#define USE_SD_CARD     1
#define USE_NOR_FLASH   0
#else
#define USE_SD_CARD     0
#define USE_NOR_FLASH   1
#endif
#endif

#if (!USE_SD_CARD && !USE_NOR_FLASH)
#define USE_NOR_FLASH   1
#endif

#define LOGS_DIR "logs"
#define CONFIG_DIR "config"

//#define NAATOS_CONFIG_FILE "config_v2.0a.txt"
#define NAATOS_CONFIG_FILE "config_v" VERSION ".txt"
#define FIRST_CYCLE_CONFIG_FILE "cycle_config_1.txt"

#ifdef SAMPLE_PREP_BOARD
#define CSV_HEADER "Time,HeaterTemp,HeaterPWM,MotorSpeed,MotorPWM,Battery,BatteryV,BatteryT,Event\r\n"
#define CSV_HEADER_SIZE 79
#else
#define CSV_HEADER "Time,ValveTemp,ValvePWM,AmpTemp,AmpPWM,Batt,BatteryV,BatteryT,Event\r\n"
#define CSV_HEADER_SIZE 70
#endif

#if USE_SD_CARD
#define BLOCKDEV_LIST() ( \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev))
#endif 

#if USE_NOR_FLASH
#define BLOCKDEV_LIST() ( \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev))
#endif

typedef enum {
  SAMPLE_RATE,
  LOGGING_RATE,
  LOW_POWER_THRESHOLD,
  RECOVERY_POWER_THRESHOLD,
  SAMPLE_VALID_TIMEOUT,
  ALERT_TIMEOUT_TIME,
  DEBUG_TO_COM_EN,
#ifndef SAMPLE_PREP_BOARD
  OPTICAL_DISTANCE,
  MAX_AMP_PID_PWM,
  MAX_VALVE_PID_PWM,
  VALVE_MAX_TEMP_C,
  AMP_MAX_TEMP_C,
#else 
  HEATER_MAX_TEMP_C,
  MAX_HEATER_PID_PWM,
  MOTOR_SWTICH_CCW_CW,
  HAL_SENSOR_THRESH,
  MOTOR_STALL_PERCENT,
  MOTOR_STALL_PWM,
  MOTOR_STALL_ENABLE,
#endif
  MMDDYY,
  HHMMSS,
  SET_DATE_TIME,
  NUM_MASTER_CONFIG_PARAMETERS
} naatos_config_params_t;

typedef enum {
  CYCLE_RUN_TIME,
  MIN_RUN_ZONE_TEMP_C,
  MIN_RUN_ZONE_TEMP_EN,
  CYCLE_DELAY_TIME,
  RAMP_TO_TEMP_BEFORE_CYCLE_START,
  RAMP_TO_TEMP_TIMEOUT,
#ifndef SAMPLE_PREP_BOARD
  AMP_SETPOINT,
  VALVE_SETPOINT,
  RUN_AMP,
  RUN_VALVE,
  AMP_KP,
  AMP_KI,
  AMP_KD,
  VALVE_KP,
  VALVE_KI,
  VALVE_KD,
#else 
  HEATER_SETPOINT,
  MOTOR_SETPOINT,
  RUN_HEATER,
  RUN_MOTOR,
  HEATER_KP,
  HEATER_KI,
  HEATER_KD,
  MOTOR_KP,
  MOTOR_KI,
  MOTOR_KD,
#endif
  NUM_CYCLE_CONFIG_PARAMS,
} cycle_config_params_t;

typedef struct {
  bool log_dir_needed;
  bool config_dir_needed;
} directories_needed_t;

extern bool naatos_storage_initalized;

// NAATOS Storage Functions
void init_naatos_storage(void);
void uninit_naatos_storage(void);
void mount_storage(void);
void unmount_storage(void);
void create_naatos_directories(void);
FRESULT check_for_naatos_config_file(void);
FRESULT check_for_cycle_config_files(void);
FRESULT get_naatos_configuration_parameters(naatos_config_parameters *parameters);
FRESULT get_cycle_configurations(cycle_config_parameters *cycle_configurations_arr);
FRESULT reset_set_time_date(void);
FRESULT create_log_file(const char *file_name);
FRESULT write_log_line(const char *logName, const char *writeBuff, uint32_t writeBuffSize);
FRESULT remount_goto_logs_dir(void);

// Parse Helpers
double parse_double(const char *str, double default_value);
int parse_int(const char *str, int default_value);