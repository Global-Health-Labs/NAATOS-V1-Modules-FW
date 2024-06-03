#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_gpiote.h"
#include "timers.h"

#include "motor.h"

//Use one hardware timer (which is set up as a counter)
static const nrf_drv_timer_t m_counter1 = NRF_DRV_TIMER_INSTANCE(1);

static nrf_ppi_channel_t ppi_channel_1;

/* Empty event handler. Not used since tasks/events are handled in hardware (PPI), but needs to be defined for semantical reasons. */
static void empty_gpiote_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    //uint32_t count = nrf_drv_timer_capture(&m_counter1, NRF_TIMER_CC_CHANNEL0);
    //printf("Value on MOTOR_INPUT_PIN changed. Counter value = %d\r\n", count);
}

/* Empty event handler. Not used since tasks/events are handled in hardware (PPI), but needs to be defined for semantical reasons. */
static void empty_timer_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    //printf("Timer event triggered\r\n");

}

/** @brief Function GPIOTE initialization
 *  @details GPIOTE event triggered by pulses from motor speed sensor output
 */
static void gpiote_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.pull = NRF_GPIO_PIN_NOPULL;

    err_code = nrf_drv_gpiote_in_init(MOTOR_INPUT_PIN, &in_config, empty_gpiote_event_handler);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(MOTOR_INPUT_PIN, true); 
}


/** @brief Function for Counter 1 initialization.
 *  @details Counter 1 will be incremented by toggling state on GPIO pin via PPI.
 */
