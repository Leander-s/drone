#include "nrf24.h"

void setup(){
    stdio_init_all();
    nrf24_init();
    nrf24_setup();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 0);
}

void loop(){
    while(1){
        uint8_t data[128];
        memset(data, '\0', 128);
        nrf24_read(data, 128);
        if(data[0] != '\0'){
            gpio_put(25, 1);
            pico_print("Received data");
        }else{
            gpio_put(25, 0);
        }
        nrf24_flush_rx();
    }
}

int main(){
    setup();
    loop();
    return 0;
}
