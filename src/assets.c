#include "../include/game/assets.h"
#include "../include/game/configuration.h"
#include "../include/game/renderer.h"

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>

#include <stdlib.h>

static SDL_Texture* WALL_TEXTURES[ASSETS_WALL_TEXTURES_COUNT];

SDL_Texture* LoadTexture(const char* path);

void AssetsInitialize(void) {
    WALL_TEXTURES[0] = LoadTexture("./assets/walls/0.png");
    WALL_TEXTURES[1] = LoadTexture("./assets/walls/1.png");
    WALL_TEXTURES[2] = LoadTexture("./assets/walls/2.png");
}

SDL_Texture* const* AssetsGetWallTextures(void) {
    return WALL_TEXTURES;
}

void AssetsDestroy(void) {
    SDL_DestroyTexture(WALL_TEXTURES[0]);
    SDL_DestroyTexture(WALL_TEXTURES[1]);
    SDL_DestroyTexture(WALL_TEXTURES[2]);
}

SDL_Texture* LoadTexture(const char* path) {
    SDL_Texture* texture;

    texture = IMG_LoadTexture(RendererGetContext(), path);
    if (!texture) {
        SDL_Log("%s\n", SDL_GetError());
        exit(1);
    }

    return texture;
}
