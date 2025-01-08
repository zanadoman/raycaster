#include "../include/game/window.h"
#include "../include/game/configuration.h"

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_video.h>

#include <stdlib.h>

static SDL_Window* WINDOW;

void WindowInitialize(void) {
    WINDOW = SDL_CreateWindow(
        "Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
        WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE);
    if (!WINDOW) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
}

SDL_Window* WindowGetContext(void) {
    return WINDOW;
}

void WindowDestroy(void) {
    SDL_DestroyWindow(WINDOW);
}
