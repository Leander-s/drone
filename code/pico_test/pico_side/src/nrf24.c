#include "nrf24.h"

void nrf24_init(){
    spi_init(spi0, 1000000);

    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    gpio_init(PIN_CE);
    gpio_set_dir(PIN_CE, GPIO_OUT);
    gpio_put(PIN_CE, 0);
}

uint8_t nrf24_read_register(uint8_t reg){
    uint8_t result;
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &reg, 1);
    spi_read_blocking(SPI_PORT, 0, &result, 1);
    gpio_put(PIN_CS, 1);
    return result;
}

void nrf24_write_register(uint8_t reg, uint8_t value){
    reg |= W_REGISTER;
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &reg, 1);
    spi_write_blocking(SPI_PORT, &value, 1);
    gpio_put(PIN_CS, 1);
}

void nrf24_send(uint8_t *data, int len){
    gpio_put(PIN_CE, 0);

    uint8_t cmd = FLUSH_TX;
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS, 1);

    cmd = W_TX_PAYLOAD;
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS, 1);

    cmd = W_TX_PAYLOAD;
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    spi_write_blocking(SPI_PORT, data, len);
    gpio_put(PIN_CS, 1);

    gpio_put(PIN_CS, 1);
    sleep_ms(10);
    gpio_put(PIN_CE, 0);
}
