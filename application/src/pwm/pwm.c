#include "pwm.h"
#include "../motor.h"
#include "timers.h"

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

xQueueHandle pwmRxQueue;

void pwm0_ready_callback(uint32_t pwm_id) {
  pwm0_ready_flag = true;
}

void pwm2_ready_callback(uint32_t pwm_id) {
  pwm2_ready_flag = true;
}

bool pwmEnabled = false;

void init_pwms() {
  ret_code_t err;

  /* Create Configurations */

  /* 1 Channel PWM, 200Hz, Active High, Valve Zone Pin */
  app_pwm_config_t pwm0_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, VALVE_ZONE_PIN, SAMPLE_HEATER_PIN);
  pwm0_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwm0_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
  //1 Channel PWM, 10kHz, Active High, Motor Control Pin */
  app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_2CH(100L, MOTOR_OUTPUT_PIN, MOTOR_OUTPUT_PIN2);
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
  pwmEnabled = true;

  // Set Original Duty Cycles to 0
  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 0);
  app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL, 0);
  app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL, 0);
}

void updateDutyCycles(temperature_pwm_data_t pwmData) {
  PwmRxQueueMsg_t msg = {
      .type = PWM_MSG_CALLBACK_EVENT};
  BaseType_t xReturned;

  valve_duty = pwmData.valve_zone_pwm;
  amp0_duty = pwmData.amp0_zone_pwm;
  amp1_duty = pwmData.amp1_zone_pwm;
  amp2_duty = pwmData.amp2_zone_pwm;

  if (valve_duty > 0) {
    valve_zone_active = true;
  }
  if (amp0_duty > 0) {
    amp0_zone_active = true;
  }
  if (amp1_duty > 0) {
    amp1_zone_active = true;
  }
  if (amp2_duty > 0) {
    amp2_zone_active = true;
  }

  xReturned = xQueueSend(pwmRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    printf("USB: Unable to send main state response to main_mainStateRespQueue queue.\n");
  }
}

void pwm_task(void *pvParameters) {
  BaseType_t xReturned;
  usb_suspend_req_t sus_req;
  usb_suspend_acpt_t sus_acpt = {
      .task = PWM,
      .suspended = true};
  usb_suspend_over_t sus_over = {
      .task = PWM,
      .over = true};

// Set Original Duty Cycles to 0
#ifdef SAMPLE_PREP_BOARD
  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 0);
  app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL, amp0_duty);
#else
  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);
  app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL, amp0_duty);
  app_pwm_channel_duty_set(&PWM2, AMP2_CHANNEL, amp2_duty);
#endif

  // for motor
  app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL, amp1_duty);

  PwmRxQueueMsg_t pwmMsg;

  // Main Task Loop
  for (;;) {
    xReturned = xQueueReceive(pwmRxQueue, &pwmMsg, portMAX_DELAY);
    if (xReturned != pdPASS) {
      printf("Unable to Rx data to sensor queue\n");
    } else {
      switch (pwmMsg.type) {
      case PWM_MSG_UPDATE_DUTY: {

        break;
      }
      case PWM_MSG_CALLBACK_EVENT: {
        if (pwm0_ready_flag) {
          if (valve_zone_active || amp0_zone_active) {
            pwm0_ready_flag = false;
          }

          if (valve_duty > 0) {
            app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);
          } else if (valve_zone_active) {
            valve_zone_active = false;
            app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 0);
          }

          if (amp0_duty > 0) {
            app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL, amp0_duty);
          } else if (amp0_zone_active) {
            amp0_zone_active = false;
            app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL, 0);
          }
        }

        if (pwm2_ready_flag) {
          if (amp1_zone_active) {
            pwm2_ready_flag = false;
          }

          if (amp1_duty > 0) {
            app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL, amp1_duty);
          } else if (amp1_zone_active) {
            app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL, 0);
            amp1_zone_active = false;
          }
        }
        break;
      }

      case PWM_MSG_DISABLE: {
        app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 0);
        app_pwm_channel_duty_set(&PWM0, AMP0_CHANNEL, 0);
        app_pwm_channel_duty_set(&PWM2, AMP1_CHANNEL, 0);
        app_pwm_channel_duty_set(&PWM2, AMP2_CHANNEL, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
        if (pwmEnabled) {
          app_pwm_disable(&PWM0);
          app_pwm_disable(&PWM2);
          pwmEnabled = false;
        }
        break;
      }

      case PWM_MSG_ENABLE: {
        if (!pwmEnabled) {
          app_pwm_enable(&PWM0);
          app_pwm_enable(&PWM2);
          pwmEnabled = true;
        }
        break;
      }

      default:
        break;
      }
    }
  }
}