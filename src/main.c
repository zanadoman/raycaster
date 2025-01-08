#include "../include/game/assets.h"
#include "../include/game/player.h"
#include "../include/game/renderer.h"
#include "../include/game/timer.h"
#include "../include/game/window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>

#include <stdlib.h>

SDL_bool IsRunning(void);

SDL_bool IsRunning(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return SDL_FALSE;
        }
    }
    return SDL_TRUE;
}

Sint32 main([[maybe_unused]] Sint32 argc, [[maybe_unused]] char* argv[]) {
    Player player;

    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }

    WindowInitialize();
    RendererInitialize();
    AssetsInitialize();
    TimerInitialize();
    PlayerInitialize(&player);

    while (IsRunning()) {
        PlayerUpdate(&player);
        SDL_Log("%f, %f\n", (double)player.x, (double)player.y);

        RendererRenderSpatialSpace(&player);
        RendererPresentFrame();
        TimerUpdate();
    }

    AssetsDestroy();
    RendererDestroy();
    WindowDestroy();
    SDL_Quit();

    return 0;
}
