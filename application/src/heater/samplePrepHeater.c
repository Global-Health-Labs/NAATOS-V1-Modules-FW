#include "samplePrepHeater.h"
#include "../motor.h"
#include "timers.h"

void samplePrepResetHeaterPIDs(void) {
  // Create PID Controllers
  if (use_default_configuration_parameters) {
    heater1SetPoint = DEFAULT_HEATER_SETPOINT;
    pid_controller_init(&heater_pid_1, DEFAULT_HEATER_SETPOINT, H_KP, H_KI, H_KD);
  } else {
    heater1SetPoint = config.heater_setpoint_1;
    pid_controller_init(&heater_pid_1, config.heater_setpoint_1, config.heater_kp_1, config.heater_ki_1, config.heater_kd_1);
  }

  if (use_default_configuration_parameters) {
    heater2SetPoint = DEFAULT_HEATER_SETPOINT;
    pid_controller_init(&heater_pid_2, DEFAULT_HEATER_SETPOINT, H_KP, H_KI, H_KD);
  } else {
    heater2SetPoint = config.heater_setpoint_2;
    pid_controller_init(&heater_pid_2, config.heater_setpoint_2, config.heater_kp_2, config.heater_ki_2, config.heater_kd_2);
  }

  if (use_default_configuration_parameters) {   
    pid_controller_init(&motor_pid_1, MOTOR_SETPOINT_1, M_KP, M_KI, M_KD);
  } else {
    pid_controller_init(&motor_pid_1, config.motor_setpoint_1, config.motor_kp_1, config.motor_ki_1, config.motor_kd_1);
  }

  if (use_default_configuration_parameters) {   
    pid_controller_init(&motor_pid_2, MOTOR_SETPOINT_2, M_KP, M_KI, M_KD);
  } else {
    pid_controller_init(&motor_pid_2, config.motor_setpoint_2, config.motor_kp_2, config.motor_ki_2, config.motor_kd_2);
  }
}

void samplePrepHandleHeaterSensorDataRx(temperature_data_t temperature_data) {
  BaseType_t xReturned;

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
    // Update Amplification 2 PID loop with new temperatures
    if (config.run_heater_1) {
      pid_controller_compute(&heater_pid_1, temperature_data.amp2_zone_temp);
      if(rampToTemp && (temperature_data.amp2_zone_temp >= heater1SetPoint)) {
        xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send set point reached message.\n");
        }
        rampToTemp = false;
      }
    }

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = 0,
        .amp0_zone_pwm = heater_pid_1.out,
        .amp1_zone_pwm = 0,
        .amp2_zone_pwm = 0};

    // Update Amplification 2 PWM with PID output
    h_pwm_data.valve_zone_pwm = pwmData.valve_zone_pwm;
    h_pwm_data.amp0_zone_pwm = pwmData.amp0_zone_pwm;
    // Dont want Amp1, its from the motor task
    h_pwm_data.amp2_zone_pwm = pwmData.amp2_zone_pwm;

    updateDutyCycles(h_pwm_data);

    if ((config.heater_max_temp < temperature_data.amp2_zone_temp) || temperature_data.amp2_zone_temp < 0 || temperature_data.amp2_zone_temp > 120) {
      greater_than_max = true;
    }
  }
  if (valve_zone_running) { //AMP 1 will be used for motor
    // Update Amplification 2 PID loop with new temperatures
    if (config.run_heater_2) {
      pid_controller_compute(&heater_pid_2, temperature_data.amp2_zone_temp);
      if(rampToTemp && (temperature_data.amp2_zone_temp >= heater2SetPoint)) {
        xReturned = xQueueSend(main_setPointReached, &rampToTemp, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send set point reached message.\n");
        }
        rampToTemp = false;
      }
    }

    temperature_pwm_data_t pwmData = {
        .valve_zone_pwm = 0,
        .amp0_zone_pwm = heater_pid_2.out,
        .amp1_zone_pwm = 0,
        .amp2_zone_pwm = 0};

    h_pwm_data.valve_zone_pwm = pwmData.valve_zone_pwm;
    h_pwm_data.amp0_zone_pwm = pwmData.amp0_zone_pwm;
    h_pwm_data.amp2_zone_pwm = pwmData.amp2_zone_pwm;

    updateDutyCycles(h_pwm_data);

    if ((config.heater_max_temp < temperature_data.amp2_zone_temp) || temperature_data.amp2_zone_temp < 0 || temperature_data.amp2_zone_temp > 120) {
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
    int size;
    char buff[60];
    size = sprintf(buff, "Heater: Temp: %0.2f\tDuty: %0.2f\r\n", temperature_data.amp2_zone_temp, heater_pid_1.out);
    write_to_com(buff, size);
    size = sprintf(buff, "Motor: Speed: %d\tDuty: %0.2f\r\n", last_motor_speed, h_pwm_data.amp1_zone_pwm);
    write_to_com(buff, size);
  }
  if (valve_zone_running) {
    int size;
    char buff[60];
    size = sprintf(buff, "Heater: Temp: %0.2f\tDuty: %0.2f\r\n", temperature_data.amp2_zone_temp, heater_pid_2.out);
    write_to_com(buff, size);
    size = sprintf(buff, "Motor: Speed: %d\tDuty: %0.2f\r\n", last_motor_speed, h_pwm_data.amp1_zone_pwm);
    write_to_com(buff, size);
  }
#endif

}

