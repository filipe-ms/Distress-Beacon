#pragma once

#include "raylib.h"
#include "ship.h"
#include "list.h"

typedef enum {
    ENEMY_BASIC,
    ENEMY_ZIGZAG,
    ENEMY_BOOSTER,
    ENEMY_WALLER,
    ENEMY_SPINNER,
    ENEMY_REVERSE_SPINNER,
    ENEMY_BOSS,
    ENEMY_TYPE_COUNT
} EnemyType;

typedef enum {
    ENEMY_STATE_SPAWNING,
    ENEMY_STATE_SPINNER_TURNING,
    ENEMY_STATE_SPINNER_ACTING,
} EnemyState;

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
    float rotation;

    // Behavior structs
    EnemyState state;
    float elapsed_time;

    // More variables for custom movements
    Vector2 vector2_aux1;
    Vector2 vector2_aux2;
    float float_aux1;
} Enemy;

extern List* enemies;

void ActivateEnemy(Enemy* enemy, Vector2 position, EnemyType type, int hp);
void SpawnEnemies(List* enemy_list);

void InitEnemies(void);
void UpdateEnemies(Ship* ship);
void DrawEnemies(void);
void LoadEnemyTextures(void);
void UnloadEnemyTextures(void);