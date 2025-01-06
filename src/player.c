#include "../include/game/player.h"
#include "../include/game/configuration.h"
#include "../include/game/timer.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

#include <stdlib.h>

void PlayerInitialize(Player* self) {
    self->x = 0;
    self->y = 0;
    self->direction = 0;
}

void PlayerUpdate(Player* self) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    Sint32 x = 0;

    if (keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S]) {
        self->x += PLAYER_MOVEMENT_SPEED * SDL_cosf(self->direction) *
                   TimerGetDeltaTime();
        self->y += PLAYER_MOVEMENT_SPEED * SDL_sinf(self->direction) *
                   TimerGetDeltaTime();
    } else if (keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_W]) {
        self->x -= PLAYER_MOVEMENT_SPEED * SDL_cosf(self->direction) *
                   TimerGetDeltaTime();
        self->y -= PLAYER_MOVEMENT_SPEED * SDL_sinf(self->direction) *
                   TimerGetDeltaTime();
    }
    if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) {
        self->x -= PLAYER_MOVEMENT_SPEED *
                   SDL_cosf(self->direction + (PIf / 180 * 90)) *
                   TimerGetDeltaTime();
        self->y -= PLAYER_MOVEMENT_SPEED *
                   SDL_sinf(self->direction + (PIf / 180 * 90)) *
                   TimerGetDeltaTime();
    } else if (keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]) {
        self->x += PLAYER_MOVEMENT_SPEED *
                   SDL_cosf(self->direction + (PIf / 180 * 90)) *
                   TimerGetDeltaTime();
        self->y += PLAYER_MOVEMENT_SPEED *
                   SDL_sinf(self->direction + (PIf / 180 * 90)) *
                   TimerGetDeltaTime();
    }

    SDL_GetRelativeMouseState(&x, NULL);
    self->direction += PLAYER_ROTATION_SPEED * (float)x;
}
