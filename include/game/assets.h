#ifndef GAME_ASSETS_H
#define GAME_ASSETS_H

#include <SDL2/SDL_render.h>

void AssetsInitialize(void);

SDL_Texture* const* AssetsGetWallTextures(void);

void AssetsDestroy(void);

#endif
