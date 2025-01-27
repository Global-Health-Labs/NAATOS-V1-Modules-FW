#include "pwm.h"
#include "../motor.h"
#include "timers.h"

APP_PWM_INSTANCE(PWM0, 0); // Create instance "PWM0" using TIMER0
APP_PWM_INSTANCE(PWM2, 2); // Create instance "PWM2" using TIMER2

static volatile bool pwm0_ready_flag = false;
static volatile bool pwm2_ready_flag = false;

#ifdef SAMPLE_PREP_BOARD
  int heater_duty = 0;
  int motor_duty = 0;
#else
  int amp_duty = 0;
  int valve_duty = 0;
#endif

#ifdef SAMPLE_PREP_BOARD
  static bool heater_active = false;
  static bool motor_active = false;
#else
  static bool amp_active = false;
  static bool valve_active = false;
#endif

static usb_suspend_acpt_t sus_acpt = {
      .task = PWM,
      .suspended = true};

static usb_suspend_over_t sus_over = {
      .task = PWM,
      .over = true};

xQueueHandle pwmRxQueue;

void pwm0_ready_callback(uint32_t pwm_id) {
  pwm0_ready_flag = true;
}

void pwm2_ready_callback(uint32_t pwm_id) {
  pwm2_ready_flag = true;
}

bool motorPWMEnabled = false;
bool heaterPWMEnabled = false;

void init_pwms(void) {
  ret_code_t err;

  /* Create Configurations */
#ifdef SAMPLE_PREP_BOARD
  /* 1 Channel PWM, 200Hz, Active High, Valve Zone Pin */
  app_pwm_config_t pwm0_cfg = APP_PWM_DEFAULT_CONFIG_1CH(5000L, SAMPLE_HEATER_PIN); // MAKE SURE THIS CHANGE WORKS
  pwm0_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  //1 Channel PWM, 10kHz, Active High, Motor Control Pin */
  app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_1CH(100L, MOTOR_OUTPUT_PIN); // MAKE SURE THIS CHANGE WORKS
  pwm2_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_LOW;
#else 
  /* 1 Channel PWM, 200Hz, Active High, Valve Zone Pin */
  app_pwm_config_t pwm0_cfg = APP_PWM_DEFAULT_CONFIG_1CH(5000L, VALVE_ZONE_PIN);
  pwm0_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  //1 Channel PWM, 10kHz, Active High, Motor Control Pin */
  app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_1CH(5000L, AMP_ZONE_PIN);
  pwm2_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
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
  heaterPWMEnabled = true;
  motorPWMEnabled = true;

  // Set Original Duty Cycles to 0
#ifdef SAMPLE_PREP_BOARD
  app_pwm_channel_duty_set(&PWM0, HEATER_CHANNEL, 0);
  app_pwm_channel_duty_set(&PWM2, MOTOR_CHANNEL, 0);
#else 
  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 0);
  app_pwm_channel_duty_set(&PWM2, AMP_CHANNEL, 0);
#endif
  
}