void handleSampleMotorDataRx(int motor_speed) {
  if (amplification_zone_running) {
    temperature_pwm_data_t pwmData = {
      .valve_zone_pwm = 0,
      .amp0_zone_pwm = heater_pid_1.out,
      .amp1_zone_pwm = 0,
      .amp2_zone_pwm = 0
    };

    if (config.run_motor_1) {
      pid_controller_compute(&motor_pid_1, motor_speed);
      pwmData.amp1_zone_pwm = motor_pid_1.out;
    } 

    h_pwm_data.amp1_zone_pwm = pwmData.amp1_zone_pwm;
    last_motor_speed = motor_speed;

    updateDutyCycles(pwmData);
  }
  else if (valve_zone_running) {
    temperature_pwm_data_t pwmData = {
      .valve_zone_pwm = 0,
      .amp0_zone_pwm = heater_pid_1.out,
      .amp1_zone_pwm = 0,
      .amp2_zone_pwm = 0
    };

    if (config.run_motor_2) {
      pid_controller_compute(&motor_pid_2, motor_speed);
      pwmData.amp1_zone_pwm = motor_pid_2.out;
    } 

    h_pwm_data.amp1_zone_pwm = pwmData.amp1_zone_pwm;
    last_motor_speed = motor_speed;

    updateDutyCycles(pwmData);
  }
}

void samplePrepHandleHeaterZoneStateUpdate(HeaterRxQueueMsg_t heaterRxMessage) {
  // Set zones enabled
  if (heaterRxMessage.zoneSelect == AMPLIFICATION) {
    amplification_zone_running = heaterRxMessage.zoneEnabled;
    rampToTemp = config.ramp_to_temp_before_start_cycle_1;
    if (!amplification_zone_running) { //AMP2 will be used in sample prep fro heating
      heater_pid_1.out = 0;
      temperature_pwm_data_t pwmData = {
          .valve_zone_pwm = 0,
          .amp0_zone_pwm = heater_pid_1.out,
          .amp1_zone_pwm = 0,
          .amp2_zone_pwm = 0};
      updateDutyCycles(pwmData);

      // Send stop heater to sensors task
      heater_run = false;
      starting_run = false;
      handle_amplification_stopstart_heater(heater_run);
    } else {
      starting_run = true;
      heater_run = true;
      heater_reset_all_pids();
      // Send starting heater to sensors task
      handle_amplification_stopstart_heater(heater_run);
    }
  } else if (heaterRxMessage.zoneSelect = VALVE) {
    valve_zone_running = heaterRxMessage.zoneEnabled;
    rampToTemp = config.ramp_to_temp_before_start_cycle_2;
    if (!valve_zone_running) {
      heater_pid_2.out = 0;
      temperature_pwm_data_t pwmData = {
          .valve_zone_pwm = 0,
          .amp0_zone_pwm = heater_pid_2.out,
          .amp1_zone_pwm = 0,
          .amp2_zone_pwm = 0};
      updateDutyCycles(pwmData);

      // Send stop heater to sensors task
      heater_run = false;
      handle_valve_stopstart_heater(heater_run);
    } else {
      heater_run = true;
      heater_reset_all_pids();
      handle_valve_stopstart_heater(heater_run);
    }
  }
}