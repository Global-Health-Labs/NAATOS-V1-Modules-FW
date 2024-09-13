#pragma once

/* This file will contain the shared storage elements between the SD card
 * and the nor flash. It can also be used to set which interface should be
 * used for storage.
*/

#include "sd_card.h"
#include "nor_flash.h"
#include "../naatos_config.h"

#define USE_SD_CARD     0
#define USE_NOR_FLASH   1

#if (!USE_SD_CARD && !USE_NOR_FLASH)
#warning One storage peripheral must be used!
#endif

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

// NAATOS Storage Functions
void init_naatos_storage(void);
void uninit_naatos_storage(void);
void mount_storage(void);
void unmount_storage(void);
void create_naatos_directories(void);
FRESULT check_for_config_file(void);
FRESULT get_naatos_configuration_parameters(naatos_config_parameters *parameters);
FRESULT reset_set_time_date(void);

// Parse Helpers
double parse_double(const char *str, double default_value);
int parse_int(const char *str, int default_value);