#include "config.h"

int main() {

  // Setting up transceiver
  GroundTransceiverCreateInfo createInfo;
  // Not sure how to go about this yet
  createInfo.path_to_port = "/dev/ttyACM0";
  // This is the max buffer size for my modules. Should get bigger as we upgrade
  createInfo.bufferSize = 32;
  // States need to be here in the main program, they will be what we control/
  // read throughout
  createInfo.controlState = (DroneControlState*){};
  createInfo.sensorState = (DroneSensorState*){};

  // might want to do this in a seperate thread in the future
  GroundTransceiver *link = ground_transceiver_create(&createInfo);

  while(1){
      // sending to drone/receiving from drone
      ground_transceiver_update(link);
  }
}
