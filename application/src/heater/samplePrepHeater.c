#include "samplePrepHeater.h"
#include "../motor.h"
#include "../usb.h"
#include "timers.h"

const MainStateErrorQueueMsg_t motor_stall_percent_err_msg = {
  .errType = ERR_MOTOR_STALLED_PERCENT,
  .overTempData = NULL
};

const MainStateErrorQueueMsg_t motor_stall_pwm_err_msg = {
  .errType = ERR_MOTOR_STALLED_PWM,
  .overTempData = NULL
};

bool heater_running_now = false;
bool heater_running_last = false;
bool motor_running_now = false;
bool motor_running_last = false;
static char tmp[100];
bool rx_runmotor = false;
bool rx_runheater = false;

float MY_H_SP; float MY_H_KP; float MY_H_KI; float MY_H_KD;
float MY_M_SP; float MY_M_KP; float MY_M_KI; float MY_M_KD;
pid_controller_t heater_pid;
pid_controller_t motor_pid;

bool starting_sample_prep_run = true;
bool h_pwm_req = false;
bool greater_than_max = false;
bool rampToTemp = false;
bool motorStalledPercent = false;
bool motorStalledPWM = false;
bool motorReachedSpeed = false;
int last_motor_speed = 0;

temperature_data_t local_temp_data;

uint32_t samp_log_index = 0;
uint32_t samp_log_max = 0;

temperature_pwm_data_t h_pwm_data = {
    .heat_zone_0_pwm = 0,
    .heat_zone_1_pwm = 0,
    .heat_zone_2_pwm = 0,
    .heat_zone_3_pwm = 0,
    .sample_prep_heater_pwm = 0};

static log_data_message_t logMsg = {
    .data_type = TEMPERATURE_DATA,
    .event_data = NULL,
    .temperature_data = NULL};

bool doWeRunHeaterInThisCycle() {
  return rx_runheater;
}

bool doWeRunMotorInThisCycle() {
  return rx_runmotor;
}


void handle_cycle_stopstart_heater(bool heating) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  // Handle case where valve zone is on already, dont want to send stop
  //if (heater_running_now && !heating)
  //  return;
  
  // need to stop supply before stopping pwm
  //if (!heating && !config.run_motor_2 && !use_default_configuration_parameters) { 
  //  nrf_gpio_pin_clear(MOTOR_PWR_EN);
  //}
  //else if (!heating && DEFAULT_RUN_MOTOR_2 && use_default_configuration_parameters) {
  //  nrf_gpio_pin_clear(MOTOR_PWR_EN);
  //}

  // GHL: HANDLE THIS IN THE HeaterZoneStateUpdate function
  //if( !doWeRunMotorInThisCycle() )  {
  //  nrf_gpio_pin_clear(MOTOR_PWR_EN);
  //}
  bool heater_run = doWeRunHeaterInThisCycle();
  bool motor_run = doWeRunMotorInThisCycle();

  // Set heater parameters
  if(heater_run != heater_running_now)  {
    // heater state changed in this cycle
    heater_running_last = heater_running_now;

    if(heater_run) {
      nrf_gpio_pin_set(HEATER_PWR_EN);
      //send_debug_log_message("nrf_gpio_pin_set(HEATER_PWR_EN)");
    } else{
      nrf_gpio_pin_clear(HEATER_PWR_EN);
      //send_debug_log_message("nrf_gpio_pin_clear(HEATER_PWR_EN)");
    }
  }
  heater_running_now = heater_run;


  // Set motor parameters
  if(motor_run != motor_running_now)  {
    // motor state changed in this cycle
    motor_running_last = motor_running_now;
    
    if(motor_run) {
      nrf_gpio_pin_set(MOTOR_PWR_EN);
      //send_debug_log_message("nrf_gpio_pin_set(MOTOR_PWR_EN)");
    } else{
      nrf_gpio_pin_clear(MOTOR_PWR_EN);
      //send_debug_log_message("nrf_gpio_pin_clear(MOTOR_PWR_EN)");
    }
  }
  motor_running_now = motor_run;

  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  //msg.heaterRunning = heating;    // SG GHL (this has to be TRUE or else all logging will stop)
  msg.heaterRunning = heater_run || motor_run;    // SG GHL (this has to be TRUE or else all logging will stop)

  // Send the heater status
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
  }

  MotorRxQueueMsg_t motorMsg;
  motorMsg.type = MOTOR_MSG_HEATER_STATE;
  motorMsg.motorRunning = motor_running_now;

  // Send the motor status
  if(motor_running_now != motor_running_last) {
    xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send to motorRxQueue.");
    }
  }

  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  wdtUpdate.valid = heater_run || motor_run;

  // GHL: HANDLE THIS IN THE HeaterZoneStateUpdate function
  //if (heating) {
  //  nrf_gpio_pin_set(MOTOR_PWR_EN);
  //  nrf_gpio_pin_set(HEATER_PWR_EN);
  //}

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("heater: Unable to send WDT update to watchdog_rxTimesQueue. in heater task");
  }
  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

  if (heater_run || motor_run) {
    pwmMsg.type = PWM_MSG_ENABLE;
  }

  // Respond to heater change
  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
  }

  // Set the last sample based on config
  if (use_default_configuration_parameters) {
    samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
    //heater0SetPoint = DEFAULT_HEATER_SETPOINT_0;
  } else {
    samp_log_max = (config.logging_rate / config.sample_rate);
    //heater0SetPoint = config.heater_setpoint_0;
  }
