// game_behavior.c
#pragma once

#include "game_behavior.h"
#include "common.h"
#include "weapon.h"
#include "enemy.h"
#include "power_ups.h"
#include "list.h"
#include "player.h"
#include "raymath.h"

void WallBehavior(Vector2* position) {
    position->x = Clamp(position->x, DRAW_WH / 2, GAME_SCREEN_WIDTH - (DRAW_WH / 2));
    position->y = Clamp(position->y, DRAW_WH / 2, SCREEN_HEIGHT + DRAW_WH / 2);
}

static bool CheckEnemyCollisionWithPlayer(Vector2* ship_pos, Vector2* enemy_pos) {
    float ship_radius = DRAW_WH / 2.0f;
    float enemy_radius = DRAW_WH / 2.0f;

    if (DEBUG_FLAG) {
        DrawCircleV(*ship_pos, ship_radius, Fade(RED, 0.5f));
        DrawCircleV(*enemy_pos, ship_radius, Fade(RED, 0.5f));
    }

    return CheckCollisionCircles(*ship_pos, ship_radius, *enemy_pos, enemy_radius);
}

bool CheckForEnemyCollisions(Ship* ship) {
    int enemy_count = enemies->size;
    for (int i = 0; i < enemy_count; i++) {
        Enemy* enemy = (Enemy*)List_GetByIndex(enemies, i);
        Vector2 enemy_pos_vect = { enemy->position.x, enemy->position.y };
        if (CheckEnemyCollisionWithPlayer(&ship->position, &enemy_pos_vect)) return true;
    }

    return false;
}