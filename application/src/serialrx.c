//#include "serialrx.h
#include "naatos_config.h"
#include "storage/naatos_storage.h"
#include "battery.h"      // for PUBLIC FUEL GAUGE DATA
#include "sensors.h"      // for PUBLIC SENSOR DATA
#include "main.h"

static const uint8_t tmpsz = 128;
static char tmp[128];

/*
All command functions should have prototype:
  bool (*cmdFunction)(char*);

where the char* is the "argument" portion of the string received through
the uart port, after the <CMD>,
*/



// ---
// SET RTC CLOCK (argument: give ISO8601-like string 20YY-mm-dd HH:MM:SS)
// ---
// SETCLK,2024-11-14 14:27:34
// or
// SETCLK,2024-11-14T14:27:34
// convenient to use with YAT (yet another terminal) timestamp insertion: SETCLK,\!(TimeStamp())
bool serialCmd_SETCLK(char* sPtrTmp)  {
  bool success = true;
  calendar_time_t time;

  //check it's the right length
  // expect ISO8601 string strickly like: "2024-11-14T14:27:00" or "2024-11-14 14:27:34"
  // T can be interchanged with a space
  if(strlen(sPtrTmp)==19) {
    // proper length! Go through and parse the date
    send_debug_log_message("COMRXTASK: PARSE --> SETCLK command: OK length");

    // SHOW THE OLD TIME
    calendar_get_time(&time);
    sprintf(tmp,"Old read time  M: %d D: %d Y:%d h: %d m: %d s: %d",
        time.month,
        time.day,
        time.year,
        time.hour,
        time.minute,
        time.second
    );
    send_debug_log_message_reliableblocking(tmp);

    // SET DATETIME STRUCTURE
    time.year   = ((sPtrTmp[2]-'0')*10)+(sPtrTmp[3]-'0');
    time.month  = ((sPtrTmp[5]-'0')*10)+(sPtrTmp[6]-'0');
    time.day    = ((sPtrTmp[8]-'0')*10)+(sPtrTmp[9]-'0');
    time.hour   = ((sPtrTmp[11]-'0')*10)+(sPtrTmp[12]-'0');
    time.minute = ((sPtrTmp[14]-'0')*10)+(sPtrTmp[15]-'0');
    time.second = ((sPtrTmp[17]-'0')*10)+(sPtrTmp[18]-'0');

    // sanity check
    if(time.year>=30)  {
      success=false;
      send_debug_log_message("COMRXTASK: PARSE --> SETCLK command: FAIL year");
    }
    if((time.month>13) || (time.month==0))  {
      success=false;
      send_debug_log_message("COMRXTASK: PARSE --> SETCLK command: FAIL month");
    }
    if((time.day>31) || (time.day==0))  {
      success=false;
      send_debug_log_message("COMRXTASK: PARSE --> SETCLK command: FAIL day");
    }
    if(time.hour>=24)  {
      success=false;
      send_debug_log_message("COMRXTASK: PARSE --> SETCLK command: FAIL hour");
    }
    if(time.minute>=60)  {
      success=false;
      send_debug_log_message("COMRXTASK: PARSE --> SETCLK command: FAIL minute");
    }
    if(time.second>=60)  {
      success=false;
      send_debug_log_message("COMRXTASK: PARSE --> SETCLK command: FAIL second");
    }

    if(success) {
      // Reset
      calendar_reset(); //<-- needed for us because sometimes VDD does not start at zero due to battery jiggling when unit is first assembled
      vTaskDelay(pdMS_TO_TICKS(5));

      // Set the time
      calendar_set_time(&time);
      sprintf(tmp,"Requested time  M: %d D: %d Y:%d h: %d m: %d s: %d",
          time.month,
          time.day,
          time.year,
          time.hour,
          time.minute,
          time.second
      );
      send_debug_log_message_reliableblocking(tmp);

      // Readback the time
      calendar_get_time(&time);
      sprintf(tmp,"Readback time  M: %d D: %d Y:%d h: %d m: %d s: %d",
          time.month,
          time.day,
          time.year,
          time.hour,
          time.minute,
          time.second
      );
      send_debug_log_message_reliableblocking(tmp);

      return true;  // command ran successfully
    }
  }
  return false;  // command failed/did not run
}

// ---
// TEST RTC CLOCK (argument: none)
// ---
// TSTCLK,
// queries a few PCF85063A registers for debug purposes and displays
bool serialCmd_TSTCLK(char* arg)  {
  calendar_check_state();
  return true;  // command ran successfully
}

