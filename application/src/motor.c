#include "FreeRTOS.h"

#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_gpiote.h"
#include "timers.h"

#include "pid.h"
#include "motor.h"
#include "naatos_config.h"

//Hardware timer (set up as a counter)
static const nrf_drv_timer_t m_counter1 = NRF_DRV_TIMER_INSTANCE(1);

//PPI Channel to link GPIOTE event (pulses on motor speed input pin) and counter
static nrf_ppi_channel_t ppi_channel_1;

//Empty event handler. Not used since tasks/events are handled in hardware (PPI), but needs to be defined for semantical reasons
static void empty_gpiote_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    //uint32_t count = nrf_drv_timer_capture(&m_counter1, NRF_TIMER_CC_CHANNEL0);
    //printf("Value on MOTOR_INPUT_PIN changed. Counter value = %d\r\n", count);
}

//Empty event handler. Not used since tasks/events are handled in hardware (PPI), but needs to be defined for semantical reasons
static void empty_timer_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    //printf("Timer event triggered\r\n");

}

//GPIOTE initialization. Event triggered by pulses from motor speed sensor output
static void gpiote_init(void){
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.pull = NRF_GPIO_PIN_NOPULL;

    err_code = nrf_drv_gpiote_in_init(MOTOR_INPUT_PIN, &in_config, empty_gpiote_event_handler);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(MOTOR_INPUT_PIN, true); 
}


//Function for Counter 1 initialization.
//Counter 1 will be incremented by toggling state on GPIO pin via PPI.
static void counter1_init(void){
    ret_code_t err_code;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32; //Do I need to specify a width? Example did
    timer_cfg.mode = NRF_TIMER_MODE_COUNTER;
    err_code = nrf_drv_timer_init(&m_counter1, &timer_cfg, empty_timer_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_enable(&m_counter1);
    nrf_drv_timer_clear(&m_counter1);
}

//Function for initializing the PPI peripheral.
static void ppi_init(void){
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel_1);
    APP_ERROR_CHECK(err_code);

    uint32_t gpiote_evt_addr_1 = nrf_drv_gpiote_in_event_addr_get(MOTOR_INPUT_PIN);
    uint32_t timer_count_task_addr = nrf_drv_timer_task_address_get(&m_counter1, NRF_TIMER_TASK_COUNT);

    err_code = nrf_drv_ppi_channel_assign(ppi_channel_1, gpiote_evt_addr_1, timer_count_task_addr); // Trigger timer count task when GPIOTE pin detects edge
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_ppi_channel_enable(ppi_channel_1);
    APP_ERROR_CHECK(err_code);

}

//Initializes motor speed reading
nrf_drv_timer_t* motor_tach_init(void){
    counter1_init();
    gpiote_init();
    ppi_init();
    
    return &m_counter1;
}

#if 0
//Motor task under development here
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//TODO: Unsure which of these are needed
xQueueHandle motor_RunQueue;
xQueueHandle motor_speedDataQueue;
xQueueHandle motor_usbWaitQueue;
xQueueHandle motor_pwmReqQueue;
xQueueHandle motor_sensorConfQueue;

//TODO: Need to do a once-over on these
bool motor_running = false;
bool starting_motor_run = true;
bool motor_run = false;
int wdtTimeout = 0;

motor_run_req_t motor_req;
double motor_speed_data;

pid_controller_t motor_pid;

//TODO: Needs work
void handle_stopstart_motor(bool motor_running) {
  BaseType_t xReturned;

  // Handle case where motor is on already, dont want to send stop
  if (!motor_running)
    return;

  // Send the motor status
  xReturned = xQueueSend(sensor_heaterStateQueue, &motor_running, 0); //TODO: This line
  if (xReturned != pdPASS) {
    printf("MOTOR_TASK: Unable to send motor state to sensor_motorStateQueue.\n");
  }
  // Get the response that the sensors task has been updated
  xReturned = xQueueReceive(motor_sensorConfQueue, &motor_running, portMAX_DELAY);
  if (xReturned != pdPASS) {
    printf("MOTOR_TASK: Unable to receive sensor confirmation from motor_sensorConfQueue.\n");
  }
  // Send the confirmation to the main state
  xReturned = xQueueSend(main_runConfRespQueue, &motor_running, 0);
  if (xReturned != pdPASS) {
     printf("MOTOR_TASK: Unable to send run response to main_runConfRespQueue.\n");
  }
}

//TODO: Unsure of what this is used for but the heater task had it
//void handle_usb_sus_req(void) {
//  BaseType_t xReturned;
//  usb_suspend_req_t sus_req;
//  usb_suspend_acpt_t sus_acpt = {
//    .task = MOTOR,
//    .suspended = true
//  };
//  usb_suspend_over_t sus_over = {
//    .task = MOTOR,
//    .over = true
//  };

