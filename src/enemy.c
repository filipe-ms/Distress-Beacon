#include "enemy.h"
#include "common.h"
#include "behavior.h"
#include "raylib.h"
#include "list.h"

static const float BASE_SPEED_Y = 120.0f;
static const int SPAWN_Y_MIN = -200;
static const int SPAWN_Y_MAX = -50;
static const float ENEMY_BASE_WIDTH = 48.0f;
static const float ENEMY_BASE_HEIGHT = 48.0f;

static Texture2D texture;
static Rectangle source_rects[ENEMY_TYPE_COUNT];

List* enemiesx;

Enemy enemies[MAX_ENEMY_NUMBER];

static void ActivateEnemy(Enemy* enemy, EnemyType type, int hp) {
    enemy->active = true;
    enemy->type = type;
    enemy->hp = (float)hp;
    enemy->color = WHITE;
    enemy->move_time = 0.0f;
    enemy->action_flag = false;

    enemy->position.width = ENEMY_BASE_WIDTH;
    enemy->position.height = ENEMY_BASE_HEIGHT;

    enemy->position.x = (float)GetRandomValue(0, (int)(GAME_SCREEN_WIDTH - enemy->position.width));
    enemy->position.y = (float)GetRandomValue(SPAWN_Y_MIN, SPAWN_Y_MAX);

    enemy->speed = (Vector2){ 0, BASE_SPEED_Y };

    switch (type) {
    case ENEMY_ZIGZAG:  enemy->exp = 20.0f; break;
    case ENEMY_BOOSTER: enemy->exp = 25.0f; break;
    case ENEMY_WALLER:  enemy->exp = 15.0f; break;
    case ENEMY_BASIC:
    default:            enemy->exp = 10.0f; break;
    }
}

void InitEnemies(void) {
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        enemies[i].active = false;
        enemies[i].position.width = DRAW_WH;
        enemies[i].position.height = DRAW_WH;
    }
	enemiesx = List_Create(sizeof(Enemy));
}

static void Spawn(int amount, int hp, EnemyType type, bool random) {
    int spawned_count = 0;
    for (int i = 0; i < MAX_ENEMY_NUMBER && spawned_count < amount; i++) {
        if (!enemies[i].active) {
            EnemyType final_type = random ? (EnemyType)GetRandomValue(ENEMY_BASIC, ENEMY_WALLER) : type;
            ActivateEnemy(&enemies[i], final_type, hp);
            spawned_count++;
        }
    }
}

void SpawnEnemies(int amount, EnemyType type, int hp) {
    Spawn(amount, hp, type, false);
}

void SpawnRandomEnemies(int amount, int hp) {
    Spawn(amount, hp, ENEMY_BASIC, true);
}

void UpdateEnemies(Ship* ship) {
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        if (!enemies[i].active) continue;

        Enemy* e = &enemies[i];

        switch (e->type) {
        case ENEMY_BASIC:   BehaviorBasic(&e->position, BASE_SPEED_Y); break;
        case ENEMY_ZIGZAG:  BehaviorZigZag(&e->position, BASE_SPEED_Y, &e->speed.x, &e->move_time, &e->action_flag); break;
        case ENEMY_BOOSTER: BehaviorBooster(&e->position, ship, &e->move_time, &e->action_flag); break;
        case ENEMY_WALLER:  BehaviorWaller(&e->position, e->speed.x); break;
        default:            BehaviorBasic(&e->position, BASE_SPEED_Y); break;
        }

        if (e->position.x <= 0) {
            e->position.x = 0;
            if (e->type == ENEMY_ZIGZAG) e->speed.x *= -1;
        }
        else if (e->position.x >= GAME_SCREEN_WIDTH - e->position.width) {
            e->position.x = GAME_SCREEN_WIDTH - e->position.width;
            if (e->type == ENEMY_ZIGZAG) e->speed.x *= -1;
        }

        if (e->position.y > SCREEN_HEIGHT + 20) {
            e->active = false;
        }
    }
}

void DrawEnemies(void) {
    Vector2 origin = { 0, 0 };
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        if (enemies[i].active) {
            if (DEBUG_FLAG) {
				Vector2 center = { enemies[i].position.x, enemies[i].position.y };
                DrawCircleV(center, 20.0f, Fade(GREEN, 0.5f));
            }

            Rectangle destination = {
                enemies[i].position.x - enemies[i].position.width / 2,
                enemies[i].position.y - enemies[i].position.height / 2,
                enemies[i].position.width,
                enemies[i].position.height
            };

            DrawTexturePro(texture, source_rects[enemies[i].type], destination, origin, 0, enemies[i].color);
        }
    }
}

void LoadEnemyTextures(void) {
    texture = LoadTexture("ships.png");
    if (texture.id <= 0) {
        TraceLog(LOG_WARNING, "Textura de inimigos (ships.png) não encontrada.");
        return;
    }

    source_rects[ENEMY_BASIC] = (Rectangle){ 32, 0, 8, 8 };
    source_rects[ENEMY_ZIGZAG] = (Rectangle){ 32, 24, 8, 8 };
    source_rects[ENEMY_BOOSTER] = (Rectangle){ 48, 24, 8, 8 };
    source_rects[ENEMY_WALLER] = (Rectangle){ 32, 8, 8, 8 };
    source_rects[ENEMY_BOSS] = (Rectangle){ 16, 8, 8, 8 };
}

void UnloadEnemyTextures(void) {
    UnloadTexture(texture);
}