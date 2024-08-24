#include "config.h"
#include "gui_app.h"

int main() {
  DroneSensorState sensorData;
  DroneControlState controlState;

  // Setting up transceiver
  GroundTransceiverCreateInfo createInfo;
  // Not sure how to go about this yet
  createInfo.path_to_port = "/dev/ttyACM0";
  // This is the max buffer size for my modules. Should get bigger as we upgrade
  createInfo.bufferSize = 64;
  // States need to be here in the main program, they will be what we control/
  // read throughout
  createInfo.controlState = &controlState;
  createInfo.sensorState = &sensorData;

  // might want to do this in a seperate thread in the future
  GroundTransceiver *link = ground_transceiver_create(&createInfo);

  GUIData guiData;
  guiData.log = &link->log;

  GUI *gui = gui_create(800, 600);

  while (!gui->shouldQuit) {
    // sending to drone/receiving from drone
    ground_transceiver_update(link);

    gui_update(gui, &guiData);
  }

  ground_transceiver_destroy(link);
  gui_destroy(gui);
}