#endif
}


void samplePrepResetHeaterPIDs(void) {
  pid_controller_init(&heater_pid, MY_H_SP, MY_H_KP, MY_H_KI, MY_H_KD, DEFAULT_MAX_HEATER_PID);
  pid_controller_init(&motor_pid, MY_M_SP, MY_M_KP, MY_M_KI, MY_M_KD, MAX_MOTOR_PID);
  //send_debug_log_message("HEATER_TASK: samplePrepResetHeaterPIDs() PID INIT");
}

void samplePrepUpdateHeaterPIDs(void) {
  pid_controller_update(&heater_pid, MY_H_SP, MY_H_KP, MY_H_KI, MY_H_KD);
  pid_controller_update(&motor_pid, MY_M_SP, MY_M_KP, MY_M_KI, MY_M_KD);
  //send_debug_log_message("HEATER_TASK: samplePrepResetHeaterPIDs() PID UPDATE");
}

void samplePrepHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
  static uint16_t counter = 0;
  counter++;
#ifdef SAMPLE_PREP_BOARD
  //send_debug_log_message("HEATER_TASK: samplePrepHandleHeaterSensorDataRx()");
  BaseType_t xReturned;
  local_temp_data = temperature_data;
  // Ensure temperatures are below the minimum run zone temperature
  if (config.min_run_zone_temp_en) {
    if (starting_sample_prep_run &&
        (temperature_data.heat_zone_0_temp > config.min_run_zone_temp || // TODO: Implement defaults
            temperature_data.heat_zone_1_temp > config.min_run_zone_temp ||
            temperature_data.heat_zone_2_temp > config.min_run_zone_temp ||
            temperature_data.heat_zone_3_temp > config.min_run_zone_temp)) {
      starting_sample_prep_run = false;
      // Send cannot start
      xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
      }
      return;
    } else if (starting_sample_prep_run &&
               (temperature_data.heat_zone_0_temp <= config.min_run_zone_temp && // TODO: Implement defaults
                   temperature_data.heat_zone_1_temp <= config.min_run_zone_temp &&
                   temperature_data.heat_zone_2_temp <= config.min_run_zone_temp &&
                   temperature_data.heat_zone_3_temp <= config.min_run_zone_temp)) {
      // Send can start
      xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
      }
      starting_sample_prep_run = false;
    }
  } else if (!config.min_run_zone_temp_en && starting_sample_prep_run) {
    // Send can start
    xReturned = xQueueSend(main_runRespQueue, &starting_sample_prep_run, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send cannot start run response.");
    }
    starting_sample_prep_run = false;
  }


  // Update PID and PWM
  if (heater_running_now) { //AMP 1 will be used for motor
    // Update Heater Zone 3 PID loop with new temperatures
    //if (config.run_heater_2) {
    pid_controller_compute(&heater_pid, temperature_data.heat_zone_3_temp);
    if (rampToTemp) {
      if (rampToTemp && (temperature_data.heat_zone_3_temp >= heater_pid.setpoint)) {
        xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("HEATER_TASK: Unable to send set point reached message.");
        }
        rampToTemp = false;
      }
    }
    //}

    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = 0,
        .heat_zone_1_pwm = heater_pid.out,
        .heat_zone_2_pwm = 0,
        .heat_zone_3_pwm = heater_pid.out};

    h_pwm_data.heat_zone_0_pwm = pwmData.heat_zone_0_pwm;
    h_pwm_data.heat_zone_1_pwm = pwmData.heat_zone_1_pwm;
    h_pwm_data.heat_zone_3_pwm = pwmData.heat_zone_3_pwm;

    updateDutyCycles(h_pwm_data);

