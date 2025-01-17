#include "samplePrepHeater.h"
#include "../motor.h"
#include "../usb.h"
#include "timers.h"
#include "../naatos_config.h"

const MainStateErrorQueueMsg_t motor_stall_percent_err_msg = {
  .errType = ERR_MOTOR_STALLED_PERCENT,
  .overTempData = NULL
};

const MainStateErrorQueueMsg_t motor_stall_pwm_err_msg = {
  .errType = ERR_MOTOR_STALLED_PWM,
  .overTempData = NULL
};

cycle_t fsm_cycle_current = CYCLE_NONE;
cycle_t fsm_cycle_last    = CYCLE_NONE;

bool heater_running_now = false;
bool heater_running_last = false;
bool motor_running_now = false;
bool motor_running_last = false;


pid_controller_t heater_pid;
//pid_controller_t heater_pid_0;
//pid_controller_t heater_pid_1;
//pid_controller_t heater_pid_1b;
//pid_controller_t heater_pid_2;
//pid_controller_t motor_pid_1;
//pid_controller_t motor_pid_2;
pid_controller_t motor_pid;

//float heater0SetPoint = 0;
//float heater1SetPoint = 0;
//float heater1bSetPoint = 0;
//float heater2SetPoint = 0;

//bool heater_cycle0_running = false;
//bool heater_cycle1_running = false;
//bool heater_cycle1b_running = false;
//bool heater_cycle2_running = false;
//bool motor_cycle1_running = false;
//bool motor_cycle1b_running = false;
//bool motor_cycle2_running = false;
//bool motor_cycle3_running = false;

bool starting_sample_prep_run = true;
bool h_pwm_req = false;
bool greater_than_max = false;
//bool heater_run = false;
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
  switch(fsm_cycle_current) {
    case CYCLE_ZERO:    return true;
    case CYCLE_ONE:     return use_default_configuration_parameters ? DEFAULT_RUN_HEATER_1 : config.run_heater_1;
    case CYCLE_ONE_B:   return use_default_configuration_parameters ? DEFAULT_RUN_HEATER_2 : config.run_heater_2;
    case CYCLE_TWO:     return false;
    default:            return false;
  }
}

bool doWeRunMotorInThisCycle() {
  switch(fsm_cycle_current) {
    case CYCLE_ZERO:    return false;
    case CYCLE_ONE:     return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_1 : config.run_motor_1;
    case CYCLE_ONE_B:   return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_2 : config.run_motor_2;
    case CYCLE_TWO:     return use_default_configuration_parameters ? DEFAULT_RUN_MOTOR_2 : config.run_motor_2;
    default:            return false;
  }
}

//void handle_cycle2_stopstart_heater(bool heating) {
//#ifdef SAMPLE_PREP_BOARD
//  BaseType_t xReturned;

//  // Handle case where cycle one is already running, dont want to send stop
//  if (heater_cycle1_running && !heating)
//    return;

//  if (!heating) { //need to stop supply before stopping pwm
//    nrf_gpio_pin_clear(HEATER_PWR_EN);
//  }

//  SensorRxQueueMsg_t msg;
//  msg.type = SENSOR_MSG_HEATER_STATE;
//  msg.heaterRunning = heating;

//  // Send the heater status
//  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
//  }

//  MotorRxQueueMsg_t motorMsg;
//  motorMsg.type = MOTOR_MSG_HEATER_STATE;
//  motorMsg.motorRunning = heating;

//  // Send the motor status
//  if (motorMsg.motorRunning) {
//    motor_cycle2_running = true;
//    xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
//    if (xReturned != pdPASS) {
//      send_debug_log_message("HEATER_TASK: Unable to send heater state to motorRxQueue.");
//    }
//  }

//  watchdog_time_update_t wdtUpdate = {
//      .taskName = HEATER,
//      .valid = false};
//  wdtUpdate.valid = heating;

//  if (heating) {
//    nrf_gpio_pin_set(MOTOR_PWR_EN);
//  }

//  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
//  }

//  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

//  if (heating) {
//    pwmMsg.type = PWM_MSG_ENABLE;
//  }

//  // Respond to heater change
//  if (motorMsg.motorRunning) {
//    xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
//    if (xReturned != pdPASS) {
//      send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
//    }
// }

//  // Set the last sample based on config
//  if (use_default_configuration_parameters) {
//    samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
//    heater2SetPoint = DEFAULT_HEATER_SETPOINT_2;
//  } else {
//    samp_log_max = (config.logging_rate / config.sample_rate);
//    heater2SetPoint = config.heater_setpoint_2;
//  }
//#endif
//}

