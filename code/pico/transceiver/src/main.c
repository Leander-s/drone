#include <pico_transceiver.h>

int main(){
    PicoTransceiver *transceiver = pico_transceiver_create();
    
    while(1){
        pico_transceiver_update(transceiver);
    }
    pico_transceiver_destroy(transceiver);
    return 0;
}
