#include "gui_app.h"
#include "SDL3_ttf/SDL_ttf.h"

GUI *gui_create(int width, int height) {
  int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
  if (result != 0) {
    SDL_Log("SDL_Init failed : %s\n", SDL_GetError());
    return NULL;
  }

  result = TTF_Init();
  if(result != 0){
      printf("Error initializing ttf. Text cannot be rendered. Error : %s\n", TTF_GetError());
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

void gui_update(GUI *gui, GUIData *data) {
  SDL_Renderer *renderer = gui->renderer;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  /*SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);

  SDL_FRect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = gui->width / 2;
  rect.h = gui->height;
  SDL_RenderRect(renderer, &rect);
  */

  // figure out how to render text
  TTF_Font* font = TTF_OpenFont("../fonts/Montserrat-Regular.ttf", 24);
  if(font == NULL){
      printf("Error opening font : %s\n", TTF_GetError());
      exit(EXIT_FAILURE);
  }

  SDL_Color White = {255, 255, 255};

  SDL_Surface* surface = TTF_RenderText_Solid(font, "Test text", White);

  SDL_Texture* testText = SDL_CreateTextureFromSurface(renderer, surface);

  TTF_CloseFont(font);

  SDL_FRect textRect;
  textRect.x = 100;
  textRect.y = 100;
  textRect.w = 100;
  textRect.h = 24;

  SDL_RenderTexture(renderer, testText, NULL, &textRect);

  
  SDL_RenderPresent(renderer);

  SDL_DestroySurface(surface);
  SDL_DestroyTexture(testText);

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

void data_sheet_draw(SDL_Renderer *renderer, GUIData* data){
    // need to draw text
}

void drone_model_draw(SDL_Renderer *renderer, GUIData* data){
    // need to make perspective matrix and run the points from data through it
    // the draw lines/ hollow simple drone model, start with cuboid
}