//void handle_cycle1b_stopstart_heater(bool heating) {
//#ifdef SAMPLE_PREP_BOARD
//  BaseType_t xReturned;

//  // Handle case where cycle 1 is already running, dont want to send stop
//  if (heater_cycle1_running && !heating)
//    return;

//  if (!heating) { //need to stop supply before stopping pwm
//    nrf_gpio_pin_clear(HEATER_PWR_EN);
//  }

//  SensorRxQueueMsg_t msg;
//  msg.type = SENSOR_MSG_HEATER_STATE;
//  msg.heaterRunning = heating;

//  // Send the heater status
//  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
//  }

//  MotorRxQueueMsg_t motorMsg;
//  motorMsg.type = MOTOR_MSG_HEATER_STATE;
//  motorMsg.motorRunning = heating;

//  // Send the motor status
//  if (motorMsg.motorRunning) {
//    motor_cycle1b_running = true;
//    xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
//    if (xReturned != pdPASS) {
//      send_debug_log_message("HEATER_TASK: Unable to send heater state to motorRxQueue.");
//    }
//  }

//  watchdog_time_update_t wdtUpdate = {
//      .taskName = HEATER,
//      .valid = false};
//  wdtUpdate.valid = heating;

//  if (heating) {
//    nrf_gpio_pin_set(MOTOR_PWR_EN);
//  }

//  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
//  }

//  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

//  if (heating) {
//    pwmMsg.type = PWM_MSG_ENABLE;
//  }

//  // Respond to heater change
//  if (motorMsg.motorRunning) {
//    xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
//    if (xReturned != pdPASS) {
//      send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
//    }
// }

//  // Set the last sample based on config
//  if (use_default_configuration_parameters) {
//    samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
//    //heater1SetPoint = DEFAULT_HEATER_SETPOINT_1;
//  } else {
//    samp_log_max = (config.logging_rate / config.sample_rate);
//    //heater1SetPoint = config.heater_setpoint_1;
//  }
//#endif
//}

//void handle_cycle1_stopstart_heater(bool heating) {
//#ifdef SAMPLE_PREP_BOARD
//  BaseType_t xReturned;

//  // Handle case where cycle zero is already running, dont want to send stop
//  if (heater_cycle0_running && !heating)
//    return;

//  if (!heating) { //need to stop supply before stopping pwm
//    nrf_gpio_pin_clear(HEATER_PWR_EN);
//  }

//  SensorRxQueueMsg_t msg;
//  msg.type = SENSOR_MSG_HEATER_STATE;
//  msg.heaterRunning = heating;

//  // Send the heater status
//  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
//  }

//  MotorRxQueueMsg_t motorMsg;
//  motorMsg.type = MOTOR_MSG_HEATER_STATE;
//  motorMsg.motorRunning = heating;

//  // Send the motor status
//  if (motorMsg.motorRunning) {
//    motor_cycle1_running = true;
//    xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
//    if (xReturned != pdPASS) {
//      send_debug_log_message("HEATER_TASK: Unable to send heater state to motorRxQueue.");
//    }
//  }

//  watchdog_time_update_t wdtUpdate = {
//      .taskName = HEATER,
//      .valid = false};
//  wdtUpdate.valid = heating;

//  if (heating) {
//    nrf_gpio_pin_set(MOTOR_PWR_EN);
//  }

//  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
//  }

//  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

//  if (heating) {
//    pwmMsg.type = PWM_MSG_ENABLE;
//  }

//  // Respond to heater change
//  if (motorMsg.motorRunning) {
//    xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
//    if (xReturned != pdPASS) {
//      send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
//    }
// }

//  // Set the last sample based on config
//  if (use_default_configuration_parameters) {
//    samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
//    heater1SetPoint = DEFAULT_HEATER_SETPOINT_1;
//  } else {
//    samp_log_max = (config.logging_rate / config.sample_rate);
//    heater1SetPoint = config.heater_setpoint_1;
//  }
//#endif
//}

//void handle_cycle0_stopstart_heater(bool heating) {
//#ifdef SAMPLE_PREP_BOARD
//  BaseType_t xReturned;

//  // Handle case where valve zone is on already, dont want to send stop
//  if (heater_cycle2_running && !heating)
//    return;
  
//  // need to stop supply before stopping pwm
//  if (!heating && !config.run_motor_2 && !use_default_configuration_parameters) { 
//    nrf_gpio_pin_clear(MOTOR_PWR_EN);
//  }
//  else if (!heating && DEFAULT_RUN_MOTOR_2 && use_default_configuration_parameters) {
//    nrf_gpio_pin_clear(MOTOR_PWR_EN);
//  }
//  SensorRxQueueMsg_t msg;
//  msg.type = SENSOR_MSG_HEATER_STATE;
//  msg.heaterRunning = heating;

