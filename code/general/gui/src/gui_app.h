#include "config.h"

typedef struct {
} GUIData;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int width;
  int height;
  int shouldQuit;
} GUI;

GUI *gui_create(int width, int height);

void gui_update(GUI *gui, GUIData *data);

void gui_destroy(GUI *gui);
