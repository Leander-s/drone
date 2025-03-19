#pragma once
#include <gui_config.h>

typedef struct {
  PCSystemLog *log;
  DroneSensorState *sensorState;
  DroneControlState *controlState;
} GUIData;

typedef struct {
  // SDL
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_GLContext glContext;

  // OpenGL
  GLuint shaderProgram_3D, shaderProgram_HUD_rects, shaderProgram_HUD_lines, shaderProgram_text;
  GLuint VBO_3D, VAO_3D, IBO_3D;
  GLuint VBO_HUD_rects, VAO_HUD_rects, IBO_HUD_rects;
  GLuint VBO_HUD_lines, VAO_HUD_lines, IBO_HUD_lines;
  GLuint VBO_text, VAO_text, IBO_text;

  // logic
  int width;
  int height;
  int shouldQuit;
  Model *droneModel;
  Model **hudModels;
  int updateCounter;
  uint8_t keyState[322];
} GUI;

GUI *gui_create(int width, int height);
void gui_update(GUI *gui, const GUIData *data);
void gui_destroy(GUI *gui);

Model **hud_models_create();
Model *drone_model_create();

void model_destroy(Model *model);

void data_sheet_draw(GUI *gui, const GUIData *data);
void hud_draw(GUI *gui, const GUIData *data);
void drone_model_draw(GUI *gui, const GUIData *data);

void line_draw(SDL_Renderer *renderer, int x, int y, int endX, int endY,
               const SDL_Color *color);
void rect_draw(SDL_Renderer *renderer, int x, int y, int w, int h,
               const SDL_Color *color);
void rect_fill(SDL_Renderer *renderer, int x, int y, int w, int h,
               const SDL_Color *color);
void text_draw(GUI *gui, int x, int y, const char *text,
               int fontSize, const SDL_Color *color);
