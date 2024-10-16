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
  DroneSensorState sensorState = (DroneSensorState){.orientation = (Quaternion){.w = 0.2, .v = (vec3){.x = 0.3, .y = 0.2, .z = 0.1}}};
  dummyData.sensorState = &sensorState;
  printf("Dummy data created\n");
  while (!main_gui->shouldQuit) {
    gui_update(main_gui, &dummyData);
  }
  gui_destroy(main_gui);
  return 0;
}
