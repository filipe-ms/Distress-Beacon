#include "enemy.h"
#include "common.h"
#include "raylib.h"
#include "raymath.h"
#include "list.h"
#include "game_behavior.h"

static const int SPAWN_Y_MIN = -200;
static const float BASE_SPEED_Y = 120.0f;
static const float ENEMY_BASE_WIDTH = 48.0f;
static const float ENEMY_BASE_HEIGHT = 48.0f;

static Texture2D texture;
static Rectangle source_rects[ENEMY_TYPE_COUNT];

List* enemies;

static void BehaviorBasic(Rectangle* position, float speed) {
    position->y += speed * GetFrameTime();
}

static void BehaviorZigZag(Rectangle* position, float speed_y, float* speed_x, float* move_time, bool* action_flag) {
    if (*action_flag == false) {
        *move_time = (float)GetRandomValue(1, 2);
        *speed_x = (GetRandomValue(0, 1) == 0) ? 100 : -100;
        *action_flag = true;
    }

    *move_time -= GetFrameTime();
    if (*move_time <= 0) {
        *move_time = (float)GetRandomValue(1, 2);
        *speed_x *= -1;
    }

    float boost = (float)(GetRandomValue(1, 5));

    position->y += speed_y * GetFrameTime();
    position->x += *speed_x * boost * GetFrameTime();
}

static void BehaviorWaller(Rectangle* position, float speed_x) {
    position->x += speed_x * GetFrameTime();
}

static void BehaviorBooster(Rectangle* position, const struct Ship* target, float* move_time, bool* action_flag) {
    float initial_y_target = 100.0f;
    float seek_speed = 100.0f;
    float boost_speed = 1000.0f;

    if (position->y < initial_y_target) {
        position->y += 100 * GetFrameTime();
    }
    else {
        if (*action_flag == false) {
            *move_time = 2.0f;
            *action_flag = true;
        }

        if (*move_time > 0) {
            *move_time -= GetFrameTime();
            if (position->x < target->position.x) {
                position->x += seek_speed * GetFrameTime();
                if (position->x > target->position.x) position->x = target->position.x;
            }
            else if (position->x > target->position.x) {
                position->x -= seek_speed * GetFrameTime();
                if (position->x < target->position.x) position->x = target->position.x;
            }
        }
        else {
            position->y += boost_speed * GetFrameTime();
        }
    }
}

static void BehaviorSpinner(Enemy* enemy) {
    bool has_reached_max = false;

    if (enemy->state == ENEMY_STATE_SPAWNING) {
        enemy->elapsed_time = ClampWithFlagsF(enemy->elapsed_time + GetFrameTime(), 0, 2, NULL, &has_reached_max);
        enemy->position.y = enemy->speed.y * enemy->elapsed_time;

        if (has_reached_max) {
            enemy->elapsed_time = 0;
            enemy->state = ENEMY_STATE_SPINNER_TURNING;
        }

        return;
    }
    
    if (enemy->state == ENEMY_STATE_SPINNER_TURNING) {
        enemy->elapsed_time = ClampWithFlagsF(enemy->elapsed_time + GetFrameTime(), 0, 0.5, NULL, &has_reached_max);
        enemy->rotation = (PI / 2.0f) * enemy->elapsed_time * RAD2DEG;

        if (has_reached_max) {
            enemy->elapsed_time = 0;
            enemy->state = ENEMY_STATE_SPINNER_ACTING;
            enemy->vector2_aux1 = (Vector2) { enemy->position.x, enemy->position.y - 100 };
            enemy->vector2_aux2 = (Vector2) { 0, 100 };
            enemy->float_aux1 = 0.0f;
        }

        return;
    }

    if (enemy->state == ENEMY_STATE_SPINNER_ACTING) {
        enemy->elapsed_time += GetFrameTime();
        
        float acceleration = Clamp(enemy->elapsed_time, 0.0f, 1.0f);       
        enemy->float_aux1 += (acceleration * GetFrameTime() * PI * 2.0f) / 2.0f; // 2s for a full revolution
        Vector2 rotation_vector = Vector2Rotate(enemy->vector2_aux2, enemy->float_aux1); 
                
        enemy->vector2_aux1.x += enemy->speed.x * GetFrameTime();
        enemy->vector2_aux1.y += enemy->speed.y * GetFrameTime() / 2.0f;

        Vector2* pos = &enemy->position;
        *pos = Vector2Add(enemy->vector2_aux1, rotation_vector);

        Vector2 angle_vector = Vector2Subtract(*pos, enemy->vector2_aux1);
        enemy->rotation = atan2(angle_vector.y, angle_vector.x) * RAD2DEG;
    }
}

void ActivateEnemy(Enemy* enemy, Vector2 position, EnemyType type, int hp) {
    enemy->is_on_screen = false;
    enemy->type = type;
    enemy->hp = (float)hp;
    enemy->color = WHITE;
    enemy->move_time = 0.0f;
    enemy->action_flag = false;
    enemy->rotation = 0.0f;

    enemy->position.width = ENEMY_BASE_WIDTH;
    enemy->position.height = ENEMY_BASE_HEIGHT;

    enemy->position.x = position.x;
    enemy->position.y = position.y;

    enemy->vector2_aux1 = (Vector2){ 0 };
    enemy->vector2_aux1 = (Vector2){ 0 };

    enemy->speed = (Vector2){ 0, BASE_SPEED_Y };

    // Complex behavioral structures needs extra variables
    enemy->state = ENEMY_STATE_SPAWNING;
    enemy->elapsed_time;

    switch (type) {
    case ENEMY_ZIGZAG:  enemy->exp = 20.0f; break;
    case ENEMY_BOOSTER: enemy->exp = 25.0f; break;
    case ENEMY_WALLER:  enemy->exp = 15.0f; break;
    case ENEMY_SPINNER: enemy->exp = 15.0f; break;
    case ENEMY_BASIC:
    default:            enemy->exp = 10.0f; break;
    }
}

void InitEnemies(void) {
    enemies = List_Create(sizeof(Enemy));
}

void SpawnEnemies(List* enemy_list) {
    for(Node* node = enemy_list->head; node != NULL; node = node->next) {
        List_AddLast(enemies, node->data);
        TraceLog(LOG_WARNING, "Spawn: %f %f |", ((Enemy*)node->data)->position.x, ((Enemy*)node->data)->position.y);
    }
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
    case ENEMY_SPINNER: BehaviorSpinner(enemy); break;
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

        DrawTexturePro(texture, source_rects[e->type], destination, (Vector2) { destination.width / 2.0f, destination.height / 2.0f }, e->rotation, e->color);
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
    source_rects[ENEMY_SPINNER] = (Rectangle){ 40, 8, 8, 8 };
    source_rects[ENEMY_BOSS] = (Rectangle){ 16, 8, 8, 8 };

}

void UnloadEnemyTextures(void) {
    UnloadTexture(texture);
    List_Destroy(enemies);
}
