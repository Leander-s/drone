#include "SDL3/SDL_error.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "general_math.h"
#include <colors.h>
#include <gui_app.h>

GUI *gui_create(int width, int height) {
  int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
  if (result != 0) {
    SDL_Log("SDL_Init failed : %s\n", SDL_GetError());
    return NULL;
  }

  result = TTF_Init();
  if (result != 0) {
    printf("Error initializing ttf. Text cannot be rendered. Error : %s\n",
           TTF_GetError());
  }

  GUI *gui = malloc(sizeof(GUI));
  gui->width = width;
  gui->height = height;
  gui->shouldQuit = 0;
  printf("Creating model\n");
  gui->droneModel = drone_model_create();
  printf("Model created\n");
  result = SDL_CreateWindowAndRenderer("Drone Controller", width, height,
                                       SDL_WINDOW_RESIZABLE, &gui->window,
                                       &gui->renderer);
  printf("Window and renderer created\n");
  if (result != 0) {
    SDL_Log("Creating window and renderer failed : %s\n", SDL_GetError());
    return NULL;
  }

  return gui;
}

void gui_update(GUI *gui, const GUIData *data) {
  SDL_Renderer *renderer = gui->renderer;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  data_sheet_draw(gui, data);
  drone_model_draw(gui, data);

  SDL_RenderPresent(renderer);

  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_EVENT_QUIT:
    printf("Quitting\n");
    gui->shouldQuit = 1;
    return;
  case SDL_EVENT_WINDOW_RESIZED:
    SDL_GetWindowSizeInPixels(gui->window, &gui->width, &gui->height);
    break;
  }
}

void gui_destroy(GUI *gui) {
  drone_model_destroy(gui->droneModel);
  SDL_DestroyRenderer(gui->renderer);
  SDL_DestroyWindow(gui->window);
  SDL_Quit();
}