// ---
// RESET RTC CLOCK (argument: none)
// ---
// RSTCLK,
// issues softare reset on PCF85063A
bool serialCmd_RSTCLK(char* sPtrTmp) {
  calendar_reset();
  
  return true;  // command ran successfully
}

// ---
// LIST CONTENTS (argument: none)
// ---
// LSDIR,
// queries a few PCF85063A registers for debug purposes and displays
bool serialCmd_LSDIR(char* arg) {
  DIR dir;
  FILINFO fno;
  //uint32_t bytes_written;
  FRESULT ff_result;
  char buff[50];

  if(main_state==MAIN_STANDBY)  {
    send_debug_log_message("\r\n Listing NOR Flash directory: /");
    // Open Root Directory
    ff_result = f_opendir(&dir, "/");
    if (ff_result) {
      send_debug_log_message("Directory listing failed!");
      return false;
    }

    // Get the other directories
    do {
      ff_result = f_readdir(&dir, &fno);
      if (ff_result != FR_OK) {
        send_debug_log_message("Directory read failed.");
        return false;
      }

      if (fno.fname[0]) {
        if (fno.fattrib & AM_DIR) {
          sprintf(buff, "   <DIR>   %s", (uint32_t)fno.fname);
          send_debug_log_message(buff);
        } else {
          sprintf(buff, "%9lu  %s", fno.fsize, (uint32_t)fno.fname);
          send_debug_log_message(buff);
        }
      }
    } while (fno.fname[0]);

  } else{
    send_debug_log_message("error, main_state!=MAIN_STANDBY, cannot run command");
    return false; // command did not run successfully
  }

  return true;  // command ran successfully
}

// ---
// REBOOT (argument: none)
// ---
// REBOOT,
// will do a Nordic software reset
bool serialCmd_REBOOT(char* sPtrTmp) {
  send_debug_log_message("will reboot in 1 second");
  vTaskDelay(pdMS_TO_TICKS(1000));
  reset();
  // should never continue beyond

  return true;  // command ran successfully
}

// ---
// Get Status (argument: none)
// ---
// STATUS,
bool serialCmd_STATUS(char* sPtrTmp) {
  calendar_time_t time;

  // overwrite sPtrTmp memory to hold this short state string
  if(main_state==MAIN_STANDBY)  {
    sprintf(sPtrTmp,"MAIN_STANDBY");
  } else if(main_state==MAIN_FILE)  {
    sprintf(sPtrTmp,"MAIN_FILE");
  } else{
    sprintf(sPtrTmp,"other");
  }

  // output a generic string with time, states, fw version
  calendar_get_time(&time);
  snprintf(tmp,tmpsz,"V=\"%s\" T=\"%s\" ECL=0x%02x TS=\"20%02d-%02d-%02d %02d:%02d:%02d\" MAIN_STATE=\"%s\" SN=\"",
    VERSION, BOARD_STR, logger_cumulative_errors.reg,
    time.year,
    time.month,
    time.day,
    time.hour,
    time.minute,
    time.second,
    sPtrTmp // was overwritten to hold this state string
  );
  get_nordic_uniqueid_concat_to_a_string(tmp);
  strcat(tmp,"\"");
#ifdef SAMPLE_PREP_BOARD
  snprintf(tmp,tmpsz,"%s FGREWORK=%d",
    tmp,MOTOR_FG_REWORK
  );
#endif
  send_debug_log_message_reliableblocking(tmp);
  
  // output some sensor information
  snprintf(tmp,tmpsz,"Vbatt=%f SOC=%d Tbatt=%f",
    PUBLIC_FUELGAUGE_DATA.batt_info->batt_voltage,
    PUBLIC_FUELGAUGE_DATA.batt_info->batt_percent,
    PUBLIC_FUELGAUGE_DATA.batt_info->batt_temp
  );
  #if defined(POWER_MODULE_BOARD)
    snprintf(tmp,tmpsz,"%s Tamp=%f Tvalve=%f",
      tmp,
      PUBLIC_SENSOR_DATA.temperatures->amp_temp,
      PUBLIC_SENSOR_DATA.temperatures->valve_temp
    );
  #elif defined(SAMPLE_PREP_BOARD)
    snprintf(tmp,tmpsz,"%s Theater=%f MotorRPM=%f",
      tmp,
      PUBLIC_SENSOR_DATA.temperatures->heater_temp,
      PUBLIC_SENSOR_DATA.temperatures->motor_speed
    );
  #endif
  send_debug_log_message_reliableblocking(tmp); // will block until things are complete

  return true;  // command ran successfully
}