//  // Send the heater status
//  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
//  }

//  MotorRxQueueMsg_t motorMsg;
//  motorMsg.type = MOTOR_MSG_HEATER_STATE;
//  motorMsg.motorRunning = heating;

//  // Send the motor status
//  xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("HEATER_TASK: Unable to send heater state to motorRxQueue.");
//  }

//  watchdog_time_update_t wdtUpdate = {
//      .taskName = HEATER,
//      .valid = false};
//  wdtUpdate.valid = heating;

//  if (heating) {
//    nrf_gpio_pin_set(MOTOR_PWR_EN);
//    nrf_gpio_pin_set(HEATER_PWR_EN);
//  }

//  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
//  }

//  PwmRxQueueMsg_t pwmMsg = {.type = PWM_MSG_DISABLE};

//  if (heating) {
//    pwmMsg.type = PWM_MSG_ENABLE;
//  }

//  // Respond to heater change
//  xReturned = xQueueSend(pwmRxQueue, &pwmMsg, 0);
//  if (xReturned != pdPASS) {
//    send_debug_log_message("heater: Unable to send stop to pwmRxQueue.");
//  }

//  // Set the last sample based on config
//  if (use_default_configuration_parameters) {
//    samp_log_max = (DEFAULT_LOGGING_RATE / DEFAULT_SAMPLE_RATE);
//    heater0SetPoint = DEFAULT_HEATER_SETPOINT_0;
//  } else {
//    samp_log_max = (config.logging_rate / config.sample_rate);
//    heater0SetPoint = config.heater_setpoint_0;
//  }
//#endif
//}


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

  SensorRxQueueMsg_t msg;
  msg.type = SENSOR_MSG_HEATER_STATE;
  //msg.heaterRunning = heating;    // SG GHL (this has to be TRUE or else all logging will stop)
  msg.heaterRunning = doWeRunHeaterInThisCycle() || doWeRunMotorInThisCycle();    // SG GHL (this has to be TRUE or else all logging will stop)

  // Send the heater status
  xReturned = xQueueSend(sensorRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to sensorRxQueue.");
  }

  MotorRxQueueMsg_t motorMsg;
  motorMsg.type = MOTOR_MSG_HEATER_STATE;
  motorMsg.motorRunning = heating;

  // Send the motor status
  xReturned = xQueueSend(motorRxQueue, &motorMsg, 10);
  if (xReturned != pdPASS) {
    send_debug_log_message("HEATER_TASK: Unable to send heater state to motorRxQueue.");
  }

  watchdog_time_update_t wdtUpdate = {
      .taskName = HEATER,
      .valid = false};
  wdtUpdate.valid = heating;

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

  if (heating) {
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
#ifdef SAMPLE_PREP_BOARD
  // CYCLE -ZERO-
  //// Create PID Controllers
  //if (use_default_configuration_parameters) {
  //  pid_controller_init(&heater_pid_0, DEFAULT_HEATER_SETPOINT_0, H_KP_1, H_KI_1, H_KD_1, DEFAULT_MAX_HEATER_PID);
  //} else {
  //  pid_controller_init(&heater_pid_0, config.heater_setpoint_0, config.heater_kp_1, config.heater_ki_1, config.heater_kd_1, config.max_heater_pid_pwm);
  //}

  //// Create PID Controllers
  //if (use_default_configuration_parameters) {
  //  pid_controller_init(&heater_pid_1, DEFAULT_HEATER_SETPOINT_1, H_KP_1, H_KI_1, H_KD_1, DEFAULT_MAX_HEATER_PID);
  //  pid_controller_init(&motor_pid_1, MOTOR_SETPOINT_1, M_KP, M_KI, M_KD, 100);
  //} else {
  //  pid_controller_init(&heater_pid_1, config.heater_setpoint_1, config.heater_kp_1, config.heater_ki_1, config.heater_kd_1, config.max_heater_pid_pwm);
  //  pid_controller_init(&motor_pid_1, config.motor_setpoint_1, config.motor_kp_1, config.motor_ki_1, config.motor_kd_1, MAX_MOTOR_PID);
  //}
  //// Create PID Controllers
  //if (use_default_configuration_parameters) {
  //  pid_controller_init(&heater_pid_2, DEFAULT_HEATER_SETPOINT_2, H_KP_2, H_KI_2, H_KD_2, DEFAULT_MAX_HEATER_PID);
  //  pid_controller_init(&motor_pid_2, MOTOR_SETPOINT_2, M_KP, M_KI, M_KD, 100);
  //} else {
  //  pid_controller_init(&heater_pid_2, config.heater_setpoint_2, config.heater_kp_2, config.heater_ki_2, config.heater_kd_2, config.max_heater_pid_pwm);
  //  pid_controller_init(&motor_pid_2, config.motor_setpoint_2, config.motor_kp_2, config.motor_ki_2, config.motor_kd_2, MAX_MOTOR_PID);
  //}

  //pid_controller_init(&heater_pid_2, DEFAULT_HEATER_SETPOINT_2, H_KP_2, H_KI_2, H_KD_2, DEFAULT_MAX_HEATER_PID);
  //pid_controller_init(&motor_pid_2, MOTOR_SETPOINT_2, M_KP, M_KI, M_KD, 100);
  float MY_H_SP; float MY_H_KP; float MY_H_KI; float MY_H_KD;
  float MY_M_SP; float MY_M_KP; float MY_M_KI; float MY_M_KD;

  switch(fsm_cycle_current) {
    case CYCLE_ZERO:
      MY_H_SP = use_default_configuration_parameters ? DEFAULT_HEATER_SETPOINT_0 : config.heater_setpoint_0;
      MY_H_KP = use_default_configuration_parameters ? H_KP_1 : config.heater_kp_1;
      MY_H_KI = use_default_configuration_parameters ? H_KI_1 : config.heater_ki_1;
      MY_H_KD = use_default_configuration_parameters ? H_KD_1 : config.heater_kd_1;

      MY_M_SP = use_default_configuration_parameters ? MOTOR_SETPOINT_1 : config.motor_setpoint_1;
      MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_1;
      MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_1;
      MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_1;
      break;
    case CYCLE_ONE:
      MY_H_SP = use_default_configuration_parameters ? DEFAULT_HEATER_SETPOINT_1 : config.heater_setpoint_1;
      MY_H_KP = use_default_configuration_parameters ? H_KP_1 : config.heater_kp_1;
      MY_H_KI = use_default_configuration_parameters ? H_KI_1 : config.heater_ki_1;
      MY_H_KD = use_default_configuration_parameters ? H_KD_1 : config.heater_kd_1;

      MY_M_SP = use_default_configuration_parameters ? MOTOR_SETPOINT_1 : config.motor_setpoint_1;
      MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_1;
      MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_1;
      MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_1;
      break;
    case CYCLE_ONE_B:
      MY_H_SP = use_default_configuration_parameters ? DEFAULT_HEATER_SETPOINT_1 : config.heater_setpoint_1;
      MY_H_KP = use_default_configuration_parameters ? H_KP_2 : config.heater_kp_2;
      MY_H_KI = use_default_configuration_parameters ? H_KI_2 : config.heater_ki_2;
      MY_H_KD = use_default_configuration_parameters ? H_KD_2 : config.heater_kd_2;

      MY_M_SP = use_default_configuration_parameters ? MOTOR_SETPOINT_1 : config.motor_setpoint_1;
      MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_1;
      MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_1;
      MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_1;
      break;
    case CYCLE_TWO:
      MY_H_SP = 0;
      MY_H_KP = use_default_configuration_parameters ? H_KP_2 : config.heater_kp_2;
      MY_H_KI = use_default_configuration_parameters ? H_KI_2 : config.heater_ki_2;
      MY_H_KD = use_default_configuration_parameters ? H_KD_2 : config.heater_kd_2;

      MY_M_SP = use_default_configuration_parameters ? MOTOR_SETPOINT_2 : config.motor_setpoint_2;
      MY_M_KP = use_default_configuration_parameters ? M_KP : config.motor_kp_2;
      MY_M_KI = use_default_configuration_parameters ? M_KI : config.motor_ki_2;
      MY_M_KD = use_default_configuration_parameters ? M_KD : config.motor_kd_2;
      break;
    default:
      send_debug_log_message("HEATER_TASK: samplePrepResetHeaterPIDs() DEFAULT CASE1");
      break;
  }

  switch(fsm_cycle_current) {
    case CYCLE_ZERO:
      pid_controller_init(&heater_pid, MY_H_SP, MY_H_KP, MY_H_KI, MY_H_KD, DEFAULT_MAX_HEATER_PID);
      pid_controller_init(&motor_pid, MY_M_SP, MY_M_KP, MY_M_KI, MY_M_KD, MAX_MOTOR_PID);
      send_debug_log_message("HEATER_TASK: samplePrepResetHeaterPIDs() PID INIT");
      break;
    case CYCLE_ONE:
    case CYCLE_ONE_B:
    case CYCLE_TWO:
      pid_controller_update(&heater_pid, MY_H_SP, MY_H_KP, MY_H_KI, MY_H_KD);
      pid_controller_update(&motor_pid, MY_M_SP, MY_M_KP, MY_M_KI, MY_M_KD);
      send_debug_log_message("HEATER_TASK: samplePrepResetHeaterPIDs() PID UPDATE");
      break;
    default:
      send_debug_log_message("HEATER_TASK: samplePrepResetHeaterPIDs() DEFAULT CASE2");
      break;
  }
  #endif
}

void samplePrepHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
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

  /*
  // Update PID and PWM
  if (heater_cycle0_running) {
    // Update Heater Zone 3 PID loop with new temperatures
    if (config.run_heater_1) {
      pid_controller_compute(&heater_pid_0, temperature_data.heat_zone_3_temp);
      if (rampToTemp) {
        if ((temperature_data.heat_zone_3_temp >= config.heater_setpoint_0)) {
          xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
          if (xReturned != pdPASS) {
            send_debug_log_message("HEATER_TASK: Unable to send set point reached message.");
          }
          rampToTemp = false;
        }
      }
    }

    temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = 0,
          .heat_zone_1_pwm = heater_pid_0.out,        //<--- heat_zone_1 makes it actually work!!
          .sample_prep_heater_pwm = heater_pid_0.out,
          .heat_zone_2_pwm = 0,
          .heat_zone_3_pwm = heater_pid_0.out};       //<--- but heat_zone 3 is the only one logged!!

    // Update Heater Zone 3 PWM with PID output
    h_pwm_data.heat_zone_0_pwm = pwmData.heat_zone_0_pwm;
    h_pwm_data.heat_zone_1_pwm = pwmData.heat_zone_1_pwm;
    // Dont Heater Zone 2, its from the motor task
    h_pwm_data.heat_zone_3_pwm = pwmData.heat_zone_3_pwm;

    updateDutyCycles(h_pwm_data);

#if VERBOSE_HEATING
    char tmp[150];
    sprintf(tmp, "Heat Zones: %0.2f,%0.2f,%0.2f,%0.2f; PWM: %0.2f,%0.2f,%0.2f,%0.2f", 
                  temperature_data.heat_zone_0_temp, temperature_data.heat_zone_1_temp, temperature_data.heat_zone_2_temp, temperature_data.heat_zone_3_temp, 
                  h_pwm_data.heat_zone_0_pwm, h_pwm_data.heat_zone_1_pwm, h_pwm_data.heat_zone_2_pwm, h_pwm_data.heat_zone_3_pwm);
    send_debug_log_message(tmp);
#endif

    if ((config.heater_max_temp < temperature_data.heat_zone_3_temp) || temperature_data.heat_zone_3_temp < 0) {
      greater_than_max = true;
    }
  }

  if (heater_cycle1_running) {
    // Update Heater Zone 3 PID loop with new temperatures
    if (config.run_heater_1) {
      pid_controller_compute(&heater_pid_1, temperature_data.heat_zone_3_temp);
      if (rampToTemp) {
        if ((temperature_data.heat_zone_3_temp >= config.heater_setpoint_1)) {
          xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
          if (xReturned != pdPASS) {
            send_debug_log_message("HEATER_TASK: Unable to send set point reached message.");
          }
          rampToTemp = false;
        }
      }
    }

    temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = 0,
          .heat_zone_1_pwm = heater_pid_1.out,
          .sample_prep_heater_pwm = heater_pid_1.out,
          .heat_zone_2_pwm = 0,
          .heat_zone_3_pwm = heater_pid_1.out};

    // Update Heater Zone 3 PWM with PID output
    h_pwm_data.heat_zone_0_pwm = pwmData.heat_zone_0_pwm;
    h_pwm_data.heat_zone_1_pwm = pwmData.heat_zone_1_pwm;
    // Dont Heater Zone 2, its from the motor task
    h_pwm_data.heat_zone_3_pwm = pwmData.heat_zone_3_pwm;

    updateDutyCycles(h_pwm_data);

#if VERBOSE_HEATING
    char tmp[150];
    sprintf(tmp, "Heat Zones: %0.2f,%0.2f,%0.2f,%0.2f; PWM: %0.2f,%0.2f,%0.2f,%0.2f", 
                  temperature_data.heat_zone_0_temp, temperature_data.heat_zone_1_temp, temperature_data.heat_zone_2_temp, temperature_data.heat_zone_3_temp, 
                  h_pwm_data.heat_zone_0_pwm, h_pwm_data.heat_zone_1_pwm, h_pwm_data.heat_zone_2_pwm, h_pwm_data.heat_zone_3_pwm);
    send_debug_log_message(tmp);
#endif

    if ((config.heater_max_temp < temperature_data.heat_zone_3_temp) || temperature_data.heat_zone_3_temp < 0) {
      greater_than_max = true;
    }
  }

  if (heater_cycle2_running) { //AMP 1 will be used for motor
    // Update Heater Zone 3 PID loop with new temperatures
    if (config.run_heater_2) {
      pid_controller_compute(&heater_pid_2, temperature_data.heat_zone_3_temp);
      if (rampToTemp) {
        if (rampToTemp && (temperature_data.heat_zone_3_temp >= config.heater_setpoint_2)) {
          xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
          if (xReturned != pdPASS) {
            send_debug_log_message("HEATER_TASK: Unable to send set point reached message.");
          }
          rampToTemp = false;
        }
      }
    }

    temperature_pwm_data_t pwmData = {
        .heat_zone_0_pwm = 0,
        .heat_zone_1_pwm = heater_pid_2.out,
        .heat_zone_2_pwm = 0,
        .heat_zone_3_pwm = heater_pid_2.out};

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

    if ((config.heater_max_temp < temperature_data.heat_zone_3_temp) || temperature_data.heat_zone_3_temp < 0) {
      greater_than_max = true;
    }
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

  */

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
  char tmp[100];

  float this_motor_setpoint;
  bool this_run_motor;
  pid_controller_t* this_pid_motor;
  pid_controller_t* this_pid_temp;


  /*

  // SET VALUES AND POINTERS FOR THE CYCLE WE'RE ON
  if(heater_cycle1_running) {
    this_pid_temp = &heater_pid_1;    
    this_pid_motor = &motor_pid_1;
    this_run_motor = config.run_motor_1;
    if( use_default_configuration_parameters )  {
      this_motor_setpoint = MOTOR_SETPOINT_1;
    } else{
      this_motor_setpoint = config.motor_setpoint_1;
    }
  } else if(heater_cycle2_running) {
    this_pid_temp = &heater_pid_2;    
    this_pid_motor = &motor_pid_2;
    this_run_motor = config.run_motor_2;
    if( use_default_configuration_parameters )  {
      this_motor_setpoint = MOTOR_SETPOINT_2;
    } else{
      this_motor_setpoint = config.motor_setpoint_2;
    }
  }

  // CONSOLIDATED OLD CODE FROM LIAM
  temperature_pwm_data_t pwmData = {
    .heat_zone_0_pwm = 0,
    //.heat_zone_1_pwm = heater_pid_2.out,
    .heat_zone_1_pwm = this_pid_temp->out,
    .heat_zone_2_pwm = 0,
    .heat_zone_3_pwm = 0
  };

  if (this_run_motor) {
    pid_controller_compute(this_pid_motor, motor_speed);
    pwmData.heat_zone_2_pwm = this_pid_motor->out;
  }

  h_pwm_data.heat_zone_2_pwm = pwmData.heat_zone_2_pwm;
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

  if (motorReachedSpeed && (this_pid_motor->out > config.motor_stall_pwm)) {
    motorStalledPWM = true;
    motorReachedSpeed = false;
  }

  updateDutyCycles(pwmData);

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
  */
