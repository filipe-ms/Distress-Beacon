// game_behavior.c
#pragma once

#include "game_behavior.h"
#include "common.h"

void WallBehavior(Vector2* position) {
	position->x = Clamp(position->x, DRAW_WH/2, SCREEN_WIDTH - UI_WIDTH - (DRAW_WH/2) );
	position->y = Clamp(position->y, 0, SCREEN_HEIGHT);
}

bool CheckEnemyCollisionWithPlayer(Vector2* ship_pos, Vector2* enemy_pos) {
    float ship_radius = DRAW_WH / 2.0f;
    float enemy_radius = DRAW_WH / 2.0f;

    if (DEBUG_FLAG) {
        DrawCircleV(*ship_pos, ship_radius, Fade(RED, 0.5f));
        DrawCircleV(*enemy_pos, ship_radius, Fade(RED, 0.5f));
    }

    return CheckCollisionCircles(*ship_pos, ship_radius, *enemy_pos, enemy_radius);
}