// ---
// CFGGET (argument: none)
// ---
// CFGGET,
bool serialCmd_CFGGET(char* sPtrTmp)  {
  vTaskDelay(pdMS_TO_TICKS(50));

  // we will be directly writing to COM in this command, since
  // we have to dump a lot of lines back to the UART
  // and the message/queue-based logger syste is just a bit too slow for us

  // main global config
  //send_debug_log_message("---Config Dump: Global---");
  snprintf(tmp,tmpsz,"---Config Dump: Global---\r\n");
  write_to_com(tmp,strlen(tmp));
  // Iterate through the key-value table and write everything to file
  for(uint8_t i = 0; i<KV_TABLE_GLOBAL_SIZE; i++) {
    const naatos_kv_table_entry_t * item = &(KV_TABLE_GLOBAL_PTR[i]);

    switch(item->dtype) {
      case NAATOS_KV_DT_FLOAT:
        snprintf(tmp,tmpsz,"%s:%g\r\n",item->name,*((float*) item->dataptr)); break;
      case NAATOS_KV_DT_INT:
        snprintf(tmp,tmpsz,"%s:%d\r\n",item->name,*((int*) item->dataptr)); break;
      case NAATOS_KV_DT_UINT16:
        snprintf(tmp,tmpsz,"%s:%d\r\n",item->name,*((uint16_t*) item->dataptr)); break;
      case NAATOS_KV_DT_BOOLS:
        if(*((bool*) item->dataptr))
          snprintf(tmp,tmpsz,"%s:true\r\n",item->name);
        else
          snprintf(tmp,tmpsz,"%s:false\r\n",item->name);
        break;
      default:
        snprintf(tmp,tmpsz,"%s:<unknown / not implemented>\n",item->name);
        break;
    };
    //send_debug_log_message(tmp);
    write_to_com(tmp,strlen(tmp));  // write directly to COM, the debug log interface is a bit slow for so many lines and queue-based
  }


  // cycle config
  for(uint8_t i=0; i<total_cycles; i++) {
    snprintf(tmp,tmpsz,"---Config Dump: Cycle %d/%d---\r\n",
      i+1,total_cycles
    );
    //send_debug_log_message(tmp);
    write_to_com(tmp,strlen(tmp));
#ifdef SAMPLE_PREP_BOARD
    snprintf(tmp,tmpsz,"time_s:%.1f temp_ramp:%d,%.1f\r\n",
      cycle_configs[i].cycle_run_time_s,cycle_configs[i].ramp_to_temp_before_start_cycle,cycle_configs[i].ramp_to_temp_timeout
    );
    //send_debug_log_message(tmp);
    write_to_com(tmp,strlen(tmp));
    if(cycle_configs[i].run_heater) {
      snprintf(tmp,tmpsz,"heater sp:%.1f kp:%.4f ki:%.4f kd:%.4f\r\n",
        cycle_configs[i].heater_setpoint,
        cycle_configs[i].heater_kp,
        cycle_configs[i].heater_ki,
        cycle_configs[i].heater_kd
      );
      //send_debug_log_message(tmp);
      write_to_com(tmp,strlen(tmp));
    }
    if(cycle_configs[i].run_motor) {
      snprintf(tmp,tmpsz,"motor  sp:%d kp:%.4f ki:%.4f kd:%.4f\r\n",
        cycle_configs[i].motor_setpoint,
        cycle_configs[i].motor_kp,
        cycle_configs[i].motor_ki,
        cycle_configs[i].motor_kd
      );
      //send_debug_log_message(tmp);
      write_to_com(tmp,strlen(tmp));
    }
#endif
#ifdef POWER_MODULE_BOARD
    snprintf(tmp,tmpsz,"time_s:%.1f dblylw_s:%.1f temp_ramp:%d,%.1f\r\n",
      cycle_configs[i].cycle_run_time_s,
      cycle_configs[i].double_yellow_grace_period_s,
      cycle_configs[i].ramp_to_temp_before_start_cycle,cycle_configs[i].ramp_to_temp_timeout
    );
    //send_debug_log_message(tmp);
    write_to_com(tmp,strlen(tmp));
    if(cycle_configs[i].run_amp) {
      snprintf(tmp,tmpsz,"ampl. sp:%.1f kp:%.4f ki:%.4f kd:%.4f\r\n",
        cycle_configs[i].amp_setpoint,
        cycle_configs[i].amp_kp,
        cycle_configs[i].amp_ki,
        cycle_configs[i].amp_kd
      );
      //send_debug_log_message(tmp);
      write_to_com(tmp,strlen(tmp));
    }
    if(cycle_configs[i].run_valve) {
      snprintf(tmp,tmpsz,"valve sp:%.1f kp:%.4f ki:%.4f kd:%.4f\r\n",
        cycle_configs[i].valve_setpoint,
        cycle_configs[i].valve_kp,
        cycle_configs[i].valve_ki,
        cycle_configs[i].valve_kd
      );
      //send_debug_log_message(tmp);
      write_to_com(tmp,strlen(tmp));
    }
#endif
  }
  return true;  // command ran successfully
}

