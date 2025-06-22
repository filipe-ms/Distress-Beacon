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

        if (!enemy->is_collidable)
            continue;

        Vector2 enemy_pos_vect = { enemy->position.x, enemy->position.y };
        if (CheckEnemyCollisionWithPlayer(&ship->position, &enemy_pos_vect)) {

            // --- VERIFICA SE A NAVE ESTÁ INVENCÍVEL ---
            if (ship->isInvincible) {
                continue; // Se estiver, ignora a colisão e continua o loop
            }

            if (IsShieldActive()) {
                shield.capacity--;
                enemy->is_on_screen = false;

                // --- ATIVA A INVENCIBILIDADE APÓS O HIT ---
                ship->isInvincible = true;
                ship->invincibilityTimer = 1.0f; // Duração de 1 segundo

                if (shield.capacity <= 0) {
                    DeactivateShield();
                }

                continue;
            }
            else {
                return true;
            }
        }
    }

    return false;
}

float ClampWithFlagsF(float value, float min, float max, bool* has_reached_min, bool* has_reached_max) {
    if (value < min) {
        value = min;
        if (has_reached_min)
            *has_reached_min = true;
    } else if (value > max) {
        value = max;
        if (has_reached_max)
            *has_reached_max = true;
    }

    return value;
}