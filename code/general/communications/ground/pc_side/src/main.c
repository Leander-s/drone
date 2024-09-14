#include <protocol.h>

// This file is used to test the ground program
// in the actual ground program, we will figure out a way to either automate
// setting the port, or get one from user input

int main(){
    GroundTransceiverCreateInfo createInfo;
    createInfo.bufferSize = 32;

// put your usb path/port here
#ifdef _WIN32
    // For windows
    createInfo.path_to_port = "COM3";
#else
    // For linux
    createInfo.path_to_port = "/dev/ttyACM0";
#endif

    createInfo.controlState = (DroneControlState*){};
    createInfo.sensorState = (DroneSensorState*){};
    GroundTransceiver *tx = ground_transceiver_create(&createInfo);
    ground_transceiver_run(tx);
    return 0;
}
