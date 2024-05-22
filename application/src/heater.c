#include "heater.h"
#include "timers.h"

xQueueHandle heater_zoneRunQueue;
xQueueHandle heater_temperatureDataQueue;
xQueueHandle heater_usbWaitQueue;
xQueueHandle heater_pwmReqQueue;

bool amplification_zone_running = false;
bool valve_zone_running = false;
bool starting_run = true;
bool h_pwm_req = false;
bool greater_than_max = false;

zone_run_req_t zone_req;
temperature_data_t temperature_data;

temperature_pwm_data_t h_pwm_data = {
  .valve_zone_pwm = 0,
  .amp0_zone_pwm = 0,
  .amp1_zone_pwm = 0,
  .amp2_zone_pwm = 0
};

pid_controller_t valve_pid;
pid_controller_t amp0_pid;
pid_controller_t amp1_pid;
pid_controller_t amp2_pid;

pid_controller_t valve_pid_2;
pid_controller_t amp0_pid_2;
pid_controller_t amp1_pid_2;
pid_controller_t amp2_pid_2;

void heater_task(void * pvParameters) {
  BaseType_t xReturned;
  usb_suspend_req_t sus_req;
  usb_suspend_acpt_t sus_acpt = {
    .task = HEATER,
    .suspended = true
  };
  usb_suspend_over_t sus_over = {
    .task = HEATER,
    .over = true
  };

  // Create PID Controllers 
  if (use_default_configuration_parameters) {
    pid_controller_init(&valve_pid, VALVE_SETPOINT, V_KP, V_KI, V_KD);  
    pid_controller_init(&amp0_pid, AMP0_SETPOINT, A0_KP, A0_KI, A0_KD);
    pid_controller_init(&amp1_pid, AMP1_SETPOINT, A1_KP, A1_KI, A1_KD);
    pid_controller_init(&amp2_pid, AMP1_SETPOINT, A2_KP, A2_KI, A2_KD);
  } else {
    pid_controller_init(&valve_pid, config.valve_setpoint, config.valve_kp, config.valve_ki, config.valve_kd);   
    pid_controller_init(&amp0_pid, config.amp0_setpoint, config.amp0_kp, config.amp0_ki, config.amp0_kd);
    pid_controller_init(&amp1_pid, config.amp1_setpoint, config.amp1_kp, config.amp1_ki, config.amp1_kd);
    pid_controller_init(&amp2_pid, config.amp2_setpoint, config.amp2_kp, config.amp2_ki, config.amp2_kd);
  }

  
  // Create PID Controllers 
  if (use_default_configuration_parameters) {
    pid_controller_init(&valve_pid_2, VALVE_SETPOINT_2, V_KP_2, V_KI_2, V_KD_2);  
    pid_controller_init(&amp0_pid_2, AMP0_SETPOINT_2, A0_KP_2, A0_KI_2, A0_KD_2);
    pid_controller_init(&amp1_pid_2, AMP1_SETPOINT_2, A1_KP_2, A1_KI_2, A1_KD_2);
    pid_controller_init(&amp2_pid_2, AMP1_SETPOINT_2, A2_KP_2, A2_KI_2, A2_KD_2);
  } else {
    pid_controller_init(&valve_pid_2, config.valve_setpoint_2, config.valve_kp_2, config.valve_ki_2, config.valve_kd_2);  
    pid_controller_init(&amp0_pid_2, config.amp0_setpoint_2, config.amp0_kp_2, config.amp0_ki_2, config.amp0_kd_2);
    pid_controller_init(&amp1_pid_2, config.amp1_setpoint_2, config.amp1_kp_2, config.amp1_ki_2, config.amp1_kd_2);
    pid_controller_init(&amp2_pid_2, config.amp2_setpoint_2, config.amp2_kp_2, config.amp2_ki_2, config.amp2_kd_2);
  }

  for (;;) {
    // Check for run heater zone message
    if (uxQueueMessagesWaiting(heater_zoneRunQueue) == 0) {
      // Check to see if we need to suspend for USB to be enabled
      if (uxQueueMessagesWaiting(heater_usbWaitQueue) > 0) {
        xReturned = xQueueReceive(heater_usbWaitQueue, &sus_req, 0) ;
        if (xReturned != pdPASS) {
          printf("HEATER: Unable to receive usb suspend request from heater_usbWaitQueue\n");
        }
        // Send Suspend Accepted
        xReturned = xQueueSend(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0); 
        if (xReturned != pdPASS) {
          printf("HEATER: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue\n");
        }
        printf("HEATER: Suspending for 15 seconds.\n");
        // Delay Task for 15 Seconds
        vTaskDelay(pdMS_TO_TICKS(USB_SUSPEND_TASKS_TIME));
        // Send Suspend Over
        xReturned = xQueueSend(usb_usbWaitOverQueue, &sus_over, 0); 
        if (xReturned != pdPASS) {
          printf("HEATER: Unable to send usb suspend over to usb_usbWaitOverQueue\n");
        }
      }
      vTaskDelay(100);
      if (!amplification_zone_running & !valve_zone_running)
        continue; // Go back to top of loop if no zones running
    }
    // Take message from queue
    else {
      xReturned = xQueueReceive(heater_zoneRunQueue, &zone_req, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: unable to receive zone run request from heater_zoneRunQueue\n");
      }
      // Set zones enabled
      if (zone_req.zone == AMPLIFICATION) {
        amplification_zone_running = zone_req.on;
        if (!amplification_zone_running)  {
          amp0_pid.out = 0;
          amp1_pid.out = 0;
          amp2_pid.out = 0;
          update_amp0_duty(amp0_pid.out);
          update_amp1_duty(amp1_pid.out);
          update_amp2_duty(amp2_pid.out);

          valve_pid.out = 0;
          update_valve_duty(valve_pid.out);
          pid_controller_init(&valve_pid, config.valve_setpoint, config.valve_kp, config.valve_ki, config.valve_kd);  // TODO: Implement defaults
          // Reinitalize PID Values 
          pid_controller_init(&amp0_pid, config.amp0_setpoint, config.amp0_kp, config.amp0_ki, config.amp0_kd);
          pid_controller_init(&amp1_pid, config.amp1_setpoint, config.amp1_kp, config.amp1_ki, config.amp1_kd);
          pid_controller_init(&amp2_pid, config.amp2_setpoint, config.amp2_kp, config.amp2_ki, config.amp2_kd);
        }
        else {
          starting_run = true;
        }
      }
      else if (zone_req.zone = VALVE) {
        valve_zone_running = zone_req.on;
        if (!valve_zone_running) {
          valve_pid.out = 0;
          amp0_pid.out = 0;
          amp1_pid.out = 0;
          amp2_pid.out = 0;
          update_valve_duty(valve_pid.out);
          update_amp0_duty(amp0_pid.out);
          update_amp1_duty(amp1_pid.out);
          update_amp2_duty(amp2_pid.out);
          // Reinitalize PID Values
          pid_controller_init(&valve_pid_2, config.valve_setpoint_2, config.valve_kp_2, config.valve_ki_2, config.valve_kd_2);  
          pid_controller_init(&amp0_pid_2, config.amp0_setpoint_2, config.amp0_kp_2, config.amp0_ki_2, config.amp0_kd_2);
          pid_controller_init(&amp1_pid_2, config.amp1_setpoint_2, config.amp1_kp_2, config.amp1_ki_2, config.amp1_kd_2);
          pid_controller_init(&amp2_pid_2, config.amp2_setpoint_2, config.amp2_kp_2, config.amp2_ki_2, config.amp2_kd_2);
        }
      }
    }

    if (!amplification_zone_running & !valve_zone_running)
        continue; // Go back to top of loop if no zones running
    
    // Receive temperature data (blocking till data comes in)
    xReturned = xQueueReceive(heater_temperatureDataQueue, &temperature_data, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("HEATER_TASK: unable to receive temperature data from heater_temperatureDataQueue\n");
    }
    
    // Ensure temperatures are below the minimum run zone temperature
    if (config.min_run_zone_temp_en) {
      if (starting_run && 
         (temperature_data.valve_zone_temp > config.min_run_zone_temp ||  // TODO: Implement defaults
          temperature_data.amp0_zone_temp > config.min_run_zone_temp ||
          temperature_data.amp1_zone_temp > config.min_run_zone_temp || 
          temperature_data.amp2_zone_temp > config.min_run_zone_temp)) 
      {
        starting_run = false;
        // Send cannot start
        xReturned = xQueueSend(main_startRunRespQueue, &starting_run, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send cannot start run response.\n");
        }
        continue;
      }
      else if (starting_run && 
          (temperature_data.valve_zone_temp <= config.min_run_zone_temp &&  // TODO: Implement defaults
           temperature_data.amp0_zone_temp <= config.min_run_zone_temp &&
           temperature_data.amp1_zone_temp <= config.min_run_zone_temp && 
           temperature_data.amp2_zone_temp <= config.min_run_zone_temp)) 
      {
        // Send can start
        xReturned = xQueueSend(main_startRunRespQueue, &starting_run, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send cannot start run response.\n");
        }
        starting_run = false;
      }
    }
    else if (!config.min_run_zone_temp_en && starting_run) {
      // Send can start
      xReturned = xQueueSend(main_startRunRespQueue, &starting_run, 0);
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
      update_amp0_duty(amp0_pid.out);
      // Update Amplification 1 PID loop with new temperatures
      pid_controller_compute(&amp1_pid, temperature_data.amp1_zone_temp);
      // Update Amplification 1 PWM with PID output
      update_amp1_duty(amp1_pid.out);
      // Update Amplification 2 PID loop with new temperatures
      pid_controller_compute(&amp2_pid, temperature_data.amp2_zone_temp);
      // Update Amplification 2 PWM with PID output
      update_amp2_duty(amp2_pid.out);
      // Update Valve PID loop with new temperatures
      pid_controller_compute(&valve_pid, temperature_data.valve_zone_temp);
      // Update Valve PWM with PID output
      update_valve_duty(valve_pid.out);
      h_pwm_data.valve_zone_pwm = valve_pid.out;
      // Set the PWMs for the logger
      h_pwm_data.amp0_zone_pwm = amp0_pid.out;
      h_pwm_data.amp1_zone_pwm = amp1_pid.out;
      h_pwm_data.amp2_zone_pwm = amp2_pid.out;
      // Ensure that the temperatures are not greater than the max temperatures allowed
      if (config.amp0_max_temp < temperature_data.amp0_zone_temp) {
        greater_than_max = true;
      }
      if (config.amp1_max_temp < temperature_data.amp1_zone_temp) {
        greater_than_max = true;
      }
      if (config.amp2_max_temp < temperature_data.amp2_zone_temp) {
        greater_than_max = true;
      }
      if (config.valve_max_temp < temperature_data.valve_zone_temp) {
        greater_than_max = true;
      }
   }
   if (valve_zone_running) {
      pid_controller_compute(&amp0_pid_2, temperature_data.amp0_zone_temp);
      update_amp0_duty(amp0_pid_2.out);
      pid_controller_compute(&amp1_pid_2, temperature_data.amp1_zone_temp);
      update_amp1_duty(amp1_pid_2.out);
      pid_controller_compute(&amp2_pid_2, temperature_data.amp2_zone_temp);
      update_amp2_duty(amp2_pid_2.out);
      pid_controller_compute(&valve_pid_2, temperature_data.valve_zone_temp);
      update_valve_duty(valve_pid_2.out);
      // Set the PWMs for the logger
      h_pwm_data.valve_zone_pwm = valve_pid.out;
      // Ensure that the temperatures are not greater than the max temperatures allowed
      if (config.valve_max_temp < temperature_data.valve_zone_temp) {
        greater_than_max = true;
      }
      if (config.amp1_max_temp < temperature_data.amp1_zone_temp) {
        greater_than_max = true;
      }
      if (config.amp2_max_temp < temperature_data.amp2_zone_temp) {
        greater_than_max = true;
      }
      if (config.valve_max_temp < temperature_data.valve_zone_temp) {
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

    // Check to see if the logger needs the pwm data
    if (uxQueueMessagesWaiting(heater_pwmReqQueue) > 0) {
      // Retrieve the request
      xReturned = xQueueReceive(heater_pwmReqQueue, &h_pwm_req, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: unable to receive pwm request from heater_pwmReqQueue queue.\n");
      }
      // Send back the pwm data
      xReturned = xQueueSend(sensor_pwmRecvQueue, &h_pwm_data, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: unable to send pwm data to sensor_pwmRecvQueue queue.\n");
      }
    }

#if VERBOSE_PID 
    if (amplification_zone_running) {
      printf("Amp0: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp0_zone_temp, amp0_pid.out);
      printf("Amp1: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp1_zone_temp, amp1_pid.out);
      printf("Amp2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp2_zone_temp, amp2_pid.out);
      printf("Valve: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.valve_zone_temp, valve_pid.out);
    }
    if (valve_zone_running) {
      printf("Amp0_2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp0_zone_temp, amp0_pid_2.out);
      printf("Amp1_2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp1_zone_temp, amp1_pid_2.out);
      printf("Amp2_2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp2_zone_temp, amp2_pid_2.out);
      printf("Valve_2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.valve_zone_temp, valve_pid_2.out);
    }
#endif
  }
}