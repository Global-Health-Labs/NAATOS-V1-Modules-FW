#include "naatos_storage.h"
#include "sample_prep_default_cycles.h"
#include "power_module_default_cycles.h"
//#include "naatos_config_file.h"

/* Storage Variables */
static FIL file;
static DIR dir;
static FILINFO fno;
uint32_t b_written;

bool naatos_storage_initalized = false;
int total_cycles = 0;

// Initalizes either the SD Card or NOR Flash storage
void init_naatos_storage(void) {
#if USE_SD_CARD
  init_sd_card();
#elif USE_NOR_FLASH
  init_nor_flash();
#endif

  // Create directories if needed
  create_naatos_directories();
  // Set storage initalized to true
  naatos_storage_initalized = true;
}

// Uninitalizes either the SD Card or the NOR Flash storage
void uninit_naatos_storage(void) {
#if USE_SD_CARD
  uninit_sd_card();
#elif USE_NOR_FLASH
  uninit_nor_flash();
#endif
  // Set storage initalized to false
  naatos_storage_initalized = false;
}

// Mount SD Card or NOR Flash Storage
void mount_storage(void) {
#if USE_SD_CARD
  if (sd_card_mount() != FR_OK) {
    send_debug_log_message("Unable to unmount SD Card");
  }
#elif USE_NOR_FLASH
  if (mount_nor_flash() != FR_OK) {
    send_debug_log_message("Unable to mount NOR Flash");
  }
#endif
}

// Unmount SD Card or NOR Flash Storage
void unmount_storage(void) {
#if USE_SD_CARD
  if (sd_card_unmount() != FR_OK) {
    send_debug_log_message("Unable to unmount SD Card");
  }
#elif USE_NOR_FLASH
  if (unmount_nor_flash() != FR_OK) {
    send_debug_log_message("Unable to unmount NOR Flash");
  }
#endif
}

// Creates the needed naatos directories if they dont already exist
void create_naatos_directories() {
  FRESULT res;
  // Create logs directory
  res = f_mkdir(LOGS_DIR);
  if (res == FR_OK) {
    send_debug_log_message("Logs directory created");
  }
  // Create config directory
  res = f_mkdir(CONFIG_DIR);
  if (res == FR_OK) {
    send_debug_log_message("Config directory created");
  }
  res = check_for_naatos_config_file();
  if (res == FR_OK) {
    send_debug_log_message("New config.txt config file in config subdirectory created with default parameters.");
  } else if (res != FR_EXIST) {
    send_debug_log_message("Unable to retreive config.txt");
  }
  res = check_for_cycle_config_files();
  if (res == FR_OK) {
    send_debug_log_message("New cycle configuration files in subdirectory created with default parameters");
  } else if (res != FR_EXIST) {
    send_debug_log_message("Unable to retreive cycle configuration files");
  }
}

