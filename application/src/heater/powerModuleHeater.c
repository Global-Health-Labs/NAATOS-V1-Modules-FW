#include "powerModuleHeater.h"
#include "timers.h"

pid_controller_t valve_pid;
pid_controller_t amp0_pid;
pid_controller_t amp1_pid;
pid_controller_t amp2_pid;

pid_controller_t valve_pid_2;
pid_controller_t amp0_pid_2;
pid_controller_t amp1_pid_2;
pid_controller_t amp2_pid_2;

temperature_pwm_data_t pm_h_pwm_data = {
    .valve_zone_pwm = 0,
    .amp0_zone_pwm = 0,
    .amp1_zone_pwm = 0,
    .amp2_zone_pwm = 0};

temperature_pwm_data_t outputPwmData = {
    .valve_zone_pwm = 0,
    .amp0_zone_pwm = 0,
    .amp1_zone_pwm = 0,
    .amp2_zone_pwm = 0};


void powerModuleHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
#ifndef SAMPLE_PREP_BOARD
  BaseType_t xReturned;
  if (wdtTimeout++ > (1 / config.sample_rate)) { // send out once a second
    wdtTimeout = 0;
    sendWdtHeaterValid(); // update watchdog
  }

  // Ensure temperatures are below the minimum run zone temperature
  if (config.min_run_zone_temp_en) {
    if (starting_run &&
        (temperature_data.valve_zone_temp > config.min_run_zone_temp || // TODO: Implement defaults
            temperature_data.amp0_zone_temp > config.min_run_zone_temp ||
            temperature_data.amp1_zone_temp > config.min_run_zone_temp ||
            temperature_data.amp2_zone_temp > config.min_run_zone_temp)) {
      starting_run = false;
      // Send cannot start
      xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: Unable to send cannot start run response.\n");
      }
      return;
    } else if (starting_run &&
               (temperature_data.valve_zone_temp <= config.min_run_zone_temp && // TODO: Implement defaults
                   temperature_data.amp0_zone_temp <= config.min_run_zone_temp &&
                   temperature_data.amp1_zone_temp <= config.min_run_zone_temp &&
                   temperature_data.amp2_zone_temp <= config.min_run_zone_temp)) {
      // Send can start
      xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: Unable to send cannot start run response.\n");
      }
      starting_run = false;
    }
  } else if (!config.min_run_zone_temp_en && starting_run) {
    // Send can start
    xReturned = xQueueSend(main_runRespQueue, &starting_run, 0);
    if (xReturned != pdPASS) {
      printf("HEATER_TASK: Unable to send cannot start run response.\n");
    }
    starting_run = false;
  }

  // Update PID and PWM
  if (amplification_zone_running) {
    // Update Amplification 0 PID loop with new temperatures
    pid_controller_compute(&amp0_pid, temperature_data.amp0_zone_temp);
    // Update Amplification 0 PWM with PID output
    pid_controller_compute(&amp1_pid, temperature_data.amp1_zone_temp);
    // Update Amplification 2 PID loop with new temperatures
    pid_controller_compute(&amp2_pid, temperature_data.amp2_zone_temp);
    // Update Valve PID loop with new temperatures
    pid_controller_compute(&valve_pid, temperature_data.valve_zone_temp);

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = valve_pid.out,
        .amp0_zone_pwm = amp0_pid.out,
        .amp1_zone_pwm = amp1_pid.out,
        .amp2_zone_pwm = amp2_pid.out};

    updateDutyCycles(pwmData);

    h_pwm_data.valve_zone_pwm = valve_pid.out;
    // Set the PWMs for the logger
    h_pwm_data.amp0_zone_pwm = amp0_pid.out;
    h_pwm_data.amp1_zone_pwm = amp1_pid.out;
    h_pwm_data.amp2_zone_pwm = amp2_pid.out;
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if ((config.amp0_max_temp < temperature_data.amp0_zone_temp) || temperature_data.amp0_zone_temp < 0 || temperature_data.amp0_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.amp1_max_temp < temperature_data.amp1_zone_temp || temperature_data.amp1_zone_temp < 0 || temperature_data.amp1_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.amp2_max_temp < temperature_data.amp2_zone_temp || temperature_data.amp2_zone_temp < 0 || temperature_data.amp2_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.valve_zone_temp || temperature_data.valve_zone_temp < 0 || temperature_data.valve_zone_temp > 110) {
      greater_than_max = true;
    }
  }
  if (valve_zone_running) {
    pid_controller_compute(&amp0_pid_2, temperature_data.amp0_zone_temp);
    pid_controller_compute(&amp1_pid_2, temperature_data.amp1_zone_temp);
    pid_controller_compute(&amp2_pid_2, temperature_data.amp2_zone_temp);
    pid_controller_compute(&valve_pid_2, temperature_data.valve_zone_temp);

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = valve_pid_2.out,
        .amp0_zone_pwm = amp0_pid_2.out,
        .amp1_zone_pwm = amp1_pid_2.out,
        .amp2_zone_pwm = amp2_pid_2.out};

    updateDutyCycles(pwmData);

    // Set the PWMs for the logger
    h_pwm_data.valve_zone_pwm = valve_pid_2.out;
    // Ensure that the temperatures are not greater than the max temperatures allowed
    if ((config.amp0_max_temp < temperature_data.amp0_zone_temp) || temperature_data.amp0_zone_temp < 0 || temperature_data.amp0_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.amp1_max_temp < temperature_data.amp1_zone_temp || temperature_data.amp1_zone_temp < 0 || temperature_data.amp1_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.amp2_max_temp < temperature_data.amp2_zone_temp || temperature_data.amp2_zone_temp < 0 || temperature_data.amp2_zone_temp > 110) {
      greater_than_max = true;
    }
    if (config.valve_max_temp < temperature_data.valve_zone_temp || temperature_data.valve_zone_temp < 0 || temperature_data.valve_zone_temp > 110) {
      greater_than_max = true;
    }
  }

  // Handle being greater than the maximum temperature
  if (greater_than_max) {
    // Send alert message to main task
    xReturned = xQueueSend(main_runErrorQueue, &greater_than_max, 0);
    if (xReturned != pdPASS) {
      printf("HEATER_TASK: Unable to send run error for greater than max temp to main_runErrorQueue.\n");
    }
    greater_than_max = false;
  }

#if VERBOSE_PID
  if (amplification_zone_running) {
    printf("Amp0: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp0_zone_temp, amp0_pid.out);
    printf("Amp1: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp1_zone_temp, amp1_pid.out);
    printf("Amp2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp2_zone_temp, amp2_pid.out);
    printf("Valve: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.valve_zone_temp, valve_pid.out);
  }
  if (valve_zone_running) {
    printf("Amp0_2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp0_zone_temp, amp0_pid_2.out);
    printf("Amp1_2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp1_zone_temp, amp1_pid_2.out);
    printf("Amp2_2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.amp2_zone_temp, amp2_pid_2.out);
    printf("Valve_2: Temp: %0.2f\tDuty: %0.2f\n", temperature_data.valve_zone_temp, valve_pid_2.out);
  }
#endif
#endif
}

void powerModuleHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {


}

void powerModuleResetHeaterPIDs(void) {


}

temperature_pwm_data_t getPowerModulePwmData(void) {


}

bool getPowerModuleOverTempStatus(void) {


}

bool getPowerModuleHeaterRunningStatus(void) {


}
