#include "pwm.h"
#include "../motor.h"
#include "timers.h"

// MUSICAL NOTE FREQUENCIES
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define NOTE_REST 100

// ACTUAL STUFF BELOW
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
  //app_pwm_enable(&PWM0);
  //app_pwm_enable(&PWM2);
  //heaterPWMEnabled = true;
  //motorPWMEnabled = true;

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

  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);
  app_pwm_channel_duty_set(&PWM2, AMP_CHANNEL, amp_duty);

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
  app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, 1);
  app_pwm_channel_duty_set(&PWM2, AMP_CHANNEL, 1);
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
            //app_pwm_channel_duty_set(&PWM0, VALVE_CHANNEL, valve_duty);
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
            //app_pwm_channel_duty_set(&PWM2, AMP_CHANNEL, amp_duty);
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

      case PWM_MSG_BUZZER_TONE_1SEC_1: {
        // tone freq, duration=ms
        pwm_buzzer_blocking_sound(1000,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(1200,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(1400,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(1800,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(500,500,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(800,100,pwmMsg.buzzer_pwm_duty);
        break;
      }

      case PWM_MSG_BUZZER_TONE_1SEC_2: {
        // tone freq, duration=ms
        pwm_buzzer_blocking_sound(1000,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(900,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(800,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(700,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(250,500,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(150,100,pwmMsg.buzzer_pwm_duty);
        break;
      }

      case PWM_MSG_BUZZER_TONE_500ms_1: {
        // tone freq, duration=ms
        pwm_buzzer_blocking_sound(NOTE_DS3,50,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_E3, 50,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_DS3,50,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_E3, 50,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_DS3,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_CS3,200,pwmMsg.buzzer_pwm_duty);
        break;
      }

      case PWM_MSG_BUZZER_SONG_mariointro: {
        // tone freq, duration=ms
        pwm_buzzer_blocking_sound(NOTE_E5,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_REST,25,0);
        pwm_buzzer_blocking_sound(NOTE_E5,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_REST,100,0);
        pwm_buzzer_blocking_sound(NOTE_E5,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_REST,100,0);
        pwm_buzzer_blocking_sound(NOTE_C5,150,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_REST,50,0);
        pwm_buzzer_blocking_sound(NOTE_E5,100,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_REST,100,0);
        pwm_buzzer_blocking_sound(NOTE_G5,150,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(NOTE_REST,250,0);
        pwm_buzzer_blocking_sound(NOTE_G4,350,pwmMsg.buzzer_pwm_duty);
        break;
      }

      case PWM_MSG_BUZZER_TONE_200ms_1: {
        // tone freq, duration=ms
        pwm_buzzer_blocking_sound(100,25,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(150,25,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(200,25,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(250,25,pwmMsg.buzzer_pwm_duty);
        
        pwm_buzzer_blocking_sound(400,25,pwmMsg.buzzer_pwm_duty);
        pwm_buzzer_blocking_sound(500,25,pwmMsg.buzzer_pwm_duty);

        pwm_buzzer_blocking_sound(1000,50,pwmMsg.buzzer_pwm_duty);
        break;
      }

      default:
        break;
      }
    }
  }
}

#ifdef POWER_MODULE_BOARD
APP_PWM_INSTANCE(PWMbuzzer, 1); // Create instance "PWM1" using TIMER1/(PWM1?)
#define BUZZER_PIN_1 NRF_GPIO_PIN_MAP(0, 25)    // one side of buzzer
#define BUZZER_PIN_2 NRF_GPIO_PIN_MAP(0, 26)    // other side of buzzer

static volatile bool pwmbuzzer_ready_flag = false;

void pwmbuzzer_ready_callback(uint32_t pwm_id) {
  pwmbuzzer_ready_flag = true;
}
#endif

void pwm_buzzer_blocking_sound(uint16_t frequency, uint16_t durationms, uint8_t duty) {
#ifdef POWER_MODULE_BOARD & POWER_MODULE_REV_B
  ret_code_t err;

  // INIT PWM
  // ---- 2 Channel PWM, frequency (?), COMPLEMENTARY, BUZZER
  // period given in microseconds
  app_pwm_config_t pwmbuzzer_cfg = APP_PWM_DEFAULT_CONFIG_2CH((uint32_t) ((1.0/(float)frequency)*1e6), BUZZER_PIN_1, BUZZER_PIN_2);
  pwmbuzzer_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
  pwmbuzzer_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_LOW;


  /* Initalize with configurations */
  /* Initalize PWM */
  err = app_pwm_init(&PWMbuzzer, &pwmbuzzer_cfg, pwmbuzzer_ready_callback);
  APP_ERROR_CHECK(err);

  /* Enable PWMs */
  app_pwm_enable(&PWMbuzzer);

  //vTaskDelay(pdMS_TO_TICKS(5));

  // duty
  // 50% should be loudest, 0% inaudible; what about >50% ? maybe 100% also inaudible
  app_pwm_channel_duty_set(&PWMbuzzer, 0, duty);
  app_pwm_channel_duty_set(&PWMbuzzer, 1, duty);

  vTaskDelay(pdMS_TO_TICKS(durationms));

  // duty
  app_pwm_channel_duty_set(&PWMbuzzer, 0, 0);
  app_pwm_channel_duty_set(&PWMbuzzer, 1, 0);

  // disable
  app_pwm_disable(&PWMbuzzer);

  //uninit
  err = app_pwm_uninit(&PWMbuzzer);
  APP_ERROR_CHECK(err);


#endif
}
