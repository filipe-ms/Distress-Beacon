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
#include "hit_confirmation.h"

void WallBehavior(Vector2* position) {
	position->x = Clamp(position->x, DRAW_WH/2, SCREEN_WIDTH - UI_WIDTH - (DRAW_WH/2) );
	position->y = Clamp(position->y, 0, SCREEN_HEIGHT);
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

static bool CheckForHits(void* context, void* data) {
    Enemy* enemy = (Enemy*)context;
    Shoot* shoot = (Shoot*)data;

    Vector2 enemy_pos = { enemy->position.x, enemy->position.y };

    if (CheckCollisionCircles(enemy_pos, 20, shoot->position, shoot->size.x / 2.0f)) {
        enemy->hp -= shoot->damage;
		ConfirmHit(SHOCKWAVE, enemy_pos);

        if (enemy->hp <= 0) {
            AddExperience(enemy->exp);
            AddScore(100);
            enemy->active = false;
        }

        return true;
    }

    return false;
}

bool CheckForAllCollisions(Ship* ship) {
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        if (!enemies[i].active) continue;
        List_RemoveWithFn(pulse.pulse_shoots, &enemies[i], (MatchFunction)CheckForHits);
        List_RemoveWithFn(photon.photon_shoots, &enemies[i], (MatchFunction)CheckForHits);
        List_RemoveWithFn(shotgun.shotgun_shoots, &enemies[i], (MatchFunction)CheckForHits);

        Vector2 enemy_pos_vect = { enemies[i].position.x, enemies[i].position.y };
        if (CheckEnemyCollisionWithPlayer(&ship->position, &enemy_pos_vect)) return true;
    }
    return false;
}