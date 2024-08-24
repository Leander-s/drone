#include "gui_app.h"

int main() {
  GUI *main_gui = gui_create(800, 600);
  while (!main_gui->shouldQuit) {
    gui_update(main_gui, NULL);
  }
  gui_destroy(main_gui);
  return 0;
}
