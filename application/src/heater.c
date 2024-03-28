#include "heater.h"
#include "timers.h"

xQueueHandle heater_zoneRunQueue;
xQueueHandle heater_temperatureDataQueue;

bool amplification_zone_running = false;
bool valve_zone_running = false;

zone_run_req_t zone_req;
temperature_data_t temperature_data;

void heater_task(void * pvParameters) {
  BaseType_t xReturned;
  
  // TODO: REMOVE
  update_valve_duty(90);
  update_amp0_duty(20);
  update_amp1_duty(70);
  update_amp2_duty(50);

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
      // TODO: Update PID loop with new temperatures
      // TODO: Update PWM with PID output
    }
    if (valve_zone_running) {
      // TODO: Update PID loop with new temperatures
      // TODO: Update PWM with PID output
    }

  }
}