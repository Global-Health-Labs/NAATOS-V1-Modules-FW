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
#define SECOND_CYCLE_CONFIG_FILE "cycle_config_2.txt"
#define THIRD_CYCLE_CONFIG_FILE "cycle_config_3.txt"
#define FOURTH_CYCLE_CONFIG_FILE "cycle_config_4.txt"

#ifdef SAMPLE_PREP_BOARD
#define CSV_HEADER "Time,HeaterTemp,HeaterPWM,MotorSpeed,MotorPWM,Battery,BatteryV,BatteryT,Event\r\n"
#define CSV_HEADER_SIZE 79
#else
#define CSV_HEADER "Time,ValveTemp,ValvePWM,AmpTemp,AmpPWM,Batt,BatteryV,BatteryT,Event\r\n"
#define CSV_HEADER_SIZE 69
#endif

#if USE_SD_CARD
#define BLOCKDEV_LIST() ( \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev))
#endif 

#if USE_NOR_FLASH
#define BLOCKDEV_LIST() ( \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev))
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
FRESULT check_for_naatos_config_file(void);
FRESULT check_for_cycle_config_files(void);
FRESULT get_naatos_configuration_parameters(naatos_config_parameters *parameters);
FRESULT get_cycle_configurations_parameters();
FRESULT reset_set_time_date(void);
FRESULT create_log_file(const char *file_name);
FRESULT write_log_line(const char *logName, const char *writeBuff, uint32_t writeBuffSize);
FRESULT remount_goto_logs_dir(void);

// Parse Helpers
double parse_double(const char *str, double default_value);
int parse_int(const char *str, int default_value);