#include "protocol.h"

int main(){
    GroundTransceiverCreateInfo createInfo;
    createInfo.bufferSize = 32;
    createInfo.path_to_port = "/dev/ttyACM0";
    createInfo.controlState = (DroneControlState*){};
    createInfo.sensorState = (DroneSensorState*){};
    GroundTransceiver *tx = ground_transceiver_create(&createInfo);
    ground_transceiver_run(tx);
    return 0;
}
