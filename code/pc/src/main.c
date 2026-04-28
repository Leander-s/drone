#include <connection.h>
#include <encoding.h>
#include <gui_app.h>
#include <protocol.h>

int main() {
  DroneSensorState sensorData;
  sensorData.orientation = (Quaternion){.x = 1, .i = 0, .j = 0, .k = 0};
  DroneControlState controlState;

  // Setting up transceiver
  GroundTransceiverCreateInfo createInfo; // Not sure how to go about this yet
#ifdef _WIN32
  createInfo.path_to_port = find_device_path("Pico");
#else
  createInfo.path_to_port = find_device_path("Pico");
#endif
  // Buffer size between pico transceiver and pc. Buffer size for radio
  // transmissions is hardcoded and also capped at 32 atm
  createInfo.bufferSize = 64;
  // States need to be here in the main program, they will be what we control/
  // read throughout
  createInfo.controlState = &controlState;
  createInfo.sensorState = &sensorData;

  // might want to do this in a seperate thread in the future
  GroundTransceiver link;
  ground_transceiver_init(&link, &createInfo);

  GUIData guiData;
  guiData.log = &link.log;
  guiData.sensorState = &sensorData;
  guiData.controlState = &controlState;

  GUI gui;
  int result = gui_init(&gui, 800, 600);
  if (result != 0) {
    return 1;
  }

  while (!gui.shouldQuit) {
    // sending to drone/receiving from drone
    ground_transceiver_update(&link);

    // updating gui
    gui_update(&gui, &guiData);
  }

  ground_transceiver_destroy(&link);
  gui_destroy(&gui);
  return 0;
}