//  // Check to see if we need to suspend for USB to be enabled
//  if (uxQueueMessagesWaiting(motor_usbWaitQueue) > 0) {
//    xReturned = xQueueReceive(motor_usbWaitQueue, &sus_req, 0) ;
//    if (xReturned != pdPASS) {
//      printf("MOTOR: Unable to receive USB suspend request from motor_usbWaitQueue\n");
//    }
//    // Send Suspend Accepted
//    xReturned = xQueueSend(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0); 
//    if (xReturned != pdPASS) {
//      printf("MOTOR: Unable to send USB suspend accept from usb_recvUsbWaitAcceptQueue\n");
//    }
//    printf("MOTOR: Suspending for 15 seconds.\n");
//    // Delay Task for 15 Seconds
//    vTaskDelay(pdMS_TO_TICKS(USB_SUSPEND_TASKS_TIME));
//    // Send Suspend Over
//    xReturned = xQueueSend(usb_usbWaitOverQueue, &sus_over, 0); 
//    if (xReturned != pdPASS) {
//      printf("MOTOR: Unable to send USB suspend over to usb_usbWaitOverQueue\n");
//    }
//  }
//}

void motor_reset_pid(void){
  // Create PID Controller
  pid_controller_init(&motor_pid, MOTOR_SETPOINT, M_KP, M_KI, M_KD);
}

void sendWdtMotorValid() {
  BaseType_t xReturned;
  watchdog_time_update_t wdtUpdate = {};
  wdtUpdate.taskName = MOTOR;
  wdtUpdate.valid = true;

    xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
    if (xReturned != pdPASS) {
      printf("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue in motor task \n");
    }
}

void motor_task(void * pvParameters) {
  BaseType_t xReturned;

  motor_reset_pid();

  for (;;) {
    // Check for run motor message
    if (uxQueueMessagesWaiting(motor_RunQueue) == 0) {
      // Check to see if there is a usb suspend request
      //handle_usb_sus_req();
      // Task Delay
      vTaskDelay(100);
      // Continue
      if (!motor_running)
        continue; // Go back to top of loop if motor not running, no messages to start it
    }
    // Take message from queue
    else {
      xReturned = xQueueReceive(motor_RunQueue, &motor_req, 0);
      if (xReturned != pdPASS) {
        printf("MOTOR_TASK: unable to receive motor request from motor_RunQueue\n");
      }
      // Set speed
      motor_running = motor_req.on;
      //Check if motor is already running
      if (!motor_running)  {
	motor_pid.out = 0;
	update_motor_duty(motor_pid.out);

        //TODO: Reinitialize the PID values?
	
	// Send stop motor to sensors task
	motor_run = false;
	starting_motor_run = false;
	handle_stopstart_motor(motor_run);
    }

    if (!motor_running)
        continue; // Go back to top of loop if motor not running

    if(wdtTimeout++ > (1/config.sample_rate)){ // send out once a second
      wdtTimeout = 0;
      sendWdtMotorValid(); // update watchdog
    }

    // Receive motor speed data (blocking till data comes in)
    xReturned = xQueueReceive(motor_speedDataQueue, &motor_speed_data, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("MOTOR_TASK: unable to receive temperature data from motor_speedDataQueue\n");
    }
    
    // Send start
    xReturned = xQueueSend(main_runRespQueue, &starting_motor_run, 0);
    if (xReturned != pdPASS) {
      printf("MOTOR_TASK: Unable to send cannot start run response.\n");
    }
    
    starting_motor_run = false;

    }

    // Update PID and PWM
   if (motor_running) {
      pid_controller_compute(&motor_pid, motor_speed_data);
      update_motor_duty(motor_pid.out);
    }
    
    //TODO: Currently no defined min/max motor speeds, or amy handling if they are infringed upon


    //TODO: Update the below if you want...
#if VERBOSE_PID 
    //if (amplification_zone_running) {
    //  printf("Amp0: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp0_zone_temp, amp0_pid.out);
    //  printf("Amp1: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp1_zone_temp, amp1_pid.out);
    //  printf("Amp2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp2_zone_temp, amp2_pid.out);
    //  printf("Valve: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.valve_zone_temp, valve_pid.out);
    //}
    //if (valve_zone_running) {
    //  printf("Amp0_2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp0_zone_temp, amp0_pid_2.out);
    //  printf("Amp1_2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp1_zone_temp, amp1_pid_2.out);
    //  printf("Amp2_2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.amp2_zone_temp, amp2_pid_2.out);
    //  printf("Valve_2: Temp: %0.2f\tDuty: %0.2f\n",temperature_data.valve_zone_temp, valve_pid_2.out);
    //}
#endif
  }
}
#endif