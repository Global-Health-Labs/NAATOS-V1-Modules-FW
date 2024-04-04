#include "heater.h"
#include "timers.h"

xQueueHandle heater_zoneRunQueue;
xQueueHandle heater_temperatureDataQueue;

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

  // Create PID Controllers
  pid_controller_init(&amp0_pid, AMP0_SETPOINT, A0_KP, A0_KI, A0_KD);

  // TODO: Get configuration parameters (temperature setpoints)
  
  for (;;) {
    // Check for run heater zone message
    if (uxQueueMessagesWaiting(heater_zoneRunQueue) == 0) {
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
        // TODO: Begin PID loop with temperature setpoint
      }
      else if (zone_req.zone = VALVE) {
        valve_zone_running = zone_req.on;
      }
    }
    
    // Receive temperature data (blocking till data comes in)
    xReturned = xQueueReceive(heater_temperatureDataQueue, &temperature_data, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("HEATER_TASK: unable to receive temperature data from heater_temperatureDataQueue\n");
    }
    
    // Update PID and PWM
    if (amplification_zone_running) {
      printf("Recvd Amp0 Temp: %0.3f\n", temperature_data.amplification_zone_temp);
      // Update PID loop with new temperatures
      pid_controller_compute(&amp0_pid, temperature_data.amplification_zone_temp);
      printf("Amp0 Duty: %0.2f\n", amp0_pid.out);
      // Update PWM with PID output
      update_amp0_duty(amp0_pid.out);
    }
    if (valve_zone_running) {
      // TODO: Update PID loop with new temperatures
      // TODO: Update PWM with PID output
    }

  }
}