void data_sheet_draw(GUI *gui, const GUIData *data) {
  SDL_Renderer *renderer = gui->renderer;
  const int x = gui->width / 2;
  const int y = 0;
  const int width = gui->width - x;
  const int height = gui->height;

  const int padding = 20;
  const int textX = x + padding;
  int yOffset = padding;

  rect_fill(renderer, x, y, width, height, &DARK_GREY);
  rect_draw(renderer, x, y, width, height, &GREY);

  text_draw(renderer, textX, yOffset, "System logs", 30, &LIGHT_GREY);
  yOffset += 30;

  // maybe abstract this away into a pure function that takes the data and
  // returns the text pointer
  char transmissionsBuffer[64];
  sprintf(transmissionsBuffer, "Transmissions per second: %f",
          data->log->transmissionsPerSecond);
  const char *transmissionsText = transmissionsBuffer;
  text_draw(renderer, textX, yOffset, transmissionsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char picoTimeoutsBuffer[64];
  sprintf(picoTimeoutsBuffer, "Pico read timeouts: %d",
          data->log->picoReadTimeouts);
  const char *picoTimeoutsText = picoTimeoutsBuffer;
  text_draw(renderer, textX, yOffset, picoTimeoutsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbDisconnectsBuffer[64];
  sprintf(usbDisconnectsBuffer, "USB disconnects: %d",
          data->log->usbDisconnects);
  const char *usbDisconnectsText = usbDisconnectsBuffer;
  text_draw(renderer, textX, yOffset, usbDisconnectsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbReadErrorsBuffer[64];
  sprintf(usbReadErrorsBuffer, "USB read errors: %d", data->log->usbReadErrors);
  const char *usbReadErrorsText = usbReadErrorsBuffer;
  text_draw(renderer, textX, yOffset, usbReadErrorsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbWriteErrorsBuffer[64];
  sprintf(usbWriteErrorsBuffer, "USB write errors: %d",
          data->log->usbWriteErrors);
  const char *usbWriteErrorsText = usbWriteErrorsBuffer;
  text_draw(renderer, textX, yOffset, usbWriteErrorsText, 20, &LIGHT_GREY);
  yOffset += padding;
}

Model *drone_model_create() {
  Model *droneModel = malloc(sizeof(Model));
  droneModel->vertices[0] = (vec3){.x = 0.5f, .y = -0.1f, .z = -0.1f};
  droneModel->vertices[1] = (vec3){.x = 0.5f, .y = 0.1f, .z = -0.1f};
  droneModel->vertices[2] = (vec3){.x = 0.5f, .y = 0.1f, .z = 0.1f};
  droneModel->vertices[3] = (vec3){.x = 0.5f, .y = -0.1f, .z = 0.1f};
  droneModel->vertices[4] = (vec3){.x = -0.5f, .y = -0.1f, .z = -0.1f};
  droneModel->vertices[5] = (vec3){.x = -0.5f, .y = 0.1f, .z = -0.1f};
  droneModel->vertices[6] = (vec3){.x = -0.5f, .y = 0.1f, .z = 0.1f};
  droneModel->vertices[7] = (vec3){.x = -0.5f, .y = -0.1f, .z = 0.1f};

  /* Rect indices */
  droneModel->indices[0] = 0;
  droneModel->indices[1] = 1;
  droneModel->indices[2] = 2;
  droneModel->indices[3] = 3;

  droneModel->indices[4] = 4;
  droneModel->indices[5] = 5;
  droneModel->indices[6] = 6;
  droneModel->indices[7] = 7;

  droneModel->indices[8] = 4;
  droneModel->indices[9] = 0;
  droneModel->indices[10] = 3;
  droneModel->indices[11] = 7;

  droneModel->indices[12] = 5;
  droneModel->indices[13] = 1;
  droneModel->indices[14] = 2;
  droneModel->indices[15] = 6;

  droneModel->indices[16] = 4;
  droneModel->indices[17] = 0;
  droneModel->indices[18] = 1;
  droneModel->indices[19] = 5;

  droneModel->indices[20] = 7;
  droneModel->indices[21] = 3;
  droneModel->indices[22] = 2;
  droneModel->indices[23] = 6;

  /* Line indices
  droneModel->indices[0] = 0;
  droneModel->indices[1] = 1;
  droneModel->indices[2] = 1;
  droneModel->indices[3] = 2;
  droneModel->indices[4] = 2;
  droneModel->indices[5] = 3;
  droneModel->indices[6] = 3;
  droneModel->indices[7] = 0;
  droneModel->indices[8] = 0;
  droneModel->indices[9] = 4;
  droneModel->indices[10] = 4;
  droneModel->indices[11] = 5;
  droneModel->indices[12] = 5;
  droneModel->indices[13] = 6;
  droneModel->indices[14] = 6;
  droneModel->indices[15] = 7;
  droneModel->indices[16] = 7;
  droneModel->indices[17] = 4;
  droneModel->indices[18] = 5;
  droneModel->indices[19] = 1;
  droneModel->indices[20] = 6;
  droneModel->indices[21] = 2;
  droneModel->indices[22] = 7;
  droneModel->indices[23] = 3;
  droneModel->indices[24] = 8;
  droneModel->indices[25] = 9;
  droneModel->indices[26] = 9;
  droneModel->indices[27] = 10;
  droneModel->indices[28] = 10;
  droneModel->indices[29] = 11;
  droneModel->indices[30] = 11;
  droneModel->indices[31] = 8;
  droneModel->indices[32] = 8;
  droneModel->indices[33] = 12;
  droneModel->indices[34] = 12;
  droneModel->indices[35] = 13;
  droneModel->indices[36] = 13;
  droneModel->indices[37] = 14;
  droneModel->indices[38] = 14;
  droneModel->indices[39] = 15;
  droneModel->indices[40] = 15;
  droneModel->indices[41] = 12;
  droneModel->indices[42] = 13;
  droneModel->indices[43] = 9;
  droneModel->indices[44] = 14;
  droneModel->indices[45] = 10;
  droneModel->indices[46] = 15;
  droneModel->indices[47] = 11;
  */

  return droneModel;
}

void drone_model_destroy(Model *droneModel) { free(droneModel); }

void drone_model_draw(GUI *gui, const GUIData *data) {
  Model *droneModel = gui->droneModel;

  mat4 projection, viewPort;
  create_view_port(gui->width / 2.0f, gui->height, 100.0f, 0.1f, &viewPort);
  create_mvp((float)gui->width / 2.0f / gui->height, deg_to_rad(60), 100.0f,
             0.1f, &projection);

  vec3 rotatedPoints[8];
  vec2 screenPoints[8];

  Quaternion iq = (Quaternion){
      .w = data->sensorState->orientation.w,
      .v.x = -data->sensorState->orientation.v.x,
      .v.y = -data->sensorState->orientation.v.y,
      .v.z = -data->sensorState->orientation.v.z,
  };

  for (int i = 0; i < 8; i++) {
    rotate_point(&iq, &droneModel->vertices[i], &rotatedPoints[i]);
    translate_point(&projection, &viewPort, &rotatedPoints[i], 20.0f,
                    &screenPoints[i]);
  }


  SDL_Color colors[6];
  colors[0] = RED;
  colors[1] = BLUE;
  colors[2] = GREEN;
  colors[3] = LIGHT_GREY;
  colors[4] = WHITE;
  colors[5] = GREY;

  for (int i = 0; i < 24; i += 4) {
    SDL_Vertex verts[4];
    int indices[6];
    indices[0] = 2;
    indices[1] = 1;
    indices[2] = 0;
    indices[3] = 3;
    indices[4] = 2;
    indices[5] = 0;
    SDL_Color color = colors[i / 4];
    for (int j = 0; j < 4; j++) {
      verts[j] = (SDL_Vertex){
          .position =
              (SDL_FPoint){.x = screenPoints[droneModel->indices[i + j]].x,
                           .y = screenPoints[droneModel->indices[i + j]].y},
          .color = (SDL_FColor){.r = (float)color.r / 255,
                                .g = (float)color.g / 255,
                                .b = (float)color.b / 255,
                                .a = 1.0},
      };
    }
    if (!SDL_RenderGeometry(gui->renderer, NULL, verts, 4, indices, 6)) {
      printf("Error on rendering : %s\n", SDL_GetError());
    }
  }

  /*
  SDL_Vertex test[3];
  test[0] = (SDL_Vertex){
      .position = (SDL_FPoint){.x = 400, .y = 150},
      .color = (SDL_FColor){.r = 1.0, .g = 0.0, .b = 0.0, .a = 1.0},
  };
  test[1] = (SDL_Vertex){
      .position = (SDL_FPoint){.x = 200, .y = 450},
      .color = (SDL_FColor){.r = 0.0, .g = 1.0, .b = 0.0, .a = 1.0},
  };
  test[2] = (SDL_Vertex){
      .position = (SDL_FPoint){.x = 600, .y = 450},
      .color = (SDL_FColor){.r = 0.0, .g = 0.0, .b = 1.0, .a = 1.0},
  };
  if (!SDL_RenderGeometry(gui->renderer, NULL, test, 3, NULL, 0)) {
    printf("Error on test rendering : %s\n", SDL_GetError());
  }
  */

  /* Drawing lines
  for (int i = 0; i < 48; i += 2) {
    SDL_Color color = LIGHT_GREY;
    if (droneModel->indices[i] < 4 && droneModel->indices[i + 1] < 4) {
      color = RED;
    }
    int startIndex = droneModel->indices[i];
    int endIndex = droneModel->indices[i + 1];
    line_draw(gui->renderer, screenPoints[startIndex].x,
              screenPoints[startIndex].y, screenPoints[endIndex].x,
              screenPoints[endIndex].y, &color);
  }
  */
}

void line_draw(SDL_Renderer *renderer, int x, int y, int endX, int endY,
               const SDL_Color *color) {
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

  SDL_RenderLine(renderer, x, y, endX, endY);
}

void rect_draw(SDL_Renderer *renderer, int x, int y, int w, int h,
               const SDL_Color *color) {
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

  SDL_FRect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_RenderRect(renderer, &rect);
}

void rect_fill(SDL_Renderer *renderer, int x, int y, int w, int h,
               const SDL_Color *color) {
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

  SDL_FRect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_RenderFillRect(renderer, &rect);
}

void text_draw(SDL_Renderer *renderer, int x, int y, const char *text,
               int fontSize, const SDL_Color *color) {
  TTF_Font *font = TTF_OpenFont("../fonts/Montserrat-Regular.ttf", fontSize);
  if (font == NULL) {
    printf("Error opening font : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Surface *surface = TTF_RenderText_Solid(font, text, *color);

  SDL_Texture *textTex = SDL_CreateTextureFromSurface(renderer, surface);

  TTF_CloseFont(font);

  SDL_FRect textRect;
  textRect.x = x;
  textRect.y = y;
  textRect.w = surface->w;
  textRect.h = surface->h;

  SDL_RenderTexture(renderer, textTex, NULL, &textRect);
  SDL_DestroySurface(surface);
  SDL_DestroyTexture(textTex);
}
