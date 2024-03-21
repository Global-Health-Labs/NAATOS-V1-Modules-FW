#include "pwm.h"

APP_PWM_INSTANCE(PWM0, 0); // Create instance "PWM0" using TIMER0
APP_PWM_INSTANCE(PWM2, 2); // Create instance "PWM2" using TIMER2

static volatile bool pwm0_ready_flag = false;
static volatile bool pwm2_ready_flag = false;

static volatile unsigned int valve_duty;
static volatile unsigned int amp0_duty;
static volatile unsigned int amp1_duty;
static volatile unsigned int amp2_duty;

static bool valve_zone_active = false;
static bool amp0_zone_active = false;
static bool amp1_zone_active = false;
static bool amp2_zone_active = false;

void pwm0_ready_callback(uint32_t pwm_id) {
  pwm0_ready_flag = true;
}

void pwm2_ready_callback(uint32_t pwm_id) {
  pwm2_ready_flag = true;
}

void init_pwms() {
  ret_code_t err;

  /* Create Configurations */
  /* 1 Channel PWM, 200Hz, Active High, Valve Zone Pin */
  app_pwm_config_t pwm0_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, VALVE_ZONE_PIN, AMP0_ZONE_PIN);
  pwm0_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwm0_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
  /* 1 Channel PWM, 200Hz, Active High, Amplification Zone Pin */
  app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, AMP1_ZONE_PIN, AMP2_ZONE_PIN);
  pwm2_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwm2_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
  
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
  
  /* Set duty cycles to 0 */
  valve_duty = 0;
  amp0_duty = 0;
  amp1_duty = 0;
  amp2_duty = 0;
}

// Updates the Valve PWM Duty Cycle
void update_valve_duty(int duty) {
  valve_duty = duty;
  if (valve_duty > 0)
    valve_zone_active = true; 
  else 
    valve_zone_active = false;
}

// Updates the Amplification 0 PWM Duty Cycle
void update_amp0_duty(int duty) {
  amp0_duty = duty;
  if (amp0_duty > 0) 
    amp0_zone_active = true; 
  else 
    amp0_zone_active = false;
}

// Updates the Amplification 1 PWM Duty Cycle
void update_amp1_duty(int duty) {
  amp1_duty = duty;
  if (amp1_duty > 0) 
    amp1_zone_active = true; 
  else 
    amp1_zone_active = false;
}

// Updates the Amplification 2 PWM Duty Cycle
void update_amp2_duty(int duty) {
  amp2_duty = duty;
  if (amp2_duty > 0) 
    amp2_zone_active = true; 
  else 
    amp2_zone_active = false;
}

void pwm_task(void * pvParameters) {
  // Initalize the pwm channels
  init_pwms();

  // Set Original Duty Cycles to 0
  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);
  app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL,  amp0_duty);
  app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL,  amp1_duty);
  app_pwm_channel_duty_set(&PWM2, AMP2_CHANNEL,  amp2_duty);

  // remove
  update_valve_duty(70);
  update_amp0_duty(50);
  update_amp1_duty(30);
  update_amp2_duty(10);
  
  // Main Task Loop
  for (;;) {
    // If not running a duty cycle do nothing
    if (!valve_zone_active && !amp0_zone_active && !amp1_zone_active && !amp2_zone_active)
      vTaskDelay(100);
  
    // PWM0 Control
    if (pwm0_ready_flag) {
      pwm0_ready_flag = false;
      if (valve_zone_active)
        app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty); 
      if (amp0_zone_active) 
         app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL,  amp0_duty);
    }
    else if (!pwm0_ready_flag && (valve_zone_active || amp0_zone_active)) {
      vTaskDelay(5);
    }
    
    // PWM2 Control
    if (pwm2_ready_flag) {
      pwm2_ready_flag = false;
      if (amp1_zone_active) 
        app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL,  amp1_duty);
      if (amp2_zone_active)
        app_pwm_channel_duty_set(&PWM2, AMP2_CHANNEL, amp2_duty);
    }
    else if (!pwm2_ready_flag && (amp1_zone_active || amp2_zone_active)) {
      vTaskDelay(5);
    }
  }
  /*
  app_pwm_channel_duty_set(&PWM0, 0, 50);
  while(1) {
    pwm0_ready_flag = false;
    while(!pwm0_ready_flag);
    app_pwm_channel_duty_set(&PWM0, 0, 50);
  }
  */
}