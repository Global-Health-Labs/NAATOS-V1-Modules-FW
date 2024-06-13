#include "spi.h"

#if (SPI0_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master = NRF_DRV_SPI_INSTANCE(0);
#elif (SPI1_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master = NRF_DRV_SPI_INSTANCE(1);
#elif (SPI2_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master = NRF_DRV_SPI_INSTANCE(2);
#else
#error "No SPI enabled."
#endif

/* SPI transaction buffer length. */
#define TX_RX_BUF_LENGTH 16

/* Data buffers */
static uint8_t m_tx_data[TX_RX_BUF_LENGTH] = {0}; /* A buffer with data to transfer. */
static uint8_t m_rx_data[TX_RX_BUF_LENGTH] = {0}; /* A buffer for incoming data. */

static volatile bool m_transfer_complete = true;
static volatile bool spi_xfer_done;

void spi_event_handler(nrf_drv_spi_evt_t const *p_event, void *p_context) {
  // TODO: Something with callback?
}

/* Initalization */
void init_SPI_Hardware(uint32_t miso_pin, uint32_t mosi_pin, uint32_t clk_pin, uint32_t ss_pin) {
  nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
  spi_config.ss_pin = ss_pin;
  spi_config.miso_pin = miso_pin;
  spi_config.mosi_pin = mosi_pin;
  spi_config.sck_pin = clk_pin;
  APP_ERROR_CHECK(nrf_drv_spi_init(&m_spi_master, &spi_config, spi_event_handler, NULL));
}