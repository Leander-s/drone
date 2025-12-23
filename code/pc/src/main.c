#include <protocol.h>
#include <gui_app.h>
#include <connection.h>
#include <encoding.h>

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
  GroundTransceiver *link = ground_transceiver_create(&createInfo);

  GUIData guiData;
  guiData.log = &link->log;
  guiData.sensorState = &sensorData;
  guiData.controlState = &controlState;

  GUI *gui = gui_create(800, 600);

  while (!gui->shouldQuit) {
    // sending to drone/receiving from drone
    ground_transceiver_update(link);

    /*
     * This is printing the decoded sendbuffer so hopefully what is received
     * by the drone
    uint8_t sendBufferCopy[64];
    memcpy(&sendBufferCopy, link->sendBuffer, 64);
    decode_buffer(sendBufferCopy, 64);
    printf("[");
    for(int i = 0; i < 32; i++){
        printf("%u, ", sendBufferCopy[i]); 
    }
    printf("]\n");
    */

    // updating gui
    gui_update(gui, &guiData);
  }

  ground_transceiver_destroy(link);
  gui_destroy(gui);
}
