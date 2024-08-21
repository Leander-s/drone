#include "protocol.h"
#include <nrf24.h>

int main(){
    DroneTransceiverCreateInfo createInfo;
    createInfo.init = &nrf24_init;
    createInfo.send = &nrf24_send;
    createInfo.recv = &nrf24_read;
    createInfo.bufferSize = 32;

    DroneTransceiver* drone = drone_protocol_init(&createInfo);
    drone_protocol_run(drone);

    return 0;
}
