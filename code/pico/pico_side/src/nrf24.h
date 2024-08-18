#include "config.h"
#include "util.h"

#define SPI_PORT spi0
#define PIN_MISO 4
#define PIN_CS 5
#define PIN_SCK 6
#define PIN_MOSI 7
#define PIN_CE 8

#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define NOP 0xFF

#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define FIFO_STATUS 0x17

#define TX_MODE 0x0A
#define RX_MODE 0x0F

void nrf24_init();

void nrf24_setup();

void set_mode(uint8_t mode);

uint8_t nrf24_read_register(uint8_t reg);

void nrf24_write_register(uint8_t reg, uint8_t value);

void nrf24_send(uint8_t *data, int len);

void nrf24_read(uint8_t *data, int len);

void nrf24_flush_rx();

bool nrf24_data_available();