// Create a new file under the log subdirectory ** NO NAMES WITH : ALLOWED **
FRESULT create_log_file(const char *file_name) {
  FRESULT res;

  // Go to the logs directory
  res = remount_goto_logs_dir();
  if (res != FR_OK) {
    return res;
  }

  // Make the log file
  res = f_open(&file, file_name, FA_CREATE_NEW | FA_WRITE);
  if (res != FR_OK) {
    return res;
  }

  // Write the csv header to the file
  res = f_write(&file, CSV_HEADER, CSV_HEADER_SIZE, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return res;
}

// Write a line to the log file
FRESULT write_log_line(const char *logName, const char *writeBuff, uint32_t writeBuffSize) {
  FRESULT res;
  uint32_t b_written;

  // Go to the logs directory
  res = remount_goto_logs_dir();
  if (res != FR_OK) {
    return res;
  }

  // Open the log file
  res = f_open(&file, logName, FA_WRITE | FA_OPEN_APPEND);
  if (res != FR_OK) {
    return res;
  }

  // Write the given line
  res = f_write(&file, writeBuff, writeBuffSize, &b_written);
  if (res != FR_OK) {
    return res;
  }

  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return res;
}

FRESULT remount_goto_logs_dir(void) {
  FRESULT res;

  // Re-Mount Storage
  mount_storage();

  // Open root directory
  res = f_opendir(&dir, "/");
  if (res != FR_OK) {
    return res;
  }

  // Change directory into logs
  res = f_chdir(LOGS_DIR);
  if (res != FR_OK) {
    return res;
  }
}

bool _assignParameterUsingKeyValueTable_given_key_and_value_from_config_file(const naatos_kv_table_entry_t * KVTABLE, const uint8_t TABLE_SIZE, const char* keystr, const char* valstr) {
  // Search the key-value table for a matching key
  const bool _printdebugs = false;
  char tmp[80];
  bool matched;

  if(_printdebugs)  {
    sprintf(tmp,"key = \"%s\" in file, checking key-value table...",keystr);
    send_debug_log_message(tmp);
  }
  matched = false;

  // Loop through the key-value table for a matching key
  for(uint8_t i=0; i<TABLE_SIZE; i++){
    // pick item from table
    const naatos_kv_table_entry_t * item = &(KVTABLE[i]);

    // check for match of the key in the table
    if(strcasecmp(keystr,item->name) == 0) {
      if(_printdebugs)  {
        sprintf(tmp,"  matched key = %s -> file value str = \"%s\"",item->name,valstr);
        send_debug_log_message(tmp);
      }

      // we have a match! now
      // parse the value string
      switch(item->dtype) {
        case NAATOS_KV_DT_FLOAT:
          *((float*) item->dataptr) = parse_double(valstr,parse_double(item->defaultcfgstr,0));
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_FLOAT. parsed = %g", *((float*) item->dataptr) );
            send_debug_log_message(tmp);
          }

          matched = true;
          break;
        case NAATOS_KV_DT_INT:
          *((int*) item->dataptr) = parse_int(valstr,parse_int(item->defaultcfgstr,0));
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_INT. parsed = %d", *((int*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        case NAATOS_KV_DT_UINT16:
          *((uint16_t*) item->dataptr) = (uint16_t) parse_int(valstr,parse_int(item->defaultcfgstr,0));
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_UINT16. parsed = %d", *((uint16_t*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        case NAATOS_KV_DT_BOOLS:
          *((bool*) item->dataptr) = strcmp(valstr, "true") ? false : true;
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_BOOLS. parsed = %d", *((bool*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        default:
          sprintf(tmp,"couldn't parse key = %s -> file value str = \"%s\" because couldn't handle the datatype",item->name,valstr);
          send_debug_log_message(tmp);
          break;
      }
      if(matched)
        break;
    }
  }
  if(!matched)  {
    sprintf(tmp,"couldn't parse key = %s -> file value str = \"%s\"",keystr,valstr);
    send_debug_log_message(tmp);
  }
  return matched;
}
void _init_global_configuration() {
  for(uint8_t i = 0; i<KV_TABLE_GLOBAL_SIZE; i++) {
    const naatos_kv_table_entry_t * item = &(KV_TABLE_GLOBAL_PTR[i]);

    _assignParameterUsingKeyValueTable_given_key_and_value_from_config_file(KV_TABLE_GLOBAL_PTR, KV_TABLE_GLOBAL_SIZE, item->name, item->defaultcfgstr);
  }
}
FRESULT get_naatos_configuration_parameters(naatos_config_parameters *parameters) {
  char configBuffer[50];
  FRESULT res;
  char *pch;
  char key[50];
  char val[50];
  char val2[50];
  int num;

  // Re-Mount Storage
  mount_storage();

  // Open root directory
  res = f_opendir(&dir, "/");
  if (res != FR_OK) {
    return res;
  }

  // Change directory into configs
  res = f_chdir(CONFIG_DIR);
  if (res != FR_OK) {
    return res;
  }
  // Open naatos config file
  res = f_open(&file, NAATOS_CONFIG_FILE, FA_READ);
  if (res != FR_OK) {
    // Return that it does not exists
    return res;
  }

  // Initialize the config struct to defaults (in case configuration file does not have certain keys defined)
  _init_global_configuration();

  // Set the struct
  //for (int i = 0; i < NUM_MASTER_CONFIG_PARAMETERS; i++) {
  while(true) {
    pch = f_gets(configBuffer, 50, &file);
    if(pch[0]==0) {
      // END OF FILE
      break;
    }

    // key
    pch = strtok(configBuffer, ":");
    sprintf(key, "%s", pch);
    
    // val
    pch = strtok(NULL, ":");
    sprintf(val, "%s", pch);
    char *newline = strchr(val, '\n');
    if (newline) {
      // Replace newline character with null terminator
      *newline = '\0';
    }

    // Search the key-value table for a matching key. if found, then parse and populate the parameters data structure
    strcpy(val2,val);
    _assignParameterUsingKeyValueTable_given_key_and_value_from_config_file(KV_TABLE_GLOBAL_PTR,KV_TABLE_GLOBAL_SIZE,key,val2);
    // after this function runs, if a match was found, it should have been populated int the config structure

  } // file for-loop


  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return FR_OK;
}

