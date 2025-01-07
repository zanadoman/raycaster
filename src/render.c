#include "../include/game/render.h"
#include "../include/game/configuration.h"
#include "../include/game/player.h"

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

#include <float.h>
#include <stdlib.h>

#define PLAYER_HALF_FIELD_OF_VIEW (PLAYER_FIELD_OF_VIEW / 2)
#define ANGLE_STEP (1.0F / WINDOW_WIDTH * PLAYER_FIELD_OF_VIEW)
#define WINDOW_RATIO ((float)WINDOW_WIDTH / WINDOW_HEIGHT)

static Uint8 MAP[9][9] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 1, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1}};

void RenderSpatialSpace(SDL_Renderer* renderer, const Player* player) {
    const float initialAngle = player->angle - PLAYER_HALF_FIELD_OF_VIEW;

    Sint32 column;
    SDL_Rect rectangle;

    rectangle.w = 1;

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
            if (MAP[rayX][rayY]) {
                break;
            }
        }

        rayLength = side ? sideX - deltaX : sideY - deltaY;
        rayLength *= SDL_cosf(rayAngle - player->angle);
        rectangle.x = column;
        rectangle.h = (Sint32)(WINDOW_HEIGHT * WINDOW_RATIO / rayLength);
        rectangle.y = (WINDOW_HEIGHT - rectangle.h) / 2;
        if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)) {
            SDL_Log("%s\n", SDL_GetError());
            exit(1);
        }
        if (SDL_RenderFillRect(renderer, &rectangle)) {
            SDL_Log("%s\n", SDL_GetError());
            exit(1);
        }
    }
}
