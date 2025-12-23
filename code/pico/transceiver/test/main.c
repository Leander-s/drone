#include <pico_transceiver.h>

int main(){
    PicoTransceiver *trans = pico_transceiver_create();
    while(1){
        pico_transceiver_update(trans);
    }
    pico_transceiver_destroy(trans);
}
