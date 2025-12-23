#pragma once
// sdl
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

// GL
#include <glad/glad.h>

// standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// mine
#include <diagnostics.h>
#include <drone_states.h>
#include <gui_math.h>

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
  GLuint shaderProgram_3D, shaderProgram_HUD_rects, shaderProgram_HUD_lines,
      shaderProgram_text, shaderProgram_HUD_throttle, shaderProgram_HUD_roll;
  GLuint VAO_3D, VBO_3D, IBO_3D;
  GLuint VAO_HUD_roll, VBO_HUD_roll, IBO_HUD_roll;
  GLuint VAO_HUD_throttle, VBO_HUD_throttle, IBO_HUD_throttle;
  GLuint VAO_HUD_py_indicator, VBO_HUD_py_indicator, IBO_HUD_py_indicator;
  GLuint VAO_HUD_lines, VBO_HUD_lines, IBO_HUD_lines;
  GLuint VAO_text, VBO_text, IBO_text;

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
void text_draw(GUI *gui, int x, int y, const char *text, int fontSize,
               const SDL_Color *color);
