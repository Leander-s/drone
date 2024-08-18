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
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define RX_ADDR_P0 0x0A
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define FIFO_STATUS 0x17

void nrf24_init();

uint8_t nrf24_read_register(uint8_t reg);

void nrf24_write_register(uint8_t reg, uint8_t value);

void nrf24_send(uint8_t *data, int len);
