#include "nrf24_config.h"

// PINs
#define SPI_PORT spi0
#define PIN_MISO 4
#define PIN_CS 14
#define PIN_SCK 6
#define PIN_MOSI 7
#define PIN_CE 17

// Commands
#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define NOP 0xFF
#define ACTIVATE 0x50
#define ACTIVATE_BYTE 0x73

// Registers
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RX_ADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define OBSERVE_TX 0x08
#define CD 0x09
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17
#define DYNPD 0x1C
#define FEATURE 0x1D

// Bits
// CONFIG
#define MASK_RX_DR 64
#define MASK_TX_DS 32
#define MASK_MAX_RT 16
#define EN_CRC 8
#define CRCO 4
#define PWR_UP 2
#define PRIM_RX 1

// Predefined Configs
#define TX_MODE (PWR_UP | CRCO | EN_CRC)
#define RX_MODE (PRIM_RX | PWR_UP | CRCO | EN_CRC)
#define TX_MODE_NO_CRC (PWR_UP)
#define RX_MODE_NO_CRC (PRIM_RX | PWR_UP)

void nrf24_init();
void nrf24_send(uint8_t *data, uint32_t len);
void nrf24_read(uint8_t *data, uint32_t len);

void set_mode(uint8_t mode);
uint8_t nrf24_read_register(uint8_t reg);
void nrf24_write_register(uint8_t reg, uint8_t value);
void nrf24_flush_tx();
void nrf24_flush_rx();
bool nrf24_data_available();