// ---
// Initiate DFU (argument: none)
// ---
// TODFU,
bool serialCmd_TODFU(char* sPtrTmp)  {
  button_update_t button_update_msg;  //<-- for sending button events to rest of tasks

  if(main_state==MAIN_STANDBY)  {
    // we will take the approach of simulating button pushes, so that the main_task() is handling the work
    // I think this will reduce the changes of bugs being introduced
    send_debug_log_message("simulate DFU entry by sending bootloader button event (3 presses)");

    button_update_msg.event = BOOTLOADER_EVENT;  //<-- when triple-pressed
    BaseType_t xReturned = xQueueSend(button_mainStateQueue, &button_update_msg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("COMRXTASK: Unable to send to button_mainStateQueue.");
    }
    return true;  // command ran successfully
  } else{
    send_debug_log_message("error, main_state!=MAIN_STANDBY, cannot enter DFU");
    return false;  // command failed/did not run
  }

}

// ---
// Initiate USB-MSC (argument: none)
// ---
// TOMSC,
bool serialCmd_TOMSC(char* sPtrTmp)  {
  button_update_t button_update_msg;  //<-- for sending button events to rest of tasks

  if(main_state==MAIN_STANDBY)  {
    // we will take the approach of simulating button pushes, so that the main_task() is handling the work
    // I think this will reduce the changes of bugs being introduced
    send_debug_log_message("simulate USB-MSC entry by sending a long-press of button");

    button_update_msg.event = OFF_EVENT;  //<-- when long-pressed
    BaseType_t xReturned = xQueueSend(button_mainStateQueue, &button_update_msg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("COMRXTASK: Unable to send to button_mainStateQueue.");
    }

    // move the main state machine to main_bootloader
    //next_state = MAIN_BOOTLOADER;
    
    return true;  // command ran successfully
  } else{
    send_debug_log_message("error, main_state!=MAIN_STANDBY, canoot start USB-MSC now");
    return false;  // command failed/did not run
  }
}

// ---
// Exit USB-MSC (argument: none)
// ---
// EXITMSC,
bool serialCmd_EXITMSC(char* sPtrTmp)  {
  button_update_t button_update_msg;  //<-- for sending button events to rest of tasks

  if(main_state==MAIN_FILE)  {
    // we will take the approach of simulating button pushes, so that the main_task() is handling the work
    // I think this will reduce the changes of bugs being introduced
    send_debug_log_message("simulate USB-MSC exit by sending a buttonpress");

    button_update_msg.event = ON_EVENT;  //<-- when pushed once
    BaseType_t xReturned = xQueueSend(button_mainStateQueue, &button_update_msg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("COMRXTASK: Unable to send to button_mainStateQueue.");
    }

    // move the main state machine to main_bootloader
    //next_state = MAIN_BOOTLOADER;
    return true;  // command ran successfully
  } else{
    send_debug_log_message("error, main_state!=MAIN_FILE, canoot exit USB-MSC now");
    return false;  // command failed/did not run
  }
}

// ---
// REFORMAT (argument: none)
// ---
// REFORMAT,
//
// This will set a value in GPREG2 and initiate a software reset; upon bootup at main_task() initialization reset process will occur
// This should act just like the hold-button-while-power-on-reset
bool serialCmd_REFORMAT(char* sPtrTmp)  {
  if(main_state==MAIN_STANDBY)  {
    send_debug_log_message("set a retained bitfield and reset, to reformat the filesystem, soon");
    vTaskDelay(pdMS_TO_TICKS(50));
    gpregret2.bit.reformat = true;
    NRF_POWER->GPREGRET2 = gpregret2.reg;
    
    //TODO: ideally i'd like to recreate the button-holding action, but the GPREGRET2 seems to not persist after the NVIC_SystemReset():
    //TODO: so as a work-around we'll just do the reformat here, then reset
    reformat_filesystem_and_reread();
    vTaskDelay(pdMS_TO_TICKS(50));
    send_debug_log_message("SUCCESS!! Will reboot soon with cleared filesystem...");

    vTaskDelay(pdMS_TO_TICKS(100));
    reset();
    // should never continue beyond
    return true;  // command ran successfully
  } else{
    send_debug_log_message("error, main_state!=MAIN_FILE, canoot exit USB-MSC now");
    return false;  // command failed/did not run
  }
}