bool _assignParameterToCycleStruct_given_key_and_value_from_config_file( const char* keystr, const char* valstr) {
  // Search the key-value table for a matching key
  const bool _printdebugs = false;
  char tmp[80];
  bool matched;

  if(_printdebugs)  {
    sprintf(tmp,"key = \"%s\" in file, checking key-value table...",keystr);
    send_debug_log_message(tmp);
  }
  matched = false;

  // Loop through the key-value table for a matching key
  for(uint8_t i=0; i<KV_TABLE_CYCLE_SIZE; i++){
    // pick item from table
    const naatos_kv_table_entry_cycle_t * item = &(KV_TABLE_CYCLE_PTR[i]);

    // check for match of the key in the table
    if(strcasecmp(keystr,item->name) == 0) {
      if(_printdebugs)  {
        sprintf(tmp,"  matched key = %s -> file value str = \"%s\"",item->name,valstr);
        send_debug_log_message(tmp);
      }

      // we have a match! now
      // parse the value string
      switch(item->dtype) {
        case NAATOS_KV_DT_FLOAT:
          *((float*) item->dataptr) = parse_double(valstr,0);
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_FLOAT. parsed = %g", *((float*) item->dataptr) );
            send_debug_log_message(tmp);
          }

          matched = true;
          break;
        case NAATOS_KV_DT_INT:
          *((int*) item->dataptr) = parse_int(valstr,0);
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_INT. parsed = %d", *((int*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        case NAATOS_KV_DT_UINT16:
          *((uint16_t*) item->dataptr) = (uint16_t) parse_int(valstr,0);
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_UINT16. parsed = %d", *((uint16_t*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        case NAATOS_KV_DT_BOOLS:
          *((bool*) item->dataptr) = strcmp(valstr, "true") ? false : true;
          if(_printdebugs)  {
            sprintf(tmp,"  was DT_BOOLS. parsed = %d", *((bool*) item->dataptr) );
            send_debug_log_message(tmp);
          }
          matched = true;
          break;
        default:
          sprintf(tmp,"couldn't parse key = %s -> file value str = \"%s\" because couldn't handle the datatype",item->name,valstr);
          send_debug_log_message(tmp);
          break;
      }
      if(matched)
        break;
    }
  }
  if(!matched)  {
    sprintf(tmp,"couldn't parse key = %s -> file value str = \"%s\"",keystr,valstr);
    send_debug_log_message(tmp);
  }
  return matched;
}


