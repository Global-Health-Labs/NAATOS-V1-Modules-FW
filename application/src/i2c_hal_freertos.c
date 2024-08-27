#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "i2c_hal_freertos.h"
#include "nrf_drv_twi.h"
#include "nrf_error.h"
#include "nrf_gpio.h"
#include "nrfx_twi.h"
#include "sdk_errors.h"
#include "usb.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

static const nrf_drv_twi_t m_i2c[i2c_num_interfaces] = {
    NRF_DRV_TWI_INSTANCE(I2C_SYSTEM_INSTANCE_ID),
    NRF_DRV_TWI_INSTANCE(I2C_SENSOR_INSTANCE_ID),
};

static SemaphoreHandle_t m_i2c_semaphores[i2c_num_interfaces];
static nrf_drv_twi_config_t twi_configs[i2c_num_interfaces];

#define MAX_I2C_RETRIES 3

void i2c_error_recovery(i2c_interface_selection_t interface) {
  nrf_drv_twi_disable(&m_i2c[interface]);
  nrf_drv_twi_uninit(&m_i2c[interface]);

  nrf_gpio_cfg(twi_configs[interface].scl, // pin_number
      NRF_GPIO_PIN_DIR_OUTPUT,             // Input.
      NRF_GPIO_PIN_INPUT_DISCONNECT,       // Connect input buffer.
      NRF_GPIO_PIN_PULLUP,                 // Pin pull-up resistor disabled.
      NRF_GPIO_PIN_H0D1,                   // Standard '0', standard '1'.
      //NRF_GPIO_PIN_S0S1,
      NRF_GPIO_PIN_NOSENSE // Pin sense level disabled.
  );

  for (int i = 0; i < 9; i++) {
    nrf_gpio_pin_clear(twi_configs[interface].scl);
    vTaskDelay(pdMS_TO_TICKS(1));
    nrf_gpio_pin_set(twi_configs[interface].scl);
    vTaskDelay(pdMS_TO_TICKS(1));
  }

  nrf_gpio_cfg(twi_configs[interface].scl, // pin_number
      NRF_GPIO_PIN_DIR_INPUT,              // Input.
      NRF_GPIO_PIN_INPUT_CONNECT,          // Connect input buffer.
      NRF_GPIO_PIN_PULLUP,                 // Pin pull-up resistor disabled.
      NRF_GPIO_PIN_H0D1,                   // Standard '0', standard '1'.
      //NRF_GPIO_PIN_S0S1,
      NRF_GPIO_PIN_NOSENSE // Pin sense level disabled.
  );
  vTaskDelay(pdMS_TO_TICKS(5)); // Short delay to ensure the bus is idle
  nrf_drv_twi_init(&m_i2c[interface], &twi_configs[interface], NULL, NULL);

  int size;
  char buff[60];
  size = sprintf(buff, "I2C error recovery attempt on interface: %i\n", interface);
  printf("I2C error recovery attempt on interface: %i\n", interface);
  write_to_com(buff, size);
}

/* INIT for TWI hardware for peripheral */
void vInit_TWI_Hardware(i2c_interface_selection_t interface, uint32_t sda_pin, uint32_t scl_pin, i2c_speed_selection_t frequency) {
  //NRF_LOG_INFO( "vInit_TWI_Hardware" );
  ret_code_t err_code;
  nrf_drv_twi_frequency_t nrf_freq;

  if (interface >= i2c_num_interfaces) {
    return;
  }

  if (i2c_speed_100k == frequency) {
    nrf_freq = NRF_DRV_TWI_FREQ_100K;
  } else {
    nrf_freq = NRF_DRV_TWI_FREQ_400K;
  }
  nrf_drv_twi_config_t twi_config = {
      .scl = scl_pin,                              // SCL pin number.
      .sda = sda_pin,                              // SDA pin number.
      .frequency = nrf_freq,                       // TWI frequency.
      .interrupt_priority = APP_IRQ_PRIORITY_HIGH, // Interrupt priority.
      .clear_bus_init = false,                     // Clear bus during init.
      .hold_bus_uninit = false                     // Hold pull up state on gpio pins after uninit.

  };
  twi_configs[interface] = twi_config;

  err_code = nrf_drv_twi_init(&m_i2c[interface], &twi_configs[interface], NULL, NULL);

  //APP_ERROR_CHECK( err_code );
  //
  nrf_gpio_cfg(scl_pin,           // pin_number
      NRF_GPIO_PIN_DIR_INPUT,     // Input.
      NRF_GPIO_PIN_INPUT_CONNECT, // Connect input buffer.
      NRF_GPIO_PIN_PULLUP,        // Pin pull-up resistor disabled.
      NRF_GPIO_PIN_H0D1,
      //NRF_GPIO_PIN_S0S1,
      NRF_GPIO_PIN_NOSENSE // Pin sense level disabled.
  );
  nrf_gpio_cfg(sda_pin,           // pin_number
      NRF_GPIO_PIN_DIR_INPUT,     // Input.
      NRF_GPIO_PIN_INPUT_CONNECT, // Connect input buffer.
      NRF_GPIO_PIN_PULLUP,        // Pin pull-up resistor disabled.
      NRF_GPIO_PIN_H0D1,
      //NRF_GPIO_PIN_S0S1,
      NRF_GPIO_PIN_NOSENSE // Pin sense level disabled.
  );
  // RTOS init
  m_i2c_semaphores[interface] = xSemaphoreCreateBinary(); // Ensure the semaphore is created before it gets used.
  ASSERT(m_i2c_semaphores[interface]);                    // LOCK HERE: the semaphore could not be created
  xSemaphoreGive(m_i2c_semaphores[interface]);            // 'Give' the peripheral protection semaphore
  //NRF_LOG_INFO( "vInit_TWI_Hardware: DONE" );
  return;
}