typedef struct serialCmd_list_entry {
  const char* strCmdName;
  bool (*cmdFunction)(char*);
} serialCmd_list_entry_t;

const serialCmd_list_entry_t serialCmd_list[] = {
  { "SETCLK",   serialCmd_SETCLK  },
  { "TSTCLK",   serialCmd_TSTCLK  },
  { "RSTCLK",   serialCmd_RSTCLK  },
  { "LSDIR",    serialCmd_LSDIR   },
  { "REBOOT",   serialCmd_REBOOT  },
  { "STATUS",   serialCmd_STATUS  },
  { "CFGGET",   serialCmd_CFGGET  },
  { "TODFU",    serialCmd_TODFU   },
  { "TOMSC",    serialCmd_TOMSC   },
  { "EXITMSC",  serialCmd_EXITMSC },
  { "REFORMAT", serialCmd_REFORMAT},
};
const uint8_t serialCmd_list_size = sizeof(serialCmd_list)/sizeof(serialCmd_list_entry_t);





/*********************************************************************
*
*       parseIncomingSerialMessageAndAct(char* strmsg)
*
*   Parse incoming serial message and act upon-it ( called from main_task() )
*/
void parseIncomingSerialMessageAndAct(char* strmsg)  {
  char * sPtrTmp;
  bool success;
  bool commandWasHandled = false;

  //sprintf(tmp,"COMRXTASK: PARSE RX'ed->""%s""",strmsg);
  //send_debug_log_message(tmp);

  // add termination nulls in the string to the first \n or \r characters
  sPtrTmp = strchr(strmsg,'\r');
  if(sPtrTmp != NULL) {
    *sPtrTmp = '\0';
  }
  sPtrTmp = strchr(strmsg,'\n');
  if(sPtrTmp != NULL) {
    *sPtrTmp = '\0';
  }

  // search for COMMA, which separates LEFTSIDE command, and RIGHTSIDE arguments
  sPtrTmp = strchr(strmsg,',');
  if(sPtrTmp != NULL) {
    // found comma separator
  
    // move pointer just to the right of the COMMA if not end of string
    if(*(sPtrTmp+1)!='\0')
      sPtrTmp++;

    // check for commands we will recognize and handle, by searching through command table
    commandWasHandled = false;
    for(uint8_t i = 0; i<serialCmd_list_size; i++)  {
      //serialCmd_list_entry_t serialCmd_list
      serialCmd_list_entry_t * entry = &(serialCmd_list[i]);

      if(strncmp(strmsg,entry->strCmdName,strlen(entry->strCmdName)) == 0)  {
        snprintf(tmp,tmpsz,"COMRXTASK: PARSE --> %s command handler",entry->strCmdName);
        send_debug_log_message(tmp);

        // Call the command handler, passing in any further arguments, and collect a true/false success result
        success = (entry->cmdFunction)(sPtrTmp);
        if(success) {
          snprintf(tmp,tmpsz,"COMRXTASK: %s completed successfully",entry->strCmdName);
          send_debug_log_message(tmp);
        } else{
          snprintf(tmp,tmpsz,"COMRXTASK: %s failed",entry->strCmdName);
          send_debug_log_message(tmp);
        }

        commandWasHandled = true;
        break;  // exit the for-loop
      }
    }

    if(!commandWasHandled)  {
      // ---
      // unrecognized command
      // ---
      // message had comma, but we did not understand it
      send_debug_log_message("COMRXTASK: PARSE --> command unhandled or unsupported");
    }

  } else  {
    send_debug_log_message("COMRXTASK: PARSE --> couldn't parse input (no comma)");
  }
}







void serialrx_task(void *pvParameters) {
  /* **** HANDLE USB CDC SERIAL COM PORT RX **** */
  /*
  *
  *
  * Parse and act-upon any serial message
  *
  *
  **/
  (void)pvParameters;
  BaseType_t xReturned;
  SerialRXQueue_msg_t serial_rx_msg;

  for (;;) {
    if (xQueueReceive(main_SerialRXQueue, &serial_rx_msg, portMAX_DELAY) != pdPASS) {
      send_debug_log_message("Unable to Rx data from main_SerialRXQueue");
    
    } else {
      // message received

      // we got a new item from the serial port (see usb.c)
      parseIncomingSerialMessageAndAct(serial_rx_msg.message);

    }
  } // task end for(;;)

}

