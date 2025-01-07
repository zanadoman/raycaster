#include "../include/game/timer.h"
#include "../include/game/configuration.h"

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

static Uint64 LAST_TIME;
static float DELTA_TIME;

float TimerGetDeltaTime(void) {
    return DELTA_TIME;
}

void TimerUpdate(void) {
    const Uint64 end = LAST_TIME + TIMER_MINIMUM_FRAME_TIME;
    Uint64 now = SDL_GetTicks64();

    if (now < end) {
        SDL_Delay((Uint32)(end - now));
        now = end;
    }

    DELTA_TIME = (float)(now - LAST_TIME);
    LAST_TIME = now;
}
