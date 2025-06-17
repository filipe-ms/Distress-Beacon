#include "enemy.h"
#include "common.h"
#include "ship.h"
#include "behavior.h"

#include <math.h>

Rectangle enemy_rectangles[5] = {
    { 32, 0, 8, 8 },
    { 32, 24, 8, 8 },
    { 48, 24, 8, 8 },
    { 32, 8, 8, 8 },
    { 16, 8, 8, 8 }
};

Texture enemy_texture;
Vector2 enemy_vect = { 0, 0 };

void InitEnemies(Enemy* enemy) {
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++)
    {
        enemy[i].type = GetRandomValue(0, 4);
        enemy[i].position.width = 48;
        enemy[i].position.height = 48;
        enemy[i].position.x = (float)GetRandomValue(SCREEN_WIDTH, SCREEN_WIDTH + 1000);
        enemy[i].position.y = (float)GetRandomValue(0, (int)(SCREEN_HEIGHT - enemy[i].position.height));
        enemy[i].speed.x = 0;
        enemy[i].speed.y = 120;
        enemy[i].active = false;
        enemy[i].color = WHITE;
        enemy[i].hp = 3.0f;
        enemy[i].exp = 10.0f;
        enemy[i].move_time = 0;
        enemy[i].action_flag = false;
    }
}

void ResetEnemyForSpawning(Enemy* enemy, int type, int hp) {
    enemy->active = true;
    enemy->type = type;
    enemy->hp = (float)hp;
    enemy->color = WHITE;
    enemy->exp = 10.0f;

    enemy->position.x = (float)GetRandomValue(0, (int)(SCREEN_WIDTH - enemy->position.width));
    enemy->position.y = (float)GetRandomValue(-200, -50);
    enemy->action_flag = false;
    enemy->move_time = 0.0f;
    enemy->speed.x = 0;
    enemy->speed.y = 120;
}

void SpawnEnemies(Enemy* enemy, int amount, int type, int hp) {
    for (int i = 0; i < amount; i++) {
        for (int j = 0; j < MAX_ENEMY_NUMBER; j++) {
            if (enemy[j].active) continue;
            ResetEnemyForSpawning(&enemy[j], type, hp);
            if (type == ZIGZAG) enemy[j].exp = 20.0f;
            else if (type == BOOSTER) enemy[j].exp = 25.0f;
            else if (type == WALLER) enemy[j].exp = 15.0f;
            break;
        }
    }
}

void SpawnRandomEnemies(Enemy* enemy, int amount, int hp) {
    for (int i = 0; i < amount; i++) {
        for (int j = 0; j < MAX_ENEMY_NUMBER; j++) {
            if (enemy[j].active) continue;
            int random_type = GetRandomValue(0, 3);
            ResetEnemyForSpawning(&enemy[j], random_type, hp);
            if (random_type == ZIGZAG) enemy[j].exp = 20.0f;
            else if (random_type == BOOSTER) enemy[j].exp = 25.0f;
            else if (random_type == WALLER) enemy[j].exp = 15.0f;
            break;
        }
    }
}


void UpdateEnemies(Enemy* enemy, Ship* ship) {
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++)
    {
        if (!enemy[i].active) continue;

        switch (enemy[i].type) {
        case BASIC:
            BehaviorBasic(&enemy[i].position, 120.0f);
            break;
        case ZIGZAG:
            BehaviorZigZag(&enemy[i].position, 120.0f, &enemy[i].speed.x, &enemy[i].move_time, &enemy[i].action_flag);
            break;
        case BOOSTER:
            BehaviorBooster(&enemy[i].position, ship, &enemy[i].move_time, &enemy[i].action_flag);
            break;
        case WALLER:
            BehaviorWaller(&enemy[i].position, enemy[i].speed.x);
            break;
        }

        if (enemy[i].position.x <= 0) {
            enemy[i].position.x = 0;
            if (enemy[i].type == ZIGZAG) enemy[i].speed.x *= -1;
        }
        else if (enemy[i].position.x >= SCREEN_WIDTH - enemy[i].position.width) {
            enemy[i].position.x = SCREEN_WIDTH - enemy[i].position.width;
            if (enemy[i].type == ZIGZAG) enemy[i].speed.x *= -1;
        }

        if (enemy[i].position.y > SCREEN_HEIGHT + 20) {
            enemy[i].active = false;
        }
    }
}

void DrawEnemies(Enemy* enemy) {
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        if (enemy[i].active) {
            DrawTexturePro(enemy_texture, enemy_rectangles[enemy[i].type], enemy[i].position, enemy_vect, 0, enemy[i].color);
        }
    }
}

void LoadEnemyTextures() {
    enemy_texture = LoadTexture("ships.png");
}

void UnloadEnemyTextures() {
    UnloadTexture(enemy_texture);
}