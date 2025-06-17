// Enemy.h

#pragma once

#include "ship.h"
#include "raylib.h"

typedef enum {
    BASIC,
    ZIGZAG,
    BOOSTER,
    WALLER,
    BOSS
} EnemyName;

typedef struct Enemy {
    int type;
    bool active;
    Rectangle position;
    Vector2 speed;
    float move_time;
    bool action_flag;
    Rectangle source;
    Color color;
    float hp;
    float exp;
} Enemy;

void InitEnemies(Enemy* enemy);
void UpdateEnemies(Enemy* enemy, Ship* ship);
void DrawEnemies(Enemy* enemy);

void SpawnEnemies(Enemy* enemy, int amount, int type, int hp);
void SpawnRandomEnemies(Enemy* enemy, int amount, int hp);

void LoadEnemyTextures(void);
void UnloadEnemyTextures(void);