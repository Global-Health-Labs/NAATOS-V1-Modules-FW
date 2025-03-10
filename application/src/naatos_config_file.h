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



// Functions To Deal With GLOBAL Config
const naatos_kv_table_entry_t* naatos_config_global_get_itemptr_by_key(const char* keystr);
char * naatos_config_global_getval_as_string_from_item(const naatos_kv_table_entry_t * item);
bool   naatos_config_global_settval_for_item_from_string(const naatos_kv_table_entry_t * item, const char* valstr);

// Functions To Deal With CYCLE Config
char * naatos_config_cycle_getval_as_string_from_item(const naatos_kv_table_entry_cycle_t * item);