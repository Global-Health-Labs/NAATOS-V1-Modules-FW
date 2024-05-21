#include "motor.h"
#include "timers.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"

#define USE_MOTOR //TODO: Where should this live?



//xQueueHandle heater_zoneRunQueue;
//xQueueHandle heater_temperatureDataQueue;

////bool motor_running = false;
//bool motor_running = true;

////zone_run_req_t zone_req;
////temperature_data_t temperature_data;

//pid_controller_t motor_pid;




//const nrf_drv_timer_t TIMER_MOTOR = NRF_DRV_TIMER_INSTANCE(0);
//static nrf_ppi_channel_t m_ppi_channel1;














//// Initialize the timer driver
//static void timer_init(void)
//{
//    // Configure Timer 1
//    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
//    nrf_drv_timer_init(&TIMER_INSTANCE, &timer_cfg, NULL);
//}

//// Function to start timer
//static void timer_start(void)
//{
//    nrf_drv_timer_enable(&TIMER_INSTANCE);
//    nrf_drv_timer_clear(&TIMER_INSTANCE);
//}

//// Function to stop timer and get elapsed time
//static uint32_t timer_stop(void)
//{
//    nrf_drv_timer_disable(&TIMER_INSTANCE);
    
//    //Elapses time in microseconds
//    return (nrf_drv_timer_capture(&TIMER_INSTANCE, NRF_TIMER_CC_CHANNEL0) / (SystemCoreClock / 1000000);)
  
//}


//void motor_init(input_pin){
////Configure a pin to be an input, read the motor speed output on this pin
////Set up a timer or something so that you can determine the time between pulses
    
//    //Initializes pin as input
//    nrf_gpio_cfg_input(input_pin, NRF_GPIO_PIN_NOPULL);



//}
















//void motor_task(void * pvParameters) {
//  BaseType_t xReturned;

//  // Create PID Controllers 
//  if (use_default_configuration_parameters) {
//    pid_controller_init(&motor_pid, MOTOR_SETPOINT, MOTOR_P, MOTOR_I, MOTOR_D);
//  } else {
//      pid_controller_init(&valve_pid, config.valve_setpoint, config.valve_kp, config.valve_ki, config.valve_kd);  
//  }

//  for (;;) {
//    // Check for run motor message
//    if (uxQueueMessagesWaiting(heater_zoneRunQueue) == 0) { //TODO: Replace the heat_zoneRunQueue reference
//      vTaskDelay(100);
//      if (true)
//        continue; // Go back to top of loop if motor is not running
//    }
//    // Take message from queue
//    else {
//      xReturned = xQueueReceive(heater_zoneRunQueue, &zone_req, 0); //TODO: Replace reference to heater_zoneRunQueue and zone_req
//      if (xReturned != pdPASS) {
//        printf("HEATER_TASK: unable to receive zone run request from heater_zoneRunQueue\n");//TODO: Replace this error message
//      }
//      //TODO: Most of the code below needs to be replaced
//      // Set zones enabled
//      if (zone_req.zone == AMPLIFICATION) {
//        amplification_zone_running = zone_req.on;
//        if (!amplification_zone_running)  {
//          amp0_pid.out = 1;
//          update_amp0_duty(amp0_pid.out);
//          update_amp1_duty(amp1_pid.out);
//          update_amp2_duty(amp2_pid.out);
//        }
//      }
//      else if (zone_req.zone = VALVE) {
//        valve_zone_running = zone_req.on;
//        if (!valve_zone_running) {
//          valve_pid.out = 1;
//          update_valve_duty(valve_pid.out);
//        }
//      }
//    }

//    if (!amplification_zone_running & !valve_zone_running)
//        continue; // Go back to top of loop if no zones running
    
//    //TODO: Here is where you should read the speed data
//    // Receive temperature data (blocking till data comes in)
//    xReturned = xQueueReceive(heater_temperatureDataQueue, &temperature_data, portMAX_DELAY);
//    if (xReturned != pdPASS) {
//      printf("HEATER_TASK: unable to receive temperature data from heater_temperatureDataQueue\n");
//    }
    
//    // Update PID and PWM
//    if (motor_running) {
//      //TODO: Update parameters in below function calls
//      // Update Amplification 0 PID loop with new temperatures
//      pid_controller_compute(&amp0_pid, temperature_data.amp0_zone_temp);
//      // Update Amplification 0 PWM with PID output
//      update_amp0_duty(amp0_pid.out);
//  }
//}