#if VERBOSE_HEATING
    char tmp[150];
    sprintf(tmp, "Heat Zones: %0.2f,%0.2f,%0.2f,%0.2f; PWM: %0.2f,%0.2f,%0.2f,%0.2f", 
                  temperature_data.heat_zone_0_temp, temperature_data.heat_zone_1_temp, temperature_data.heat_zone_2_temp, temperature_data.heat_zone_3_temp, 
                  h_pwm_data.heat_zone_0_pwm, h_pwm_data.heat_zone_1_pwm, h_pwm_data.heat_zone_2_pwm, h_pwm_data.heat_zone_3_pwm);
    send_debug_log_message(tmp);
#endif

    #if 1
    if(counter%10==0) {
      sprintf(tmp, "GHL PID H[ SP=%.01f I=%.01f O=%.01f PWM1=%.01f PWM3=%.01f ]",
        heater_pid.setpoint,heater_pid.intergrator,heater_pid.out,h_pwm_data.heat_zone_1_pwm,h_pwm_data.heat_zone_3_pwm
      );
      send_debug_log_message(tmp);
    }
    #endif

    if ((config.heater_max_temp < temperature_data.heat_zone_3_temp) || temperature_data.heat_zone_3_temp < 0) {
      greater_than_max = true;
    }
  } else{
    // HEATER SHOULDN'T BE RUNNING
    h_pwm_data.heat_zone_0_pwm = 0;
    h_pwm_data.heat_zone_1_pwm = 0;
    h_pwm_data.heat_zone_3_pwm = 0;
    // ch 2 is the motor
    updateDutyCycles(h_pwm_data);
  }


  // Handle being greater than the maximum temperature
  if (greater_than_max) {
    // Send alert message to main task
    xReturned = xQueueSend(main_runErrorQueue, &greater_than_max, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send run error for greater than max temp to main_runErrorQueue.");
    }
    greater_than_max = false;
  }



  logMsg.temperature_data.heat_zone_1_temp = temperature_data.heat_zone_1_temp;
  logMsg.temperature_data.heat_zone_2_temp = temperature_data.heat_zone_2_temp;
  logMsg.temperature_data.heat_zone_3_temp = temperature_data.heat_zone_3_temp;
  logMsg.temperature_data.heat_zone_0_temp = temperature_data.heat_zone_0_temp;
  logMsg.temperature_data.heat_zone_1_pwm = h_pwm_data.heat_zone_1_pwm;
  logMsg.temperature_data.heat_zone_2_pwm = h_pwm_data.heat_zone_2_pwm;
  logMsg.temperature_data.heat_zone_3_pwm = h_pwm_data.heat_zone_3_pwm;
  logMsg.temperature_data.heat_zone_0_pwm = h_pwm_data.heat_zone_0_pwm;
  logMsg.temperature_data.motorSpeed = last_motor_speed;
  
  logMsg.data_type = UART_DATA;
  // Send the Log message
  xReturned = xQueueSend(logger_logMessageQueue, (void *)&logMsg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.");
  }
  //send_debug_log_message("GHL SEnsorDataRx\r\n");
  samp_log_index++;
  if (samp_log_index >= samp_log_max) {
    logMsg.data_type = TEMPERATURE_DATA;
    samp_log_index = 0;
    // Send the Log message
    xReturned = xQueueSend(logger_logMessageQueue, (void *)&logMsg, 0);
    if (xReturned != pdPASS) {
      send_debug_log_message("SENSORS_TASK: Unable to send log message to logger_logMessageQueue.");
    }
  }
  #endif
}

void handleSampleMotorDataRx(int motor_speed) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;

  float this_motor_setpoint = motor_pid.setpoint;

  // calculate pid
  if (motor_running_now) {
    pid_controller_compute(&motor_pid, motor_speed);
    h_pwm_data.heat_zone_2_pwm = motor_pid.out;
  } else{
    h_pwm_data.heat_zone_2_pwm = 0; // no pwm the motor
  }
  //h_pwm_data.heat_zone_2_pwm = 0; // no pwm the motor

  last_motor_speed = motor_speed;

  if (motor_speed >= this_motor_setpoint) {
    motorReachedSpeed = true;
  }

  #if VERBOSE_MOTOR
  sprintf(tmp, "Motor Speed: %d rpm; Motor PWM: %0.2f", motor_speed, pwmData.heat_zone_2_pwm);
  send_debug_log_message(tmp);
  #endif

  if( motorReachedSpeed && motor_speed<(this_motor_setpoint-((float) this_motor_setpoint * ((float)config.motor_stall_percent / 100.0 ))) ) {
    motorStalledPercent = true;
    motorReachedSpeed = false;
  }

  if (motorReachedSpeed && (motor_pid.out > config.motor_stall_pwm)) {
    motorStalledPWM = true;
    motorReachedSpeed = false;
  }

  updateDutyCycles(h_pwm_data);

  if (motorStalledPWM || motorStalledPercent) {
    // Send alert message to main task
    if (motorStalledPercent) {
      xReturned = xQueueSend(main_runErrorQueue, &motor_stall_percent_err_msg, 0);
    } else {
      xReturned = xQueueSend(main_runErrorQueue, &motor_stall_pwm_err_msg, 0);
    }
    if (xReturned != pdPASS) {
      send_debug_log_message("HEATER_TASK: Unable to send run error for motor stalled to main_runErrorQueue.");
    }
    motorStalledPWM = false;
    motorStalledPercent = false;
  }

