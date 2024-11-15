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

#define NAATOS_CONFIG_FILE "config_v2.0a.txt"

#ifdef SAMPLE_PREP_BOARD
#define CSV_HEADER "Time,HeaterTemp,HeaterPWM,MotorSpeed,MotorPWM,Battery,BatteryV,Event\r\n"
#define CSV_HEADER_SIZE 70
#else
#define CSV_HEADER "Time,ValveTemp,ValvePWM,AmpTemp,AmpPWM,Batt,Event\r\n"
#define CSV_HEADER_SIZE 53
#endif

#if USE_SD_CARD
#define BLOCKDEV_LIST() ( \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev))
#endif 

#if USE_NOR_FLASH
#define BLOCKDEV_LIST() ( \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev))
#endif

#ifndef SAMPLE_PREP_BOARD
typedef enum {
  SAMPLE_RATE,
  LOGGING_RATE,
  CYCLE_ONE_RUN_TIME,
  CYCLE_TWO_RUN_TIME,
  LOW_POWER_THRESHOLD,
  RECOVERY_POWER_THRESHOLD,
  SAMPLE_VALID_TIMEOUT,
  SAMPLE_COMPLETE_DELAY,
  ALERT_TIMEOUT_TIME,
  MIN_RUN_ZONE_TEMP_C,
  MIN_RUN_ZONE_TEMP_EN,
  DEBUG_TO_COM_EN,
  OPTICAL_DISTANCE,
  MIN_WAIT_TIME_AFTER_CYCLE,
  MAX_AMP_PID_PWM,
  MAX_VALVE_PID_PWM,
  VALVE_MAX_TEMP_C,
  AMP_MAX_TEMP_C,
  AMP_SETPOINT_C_1,
  VALVE_SETPOINT_C_1,
  VALVE_KP_1,
  VALVE_KI_1,
  VALVE_KD_1,
  AMP_KP_1,
  AMP_KI_1,
  AMP_KD_1,
  VALVE_SETPOINT_C_2,
  AMP_SETPOINT_C_2,
  VALVE_KP_2,
  VALVE_KI_2,
  VALVE_KD_2,
  AMP_KP_2,
  AMP_KI_2,
  AMP_KD_2,
  MMDDYY,
  HHMMSS,
  SET_DATE_TIME,
  NUM_PARAMETERS
} naatos_config_params_t;
#else
typedef enum {
  SAMPLE_RATE,
  LOGGING_RATE,
  CYCLE_ONE_RUN_TIME,
  CYCLE_TWO_RUN_TIME,
  LOW_POWER_THRESHOLD,
  RECOVERY_POWER_THRESHOLD,
  SAMPLE_VALID_TIMEOUT,
  SAMPLE_COMPLETE_DELAY,
  ALERT_TIMEOUT_TIME,
  MIN_RUN_ZONE_TEMP_C,
  MIN_RUN_ZONE_TEMP_EN,
  DEBUG_TO_COM_EN,
  MAX_HEATER_PID_PWM,
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
  MOTOR_SWTICH_CCW_CW,
  _MOTOR_SETPOINT_1,
  MOTOR_KP_2,
  MOTOR_KI_2,
  MOTOR_KD_2,
  _MOTOR_SETPOINT_2,
  RUN_MOTOR_1,
  RUN_HEATER_1,
  RUN_MOTOR_2,
  RUN_HEATER_2,
  HAL_SENSOR_THRESH,
  MOTOR_STALL_PERCENT,
  MOTOR_STALL_ENABLE,
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

extern bool naatos_storage_initalized;

// NAATOS Storage Functions
void init_naatos_storage(void);
void uninit_naatos_storage(void);
void mount_storage(void);
void unmount_storage(void);
void create_naatos_directories(void);
FRESULT check_for_config_file(void);
FRESULT get_naatos_configuration_parameters(naatos_config_parameters *parameters);
FRESULT reset_set_time_date(void);
FRESULT create_log_file(const char *file_name);
FRESULT write_log_line(const char *logName, const char *writeBuff, uint32_t writeBuffSize);
FRESULT remount_goto_logs_dir(void);

// Parse Helpers
double parse_double(const char *str, double default_value);
int parse_int(const char *str, int default_value);