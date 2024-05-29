#include "pwm.h"
#include "timers.h"

#define USE_MOTOR   //TODO: Remove

APP_PWM_INSTANCE(PWM0, 0); // Create instance "PWM0" using TIMER0
APP_PWM_INSTANCE(PWM2, 2); // Create instance "PWM2" using TIMER2

static volatile bool pwm0_ready_flag = false;
static volatile bool pwm2_ready_flag = false;

int valve_duty = 0;
int amp0_duty = 0;
int amp1_duty = 0;
int amp2_duty = 0;
int motor_duty = 0;

static bool valve_zone_active = false;
static bool amp0_zone_active = false;
static bool amp1_zone_active = false;
static bool amp2_zone_active = false;
static bool motor_active = false;

xQueueHandle pwm_usbWaitQueue;

void pwm0_ready_callback(uint32_t pwm_id) {
  pwm0_ready_flag = true;
}

void pwm2_ready_callback(uint32_t pwm_id) {
  pwm2_ready_flag = true;
}

void init_pwms() {
  ret_code_t err;
  
  //IEO Note: Probably want to add your PWM to this thing?

  /* Create Configurations */
  /* 1 Channel PWM, 200Hz, Active High, Valve Zone Pin */
  app_pwm_config_t pwm0_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, VALVE_ZONE_PIN, AMP0_ZONE_PIN);
  pwm0_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwm0_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
  
  
  #ifdef USE_MOTOR
  //1 Channel PWM, 10kHz, Active High, Motor Control Pin */
  app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_2CH(100L, AMP1_ZONE_PIN, AMP2_ZONE_PIN);
  pwm2_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwm2_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
  #endif
  #ifndef  USE_MOTOR
  //1 Channel PWM, 200Hz, Active High, Amplification Zone Pin */
  app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, AMP1_ZONE_PIN, AMP2_ZONE_PIN);
  pwm2_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwm2_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
  #endif

  /* Initalize with configurations */
  /* Initalize PWM0 */
  err = app_pwm_init(&PWM0, &pwm0_cfg, pwm0_ready_callback);
  APP_ERROR_CHECK(err);
  /* Initalize PWM1 */
  err = app_pwm_init(&PWM2, &pwm2_cfg, pwm2_ready_callback);
  APP_ERROR_CHECK(err);
  /* Enable PWMs */
  app_pwm_enable(&PWM0);
  app_pwm_enable(&PWM2);
}

// Updates the Valve PWM Duty Cycle
void update_valve_duty(int duty) {
  valve_duty = duty;
  if (valve_duty > 0) {
    valve_zone_active = true; 
  }
}

// Updates the Amplification 0 PWM Duty Cycle
void update_amp0_duty(int duty) {
  amp0_duty = duty;
  if (amp0_duty > 0) 
    amp0_zone_active = true; 
}

// Updates the Amplification 1 PWM Duty Cycle
void update_amp1_duty(int duty) {
  amp1_duty = 100;
  if (amp1_duty > 0) 
    amp1_zone_active = true; 
}

// Updates the Amplification 2 PWM Duty Cycle
void update_amp2_duty(int duty) {
  amp2_duty = duty;
  if (amp2_duty > 0) 
    amp2_zone_active = true; 
}

#ifdef USE_MOTOR
// Updates the Motor PWM Duty Cycle
void update_motor_duty(int duty) {
  motor_duty = duty;
  if (motor_duty > 0) 
    motor_active = true; 
  else 
    motor_active = false;
}
#endif


void pwm_task(void * pvParameters) {
  BaseType_t xReturned;
  usb_suspend_req_t sus_req;
  usb_suspend_acpt_t sus_acpt = {
    .task = PWM,
    .suspended = true
  };
  usb_suspend_over_t sus_over = {
    .task = PWM,
    .over = true
  };

  // Initalize the pwm channels
  init_pwms();

  // Set Original Duty Cycles to 0
  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);
  app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL,  amp0_duty);
  app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL,  amp1_duty);
  app_pwm_channel_duty_set(&PWM2, AMP2_CHANNEL,  amp2_duty);
  
  amp1_zone_active = true;

  // Main Task Loop
  // IEO Note: Probably want to add your own PWM to this thing too?
  for (;;) {
    // If not running a duty cycle do nothing
    if (!valve_zone_active && !amp0_zone_active && !amp1_zone_active && !amp2_zone_active) {
      vTaskDelay(100);
      // Check to see if we need to suspend for USB to be enabled
      if (uxQueueMessagesWaiting(pwm_usbWaitQueue) > 0) {
        xReturned = xQueueReceive(pwm_usbWaitQueue, &sus_req, 0) ;
        if (xReturned != pdPASS) {
          printf("PWM: Unable to receive usb suspend request from pwm_usbWaitQueue\n");
        }
        // Send Suspend Accepted
        xReturned = xQueueSend(usb_recvUsbWaitAcceptQueue, &sus_acpt, 0); 
        if (xReturned != pdPASS) {
          printf("PWM: Unable to send usb suspend accept from usb_recvUsbWaitAcceptQueue\n");
        }
        printf("PWM: Suspending for 15 seconds.\n");
        // Delay Task for 15 Seconds
        vTaskDelay(pdMS_TO_TICKS(USB_SUSPEND_TASKS_TIME));
        // Send Suspend Over
        xReturned = xQueueSend(usb_usbWaitOverQueue, &sus_over, 0); 
        if (xReturned != pdPASS) {
          printf("PWM: Unable to send usb suspend over to usb_usbWaitOverQueue\n");
        }
      }
      continue;
    }
  
    // PWM0 Control
    if (pwm0_ready_flag) {
      if (valve_zone_active || amp0_zone_active) {
        pwm0_ready_flag = false;
      }

      if (valve_duty > 0) {
        app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty); 
      } else if(valve_zone_active) {
        valve_zone_active =  false;
        app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 0);
      }

      if (amp0_duty > 0){
        app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL,  amp0_duty);
      } else if (amp0_zone_active) {
        amp0_zone_active = false;
        app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL,  0);
      }
    }
    else if (!pwm0_ready_flag && (valve_zone_active || amp0_zone_active)) {
      vTaskDelay(15);
    }
    
    // PWM2 Control
    if (pwm2_ready_flag) {
      if (amp1_zone_active || amp2_zone_active) {
        pwm2_ready_flag = false;
      }

      if (amp1_duty > 0){
        app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL,  amp1_duty);
      } else if(amp1_zone_active) {
        app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL,  0);
        amp1_zone_active = false;
      }
        
      if (amp2_duty > 0) {
        app_pwm_channel_duty_set(&PWM2, AMP2_CHANNEL, amp2_duty);
      } else if(amp2_zone_active)  {
        amp2_zone_active = false;
        app_pwm_channel_duty_set(&PWM2, AMP2_CHANNEL, 0);
      }
    }
    else if (!pwm2_ready_flag && (amp1_zone_active || amp2_zone_active)) {
      vTaskDelay(15);
    }
  }
}