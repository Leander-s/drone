#pragma once
#include <gui_config.h>

typedef struct {
  PCSystemLog *log;
  DroneSensorState *sensorState;
  DroneControlState *controlState;
} GUIData;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int width;
  int height;
  int shouldQuit;
  Model *droneModel;
  int updateCounter;
} GUI;

GUI *gui_create(int width, int height);
void gui_update(GUI *gui, const GUIData *data);
void gui_destroy(GUI *gui);

Model *drone_model_create();
void drone_model_destroy(Model *droneModel);

void data_sheet_draw(GUI *gui, const GUIData *data);
void hud_draw(GUI *gui, const GUIData *data);
void drone_model_draw(GUI *gui, const GUIData *data);

void line_draw(SDL_Renderer *renderer, int x, int y, int endX, int endY,
               const SDL_Color *color);
void rect_draw(SDL_Renderer *renderer, int x, int y, int w, int h,
               const SDL_Color *color);
void rect_fill(SDL_Renderer *renderer, int x, int y, int w, int h,
               const SDL_Color *color);
void text_draw(SDL_Renderer *renderer, int x, int y, const char *text,
               int fontSize, const SDL_Color *color);
