#include "../include/game/configuration.h"
#include "../include/game/player.h"
#include "../include/game/timer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

#include <stdlib.h>

Sint32 main(void) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    Player player;

    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_CreateWindowAndRenderer(
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }

    PlayerInitialize(&player);

    while (SDL_TRUE) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
        }

        PlayerUpdate(&player);
        SDL_Log("%f, %f\n", (double)player.x, (double)player.y);

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        TimerUpdate();
    }
}
