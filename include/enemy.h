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
	ENEMY_STALKER,
    ENEMY_GHOST,
    ENEMY_BOSS,
    ENEMY_TYPE_COUNT
} EnemyType;

typedef enum {
    ENEMY_STATE_SPAWNING,
    ENEMY_STATE_SPINNER_TURNING,
    ENEMY_STATE_SPINNER_ACTING,

    GHOST_IDLE,
    GHOST_VISIBLE,
    GHOST_INVISIBLE,
    GHOST_EMP
} EnemyState;

typedef struct {
    EnemyType type;

    Vector2 position;
    Vector2 size;
    Vector2 speed;
    float rotation;
	bool is_on_screen;

    float hp;
    float exp;
    float score;

    float timer;
    float elapsed_time;

    bool should_perform_action;
    Color color;
    
    // Behavior structs
    EnemyState state;

    // More variables for custom movements
    Vector2 vector2_aux1;
    Vector2 vector2_aux2;
    float float_aux1;
} Enemy;

extern List* enemies;

void ActivateEnemy(Enemy* enemy, Vector2 position, EnemyType type, int hp);
void SpawnEnemies(List* enemy_list);

void InitEnemies(void);
void DeInitEnemy(Enemy* enemy);
void UpdateEnemies(Ship* ship);
void DrawEnemies(void);
void LoadEnemyTextures(void);
void UnloadEnemyTextures(void);