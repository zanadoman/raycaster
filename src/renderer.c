#include "../include/game/renderer.h"
#include "../include/game/assets.h"
#include "../include/game/configuration.h"
#include "../include/game/player.h"
#include "../include/game/window.h"

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

#include <float.h>
#include <stdlib.h>

#define PLAYER_HALF_FIELD_OF_VIEW (PLAYER_FIELD_OF_VIEW / 2)
#define ANGLE_STEP (1.0F / WINDOW_WIDTH * PLAYER_FIELD_OF_VIEW)
#define WINDOW_RATIO ((float)WINDOW_WIDTH / WINDOW_HEIGHT)

static Sint8 MAP[9][9] = {
    {1, 0, 1, 2, 1, 0, 2, 1, 1},        {1, -1, -1, -1, -1, -1, -1, -1, 2},
    {0, -1, -1, -1, -1, -1, -1, -1, 0}, {2, -1, -1, -1, -1, -1, -1, -1, 1},
    {1, -1, -1, -1, -1, -1, -1, -1, 2}, {0, -1, -1, -1, -1, 1, 2, -1, 0},
    {1, -1, -1, -1, -1, 0, 1, -1, 1},   {0, -1, -1, -1, -1, -1, -1, -1, 0},
    {1, 1, 2, 1, 2, 1, 0, 1, 0}};

static SDL_Renderer* RENDERER;
static SDL_Texture* SPATIAL_TARGET;

void RendererInitialize(void) {
    RENDERER = SDL_CreateRenderer(
        WindowGet(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!RENDERER) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_RenderSetLogicalSize(RENDERER, WINDOW_WIDTH, WINDOW_HEIGHT)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    SPATIAL_TARGET = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH,
                                       WINDOW_HEIGHT);
    if (!SPATIAL_TARGET) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
}

void RendererDestroy(void) {
    SDL_DestroyTexture(SPATIAL_TARGET);
    SDL_DestroyRenderer(RENDERER);
}

SDL_Renderer* RendererGet(void) {
    return RENDERER;
}

void RendererRenderSpatialSpace(const Player* player) {
    const float initialAngle = player->angle - PLAYER_HALF_FIELD_OF_VIEW;

    Sint32 column;
    SDL_Rect source;
    SDL_Rect destination;

    if (SDL_SetRenderTarget(RENDERER, SPATIAL_TARGET)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_RenderClear(RENDERER)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }

    source.y = 0;
    source.w = 1;
    destination.w = 1;

    for (column = 0; column != WINDOW_WIDTH; ++column) {
        const float rayAngle = initialAngle + (ANGLE_STEP * (float)column);
        const float rayDirectionX = SDL_cosf(rayAngle);
        const float rayDirectionY = SDL_sinf(rayAngle);
        const float deltaX =
            rayDirectionX == 0.0F ? FLT_MAX : SDL_fabsf(1 / rayDirectionX);
        const float deltaY =
            rayDirectionY == 0.0F ? FLT_MAX : SDL_fabsf(1 / rayDirectionY);

        Sint32 rayX = (Sint32)player->x;
        Sint32 rayY = (Sint32)player->y;
        float sideX;
        float sideY;
        Sint8 stepX;
        Sint8 stepY;
        SDL_bool side;
        float rayLength;
        SDL_Texture* texture;
        Sint32 textureWidth;
        float textureX;

        if (rayDirectionX < 0) {
            stepX = -1;
            sideX = (player->x - (float)rayX) * deltaX;
        } else {
            stepX = 1;
            sideX = ((float)rayX + 1 - player->x) * deltaX;
        }
        if (rayDirectionY < 0) {
            stepY = -1;
            sideY = (player->y - (float)rayY) * deltaY;
        } else {
            stepY = 1;
            sideY = ((float)rayY + 1 - player->y) * deltaY;
        }

        while (SDL_TRUE) {
            side = sideX < sideY;
            if (side) {
                sideX += deltaX;
                rayX += stepX;
            } else {
                sideY += deltaY;
                rayY += stepY;
            }
            if (0 <= MAP[rayX][rayY]) {
                break;
            }
        }

        rayLength = side ? sideX - deltaX : sideY - deltaY;
        texture = AssetsGetWallTextures()[(Uint8)MAP[rayX][rayY] %
                                          ASSETS_WALL_TEXTURES_COUNT];

        if (SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &source.h)) {
            SDL_Log("%s\n", SDL_GetError());
            exit(1);
        }
        textureX = side ? player->y + (rayDirectionY * rayLength)
                        : player->x + (rayDirectionX * rayLength);
        textureX -= (float)(Sint32)textureX;
        source.x = (Sint32)(textureX * (float)textureWidth);
        if ((side && rayDirectionX < 0) || (!side && 0 < rayDirectionY)) {
            source.x = textureWidth - 1 - source.x;
        }

        destination.x = column;
        destination.h =
            (Sint32)(WINDOW_HEIGHT * WINDOW_RATIO /
                     (rayLength * SDL_cosf(rayAngle - player->angle)));
        destination.y = (WINDOW_HEIGHT - destination.h) / 2;

        if (SDL_RenderCopy(RENDERER, texture, &source, &destination)) {
            SDL_Log("%s\n", SDL_GetError());
            exit(1);
        }
    }
}

void RendererPresentFrame(void) {
    const SDL_Rect spatialFrame = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    if (SDL_SetRenderTarget(RENDERER, NULL)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_RenderClear(RENDERER)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_RenderCopyEx(RENDERER, SPATIAL_TARGET, NULL, &spatialFrame, 0, NULL,
                         SDL_FLIP_NONE)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    SDL_RenderPresent(RENDERER);
}
