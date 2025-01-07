#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "player.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

void RendererInitialize(SDL_Window* window);

void RendererDestroy(void);

SDL_Renderer* RendererGet(void);

void RendererRenderSpatialSpace(const Player* player);

#endif
