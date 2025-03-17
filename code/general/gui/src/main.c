#include <diagnostics.h>
#include <gui_app.h>

int main() {
  GUI *main_gui = gui_create(800, 600);
  printf("gui created\n");
  PCSystemLog *dummyLog = &(PCSystemLog){.transmissionsPerSecond = 0,
                                         .usbReadErrors = 0,
                                         .usbWriteErrors = 0,
                                         .usbDisconnects = 0};
  GUIData dummyData;
  dummyData.log = dummyLog;
  DroneSensorState sensorState = (DroneSensorState){.orientation = (Quaternion){.w = 0.0, .v = (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f}}};
  dummyData.sensorState = &sensorState;
  DroneControlState controlState = (DroneControlState){.throttle = 0, .pitch = 0, .yaw = 0, .roll = 0};
  dummyData.controlState = &controlState;
  printf("Dummy data created\n");
  while (!main_gui->shouldQuit) {
    gui_update(main_gui, &dummyData);
  }
  gui_destroy(main_gui);
  return 0;
}
