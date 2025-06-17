// player.h

#pragma once

#include "raylib.h"
#include "ship.h"


typedef struct Player {
    int enemies_killed;
    int score;
    int level;
    int experience;
} Player;

void InitPlayer(Player* player);
