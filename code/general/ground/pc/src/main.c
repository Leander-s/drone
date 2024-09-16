#include <config.h>

int main() {
  DroneSensorState sensorData;
  DroneControlState controlState;

  // Setting up transceiver
  GroundTransceiverCreateInfo createInfo;
  // Not sure how to go about this yet
#ifdef _WIN32
  createInfo.path_to_port = "COM4";
#else
  createInfo.path_to_port = "/dev/ttyACM0";
#endif
  // Buffer size between pico transceiver and pc. Buffer size for radio transmissions
  // is hardcoded and also capped at 32 atm
  createInfo.bufferSize = 64;
  // States need to be here in the main program, they will be what we control/
  // read throughout
  createInfo.controlState = &controlState;
  createInfo.sensorState = &sensorData;

  // might want to do this in a seperate thread in the future
  GroundTransceiver *link = ground_transceiver_create(&createInfo);

  GUIData guiData;
  guiData.log = &link->log;
  guiData.sensorState = &sensorData;

  GUI *gui = gui_create(800, 600);

  while (!gui->shouldQuit) {
    // sending to drone/receiving from drone
    ground_transceiver_update(link);

    // updating gui
    gui_update(gui, &guiData);
  }

  ground_transceiver_destroy(link);
  gui_destroy(gui);
}