#endif
}

void samplePrepHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  
  //bool cycle_enabled = heaterRxMessage.cycleEnabled;
  
  bool heater_run = false;
  bool motor_run = false;
  char tmp[100];


  if (heaterRxMessage.type == HEATER_MSG_ZONE_STATE) {

    // Set zones enabled
    if(heaterRxMessage.cycleEnabled) {
      // BRING IN PARAMETERS FROM THE MESSAGE
      rampToTemp = heaterRxMessage.rampToTemp;
      rx_runheater = heaterRxMessage.heaterRunning;
      MY_H_SP = heaterRxMessage.tempSetpoint1;
      MY_H_KP = heaterRxMessage.HEATER_KP;
      MY_H_KI = heaterRxMessage.HEATER_KI;
      MY_H_KD = heaterRxMessage.HEATER_KD;
      rx_runmotor = heaterRxMessage.motorSpeed>0;
      MY_M_SP = heaterRxMessage.motorSpeed;
      MY_M_KP = heaterRxMessage.MOTOR_KP;
      MY_M_KI = heaterRxMessage.MOTOR_KI;
      MY_M_KD = heaterRxMessage.MOTOR_KD;

      // UPDATE EVERYTHING
      switch(heaterRxMessage.cycleSelect) {
        case CYCLE_ZERO:
          // INITIAL STATE
          starting_sample_prep_run = true;    // GHL: appears to be used throughout this code for something useful

          // initialize
          last_motor_speed = 0;
          h_pwm_data.heat_zone_0_pwm = 0;
          h_pwm_data.heat_zone_1_pwm = 0;
          h_pwm_data.heat_zone_2_pwm = 0;
          h_pwm_data.heat_zone_3_pwm = 0;

          // PID UPDATE
          samplePrepResetHeaterPIDs();
          break;
        default:
          // PID UPDATE
          samplePrepUpdateHeaterPIDs();
          break;
      }
    } else{
      // WE ARE SHUTTING DOWN
      rx_runheater = false;
      rx_runmotor = false;
    }


    heater_run = doWeRunHeaterInThisCycle();
    motor_run = doWeRunMotorInThisCycle();
    sprintf(tmp,"samplePrepHandleHeaterZoneStateUpdate(): HEATER=%d(%.1f) MOTOR=%d(%.0f)",heater_run, MY_H_SP, motor_run, MY_M_SP);
    send_debug_log_message(tmp);
    
    motorReachedSpeed = false;

    // Actually configure this heater task and
    // Send starting heater to sensors task
    handle_cycle_stopstart_heater(heater_run);
    

  }  else if (heaterRxMessage.type == HEATER_MSG_MOTOR_STATE){
      // SG/GHL : I think no longer used?

      h_pwm_data.heat_zone_0_pwm = 0;
      h_pwm_data.heat_zone_1_pwm = 0;
      h_pwm_data.heat_zone_2_pwm = 0;
      h_pwm_data.heat_zone_3_pwm = 0;
      h_pwm_data.sample_prep_heater_pwm = 0;

      
      send_debug_log_message("heaterRxMessage.type == HEATER_MSG_MOTOR_STATE   handle_cycle_stopstart_heater(false) will now turn off, because CYCLE_NONE is active");

      handle_cycle_stopstart_heater(false);


      motorReachedSpeed = false;
      last_motor_speed = 0;

      PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

      xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
      if (xReturned != pdPASS) {
        send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
      }
  }
  #endif
}

temperature_pwm_data_t getSamplePrepPwmData(void) {
  return h_pwm_data;
}

bool getSamplePrepOverTempStatus(void) {
  return greater_than_max;
}

temperature_data_t getSamplePrepOverTempData(void) {
  return local_temp_data;
}

bool getSamplePrepHeaterRunningStatus(void) {
  return heater_running_now;
}