ret_code_t xUtil_TWI_Read(i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t *p_buff, uint16_t length) {
#ifdef VERB_HW_TWI
  NRF_LOG_INFO("xUtil_TWI_Read:\tslave = 0x%02X\taddr = 0x%02X\tlenght = %u", slave_addr, start_addr, length);
#endif
  ret_code_t err_code;
  static uint8_t reg_addr;
  reg_addr = start_addr;

  if (length == 0) {
    return NRF_ERROR_INVALID_LENGTH;
  }

  if (xSemaphoreTake(m_i2c_semaphores[interface], 50) != pdPASS) {
    return NRF_ERROR_BUSY;
  }

  for (uint8_t retries = 0; retries < MAX_I2C_RETRIES; ++retries) {
    nrf_drv_twi_enable(&m_i2c[interface]);

    err_code = nrf_drv_twi_tx(&m_i2c[interface], slave_addr, &reg_addr, 1, true);
    if (err_code == NRF_SUCCESS) {
      err_code = nrf_drv_twi_rx(&m_i2c[interface], slave_addr, p_buff, length);
    }

    nrf_drv_twi_disable(&m_i2c[interface]);

    if (err_code == NRF_SUCCESS) {
      break;
    } else {
      i2c_error_recovery(interface);
    }
  }

  xSemaphoreGive(m_i2c_semaphores[interface]);
  return err_code;
}

/*  */
ret_code_t xUtil_TWI_Write(i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t const *p_buff, uint16_t length) {
#ifdef VERB_HW_TWI
  NRF_LOG_INFO("xUtil_TWI_Write:\tslave = 0x%02X\taddr = 0x%02X\tlenght = %u", slave_addr, start_addr, length);
#endif
  ret_code_t err_code;
  static uint8_t reg_addr;
  reg_addr = start_addr;

  if (length == 0) {
    return NRF_ERROR_INVALID_LENGTH;
  }

  if (xSemaphoreTake(m_i2c_semaphores[interface], 50) != pdPASS) {
    return NRF_ERROR_BUSY;
  }

  for (uint8_t retries = 0; retries < MAX_I2C_RETRIES; ++retries) {
    nrf_drv_twi_enable(&m_i2c[interface]);

    err_code = nrf_drv_twi_tx(&m_i2c[interface], slave_addr, &reg_addr, 1, true);
    if (err_code == NRF_SUCCESS) {
      err_code = nrf_drv_twi_tx(&m_i2c[interface], slave_addr, p_buff, length, false);
    }

    nrf_drv_twi_disable(&m_i2c[interface]);

    if (err_code == NRF_SUCCESS) {
      break;
    } else {
      i2c_error_recovery(interface);
    }
  }

  xSemaphoreGive(m_i2c_semaphores[interface]);
  return err_code;
}

ret_code_t xUtil_TWI_Write_Single(i2c_interface_selection_t interface, uint8_t slave_addr, uint8_t start_addr, uint8_t const *p_buff, uint16_t length) {
#ifdef VERB_HW_TWI
  NRF_LOG_INFO("xUtil_TWI_Write:\tslave = 0x%02X\taddr = 0x%02X\tlenght = %u", slave_addr, start_addr, length);
#endif
  ret_code_t err_code;
  static uint8_t reg_addr;
  reg_addr = start_addr;

  if (length == 0) {
    return NRF_ERROR_INVALID_LENGTH;
  }

  if (xSemaphoreTake(m_i2c_semaphores[interface], 50) != pdPASS) {
    return NRF_ERROR_BUSY;
  }

  for (uint8_t retries = 0; retries < MAX_I2C_RETRIES; ++retries) {
    nrf_drv_twi_enable(&m_i2c[interface]);

    err_code = nrf_drv_twi_tx(&m_i2c[interface], slave_addr, p_buff, length, false);

    nrf_drv_twi_disable(&m_i2c[interface]);

    if (err_code == NRF_SUCCESS) {
      break;
    } else {
      i2c_error_recovery(interface);
    }
  }

  xSemaphoreGive(m_i2c_semaphores[interface]);
  return err_code;
}