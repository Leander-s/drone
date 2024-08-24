#include "gui_app.h"
#include "colors.h"

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
  result = SDL_CreateWindowAndRenderer("Drone Controller", width, height,
                                       SDL_WINDOW_RESIZABLE, &gui->window,
                                       &gui->renderer);
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
    gui->shouldQuit = 1;
    return;
  case SDL_EVENT_WINDOW_RESIZED:
    SDL_GetWindowSizeInPixels(gui->window, &gui->width, &gui->height);
    break;
  }
}

void gui_destroy(GUI *gui) {
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
  yOffset += padding;

  // maybe abstract this away into a pure function that takes the data and
  // returns the text pointer
  char transmissionsBuffer[64];
  sprintf(transmissionsBuffer, "Transmissions per second: %f",
          data->log->transmissionsPerSecond);
  const char *transmissionsText = transmissionsBuffer;
  text_draw(renderer, textX, yOffset, transmissionsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char picoTimeoutsBuffer[64];
  sprintf(picoTimeoutsBuffer, "Pico read timeouts: %d", data->log->picoReadTimeouts);
  const char* picoTimeoutsText = picoTimeoutsBuffer;
  text_draw(renderer, textX, yOffset, picoTimeoutsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbDisconnectsBuffer[64];
  sprintf(usbDisconnectsBuffer, "USB disconnects: %d", data->log->usbDisconnects);
  const char* usbDisconnectsText = usbDisconnectsBuffer;
  text_draw(renderer, textX, yOffset, usbDisconnectsText , 20, &LIGHT_GREY);
  yOffset += padding;

  char usbReadErrorsBuffer[64];
  sprintf(usbReadErrorsBuffer, "USB read errors: %d", data->log->usbReadErrors);
  const char* usbReadErrorsText = usbReadErrorsBuffer;
  text_draw(renderer, textX, yOffset, usbReadErrorsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbWriteErrorsBuffer[64];
  sprintf(usbWriteErrorsBuffer, "USB write errors: %d", data->log->usbWriteErrors);
  const char* usbWriteErrorsText = usbWriteErrorsBuffer;
  text_draw(renderer, textX, yOffset, usbWriteErrorsText, 20, &LIGHT_GREY);
  yOffset += padding;
}

void drone_model_draw(GUI *gui, const GUIData *data) {
  // need to make perspective matrix and run the points from data through it
  // the draw lines/ hollow simple drone model, start with cuboid
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
