#include "naatos_config.h"

// EXPORT FILE-SCOPE VARIABLES FROM main.c
extern main_state_t main_state;
extern naatos_gpregret2_t gpregret2;

// EXPORT FUNCTIONS FROM main.c
void reset(void);
void get_nordic_uniqueid_concat_to_a_string(char* string);
void reformat_filesystem_and_reread(void);