FRESULT get_cycle_configurations_parameters() {
  char configBuffer[50];
  FRESULT res;
  char *pch;
  char key[50];
  char val[50];
  int num;

  // Re-Mount Storage
  mount_storage();

  // Re-Mount Storage
  mount_storage();

  // Open config directory
  res = f_opendir(&dir, "/config");
  if (res != FR_OK) {
    return res;
  }
  
  // ensure we start at 0
  //...this became important in context of the hold-down reformat procedure
  // (with 4 cycle default, it was actually assuming 8 cycles on the first run after reset
  total_cycles = 0;

  // Count the number of cycle files
  for (;;) {
    res = f_readdir(&dir, &fno);
    // Break on error or end of dir 
    if (res != FR_OK || fno.fname[0] == 0) break;  
    // Check if it's a file 
    if (!(fno.fattrib & AM_DIR)) {                 
      if (strncmp(fno.fname, "cycle_config_", 13) == 0) {
        // Check if the filename starts with "cycle_config_"
        char *endptr;
        // Attempt to convert the rest to integer
        strtol(&fno.fname[13], &endptr, 10); 
        // If the rest is an integer followed by a dot
        if (*endptr == '.') { 
          total_cycles++;
        }
      }
    }
  }

  // Ensure we have a file
  if (total_cycles > 0) {
    // Malloc space for the cycle configurations array
    cycle_configs = (cycle_config_parameters *)malloc(total_cycles * sizeof(cycle_config_parameters));
    // Loop through each file
    for (int i = 0; i < total_cycles; i++) {
      char buff[128];
      sprintf(buff, "/config/cycle_config_%d.txt", (i+1));
      // Open current cycle config file
      res = f_open(&file, buff, FA_READ);
      if (res != FR_OK) {
        // Return that it does not exists
        return res;
      }

      // Loop through the file and populate the cycle configuration array
      while(true) {
        pch = f_gets(configBuffer, 50, &file);
        if(pch[0]==0) {
          // END OF FILE
          break;
        }
        
        // key
        pch = strtok(configBuffer, ":\n\r");
        sprintf(key, "%s", pch);

        // val
        pch = strtok(NULL, ":");
        sprintf(val, "%s", pch);
        char *newline = strchr(val, '\n'); 
        if (newline) {
          // Replace newline character with null terminator
          *newline = '\0';
        }

        // Search the key-value table for a matching key. if found, then parse and populate the parameters data structure
        // after this function runs, if a match was found, it should have been populated int the config structure
        _assignParameterToCycleStruct_given_key_and_value_from_config_file(key,val);
        // the above function's result was left in cycle_cfg_single. copy it to our malloc'ed cycle_config array
        cycle_configs[i]=cycle_cfg_single;

      } //loop through the lines

      // Close the current cycle file
      res = f_close(&file);
      if (res != FR_OK) {
        return res;
      }
    }
  }
  else {
    return FR_NO_FILE;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return FR_OK;
}

FRESULT check_for_naatos_config_file(void) {
  FRESULT res;
  DIR dir;
  char configBuffer[50];
  uint32_t configBufferSize;
  uint32_t b_written;

  // Change directory into configs
  res = f_chdir(CONFIG_DIR);
  if (res != FR_OK) {
    return res;
  }

  // Try to create new naatos_config.txt
  res = f_open(&file, NAATOS_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  if (res == FR_EXIST) {
    // Verify that it has non-zero size
    res = f_stat(NAATOS_CONFIG_FILE,&fno);
    if(fno.fsize>0) {
      // Return that it already exists
      return FR_EXIST;
    } else{
      send_debug_log_message("Found config file existed but it was zero size. Proceed as if it wasn't there.");
    }


  }

  // Open the directory for reading
  res = f_opendir(&dir, ".");
  if (res != FR_OK) {
      return res;  // Error opening directory
  }

  // Delete Old Config Files if they are present
  while (1) {
    res = f_readdir(&dir, &fno);  // Read a directory entry
    if (res != FR_OK || fno.fname[0] == 0) {
      break;  // no More files or Error
    }
    
    // Skip "." and ".." entries
    if (fno.fname[0] == '.' && (fno.fname[1] == '\0' || (fno.fname[1] == '.' && fno.fname[2] == '\0'))) {
        continue;
    }

    // If it's a file, delete it
    if (!(fno.fattrib & AM_DIR)) {
      res = f_unlink(fno.fname);  // Delete the file
      if (res != FR_OK) {
          return res;  // Return error if file couldn't be deleted
      }
    }   
    
  }

  f_closedir(&dir);

  // Create new naatos_config.txt
  res = f_open(&file, NAATOS_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  if (res == FR_EXIST) {
    // Return that it already exists
    return res;
  }

  // Populate new config file
  // Iterate through the key-value table and write everything to file
  for(uint8_t i = 0; i<KV_TABLE_GLOBAL_SIZE; i++) {
    const naatos_kv_table_entry_t * item = &(KV_TABLE_GLOBAL_PTR[i]);

    if(strcmp(item->name,"canary")==0)  {
      configBufferSize = sprintf(configBuffer, "%s:12345\n", item->name);
      res = f_write(&file, configBuffer, configBufferSize, &b_written);
    } else{
      configBufferSize = sprintf(configBuffer, "%s:%s\n", item->name,item->defaultcfgstr);
      res = f_write(&file, configBuffer, configBufferSize, &b_written);
    }
    if (res != FR_OK) {
      return res;
    }
  }
  
  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }

  return res;
}

FRESULT check_for_cycle_config_files(void) {
  FRESULT res;
  DIR dir;
  char configBuffer[50];
  uint32_t configBufferSize;
  uint32_t b_written;

  // Re-Mount Storage
  mount_storage();

  // Open root directory
  res = f_opendir(&dir, "/");
  if (res != FR_OK) {
    return res;
  }

  // Change directory into configs
  res = f_chdir(CONFIG_DIR);
  if (res != FR_OK) {
    return res;
  }

  // Try to create new cycle_config_1.txt
  res = f_open(&file, FIRST_CYCLE_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  if (res == FR_EXIST) {
    // Return that it already exists
    return res;
  }
  
  // TODO: Remove any possible leftover cycle configuration files
  
  // Let user know
  send_debug_log_message("Creating new default cycle configuration files.");

  // Create Sample Prep Default files
#ifdef SAMPLE_PREP_BOARD
  res = f_write(&file, sp_default_cycle_config_1, sp_default_cycle_config_1_size, &b_written);
  res = f_close(&file);

  res = f_open(&file, SECOND_CYCLE_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  res = f_write(&file, sp_default_cycle_config_2, sp_default_cycle_config_2_size, &b_written);
  res = f_close(&file);

  res = f_open(&file, THIRD_CYCLE_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  res = f_write(&file, sp_default_cycle_config_3, sp_default_cycle_config_3_size, &b_written);
  res = f_close(&file);

  res = f_open(&file, FOURTH_CYCLE_CONFIG_FILE, FA_CREATE_NEW | FA_WRITE);
  res = f_write(&file, sp_default_cycle_config_4, sp_default_cycle_config_4_size, &b_written);
  res = f_close(&file);
#else
  res = f_write(&file, pm_default_cycle_config_1, pm_default_cycle_config_1_size, &b_written);
  res = f_close(&file);

  res = f_open(&file, SECOND_CYCLE_CONFIG_FILE, FA_CREATE_ALWAYS | FA_WRITE);
  res = f_write(&file, pm_default_cycle_config_2, pm_default_cycle_config_2_size, &b_written);
  res = f_close(&file);

  res = f_open(&file, THIRD_CYCLE_CONFIG_FILE, FA_CREATE_ALWAYS | FA_WRITE);
  res = f_write(&file, pm_default_cycle_config_3, pm_default_cycle_config_3_size, &b_written);
  res = f_close(&file);

  res = f_open(&file, FOURTH_CYCLE_CONFIG_FILE, FA_CREATE_ALWAYS | FA_WRITE);
  res = f_write(&file, pm_default_cycle_config_4, pm_default_cycle_config_4_size, &b_written);
  res = f_close(&file);
#endif
  
}

// Sets the set_time_date variable to "false"
FRESULT reset_set_time_date(void) {
  FRESULT res;
  const char *var = "set_time_date";
  UINT br, bw;            // File read/write count
  char buffer[1024];      // Buffer to hold file content
  char temp_buffer[1024]; // Temporary buffer for modified content

  // Re-Mount Storage
  mount_storage();

  // Open root directory
  res = f_opendir(&dir, "/");
  if (res != FR_OK) {
    return res;
  }

  // Change directory into configs
  res = f_chdir(CONFIG_DIR);
  if (res != FR_OK) {
    return res;
  }

  // Open naatos config file
  res = f_open(&file, NAATOS_CONFIG_FILE, FA_READ | FA_WRITE);
  if (res != FR_OK) {
    // Return that it already exists
    return res;
  }

  // Read contents into a buffer
  res = f_read(&file, buffer, sizeof(buffer) - 1, &br);
  if (res != FR_OK) {
    return res;
  }

  buffer[br] = '\0';

  // Search buffer for specified var. Copy lines that do not contain the specified var into temp_buffer
  char *line = strtok(buffer, "\n");
  char *temp_ptr = temp_buffer;
  while (line != NULL) {
    if (strstr(line, var) != NULL) {
      //Specified var found in line. Rewrite line to have it set to false
      temp_ptr += sprintf(temp_ptr, "%s\n", "set_time_date:false");
    } else {
      // If the var is not found in the line, copy the line to the temp buffer
      temp_ptr += sprintf(temp_ptr, "%s\n", line);
    }
    line = strtok(NULL, "\n");
  }

  // Move file pointer back to the beginning
  res = f_lseek(&file, 0);
  if (res != FR_OK) {
    return res;
  }

  // Write the contents of temp_buffer back into the config file (with the specified var removed)
  res = f_write(&file, temp_buffer, strlen(temp_buffer), &bw);
  if (res != FR_OK || bw < strlen(temp_buffer)) {
    return res;
  }

  // Truncate file to new size
  res = f_truncate(&file);
  if (res != FR_OK) {
    return res;
  }

  // Close the file
  res = f_close(&file);
  if (res != FR_OK) {
    return res;
  }

  // Change back directories
  res = f_chdir("..");
  if (res != FR_OK) {
    return res;
  }
}

double parse_double(const char *str, double default_value) {
  char *endptr;
  double value = strtod(str, &endptr);

  // Check if the entire string was consumed and a valid number was parsed
  if (endptr == str) {
    // No valid conversion was performed
    return default_value;
  }

  return value;
}

int parse_int(const char *str, int default_value) {
  char *endptr;
  int value = strtol(str, &endptr, 10); // Base 10 for decimal

  // Check if the entire string was consumed and a valid number was parsed
  if (endptr == str) {
    // No valid conversion was performed
    return default_value;
  }

  return value;
}

bool is_naatos_storage_okay() {
  return naatos_storage_initalized;
}