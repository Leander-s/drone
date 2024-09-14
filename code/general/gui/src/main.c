#include <diagnostics.h>
#include <gui_app.h>

int main() {
  GUI *main_gui = gui_create(800, 600);
  PCSystemLog *dummyLog = &(PCSystemLog){.transmissionsPerSecond = 0,
                                         .usbReadErrors = 0,
                                         .usbWriteErrors = 0,
                                         .usbDisconnects = 0};
  GUIData dummyData = (GUIData){.log = dummyLog, .droneModel = drone_model_create()};
  while (!main_gui->shouldQuit) {
    gui_update(main_gui, &dummyData);
  }
  drone_model_destroy(dummyData.droneModel);
  gui_destroy(main_gui);
  return 0;
}
