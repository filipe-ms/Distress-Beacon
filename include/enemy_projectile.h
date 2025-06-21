#pragma once

#include "raylib.h"
#include "list.h"
#include "enemy.h"

typedef enum {
    PROJECTILE_PIDGEON_OF_PREY_1,
} ProjectileType;

typedef enum {
    PROJECTILE_STATE_SPAWNING,
    PROJECTILE_STATE_MOVING,
} ProjectileState;

typedef struct {
    bool is_on_screen;
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    ProjectileType type;
    
    Color color;
    float rotation;

    bool is_targetable;
    bool is_collidable;

    Enemy* owner;
} EnemyProjectile;

extern List* enemy_projectiles;

void EnemyProjectile_Init(EnemyProjectile enemy_projectile, Enemy* enemy, ProjectileType type);
void EnemyProjectile_Spawn();
void EnemyProjectile_Update();
void EnemyProjectile_Draw();
void EnemyProjectile_Load(void);
void EnemyProjectile_Unload(void);