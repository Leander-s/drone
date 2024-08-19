#include "nrf24.h"

void nrf24_init() {
  spi_init(spi0, 1000000);

  gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
  gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 0);

  gpio_init(PIN_CS);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_put(PIN_CS, 1);

  gpio_init(PIN_CE);
  gpio_set_dir(PIN_CE, GPIO_OUT);
  gpio_put(PIN_CE, 0);
}

void nrf24_setup() {
  nrf24_write_register(CONFIG, PWR_UP | CRCO | EN_CRC);
  nrf24_write_register(EN_AA, 0x00);
  nrf24_write_register(EN_RX_ADDR, 0x1F);
  nrf24_write_register(SETUP_AW, 0x05);
  nrf24_write_register(SETUP_RETR, 0x00);
  nrf24_write_register(RF_CH, 0x4C);
  nrf24_write_register(RF_SETUP, 0x0D);
  nrf24_write_register(RX_PW_P0, 0x20);
  nrf24_write_register(FEATURE, 0x01);
}

void set_mode(uint8_t mode) {
  nrf24_write_register(CONFIG, mode);

  uint8_t addr = 0xE7;
  uint8_t reg;
  if (mode == TX_MODE) {
    reg = W_REGISTER | TX_ADDR;
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &reg, 1);
    spi_write_blocking(SPI_PORT, &addr, 1);
    gpio_put(PIN_CS, 1);
    reg = W_REGISTER | RX_ADDR_P0;
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &reg, 1);
    spi_write_blocking(SPI_PORT, &addr, 1);
    gpio_put(PIN_CS, 1);
  }
  if (mode == RX_MODE)
    reg = W_REGISTER | RX_ADDR_P0;
  gpio_put(PIN_CS, 0);
  spi_write_blocking(SPI_PORT, &reg, 1);
  spi_write_blocking(SPI_PORT, &addr, 1);
  gpio_put(PIN_CS, 1);
}

uint8_t nrf24_read_register(uint8_t reg) {
  uint8_t result;
  uint8_t dummy;
  gpio_put(PIN_CS, 0);
  spi_write_blocking(SPI_PORT, &reg, 1);
  spi_read_blocking(SPI_PORT, 0, &dummy, 1);
  spi_write_blocking(SPI_PORT, &dummy, 1);
  spi_read_blocking(SPI_PORT, 0, &result, 1);
  gpio_put(PIN_CS, 1);
  return result;
}

void nrf24_write_register(uint8_t reg, uint8_t value) {
  reg |= W_REGISTER;
  gpio_put(PIN_CS, 0);
  spi_write_blocking(SPI_PORT, &reg, 1);
  spi_write_blocking(SPI_PORT, &value, 1);
  gpio_put(PIN_CS, 1);
}

void nrf24_send(char *data, int len) {
  gpio_put(LED_PIN, 1);

  gpio_put(PIN_CE, 0);

  set_mode(TX_MODE);

  uint8_t cmd = FLUSH_TX;
  gpio_put(PIN_CS, 0);
  spi_write_blocking(SPI_PORT, &cmd, 1);
  gpio_put(PIN_CS, 1);

  cmd = W_TX_PAYLOAD;
  gpio_put(PIN_CS, 0);
  spi_write_blocking(SPI_PORT, &cmd, 1);
  spi_write_blocking(SPI_PORT, data, len);
  gpio_put(PIN_CS, 1);

  gpio_put(PIN_CE, 1);
  sleep_us(10);
  gpio_put(PIN_CE, 0);

  gpio_put(LED_PIN, 0);
}

void nrf24_read(char *data, int len) {
  set_mode(RX_MODE);

  gpio_put(PIN_CE, 1);
  bool data_available = false;
  gpio_put(LED_PIN, 0);
  while (!data_available) {
    data_available = nrf24_data_available();
    sleep_us(10);
  }
  gpio_put(PIN_CE, 0);

  uint8_t cmd = R_RX_PAYLOAD;
  uint8_t dummy[32];
  gpio_put(PIN_CS, 0);
  spi_write_blocking(SPI_PORT, &cmd, 1);
  spi_write_blocking(SPI_PORT, dummy, len);
  spi_read_blocking(SPI_PORT, 0, data, len);
  gpio_put(PIN_CS, 1);
  sleep_us(10);

  nrf24_write_register(STATUS, 0x70);

  nrf24_flush_rx();

  gpio_put(LED_PIN, 1);
}

void nrf24_flush_rx() {
  uint8_t cmd = FLUSH_RX;
  gpio_put(PIN_CS, 0);
  spi_write_blocking(SPI_PORT, &cmd, 1);
  gpio_put(PIN_CS, 1);
}

bool nrf24_data_available() {
  uint8_t status = nrf24_read_register(STATUS);
  if (status & (1 << 6)) {
    nrf24_write_register(STATUS, (1 << 6));
    return true;
  }
  return false;
}
