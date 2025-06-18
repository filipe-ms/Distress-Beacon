#pragma once

#include "raylib.h"
#include "raymath.h"
#include "common.h"

void WallBehavior(Vector2* position);

bool CheckEnemyCollisionWithPlayer(Vector2* ship_pos, Vector2* enemy_pos);
