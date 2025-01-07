#ifndef GAME_RENDER_H
#define GAME_RENDER_H

#include "player.h"

#include <SDL2/SDL_render.h>

void RenderSpatialSpace(SDL_Renderer* renderer, const Player* player);

#endif