void uninit_pwms(void) {
  ret_code_t err;
  // Disable
  app_pwm_disable(&PWM0);
  app_pwm_disable(&PWM2);
  // Uninitialize
  err = app_pwm_uninit(&PWM0);
  APP_ERROR_CHECK(err);
  err = app_pwm_uninit(&PWM2);
  APP_ERROR_CHECK(err);
  // Set pins as input disconnect
#ifdef SAMPLE_PREP_BOARD
  nrf_gpio_cfg_input(SAMPLE_HEATER_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
  nrf_gpio_cfg_input(MOTOR_OUTPUT_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
#else
  nrf_gpio_cfg_input(VALVE_ZONE_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
  nrf_gpio_cfg_input(AMP_ZONE_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
#endif

  heaterPWMEnabled = false;
  motorPWMEnabled = false;
}

void updateDutyCycles(temperature_pwm_data_t pwmData) {
  PwmRxQueueMsg_t msg = {
      .type = PWM_MSG_CALLBACK_EVENT};
  BaseType_t xReturned;

#ifdef SAMPLE_PREP_BOARD
  heater_duty = pwmData.heater_pwm;
  motor_duty = pwmData.motor_pwm;

  app_pwm_channel_duty_set(&PWM0, HEATER_CHANNEL, heater_duty);
  app_pwm_channel_duty_set(&PWM2, MOTOR_CHANNEL, motor_duty);

  if (heater_duty > 0) {
    heater_active = true;
  }
  if (motor_duty > 0) {
    motor_active = true;
  }
#else
  amp_duty = pwmData.amp_pwm;
  valve_duty = pwmData.valve_pwm;

  if (amp_duty > 0) {
    amp_active = true;
  }
  if (valve_duty > 0) {
    valve_active = true;
  }
#endif

  

  xReturned = xQueueSend(pwmRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("USB: Unable to send main state response to main_mainStateRespQueue queue.");
  }
}

void pwm_task(void *pvParameters) {
  BaseType_t xReturned;
  PwmRxQueueMsg_t pwmMsg;
  usb_suspend_req_t sus_req;

#ifdef SAMPLE_PREP_BOARD
  app_pwm_channel_duty_set(&PWM0, HEATER_CHANNEL, 1);
  app_pwm_channel_duty_set(&PWM2, MOTOR_CHANNEL, 1);
#else
  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);
  app_pwm_channel_duty_set(&PWM2, AMP_CHANNEL, amp_duty);
#endif

  // Main Task Loop
  for (;;) {
    xReturned = xQueueReceive(pwmRxQueue, &pwmMsg, portMAX_DELAY);
    if (xReturned != pdPASS) {
      send_debug_log_message("Unable to Rx data to sensor queue");
    } else {
      switch (pwmMsg.type) {
      case PWM_MSG_UPDATE_DUTY: {

        break;
      }
      case PWM_MSG_CALLBACK_EVENT: {
#ifdef SAMPLE_PREP_BOARD
        if (pwm0_ready_flag) {
          if (heater_active) {
            pwm0_ready_flag = false;
          }

          if (heater_duty > 0) {
           //app_pwm_channel_duty_set(&PWM0, HEATER_CHANNEL, heater_duty);
          } else if (heater_active) {
            heater_active = false;
            //app_pwm_channel_duty_set(&PWM0, HEATER_CHANNEL, 0);
          }
        }

        if (pwm2_ready_flag) {
          if (motor_active) {
            pwm0_ready_flag = false;
          }

          if (motor_duty > 0) {
           //app_pwm_channel_duty_set(&PWM2, MOTOR_CHANNEL, motor_duty);
          } else if (motor_active) {
            motor_active = false;
            //app_pwm_channel_duty_set(&PWM2, MOTOR_CHANNEL, 0);
          }
        }
#else
        if (pwm0_ready_flag) {
          if (valve_active) {
            pwm0_ready_flag = false;
          }

          if (valve_duty > 0) {
            app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);
          } else if (valve_active) {
            valve_active = false;
            app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 0);
          }
        }

        if (pwm2_ready_flag) {
          if (amp_active) {
            pwm2_ready_flag = false;
          }

          if (amp_duty > 0) {
            app_pwm_channel_duty_set(&PWM2, AMP_CHANNEL, amp_duty);
          } else if (amp_active) {
            app_pwm_channel_duty_set(&PWM2, AMP_CHANNEL, 0);
            amp_active = false;
          }
        }
#endif
        break;
      }

      case PWM_MSG_HEATER_DISABLE: {
 #ifdef SAMPLE_PREP_BOARD
        app_pwm_channel_duty_set(&PWM0, HEATER_CHANNEL, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
        if (heaterPWMEnabled) {
          app_pwm_disable(&PWM0);
          heaterPWMEnabled = false;
        }
        break;
#else 
        app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 0);
        app_pwm_channel_duty_set(&PWM2, AMP_CHANNEL, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
        if (heaterPWMEnabled) {
          app_pwm_disable(&PWM0);
          app_pwm_disable(&PWM2);
          heaterPWMEnabled = false;
        }
        break;
#endif
      }

      case PWM_MSG_HEATER_ENABLE: {
#ifdef SAMPLE_PREP_BOARD
        if (!heaterPWMEnabled) {
          app_pwm_enable(&PWM0);
          heaterPWMEnabled = true;
        }
        break;
#else
        if (!heaterPWMEnabled) {
          app_pwm_enable(&PWM0);
          app_pwm_enable(&PWM2);
          heaterPWMEnabled = true;
        }
        break;
#endif
      }

      case PWM_MSG_MOTOR_DISABLE: {
        app_pwm_channel_duty_set(&PWM2, MOTOR_CHANNEL, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
        if (motorPWMEnabled) {
          app_pwm_disable(&PWM2);
          motorPWMEnabled = false;
        }
        break;
      }

      case PWM_MSG_MOTOR_ENABLE: {
        if (!motorPWMEnabled) {
          app_pwm_enable(&PWM2);
          motorPWMEnabled = true;
        }
        break;
      }

      default:
        break;
      }
    }
  }
}