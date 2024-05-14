#include "heater.h"
#include "timers.h"

xQueueHandle heater_zoneRunQueue;
xQueueHandle heater_temperatureDataQueue;
xQueueHandle heater_usbWaitQueue;

bool amplification_zone_running = false;
bool valve_zone_running = false;

zone_run_req_t zone_req;
temperature_data_t temperature_data;

pid_controller_t valve_pid;
pid_controller_t amp0_pid;
pid_controller_t amp1_pid;
pid_controller_t amp2_pid;

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
    pid_controller_init(&valve_pid, AMP0_SETPOINT, V_KP, V_KI, V_KD);  //TODO: Change back to VALVE SETPOINT
    pid_controller_init(&amp0_pid, AMP0_SETPOINT, A0_KP, A0_KI, A0_KD);
    pid_controller_init(&amp1_pid, AMP1_SETPOINT, A1_KP, A1_KI, A1_KD);
    pid_controller_init(&amp2_pid, AMP1_SETPOINT, A2_KP, A2_KI, A2_KD);
  } else {
    pid_controller_init(&valve_pid, config.amplification_setpoint, config.valve_kp, config.valve_ki, config.valve_kd);   //TODO: Change back to VALVE SETPOINT
    pid_controller_init(&amp0_pid, config.amplification_setpoint, config.amp0_kp, config.amp0_ki, config.amp0_kd);
    pid_controller_init(&amp1_pid, config.amplification_setpoint, config.amp1_kp, config.amp1_ki, config.amp1_kd);
    pid_controller_init(&amp2_pid, config.amplification_setpoint, config.amp2_kp, config.amp2_ki, config.amp2_kd);
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
          amp0_pid.out = 1;
          amp1_pid.out = 1;
          amp2_pid.out = 1;
          update_amp0_duty(amp0_pid.out);
          update_amp1_duty(amp1_pid.out);
          update_amp2_duty(amp2_pid.out);
          valve_pid.out = 1;
          update_valve_duty(valve_pid.out);
        }
      }
      else if (zone_req.zone = VALVE) {
        valve_zone_running = zone_req.on;
        if (!valve_zone_running) {
          valve_pid.out = 1;
          update_valve_duty(valve_pid.out);
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
    }
    if (valve_zone_running) {
      // Update Valve PID loop with new temperatures
      pid_controller_compute(&valve_pid, temperature_data.valve_zone_temp);
      // Update Valve PWM with PID output
      update_valve_duty(valve_pid.out);
    }

#if VERBOSE_PID 
    if (amplification_zone_running) {
      printf("Amp0: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp0_zone_temp, amp0_pid.out);
      printf("Amp1: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp1_zone_temp, amp1_pid.out);
      printf("Amp2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp2_zone_temp, amp2_pid.out);
      printf("Valv: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.valve_zone_temp, valve_pid.out);
    }
    if (valve_zone_running) {
      printf("Valv: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.valve_zone_temp, valve_pid.out);
    }
#endif

  }
}