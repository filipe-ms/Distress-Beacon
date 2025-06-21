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

List* enemies;

static void ActivateEnemy(Enemy* enemy, EnemyType type, int hp) {
    enemy->is_on_screen = false;
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
    enemies = List_Create(sizeof(Enemy));
}

static void Spawn(int amount, int hp, EnemyType type, bool random) {
    for (int i = 0; i < amount; i++) {
        Enemy new_enemy;
        EnemyType final_type = random ? (EnemyType)GetRandomValue(ENEMY_BASIC, ENEMY_WALLER) : type;
        ActivateEnemy(&new_enemy, final_type, hp);
        List_AddLast(enemies, &new_enemy);
    }
}

void SpawnEnemies(int amount, EnemyType type, int hp) {
    Spawn(amount, hp, type, false);
}

void SpawnRandomEnemies(int amount, int hp) {
    Spawn(amount, hp, ENEMY_BASIC, true);
}

static void UpdateEnemy(void* context, void* data) {
    Ship* ship = (Ship*)context;
    Enemy* e = (Enemy*)data;

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

    e->is_on_screen = (e->position.y > -20);
}

static void CheckEnemyOutOfBounds(Enemy* enemy) {
    bool isOutOfBounds = enemy->position.y > GAME_SCREEN_HEIGHT + 50;

    if (isOutOfBounds) {
        enemy->hp = 0;
        enemy->exp = 0;
    }
}

void UpdateEnemies(Ship* ship) {

    List_ForEachCtx(enemies, ship, UpdateEnemy);

    // Kill the enemy but do not remove it from the list yet
    List_ForEach(enemies, (MatchFunction)CheckEnemyOutOfBounds);
}

static void DrawEnemy(void* context, void* data) {
    Enemy* e = (Enemy*)data;
    if (e->is_on_screen) {
        if (DEBUG_FLAG) {
            Vector2 center = { e->position.x, e->position.y };
            DrawCircleV(center, 20.0f, Fade(GREEN, 0.5f));
        }

        Rectangle destination = {
            e->position.x - e->position.width / 2,
            e->position.y - e->position.height / 2,
            e->position.width,
            e->position.height
        };

        DrawTexturePro(texture, source_rects[e->type], destination, (Vector2) { 0, 0 }, 0, e->color);
    }
}

void DrawEnemies(void) {
    List_ForEachCtx(enemies, NULL, DrawEnemy);
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
    List_Destroy(enemies);
}