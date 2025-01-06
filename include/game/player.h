#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

typedef struct Player Player;

struct Player {
    float x;
    float y;
    float angle;
};

void PlayerInitialize(Player* self);

void PlayerUpdate(Player* self);

#endif
