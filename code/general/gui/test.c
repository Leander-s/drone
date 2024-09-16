#include "SDL3/SDL.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_stdinc.h>
#include <unistd.h>

int main(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0){
        SDL_Log("Well it already died : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Log("Display info:\n");
    int count_displays;
    SDL_DisplayID *displays = SDL_GetDisplays(&count_displays);

    for(int i = 0; i < count_displays; ++i){
        const SDL_DisplayMode *displayMode = SDL_GetCurrentDisplayMode(displays[i]);
        if(displayMode==NULL){
            SDL_Log("No display mode for %d : %s\n", i, SDL_GetError());
        }else{
            SDL_Log("%d Seems fine\n", i);
        }
    }
    SDL_free(displays);

    SDL_Window *window;
    SDL_Renderer *renderer;

    if(!SDL_CreateWindowAndRenderer("Test", 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer)){
        SDL_Log("Error : %s\n", SDL_GetError());
        return 1;
    }

    while(1){
        sleep(1);
    }
    return 0;
}
