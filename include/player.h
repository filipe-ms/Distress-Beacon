// player.h

#pragma once

#include "raylib.h"

typedef struct Player {
    int score;
    int enemies_killed;
    int level;
    int experience;
} Player;

void InitPlayer(Player* player);
void AddExperience(Player* player, int exp);
void AddScore(Player* player, int score);
void AddEnemyKill(Player* player);