#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "player.h"

#include <SDL2/SDL_render.h>

void RendererInitialize(void);

void RendererDestroy(void);

SDL_Renderer* RendererGetContext(void);

void RendererRenderSpatialSpace(const Player* player);

void RendererPresentFrame(void);

#endif
