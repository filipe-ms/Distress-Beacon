#pragma once

#include "raylib.h"
#include "list.h"
#include "ship.h"
#include "enemy.h"

extern List* enemy_projectiles;

void EnemyProjectile_Init(void);
void EnemyProjectile_Activate(EnemyProjectile* enemy_projectile, Enemy* enemy, ProjectileType type);
void EnemyProjectile_Spawn(Enemy* enemy, ProjectileType type);
void EnemyProjectile_SpawnPosition(Enemy* enemy, ProjectileType type, Vector2 position);
void EnemyProjectile_Update(Ship* ship);
void EnemyProjectile_Draw(void);
void EnemyProjectile_Load(void);