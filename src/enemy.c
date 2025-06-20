#include "enemy.h"
#include "common.h"
#include "behavior.h"
#include "raylib.h"
#include "raymath.h"
#include "list.h"

static const int SPAWN_Y_MIN = -200;
static const float BASE_SPEED_Y = 120.0f;
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

    enemy->position.x = (float)GetRandomValue(GAME_SCREEN_START + enemy->position.width/2, GAME_SCREEN_END - enemy->position.width / 2);
    enemy->position.y = (float)GetRandomValue(SPAWN_Y_MIN, -1 * enemy->position.height);

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

static void EnemyWallBehavior(Enemy* enemy) {
    int min_x = (int)(GAME_SCREEN_START + enemy->position.width / 2);
    int max_x = (int)(GAME_SCREEN_END - enemy->position.width / 2);

    if (enemy->position.x <= min_x || enemy->position.x >= max_x) {
        Clamp(enemy->position.x, min_x, max_x);
        if (enemy->type == ENEMY_ZIGZAG) enemy->speed.x *= -1;
    }
}

static void UpdateEnemy(void* context, void* data) {
    Ship* ship = (Ship*)context;
    Enemy* enemy = (Enemy*)data;

    switch (enemy->type) {

    case ENEMY_BASIC:   BehaviorBasic(&enemy->position, BASE_SPEED_Y); break;
    case ENEMY_ZIGZAG:  BehaviorZigZag(&enemy->position, BASE_SPEED_Y, &enemy->speed.x, &enemy->move_time, &enemy->action_flag); break;
    case ENEMY_BOOSTER: BehaviorBooster(&enemy->position, ship, &enemy->move_time, &enemy->action_flag); break;
    case ENEMY_WALLER:  BehaviorWaller(&enemy->position, enemy->speed.x); break;
    default:            BehaviorBasic(&enemy->position, BASE_SPEED_Y); break;

    }

    EnemyWallBehavior(enemy);
    enemy->is_on_screen = (enemy->position.y > -20);
}

static void CheckEnemyOutOfBounds(Enemy* enemy) {
    bool isOutOfBounds = enemy->position.y > SCREEN_HEIGHT + enemy->position.height + 10;

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