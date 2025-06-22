#include "enemy.h"
#include "common.h"
#include "raymath.h"
#include "game_behavior.h"

static const int SPAWN_Y_MIN = -200;
static const float BASE_SPEED_Y = 120.0f;
static const float ENEMY_BASE_WIDTH = 48.0f;
static const float ENEMY_BASE_HEIGHT = 48.0f;

static Texture2D texture;
static Rectangle source_rects[ENEMY_TYPE_COUNT];

// Macros

#define E_POSX enemy->position.x
#define E_POSY enemy->position.y
#define E_SIZEX enemy->size.x
#define E_SIZEY enemy->size.y

List* enemies;

static Rectangle GetEnemyRectangle(Enemy* enemy) {
	return (Rectangle) { E_POSX, E_POSY, E_SIZEX, E_SIZEY };
}

static void BehaviorBasic(Rectangle* position, float speed) {
    position->y += speed * GetFrameTime();
}

void BehaviorStalker(Enemy* enemy, Ship* target) {

    float target_distance = 320.0f;
    float attack_speed = 240.0f;

    if (enemy->action_flag == true) {
        if (!target) return;

        Vector2 enemy_center = { E_POSX + E_SIZEX / 2, E_POSY + E_SIZEY / 2 };
        Vector2 target_center = { target->position.x + target->draw_size.x / 2, target->position.y + target->draw_size.y / 2 };

        Vector2 direction = Vector2Normalize(Vector2Subtract(target_center, enemy_center));
        E_POSX += direction.x * attack_speed * GetFrameTime();
        E_POSY += direction.y * attack_speed * GetFrameTime();
    }
    else {

        if (target) {
            Vector2 enemy_center =  { E_POSX + E_SIZEX / 2, E_POSY + E_SIZEY / 2 };
            Vector2 target_center = { target->position.x + target->draw_size.x / 2, target->position.y + target->draw_size.y / 2 };
            float distance = Vector2Distance(enemy_center, target_center);

            if (distance < target_distance) {
                enemy->action_flag = true;
                return;
            }
        }

        enemy->move_time += GetFrameTime();

        E_POSY += enemy->speed.y * GetFrameTime();

        float amplitude = 200.0f;
        float frequency = 2.0f;

        float horizontal_velocity = amplitude * cosf(enemy->move_time * frequency);

        E_POSX += horizontal_velocity * GetFrameTime();
    }
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

static void BehaviorSpinner(Enemy* enemy, int modifier) {
    bool has_reached_max = false;

    if (enemy->state == ENEMY_STATE_SPINNER_SPAWNING) {
        enemy->elapsed_time = ClampWithFlagsF(enemy->elapsed_time + GetFrameTime(), 0, 2, NULL, &has_reached_max);
        E_POSY = enemy->speed.y * enemy->elapsed_time;

        if (has_reached_max) {
            enemy->elapsed_time = 0;
            enemy->state = ENEMY_STATE_SPINNER_TURNING;
        }

        return;
    }
    
    if (enemy->state == ENEMY_STATE_SPINNER_TURNING) {
        enemy->elapsed_time = ClampWithFlagsF(enemy->elapsed_time + GetFrameTime(), 0, 0.5, NULL, &has_reached_max);
        enemy->rotation = modifier * (PI / 2.0f) * enemy->elapsed_time * RAD2DEG;

        if (has_reached_max) {
            enemy->elapsed_time = 0;
            enemy->state = ENEMY_STATE_SPINNER_ACTING;
            enemy->vector2_aux1 = (Vector2) { E_POSX, E_POSY - 100 };
            enemy->vector2_aux2 = (Vector2) { 0, 100 };
            enemy->float_aux1 = 0.0f;
        }
        return;
    }

    if (enemy->state == ENEMY_STATE_SPINNER_ACTING) {
        enemy->elapsed_time += GetFrameTime();
        
        float acceleration = modifier * Clamp(enemy->elapsed_time, 0.0f, 1.0f);       
        enemy->float_aux1 += (acceleration * GetFrameTime() * PI * 2.0f) / 2.0f; // 2s for a full revolution
        Vector2 rotation_vector = Vector2Rotate(enemy->vector2_aux2, enemy->float_aux1); 
                
        enemy->vector2_aux1.x += enemy->speed.x * GetFrameTime();
        enemy->vector2_aux1.y += enemy->speed.y * GetFrameTime() / 2.0f;

        Vector2* pos = &enemy->position;
        *pos = Vector2Add(enemy->vector2_aux1, rotation_vector);

        Vector2 angle_vector = Vector2Multiply(
                Vector2Subtract(*pos, enemy->vector2_aux1),
                (Vector2){modifier, modifier}
            );

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

    E_SIZEX = ENEMY_BASE_WIDTH;
    E_SIZEY = ENEMY_BASE_HEIGHT;

    E_POSX = position.x;
    E_POSY = position.y;

    enemy->vector2_aux1 = (Vector2){ 0 };
    enemy->vector2_aux1 = (Vector2){ 0 };

    enemy->speed = (Vector2){ 0, BASE_SPEED_Y };

    // Complex behavioral structures needs extra variables
    enemy->elapsed_time;

    switch (type) {
    case ENEMY_ZIGZAG:          
        enemy->exp = 20.0f;
        break;
    case ENEMY_BOOSTER:
        enemy->exp = 25.0f;
        break;
    case ENEMY_WALLER:
        enemy->exp = 15.0f;
        break;
    case ENEMY_SPINNER:
        enemy->state = ENEMY_STATE_SPINNER_SPAWNING;
        enemy->exp = 15.0f;
        break;
    case ENEMY_REVERSE_SPINNER:
        enemy->exp = 15.0f;
        break;
    case ENEMY_BASIC:
    default:
        enemy->exp = 10.0f;
        break;
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
    int min_x = (int)(GAME_SCREEN_START + E_SIZEX / 2);
    int max_x = (int)(GAME_SCREEN_END - E_SIZEX / 2);

    if (E_POSX <= min_x || E_POSX >= max_x) {
        Clamp(E_POSX, min_x, max_x);
        if (enemy->type == ENEMY_ZIGZAG) enemy->speed.x *= -1;
    }
}

static void UpdateEnemy(void* context, void* data) {
    Ship* ship = (Ship*)context;
    Enemy* enemy = (Enemy*)data;

    switch (enemy->type) {

    case ENEMY_BASIC:           BehaviorBasic(&enemy->position, BASE_SPEED_Y); break;
    case ENEMY_ZIGZAG:          BehaviorZigZag(&enemy->position, BASE_SPEED_Y, &enemy->speed.x, &enemy->move_time, &enemy->action_flag); break;
    case ENEMY_BOOSTER:         BehaviorBooster(&enemy->position, ship, &enemy->move_time, &enemy->action_flag); break;
    case ENEMY_WALLER:          BehaviorWaller(&enemy->position, enemy->speed.x); break;
    case ENEMY_SPINNER:         BehaviorSpinner(enemy, 1); break;
    case ENEMY_REVERSE_SPINNER: BehaviorSpinner(enemy, -1); break;
    case ENEMY_STALKER: BehaviorStalker(enemy, ship);
    default:                    BehaviorBasic(&enemy->position, BASE_SPEED_Y); break;
    }

    EnemyWallBehavior(enemy);
    enemy->is_on_screen = (E_POSY > -20);
}

static void CheckEnemyOutOfBounds(Enemy* enemy) {
    bool isOutOfBounds = E_POSY > SCREEN_HEIGHT + E_SIZEY + 10;

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
    Enemy* enemy = (Enemy*)data;
    if (enemy->is_on_screen) {
        if (DEBUG_FLAG) {
            Vector2 center = { E_POSX, E_POSY };
            DrawCircleV(center, 20.0f, Fade(GREEN, 0.5f));
        }

        Vector2 origin = { enemy->size.x/2, enemy->size.y / 2 };
		Rectangle enemy_rect = GetEnemyRectangle(enemy);
        DrawTexturePro(texture, source_rects[enemy->type], enemy_rect, origin, enemy->rotation, enemy->color);
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

    source_rects[ENEMY_BASIC]                = (Rectangle){ 32, 0, 8, 8 };
    source_rects[ENEMY_ZIGZAG]               = (Rectangle){ 32, 24, 8, 8 };
    source_rects[ENEMY_BOOSTER]              = (Rectangle){ 48, 24, 8, 8 };
    source_rects[ENEMY_WALLER]               = (Rectangle){ 32, 8, 8, 8 };
    source_rects[ENEMY_SPINNER]              = (Rectangle){ 40, 8, 8, 8 };
    source_rects[ENEMY_REVERSE_SPINNER]      = (Rectangle){ 80, 0, 8, 8 };
    source_rects[ENEMY_STALKER]              = (Rectangle){ 32, 0, 8, 8 };
    source_rects[ENEMY_BOSS_PIDGEON_OF_PREY] = (Rectangle){ 8 * 8, 8 * 8, 16, 16 };
}

void UnloadEnemyTextures(void) {
    UnloadTexture(texture);
    List_Destroy(enemies);
}