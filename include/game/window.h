#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <SDL2/SDL_video.h>

void WindowInitialize(void);

SDL_Window* WindowGet(void);

void WindowDestroy(void);

#endif
