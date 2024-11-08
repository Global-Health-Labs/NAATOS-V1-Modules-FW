#include "pwm.h"
#include "../motor.h"
#include "timers.h"

APP_PWM_INSTANCE(PWM0, 0); // Create instance "PWM0" using TIMER0
APP_PWM_INSTANCE(PWM2, 2); // Create instance "PWM2" using TIMER2

static volatile bool pwm0_ready_flag = false;
static volatile bool pwm2_ready_flag = false;

int heat_zone_0_duty = 0;
int heat_zone_1_duty = 0;
int heat_zone_2_duty = 0;
int heat_zone_3_duty = 0;
int motor_duty = 0;

static bool heat_zone_0_active = false;
static bool heat_zone_1_active = false;
static bool heat_zone_2_active = false;
static bool heat_zone_3_active = false;
static bool motor_active = false;

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

bool pwmEnabled = false;

void init_pwms(void) {
  ret_code_t err;

  /* Create Configurations */
#ifdef SAMPLE_PREP_BOARD
  /* 1 Channel PWM, 200Hz, Active High, Valve Zone Pin */
  app_pwm_config_t pwm0_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, HEATER_ZONE_0_PIN, SAMPLE_HEATER_PIN);
  pwm0_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwm0_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
  //1 Channel PWM, 10kHz, Active High, Motor Control Pin */
  app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_2CH(100L, MOTOR_OUTPUT_PIN, MOTOR_OUTPUT_PIN2);
  pwm2_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_LOW;
  pwm2_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_LOW;
#else 
  /* 1 Channel PWM, 200Hz, Active High, Valve Zone Pin */
  app_pwm_config_t pwm0_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, HEATER_ZONE_0_PIN, HEATER_ZONE_1_PIN);
  pwm0_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwm0_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
  //1 Channel PWM, 10kHz, Active High, Motor Control Pin */
  app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, HEATER_ZONE_2_PIN, HEATER_ZONE_3_PIN);
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
  pwmEnabled = true;

  // Set Original Duty Cycles to 0
  app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_0_CHANNEL, 0);
  app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_1_CHANNEL, 0);
  app_pwm_channel_duty_set(&PWM2, HEAT_ZONE_2_CHANNEL, 0);
  app_pwm_channel_duty_set(&PWM2, HEAT_ZONE_3_CHANNEL, 0);
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
  nrf_gpio_cfg_input(HEATER_ZONE_0_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
  nrf_gpio_cfg_input(SAMPLE_HEATER_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
  nrf_gpio_cfg_input(MOTOR_OUTPUT_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
  nrf_gpio_cfg_input(MOTOR_OUTPUT_PIN2, NRF_GPIO_PIN_INPUT_DISCONNECT);
#else
  nrf_gpio_cfg_input(HEATER_ZONE_0_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
  nrf_gpio_cfg_input(HEATER_ZONE_1_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
  nrf_gpio_cfg_input(HEATER_ZONE_2_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
  nrf_gpio_cfg_input(HEATER_ZONE_3_PIN, NRF_GPIO_PIN_INPUT_DISCONNECT);
#endif
}

void updateDutyCycles(temperature_pwm_data_t pwmData) {
  PwmRxQueueMsg_t msg = {
      .type = PWM_MSG_CALLBACK_EVENT};
  BaseType_t xReturned;

  heat_zone_0_duty = pwmData.heat_zone_0_pwm;
  heat_zone_1_duty = pwmData.heat_zone_1_pwm;
  heat_zone_2_duty = pwmData.heat_zone_2_pwm;
  heat_zone_3_duty = pwmData.heat_zone_3_pwm;

  if (heat_zone_0_duty > 0) {
    heat_zone_0_active = true;
  }
  if (heat_zone_1_duty > 0) {
    heat_zone_1_active = true;
  }
  if (heat_zone_2_duty > 0) {
    heat_zone_2_active = true;
  }
  if (heat_zone_3_duty > 0) {
    heat_zone_3_active = true;
  }

  xReturned = xQueueSend(pwmRxQueue, &msg, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("USB: Unable to send main state response to main_mainStateRespQueue queue.");
  }
}

void pwm_task(void *pvParameters) {
  BaseType_t xReturned;
  PwmRxQueueMsg_t pwmMsg;
  usb_suspend_req_t sus_req;

  app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_0_CHANNEL, heat_zone_0_duty);
  app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_1_CHANNEL, heat_zone_1_duty);
  app_pwm_channel_duty_set(&PWM2, HEAT_ZONE_2_CHANNEL, heat_zone_2_duty);
  app_pwm_channel_duty_set(&PWM2, HEAT_ZONE_3_CHANNEL, heat_zone_3_duty);

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
        if (pwm0_ready_flag) {
          if (heat_zone_0_active || heat_zone_1_active) {
            pwm0_ready_flag = false;
          }

          if (heat_zone_0_duty > 0) {
            app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_0_CHANNEL, heat_zone_0_duty);
          } else if (heat_zone_0_active) {
            heat_zone_0_active = false;
            app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_0_CHANNEL, 0);
          }

          if (heat_zone_1_duty > 0) {
            app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_1_CHANNEL, heat_zone_1_duty);
          } else if (heat_zone_1_active) {
            heat_zone_1_active = false;
            app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_1_CHANNEL, 0);
          }
        }

        if (pwm2_ready_flag) {
          if (heat_zone_2_active) {
            pwm2_ready_flag = false;
          }

          if (heat_zone_2_duty > 0) {
            app_pwm_channel_duty_set(&PWM2, HEAT_ZONE_2_CHANNEL, heat_zone_2_duty);
          } else if (heat_zone_2_active) {
            app_pwm_channel_duty_set(&PWM2, HEAT_ZONE_2_CHANNEL, 0);
            heat_zone_2_active = false;
          }
        }
        break;
      }

      case PWM_MSG_DISABLE: {
        app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_0_CHANNEL, 0);
        app_pwm_channel_duty_set(&PWM0, HEAT_ZONE_1_CHANNEL, 0);
        app_pwm_channel_duty_set(&PWM2, HEAT_ZONE_2_CHANNEL, 0);
        app_pwm_channel_duty_set(&PWM2, HEAT_ZONE_3_CHANNEL, 0);
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






void ghl_pwm_manual_set_channel(uint8_t zone, app_pwm_duty_t dutycycle) {
  
  //app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);

  if (!pwmEnabled) {
    app_pwm_enable(&PWM0);
    app_pwm_enable(&PWM2);
    pwmEnabled = true;
  }
  ret_code_t ret;

  switch(zone)  {
    case 0:
      //if (!pwmEnabled) {
      //  app_pwm_enable(&PWM0);
      //}
      ret = app_pwm_channel_duty_set(&PWM0, 0, dutycycle);
      break;
    case 1:
      //if (!pwmEnabled) {
      //  app_pwm_enable(&PWM0);
      //}
      ret = app_pwm_channel_duty_set(&PWM0, 1, dutycycle);
      break;
  }
}

