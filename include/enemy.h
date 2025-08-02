#pragma once

#include "raylib.h"
#include "ship.h"
#include "list.h"

typedef enum {
    // Base texture
    ENEMY_BASIC,
    ENEMY_ZIGZAG,
    ENEMY_BOOSTER,
    ENEMY_WALLER,
    ENEMY_SPINNER,
    ENEMY_STALKER,
    ENEMY_REVERSE_SPINNER,
    ENEMY_BOSS_PIDGEON_OF_PREY,

    // Custom texture
    ENEMY_GHOST,
    
    ENEMY_TYPE_COUNT
} EnemyType;

typedef enum {
    ENEMY_STATE_SPINNER_SPAWNING,
    ENEMY_STATE_SPINNER_TURNING,
    ENEMY_STATE_SPINNER_ACTING,

    ENEMY_STATE_BOSS_PIDGEON_OF_PREY_PRE_SPAWN,
    ENEMY_STATE_BOSS_PIDGEON_OF_PREY_SPAWNING,
    ENEMY_STATE_BOSS_PIDGEON_OF_PREY_SHOOTING,

    GHOST_IDLE,
    GHOST_VISIBLE,
    GHOST_INVISIBLE,
    GHOST_EMP
} EnemyState;

typedef struct {
    int id;
    EnemyType type;

    Vector2 position;
    Vector2 size;
    Vector2 speed;
    float rotation;
	bool is_on_screen;

    float hp;
    float exp;
    float score;

    bool is_targetable;
    bool is_collidable;

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

typedef enum {
    PROJECTILE_PIDGEON_OF_PREY_1,

    PROJECTILE_COUNT,
} ProjectileType;

typedef enum {
    PROJECTILE_STATE_SPAWNING,
    PROJECTILE_STATE_MOVING,
} ProjectileState;

typedef struct {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    ProjectileType type;
    
    Color color;
    float rotation;

    float collision_size;

    bool is_targetable;
    bool is_collidable;

    Enemy* owner;
} EnemyProjectile;


extern List* enemies;

void InitEnemy(Enemy* enemy, Vector2 position, EnemyType type, int hp);
void SpawnEnemies(List* enemy_list);

void InitEnemies(void);
void DeInitEnemy(Enemy* enemy);
void UpdateEnemies(Ship* ship);
void CleanupEnemies(void);
void DrawEnemies(void);
void InitEnemySourceRects(void);
void UnloadEnemyList(void);

bool IsPlayerAlive(void);