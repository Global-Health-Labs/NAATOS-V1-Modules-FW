#pragma once
//#include "../naatos_config.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {
  NAATOS_KV_DT_FLOAT,
  NAATOS_KV_DT_INT,
  NAATOS_KV_DT_UINT16,
  NAATOS_KV_DT_BOOLS,
  NAATOS_KV_DT_STR,
} naatos_kv_datatype_enum_t;

typedef struct {
  const char* name;
  naatos_kv_datatype_enum_t dtype;
  void* dataptr;
  const char* defaultcfgstr;
} naatos_kv_table_entry_t;

typedef struct {
  const char* name;
  naatos_kv_datatype_enum_t dtype;
  void* dataptr;
} naatos_kv_table_entry_cycle_t;


extern const uint8_t KV_TABLE_GLOBAL_SIZE;
extern const naatos_kv_table_entry_t* KV_TABLE_GLOBAL_PTR;

extern const uint8_t KV_TABLE_CYCLE_SIZE;
extern const naatos_kv_table_entry_cycle_t* KV_TABLE_CYCLE_PTR;


//bool assignParameterUsingKeyValueTable_given_key_and_value_from_config_file(const naatos_kv_table_entry_t * KVTABLE, const uint8_t TABLE_SIZE, const char* keystr, const char* valstr);