#pragma once
//#include "../naatos_config.h"

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
} naatos_kv_table_entry_t;
