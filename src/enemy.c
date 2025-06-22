#include "enemy.h"
#include "common.h"
#include "raymath.h"
#include "game_behavior.h"

static const int SPAWN_Y_MIN = -200;

static Texture2D texture;
static Rectangle source_rects[ENEMY_TYPE_COUNT];

// Macros

#define E_POSX enemy->position.x
#define E_POSY enemy->position.y
#define E_SIZEX enemy->size.x
#define E_SIZEY enemy->size.y
#define E_SPEEDX enemy->speed.x
#define E_SPEEDY enemy->speed.y


List* enemies;

#pragma region ENEMY_BEHAVIORS
static Vector2 GetPositionNearPlayer(Ship* ship, int rangexy) {
    rangexy = abs(rangexy);

    Vector2 position = {
        GetRandomValue(ship->position.x - rangexy, ship->position.x + rangexy),
        GetRandomValue(ship->position.y - rangexy, ship->position.y + rangexy),
    };

    position.x = Clamp(position.x, DRAW_WH / 2, GAME_SCREEN_END - DRAW_WH / 2);
    position.y = Clamp(position.y, DRAW_WH / 2, SCREEN_HEIGHT - DRAW_WH / 2);

    return position;
}

int LerpInt(int start, int end, float elapsed_time, float duration) {
    if (duration <= 0) return end;
    float t = elapsed_time / duration;
    if (t > 1.0f) t = 1.0f;
    return start + (int)(t * (end - start));
}

