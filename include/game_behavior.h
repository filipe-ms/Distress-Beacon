#pragma once

#include "raylib.h"
#include "raymath.h"
#include "common.h"

void WallBehavior(Vector2* position) {
	position->x = Clamp(position->x, 0, SCREEN_WIDTH - DRAW_WH);
	position->y = Clamp(position->y, 0, SCREEN_HEIGHT - DRAW_WH);
}

bool CheckEnemyCollisionWithPlayer(Ship* ship, Enemy* enemy) {
    Vector2 ship_center = ship->position;
    float ship_radius = DRAW_WH / 2.0f;

    Rectangle enemy_rect = enemy->position;

    DrawCircleV(ship_center, ship_radius, Fade(RED, 0.5f)); // PARA DEBUG

    return CheckCollisionCircleRec(ship_center, ship_radius, enemy_rect);
}
