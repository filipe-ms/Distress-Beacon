#pragma once

#include "raylib.h"
#include "ship.h"
#include "list.h"

typedef enum {
    ENEMY_BASIC,
    ENEMY_ZIGZAG,
    ENEMY_BOOSTER,
    ENEMY_WALLER,
    ENEMY_BOSS,
    ENEMY_TYPE_COUNT
} EnemyType;

typedef struct {
    bool is_on_screen;
    Rectangle position;
    Vector2 speed;
    EnemyType type;
    float hp;
    float exp;
    float move_time;
    bool action_flag;
    Color color;
} Enemy;

extern List* enemies;

void InitEnemies(void);
void UpdateEnemies(Ship* ship);
void DrawEnemies(void);
void SpawnEnemies(int amount, EnemyType type, int hp);
void SpawnRandomEnemies(int amount, int hp);
void LoadEnemyTextures(void);
void UnloadEnemyTextures(void);