static void counter1_init(void)
{
    ret_code_t err_code;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32; //Do I need to specify a width? Example did
    timer_cfg.mode = NRF_TIMER_MODE_COUNTER;
    err_code = nrf_drv_timer_init(&m_counter1, &timer_cfg, empty_timer_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_enable(&m_counter1);
    nrf_drv_timer_clear(&m_counter1);
}

/** @brief Function for initializing the PPI peripheral.
*/
static void ppi_init(void)
{
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

/** @brief Initializes motor speed reading
 */
nrf_drv_timer_t* motor_tach_init(void)
{
    counter1_init();
    gpiote_init();
    ppi_init();
    
    return &m_counter1;
}

#if 1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//TODO: Unsure which of these you need
xQueueHandle motor_RunQueue;
xQueueHandle motor_speedDataQueue;
xQueueHandle motor_usbWaitQueue;
xQueueHandle motor_pwmReqQueue;
xQueueHandle motor_sensorConfQueue;

//TODO: Some of these need to be renamed
bool motor_running = false;
bool starting_motor_run = true;
bool h_pwm_req = false;
bool greater_than_max = false;
bool heater_run = false;
int wdtTimeout = 0;

motor_run_req_t motor_req;
double motor_speed_data;

//TODO: Unused for now
motor_pwm = 0;

pid_controller_t motor_pid;


void handle_stopstart_motor(bool motor_running) {
  BaseType_t xReturned;

  // Handle case where amplification zone is on already, dont want to send stop
  if (amplification_zone_running && !motor_running)
    return;

  // Send the motor status
  xReturned = xQueueSend(sensor_heaterStateQueue, &motor_running, 0);
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

//TODO: Unsure of what this is used for
void handle_usb_sus_req(void) {
  BaseType_t xReturned;
  usb_suspend_req_t sus_req;
  usb_suspend_acpt_t sus_acpt = {
    .task = MOTOR,
    .suspended = true
  };
  usb_suspend_over_t sus_over = {
    .task = MOTOR,
    .over = true
  };

  // Check to see if we need to suspend for USB to be enabled
  if (uxQueueMessagesWaiting(motor_usbWaitQueue) > 0) {
    xReturned = xQueueReceive(motor_usbWaitQueue, &sus_req, 0) ;
    if (xReturned != pdPASS) {
      printf("MOTOR: Unable to receive usb suspend request from motor_usbWaitQueue\n");
    }
    // Send Suspend Accepted
    xReturned = xQueueSend(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0); 
    if (xReturned != pdPASS) {
      printf("MOTOR: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue\n");
    }
    printf("MOTOR: Suspending for 15 seconds.\n");
    // Delay Task for 15 Seconds
    vTaskDelay(pdMS_TO_TICKS(USB_SUSPEND_TASKS_TIME));
    // Send Suspend Over
    xReturned = xQueueSend(usb_usbWaitOverQueue, &sus_over, 0); 
    if (xReturned != pdPASS) {
      printf("MOTOR: Unable to send usb suspend over to usb_usbWaitOverQueue\n");
    }
  }
}

void motor_reset_all_pids(void){
 // Create PID Controllers 
  if (use_default_configuration_parameters) {
    pid_controller_init(&motor_pid, VALVE_SETPOINT, V_KP, V_KI, V_KD);  //TODO: Update with motor parameters
  } else {
    pid_controller_init(&motor_pid, config.valve_setpoint, config.valve_kp, config.valve_ki, config.valve_kd);   //TODO: Update with motor parameters
  }

  //TODO: Determine if you need a second PID, update the below accordingly
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

  motor_reset_all_pids();

  for (;;) {
    // Check for run motor message
    if (uxQueueMessagesWaiting(motor_RunQueue) == 0) {
      // Check to see if there is a usb suspend request
      handle_usb_sus_req();
      // Task Delay
      vTaskDelay(100);
      // Continue
      if (!motor_running)
        continue; // Go back to top of loop if motor not running
    }
    // Take message from queue
    else {
      xReturned = xQueueReceive(motor_RunQueue, &motor_req, 0);
      if (xReturned != pdPASS) {
        printf("MOTOR_TASK: unable to receive motor request from motor_RunQueue\n");
      }
      // Set speed
      motor_runing = motor_req.on;
      if (!amplification_zone_running)  {
	motor_pid.out = 0;
	update_motor_duty(motor_pid.out);
	
        //TODO: Need to figure out what to do here once you have an answer to question about why so many PIDs are used above (from heater.c)
	// Reinitalize PID Values 
	pid_controller_init(&amp0_pid, config.amp0_setpoint, config.amp0_kp, config.amp0_ki, config.amp0_kd);
	pid_controller_init(&amp1_pid, config.amp1_setpoint, config.amp1_kp, config.amp1_ki, config.amp1_kd);
	pid_controller_init(&amp2_pid, config.amp2_setpoint, config.amp2_kp, config.amp2_ki, config.amp2_kd);
	
        //TODO: Why does heater code stop the heater after it just received a request to start it?
	// Send stop heater to sensors task
	motor_run = false;
	starting_motor_run = false;
	handle_stopstart_motor(motor_run);
    }

    if (!motor_running)
        continue; // Go back to top of loop if no zones running
    

    if(wdtTimeout++ > (1/config.sample_rate)){ // send out once a second
      wdtTimeout = 0;
      sendWdtMotorValid(); // update watchdog
    }

    // Receive motor speed data (blocking till data comes in)
    xReturned = xQueueReceive(motor_speedDataQueue, motor_speed_data, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("MOTOR_TASK: unable to receive temperature data from motor_speedDataQueue\n");
    }
    
    // Ensure temperatures are below the minimum run zone temperature
    if (config.min_run_zone_temp_en) {
      if (starting_motor_run && 
         (temperature_data.valve_zone_temp > config.min_run_zone_temp ||  // TODO: Implement defaults
          temperature_data.amp0_zone_temp > config.min_run_zone_temp ||
          temperature_data.amp1_zone_temp > config.min_run_zone_temp || 
          temperature_data.amp2_zone_temp > config.min_run_zone_temp)) 
      {
        starting_motor_run = false;
        // Send cannot start
        xReturned = xQueueSend(main_runRespQueue, &starting_motor_run, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send cannot start run response.\n");
        }
        continue;
      }
      else if (starting_motor_run && 
          (temperature_data.valve_zone_temp <= config.min_run_zone_temp &&  // TODO: Implement defaults
           temperature_data.amp0_zone_temp <= config.min_run_zone_temp &&
           temperature_data.amp1_zone_temp <= config.min_run_zone_temp && 
           temperature_data.amp2_zone_temp <= config.min_run_zone_temp)) 
      {
        // Send can start
        xReturned = xQueueSend(main_runRespQueue, &starting_motor_run, 0);
        if (xReturned != pdPASS) {
          printf("HEATER_TASK: Unable to send cannot start run response.\n");
        }
        starting_motor_run = false;
      }
    }
    else if (!config.min_run_zone_temp_en && starting_motor_run) {
      // Send can start
      xReturned = xQueueSend(main_runRespQueue, &starting_motor_run, 0);
      if (xReturned != pdPASS) {
        printf("HEATER_TASK: Unable to send cannot start run response.\n");
      }
      starting_motor_run = false;
    }
    
    // Update PID and PWM
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

    //TODO: Update the below if you want...
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
#endif