static void BehaviorBasic(Enemy* enemy) {
    E_POSY += enemy->speed.y * GetFrameTime();
}
static void BehaviorStalker(Enemy* enemy, Ship* target) {

    float target_distance = 320.0f;
    float attack_speed = 240.0f;

    if (enemy->should_perform_action == true) {
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
                enemy->should_perform_action = true;
                return;
            }
        }

        enemy->timer += GetFrameTime();

        E_POSY += enemy->speed.y * GetFrameTime();

        float amplitude = 200.0f;
        float frequency = 2.0f;

        float horizontal_velocity = amplitude * cosf(enemy->timer * frequency);

        E_POSX += horizontal_velocity * GetFrameTime();
    }
}
static void BehaviorZigZag(Enemy* enemy) {
    if (enemy->should_perform_action == false) {
        enemy->timer = (float)GetRandomValue(1, 2);
        enemy->speed.x = (GetRandomValue(0, 1) == 0) ? 100 : -100;
        enemy->should_perform_action = true;
    }

    enemy->timer -= GetFrameTime();
    if (enemy->timer <= 0) {
        enemy->timer = (float)GetRandomValue(1, 2);
        enemy->speed.x *= -1;
    }

    float boost = (float)(GetRandomValue(1, 3));

    E_POSY += enemy->speed.y * GetFrameTime();
    E_POSX += enemy->speed.x * boost * GetFrameTime();
}
static void BehaviorWaller(Rectangle* position, float speed_x) {
    position->x += speed_x * GetFrameTime();
}
static void BehaviorBooster(Enemy* enemy, Ship* ship) {
    float target_y = 100.0f; // O Y da tela quando ele para de entrar e começa o lock-on
    float boost_multiplier = 1000.0f;

    // Entrada
    if (E_POSY < target_y) {
        E_POSY += E_SPEEDY * GetFrameTime();
        if (E_POSY >= target_y) E_POSY = target_y;
        return;
    }

    // Dash
    enemy->timer -= GetFrameTime();
    
    if (enemy->timer <= 0.0f) {
        E_POSY += boost_multiplier * GetFrameTime();
        return;
    }

    // Lock on
    if (E_POSX < ship->position.x) E_POSX += E_SPEEDX * GetFrameTime();
    else if (E_POSX > ship->position.x) E_POSX -= E_SPEEDX * GetFrameTime();
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
static void BehaviorGhost(Enemy* enemy, Ship* ship) {
    float fade_duration = 0.2f;
    TraceLog(LOG_INFO, "GHOST STATE: %d", enemy->state);
    // Na primeira execução, define a posição onde o GHOST vai aparecer.
    if (!enemy->should_perform_action) {
        enemy->vector2_aux1 = GetPositionNearPlayer(ship, 200);
        enemy->should_perform_action = true;
    }

    switch (enemy->state) {
    case GHOST_IDLE:
        enemy->elapsed_time -= GetFrameTime();
        if (enemy->elapsed_time <= 0) {
            if (enemy->color.a == 0) {
                enemy->state = GHOST_VISIBLE;
                enemy->elapsed_time = 0.0f;
                enemy->position = enemy->vector2_aux1;
                enemy->float_aux1 = 1;
                enemy->is_on_screen = true;
            }
            else {
                enemy->state = GHOST_INVISIBLE;
                enemy->elapsed_time = 0.0f;
                enemy->vector2_aux1 = GetPositionNearPlayer(ship, 200);
                enemy->float_aux1 = -1;
            }
        }
        break;

    case GHOST_VISIBLE:
        enemy->elapsed_time += GetFrameTime();
        if (enemy->elapsed_time < fade_duration) {
            enemy->color.a = LerpInt(0, 255, enemy->elapsed_time, fade_duration);
        }
        else {
            enemy->color.a = 255;
            enemy->elapsed_time = enemy->timer;
            enemy->state = GHOST_IDLE;
        }
        break;

    case GHOST_INVISIBLE:
        enemy->elapsed_time += GetFrameTime();
        if (enemy->elapsed_time < fade_duration) {
            enemy->color.a = LerpInt(255, 0, enemy->elapsed_time, fade_duration);
        }
        else {
            enemy->color.a = 0;
            enemy->position.y = -500.0f;
            enemy->position.x = GAME_SCREEN_CENTER;
            enemy->is_on_screen = false;
            enemy->elapsed_time = enemy->timer;
            enemy->state = GHOST_IDLE;
        }
        break;
    }
}

#pragma endregion ENEMY_BEHAVIORS

static Rectangle GetEnemyRectangle(Enemy* enemy) {
    return (Rectangle) { E_POSX, E_POSY, E_SIZEX, E_SIZEY };
}

static void InitBooster(Enemy* enemy) {
    enemy->exp = 25.0f;
    enemy->score = 200.0f;

    enemy->speed = (Vector2){ 40.0f, 40.0f };
    enemy->timer = GetRandomValue(3, 5);
}

static void InitGhost(Enemy* enemy) {
    enemy->exp = 25.0f;
    enemy->score = 250.0f;
    enemy->speed = (Vector2){ 0, 0 };
    enemy->timer = 5.0f; // Tempo entre ciclos
    enemy->elapsed_time = enemy->timer;
    enemy->should_perform_action = false;
    enemy->state = GHOST_IDLE;
    enemy->color.a = 0; // inicia invisível
    
    enemy->float_aux1 = -1; // aux p/ saber se tá vindo de visível ou invisível
}

static void InitEnemySpecifics(Enemy* enemy) {
    switch (enemy->type) {
    case ENEMY_ZIGZAG:
        enemy->exp = 20.0f;
		enemy->score = 150.0f;
		enemy->speed = (Vector2){ 0, 100.0f };
        break;
    case ENEMY_BOOSTER:
		InitBooster(enemy);
        break;
    case ENEMY_WALLER:
        enemy->exp = 15.0f;
		enemy->score = 100.0f;
		enemy->speed = (Vector2){ 0, 50.0f };
        break;
	case ENEMY_STALKER:
		enemy->exp = 20.0f;
		enemy->score = 200.0f;
		enemy->speed = (Vector2){ 0, 100.0f };
		break;
    case ENEMY_SPINNER:
    case ENEMY_REVERSE_SPINNER:
        enemy->state = ENEMY_STATE_SPINNER_SPAWNING;
        enemy->exp = 15.0f;
		enemy->score = 100.0f;
		enemy->speed = (Vector2){ 0, 120.0f };
        break;
    case ENEMY_GHOST:
		InitGhost(enemy);
		break;
    case ENEMY_BASIC:
    default:
        enemy->exp = 10.0f;
		enemy->score = 50.0f;
		enemy->speed = (Vector2){ 0, 80.0f };
        break;
    }
}

void ActivateEnemy(Enemy* enemy, Vector2 position, EnemyType type, int hp) {
    enemy->type = type;
    enemy->hp = (float)hp;
    enemy->color = WHITE;
    enemy->timer = 0.0f;
    enemy->should_perform_action = false;
    enemy->rotation = 0.0f;

    E_SIZEX = DRAW_WH;
    E_SIZEY = DRAW_WH;

    E_POSX = position.x;
    E_POSY = position.y;

    enemy->vector2_aux1 = (Vector2){ 0 };
    enemy->vector2_aux1 = (Vector2){ 0 };

    enemy->elapsed_time = 0;

    InitEnemySpecifics(enemy);
}

void DeInitEnemy(Enemy* enemy) {
	enemy->hp = 0.0f;
	enemy->color = WHITE;
	enemy->timer = 0.0f;
	enemy->should_perform_action = false;
	enemy->rotation = 0.0f;
	E_POSX = -500.0f;
	E_POSY = -500.0f;
    enemy->is_on_screen = false;
	enemy->speed = (Vector2){ 0, 0 };
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

    if (enemy->position.x <= min_x || enemy->position.x >= max_x) {
        Clamp(enemy->position.x, min_x, max_x);
        if (enemy->type == ENEMY_ZIGZAG) enemy->speed.x *= -1;
    }
}

static void UpdateEnemy(void* context, void* data) {
    Ship* ship = (Ship*)context;
    Enemy* enemy = (Enemy*)data;

    switch (enemy->type) {
    case ENEMY_ZIGZAG:          BehaviorZigZag(&enemy); break;
    case ENEMY_BOOSTER:         BehaviorBooster(&enemy, &ship); break;
    case ENEMY_WALLER:          BehaviorWaller(&enemy->position, enemy->speed.x); break;
    case ENEMY_SPINNER:         BehaviorSpinner(enemy, 1); break;
    case ENEMY_REVERSE_SPINNER: BehaviorSpinner(enemy, -1); break;
    case ENEMY_STALKER:         BehaviorStalker(enemy, ship);
	case ENEMY_GHOST:           BehaviorGhost(enemy, ship); break;
    case ENEMY_BASIC:
    default:                    BehaviorBasic(&enemy); break;
    }

    EnemyWallBehavior(enemy);
}

static void EnemyPositionChecks(Enemy* enemy) {
	bool is_out_of_bounds = E_POSY > SCREEN_HEIGHT + E_SIZEY + 10;

    if (is_out_of_bounds) {
        DeInitEnemy(enemy);
        return;
    }

    enemy->is_on_screen = (E_POSY > -50);
}

void UpdateEnemies(Ship* ship) {
    List_ForEachCtx(enemies, ship, UpdateEnemy);

    // Position-based updates
    List_ForEach(enemies, (MatchFunction)EnemyPositionChecks);
}

static void DrawGhostShade(Enemy* enemy) {
    if (enemy->float_aux1 < 0) {
        Vector2 ghost_indicator = { enemy->vector2_aux1.x, enemy->vector2_aux1.y };
        DrawCircleLines(ghost_indicator.x, ghost_indicator.y, enemy->size.x/2, Fade(RED, 0.5f));
        float rec_w = 8;
        Rectangle rec = { ghost_indicator.x - rec_w/2, ghost_indicator.y-20, rec_w, 25 };
        DrawRectangleRounded(rec, 20, 20, Fade(RED, 0.5f));
        DrawCircle(ghost_indicator.x, ghost_indicator.y+14, 4, Fade(RED, 0.5f));

    }
}

static void DrawEnemy(void* context, void* data) {
    Enemy* enemy = (Enemy*)data;

    if (enemy->is_on_screen) {
        if (DEBUG_FLAG) {
            Vector2 center = { E_POSX, E_POSY };
            DrawCircleV(center, 20.0f, Fade(GREEN, 0.5f));
        }

        Vector2 origin = { E_SIZEX / 2, E_SIZEY / 2 };
		Rectangle enemy_rect = GetEnemyRectangle(enemy);
        DrawTexturePro(texture, source_rects[enemy->type], enemy_rect, origin, enemy->rotation, enemy->color);
    }

	if (enemy->type == ENEMY_GHOST) DrawGhostShade(enemy);
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
    source_rects[ENEMY_GHOST]                = (Rectangle){ 32, 0, 8, 8 };
    source_rects[ENEMY_BOSS_PIDGEON_OF_PREY] = (Rectangle){ 8 * 8, 8 * 8, 16, 16 };
}

void UnloadEnemyTextures(void) {
    UnloadTexture(texture);
    List_Destroy(enemies);
}