#endif
}

void samplePrepHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
#ifdef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  bool cycle_enabled = heaterRxMessage.cycleEnabled;
  bool heater_run = false;
  bool motor_run = false;
  char tmp[100];


  if (heaterRxMessage.type == HEATER_MSG_ZONE_STATE) {

    // SG/GHL: we will only use cycleEnabled signals to affect any change!!
    //if(!cycle_enabled)
    //  return;
    
    // Set zones enabled
    switch(fsm_cycle_current) {
      case CYCLE_ZERO:
        starting_sample_prep_run = true;    // GHL: appears to be used throughout this code for something useful
        rampToTemp = false;
        break;
      case CYCLE_ONE:
        rampToTemp = false;
        break;
      case CYCLE_ONE_B:
        rampToTemp = false;
        break;
      case CYCLE_TWO:
        rampToTemp = false;
        break;
      default:
        break;
      //case CYCLE_ZERO:
        //break;
    }

    // Set the cycle we are on
    if(fsm_cycle_current != heaterRxMessage.cycleSelect)
      fsm_cycle_last = fsm_cycle_current;
    fsm_cycle_current = heaterRxMessage.cycleSelect;
    heater_run = doWeRunHeaterInThisCycle();
    motor_run = doWeRunMotorInThisCycle();
    sprintf(tmp,"samplePrepHandleHeaterZoneStateUpdate(): CYCLE_LAST=%d CYCLE_NOW=%d HEATER=%d MOTOR=%d",(uint8_t) fsm_cycle_last, (uint8_t) fsm_cycle_current, heater_run, motor_run);
    send_debug_log_message(tmp);
    

    // Set heater parameters
    if(heater_run != heater_running_now)  {
      // heater state changed in this cycle
      heater_running_last = heater_running_now;

      if(heater_run) {
        nrf_gpio_pin_set(HEATER_PWR_EN);
        send_debug_log_message("nrf_gpio_pin_set(HEATER_PWR_EN)");
      } else{
        nrf_gpio_pin_clear(HEATER_PWR_EN);
        send_debug_log_message("nrf_gpio_pin_clear(HEATER_PWR_EN)");
      }
    }
    heater_running_now = heater_run;


    // Set motor parameters
    if(motor_run != motor_running_now)  {
      // motor state changed in this cycle
      motor_running_last = motor_run;
      
      if(motor_run) {
        nrf_gpio_pin_set(MOTOR_PWR_EN);
        send_debug_log_message("nrf_gpio_pin_set(MOTOR_PWR_EN)");
      } else{
        nrf_gpio_pin_clear(MOTOR_PWR_EN);
        send_debug_log_message("nrf_gpio_pin_clear(MOTOR_PWR_EN)");
      }
    }
    motor_running_now = motor_run;
    motorReachedSpeed = false;

    // Send starting heater to sensors task
    handle_cycle_stopstart_heater(heater_run);
    
    


    //if (heaterRxMessage.cycleSelect == CYCLE_ZERO) {
    //  heater_cycle0_running = heaterRxMessage.cycleEnabled;
    //  rampToTemp = false;
    //  if (!heater_cycle0_running) { //AMP2 will be used in sample prep for heating
    //    send_debug_log_message("samplePrepHandleHeaterZoneStateUpdate(): CYCLE_ZERO (not running heater)");
    //    heater_pid_0.out = 0;
    //    temperature_pwm_data_t pwmData = {
    //        .heat_zone_0_pwm = 0,
    //        .heat_zone_1_pwm = heater_pid_0.out,
    //        .sample_prep_heater_pwm = heater_pid_0.out,
    //        .heat_zone_2_pwm = 0,
    //        .heat_zone_3_pwm = 0};
    //    updateDutyCycles(pwmData);
      
    //    //if (!config.run_motor_2 && !use_default_configuration_parameters) {
    //    //  nrf_gpio_pin_clear(MOTOR_PWR_EN);
    //    //}
    //    //else if (!DEFAULT_RUN_MOTOR_2 && use_default_configuration_parameters) {
    //    //  nrf_gpio_pin_clear(MOTOR_PWR_EN);
    //    //}
    //    vTaskDelay(pdMS_TO_TICKS(200));
    //    updateDutyCycles(pwmData);

    //    // Send stop heater to sensors task
    //    heater_run = false;
    //    starting_sample_prep_run = false;
    //    motorReachedSpeed = false; // clear motor speed reached
    //    handle_cycle1_stopstart_heater(heater_run);

    //  } else {
    //    send_debug_log_message("samplePrepHandleHeaterZoneStateUpdate(): CYCLE_ZERO (yes, running the heater)");
    //    starting_sample_prep_run = true;
    //    heater_run = true;
    //    motorReachedSpeed = false;
    //    samplePrepResetHeaterPIDs();
    //    // Send starting heater to sensors task
    //    handle_cycle0_stopstart_heater(heater_run);

    //  }
    //}
    //else if (heaterRxMessage.cycleSelect == CYCLE_ONE) {
    //  heater_cycle1_running = heaterRxMessage.cycleEnabled;
    //  rampToTemp = config.ramp_to_temp_before_start_cycle_1;
    //  if (!heater_cycle1_running) { //AMP2 will be used in sample prep for heating
    //    heater_pid_1.out = 0;
    //    temperature_pwm_data_t pwmData = {
    //        .heat_zone_0_pwm = 0,
    //        .heat_zone_1_pwm = heater_pid_1.out,
    //        .sample_prep_heater_pwm = heater_pid_1.out,
    //        .heat_zone_2_pwm = 0,
    //        .heat_zone_3_pwm = 0};
    //    updateDutyCycles(pwmData);
      
    //    if (!config.run_motor_2 && !use_default_configuration_parameters) {
    //      nrf_gpio_pin_clear(MOTOR_PWR_EN);
    //    }
    //    else if (!DEFAULT_RUN_MOTOR_2 && use_default_configuration_parameters) {
    //      nrf_gpio_pin_clear(MOTOR_PWR_EN);
    //    }
    //    vTaskDelay(pdMS_TO_TICKS(200));
    //    updateDutyCycles(pwmData);
    //    // Send stop heater to sensors task
    //    heater_run = false;
    //    starting_sample_prep_run = false;
    //    motorReachedSpeed = false; // clear motor speed reached
    //    handle_cycle1_stopstart_heater(heater_run);
    //  } else {
    //    starting_sample_prep_run = true;
    //    heater_run = true;
    //    motorReachedSpeed = false;
    //    samplePrepResetHeaterPIDs();
    //    // Send starting heater to sensors task
    //    handle_cycle1_stopstart_heater(heater_run);
    //  }
    //} else if (heaterRxMessage.cycleSelect == CYCLE_ONE_B) {
    //  heater_cycle1b_running = heaterRxMessage.cycleEnabled;
    //  rampToTemp = false;
    //  if (!heater_cycle1b_running) { //AMP2 will be used in sample prep for heating
    //    heater_pid_1.out = 0;
    //    temperature_pwm_data_t pwmData = {
    //        .heat_zone_0_pwm = 0,
    //        .heat_zone_1_pwm = heater_pid_1.out,
    //        .sample_prep_heater_pwm = heater_pid_1.out,
    //        .heat_zone_2_pwm = 0,
    //        .heat_zone_3_pwm = 0};
    //    updateDutyCycles(pwmData);
      
    //    if (!config.run_motor_2 && !use_default_configuration_parameters) {
    //      nrf_gpio_pin_clear(MOTOR_PWR_EN);
    //    }
    //    else if (!DEFAULT_RUN_MOTOR_2 && use_default_configuration_parameters) {
    //      nrf_gpio_pin_clear(MOTOR_PWR_EN);
    //    }
    //    vTaskDelay(pdMS_TO_TICKS(200));
    //    updateDutyCycles(pwmData);
    //    // Send stop heater to sensors task
    //    heater_run = false;
    //    starting_sample_prep_run = false;
    //    motorReachedSpeed = false; // clear motor speed reached
    //    handle_cycle1b_stopstart_heater(heater_run);
    //  } else {
    //    starting_sample_prep_run = true;
    //    heater_run = true;
    //    motorReachedSpeed = false;
    //    samplePrepResetHeaterPIDs();
    //    // Send starting heater to sensors task
    //    handle_cycle1b_stopstart_heater(heater_run);
    //  }
    //} else if (heaterRxMessage.cycleSelect == CYCLE_TWO) {
    //  heater_cycle2_running = heaterRxMessage.cycleEnabled;
    //  rampToTemp = config.ramp_to_temp_before_start_cycle_2;
    //  if (!heater_cycle2_running) {
    //    heater_pid_2.out = 0;
    //    temperature_pwm_data_t pwmData = {
    //        .heat_zone_0_pwm = 0,
    //        .heat_zone_1_pwm = heater_pid_2.out,
    //        .sample_prep_heater_pwm = heater_pid_2.out,
    //        .heat_zone_2_pwm = motor_pid_2.out,
    //        .heat_zone_3_pwm = 0};
    //    updateDutyCycles(pwmData);

    //    // Send stop heater to sensors task
    //    heater_run = false;
    //    handle_cycle2_stopstart_heater(heater_run);
    //  } else {
    //    heater_run = true;
    //    motorReachedSpeed = false;
    //    samplePrepResetHeaterPIDs();
    //    handle_cycle2_stopstart_heater(heater_run);
    //  }
    //}
  }  else if (heaterRxMessage.type == HEATER_MSG_MOTOR_STATE){
      fsm_cycle_last = fsm_cycle_current;
      fsm_cycle_current = CYCLE_NONE;

      nrf_gpio_pin_clear(HEATER_PWR_EN);
      send_debug_log_message("nrf_gpio_pin_clear(MOTOR_PWR_EN)");

      temperature_pwm_data_t pwmData = {
          .heat_zone_0_pwm = 0,
          .heat_zone_1_pwm = 0,
          .sample_prep_heater_pwm = 0,
          .heat_zone_2_pwm = 0,
          .heat_zone_3_pwm = 0};
      updateDutyCycles(pwmData);
      //motor_cycle2_running = false;
      send_debug_log_message("heaterRxMessage.type == HEATER_MSG_MOTOR_STATE   should have set motor to off");

      vTaskDelay(pdMS_TO_TICKS(200));
      nrf_gpio_pin_clear(MOTOR_PWR_EN);
      send_debug_log_message("nrf_gpio_pin_clear(MOTOR_PWR_EN)");

      handle_cycle_stopstart_heater(false);


      motorReachedSpeed = false;

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