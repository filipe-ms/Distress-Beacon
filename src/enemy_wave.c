#include "enemy_wave.h"
#include "raylib.h"
#include "common.h"
#include "enemy.h"
#include "raymath.h"
#include <stdlib.h>

List* waves;

static float total_elapsed_time;
static float time_per_event;
static float wave_id;

static float intensity;
static float density;

static int current_wave_number;
static bool endless_mode;
static float next_wave_start_time;

#define BOSS_SPAWN_NUM_X_COLUMNS 3
#define BOSS_SPAWN_NUM_Y_ROWS 3

#pragma region HP_FUNCTIONS

static float GetEnemyBaseHp(EnemyType type) {
    switch (type) {

    case ENEMY_BASIC:   return 1.0f;
    case ENEMY_ZIGZAG:  return 1.0f;
    case ENEMY_BOOSTER: return 1.5f;
    case ENEMY_WALLER:  return 2.0f;
    case ENEMY_GHOST:   return 3.0f;
    case ENEMY_STALKER: return 5.0f;

    case ENEMY_BOSS_PIDGEON_OF_PREY: return 25.0f;

    case ENEMY_SPINNER:
    case ENEMY_REVERSE_SPINNER: return 1.2f;

    default: return 1.0f;

    }

}

static float GetEnemyHpGrowthFactor(EnemyType type) {
    switch (type) {

    case ENEMY_BASIC:   return 0.75f;
    case ENEMY_ZIGZAG:  return 1.00f;
    case ENEMY_BOOSTER: return 1.15f;
    //case ENEMY_WALLER:  return 1.25f;
    case ENEMY_GHOST:   return 1.25f;
    case ENEMY_STALKER: return 3.00f;

    case ENEMY_BOSS_PIDGEON_OF_PREY: return 12.0f;

    case ENEMY_SPINNER:
    case ENEMY_REVERSE_SPINNER: return 0.85f;

    default: return 1.0f;

    }
}

float GetEnemyHitpoints(EnemyType type) {
	return GetEnemyBaseHp(type) + GetEnemyHpGrowthFactor(type) * intensity;
}

#pragma endregion

int GetCurrentWaveNumber(void) {
	return current_wave_number;
}

typedef struct EnemyWave {
    int wave_id;
    float start_time;
    int modifier;
    List* spawns;
} EnemyWave;

#pragma region spawners

static EnemyWave* CreateWave(EnemyWave* wave, int id, EnemyType type, float start_time, int modifier) {
    wave->wave_id = id;
    wave->start_time = start_time;
    wave->modifier = (modifier % 2);
    wave->spawns = List_Create(sizeof(Enemy));
}

#pragma region FORMATIONS

static void CreateLineAtBorders(int id, EnemyType type, float start_time, int modifier) {
    EnemyWave wave;
    CreateWave(&wave, id, type, start_time, modifier);
    
    int starting_x;
    int step;
    
    if (wave.modifier == 0) {
        starting_x = GAME_SCREEN_START + 20;
        step = 40;
    } else {
        starting_x = GAME_SCREEN_END - 20;
        step = -40;
    }

    if (current_wave_number < 10) {
        for (int i = 0; i < 3; i++) {
            Vector2 position = { starting_x + i * step, ENEMY_LINE_SPAWN_START };

            Enemy enemy;
            InitEnemy(&enemy, position, type, GetEnemyHitpoints(type));
            List_Add(wave.spawns, &enemy);
        }
    }
    else {
        for (int i = 0; i < 3 + intensity * INTENSITY_SPAWN_FACTOR; i++) {
            Vector2 position = { starting_x + i * step, ENEMY_LINE_SPAWN_START };

            Enemy enemy;
            InitEnemy(&enemy, position, type, GetEnemyHitpoints(type));
            List_Add(wave.spawns, &enemy);
        }
    }

    List_AddLast(waves, &wave);
}

static int CreateVFormation(int id, EnemyType type, float start_time, int modifier) {
    EnemyWave wave;
    CreateWave(&wave, id, type, start_time, modifier);

    int enemy_count = 3 + (int)(intensity * INTENSITY_SPAWN_FACTOR);
    int rows = (int)(enemy_count / 2.0f);
    
    float center_x = GAME_SCREEN_CENTER;
    float starting_y = ENEMY_LINE_SPAWN_START;
    
    float vertical_spacing = 50.0f;
    float horizontal_spacing = 50.0f;
    
    float start_y = ENEMY_LINE_SPAWN_START;

    Enemy middle_enemy;

    InitEnemy(
        &middle_enemy,
        (Vector2){ center_x, starting_y - wave.modifier * rows * horizontal_spacing },
        type, GetEnemyHitpoints(type));

    List_Add(wave.spawns, &middle_enemy);
    
    Vector2 left_pos = { center_x, starting_y - wave.modifier * rows * horizontal_spacing };
    Vector2 right_pos = { center_x, starting_y - wave.modifier * rows * horizontal_spacing };
    
    for (int i = 0; i < rows; i++) {
        left_pos.x -= vertical_spacing;
        left_pos.y -= horizontal_spacing * ((modifier == 0) ? 1 : -1);

        Enemy left_enemy;
        InitEnemy(&left_enemy, left_pos, type, GetEnemyHitpoints(type));
        List_Add(wave.spawns, &left_enemy);


        right_pos.x += vertical_spacing;
        right_pos.y -= horizontal_spacing * ((modifier == 0) ? 1 : -1);

        Enemy right_enemy;
        InitEnemy(&right_enemy, right_pos, type, GetEnemyHitpoints(type));
        List_Add(wave.spawns, &right_enemy);
    }
    
    List_AddLast(waves, &wave);
}

static void CreateCentralLine(int id, EnemyType type, float start_time, int modifier) {
    EnemyWave wave;
    CreateWave(&wave, id, type, start_time, modifier);

    int starting_x;
    int step;


    const int screen_area = GAME_SCREEN_END - GAME_SCREEN_START;
    const int enemy_count = 3 + intensity * INTENSITY_SPAWN_FACTOR;
    static const int enemy_spacing = 50;

    for(int i = 0; i < enemy_count; i++) {
        Vector2 position; 

        if (modifier == 0) {
            position = (Vector2) { GAME_SCREEN_CENTER, ENEMY_LINE_SPAWN_START * i };
        } else {
            const int half_left = (enemy_count / 2) * enemy_spacing;

            position = (Vector2) {
                GAME_SCREEN_CENTER - half_left + i * enemy_spacing,
                ENEMY_LINE_SPAWN_START * i
            };
        }

        Enemy enemy;
        InitEnemy(&enemy, position, type, GetEnemyHitpoints(type));
        List_Add(wave.spawns, &enemy);
    }

    List_AddLast(waves, &wave);
}

#pragma endregion

static void CreateSingleBoss(int id, EnemyType type, float start_time, Vector2 initial_position, int boss_spawn_x, int boss_spawn_y) {
    EnemyWave wave;
    CreateWave(&wave, id, type, start_time, 0);

    Vector2 position = { GAME_SCREEN_CENTER, ENEMY_LINE_SPAWN_START };

    Enemy enemy;
    InitEnemy(&enemy, position, type, GetEnemyHitpoints(type));
    enemy.boss_spawn_x = boss_spawn_x;
    enemy.boss_spawn_y = boss_spawn_y;
    enemy.position = initial_position;

    List_Add(wave.spawns, &enemy);
    List_AddLast(waves, &wave);
}

static void CreateSingle(int id, EnemyType type, float start_time, Vector2 initial_position) {
    CreateSingleBoss(id, type, start_time, initial_position, -1, -1);
}

static void CreateGhostTrio(int id, float start_time) {
    CreateSingle(id, ENEMY_GHOST, start_time, (Vector2) { GAME_SCREEN_CENTER, -500 });
    CreateSingle(id, ENEMY_GHOST, start_time, (Vector2) { GAME_SCREEN_CENTER, -500 });
    CreateSingle(id, ENEMY_GHOST, start_time, (Vector2) { GAME_SCREEN_CENTER, -500 });
}

static void CreateStalker(int id, float start_time) {
	CreateSingle(id, ENEMY_STALKER, start_time, (Vector2) { GAME_SCREEN_CENTER, ENEMY_LINE_SPAWN_START });
}

static void SetIfSlotIsBeingUsed(
    int boss_spawn_position[BOSS_SPAWN_NUM_Y_ROWS][BOSS_SPAWN_NUM_X_COLUMNS],
    Enemy* enemy
) {
    if (enemy->boss_spawn_y == -1 || enemy->boss_spawn_x == -1)
        return;

    boss_spawn_position[enemy->boss_spawn_y][enemy->boss_spawn_x] = 1;
}

static GetUsedBossSpawnPoints(
    int boss_spawn_position[BOSS_SPAWN_NUM_Y_ROWS][BOSS_SPAWN_NUM_X_COLUMNS]
) {
    List_ForEachCtx(enemies, boss_spawn_position, SetIfSlotIsBeingUsed);
}

static Vector2 GetRandomAvailableBossSpawnPoint() {
    int start_pos_x = 0;
    int start_pos_y = 0;

    bool is_spawn_point_busy = true;

    int spawn_x = -1;
    int spawn_y = -1;

    int boss_spawn_position[BOSS_SPAWN_NUM_Y_ROWS][BOSS_SPAWN_NUM_X_COLUMNS] = { 0 };
    GetUsedBossSpawnPoints(boss_spawn_position);

    List* available_slots = List_Create(sizeof(Vector2));

    for(int y = 0; y < BOSS_SPAWN_NUM_Y_ROWS; y++) {
        for(int x = 0; x < BOSS_SPAWN_NUM_X_COLUMNS; x++) {
            if (boss_spawn_position[y][x] == 0) {
                Vector2 pos = { x, y };
                List_Add(available_slots, &pos);
            }
        }
    }

    if (available_slots->size == 0) {
        return (Vector2){ -1, -1 };
    }

    return *(Vector2*)List_GetByIndex(available_slots, GetRandomValue(0, available_slots->size - 1));
}

static void CreatePidgeonOfPrey(int id, float start_time) {
    Vector2 spawn_points = GetRandomAvailableBossSpawnPoint();

    if (spawn_points.x == -1 || spawn_points.y == -1)
        return;

    int boss_spawn_position[BOSS_SPAWN_NUM_Y_ROWS][BOSS_SPAWN_NUM_X_COLUMNS] = { 0 };
    GetUsedBossSpawnPoints(boss_spawn_position);

    TraceLog(LOG_WARNING,
        "[[%d, %d, %d], [%d, %d, %d], [%d, %d, %d]]",
        boss_spawn_position[0][0],
        boss_spawn_position[0][1],
        boss_spawn_position[0][2],
        boss_spawn_position[1][0],
        boss_spawn_position[1][1],
        boss_spawn_position[1][2],
        boss_spawn_position[2][0],
        boss_spawn_position[2][1],
        boss_spawn_position[2][2]);

    const int offset_x = 350;
    const int offset_y = 350;
    const int initial_x = GAME_SCREEN_CENTER - offset_x / 2.0f;
    const int initial_y = - 500 - offset_y / 2.0f;

    int start_x_coord = initial_x + Lerp(0, 1, spawn_points.x / (BOSS_SPAWN_NUM_X_COLUMNS - 1)) * offset_x;
    int start_y_coord = initial_y + Lerp(0, 1, spawn_points.y / (BOSS_SPAWN_NUM_Y_ROWS - 1)) * offset_y;

    CreateSingleBoss(
        id,
        ENEMY_BOSS_PIDGEON_OF_PREY,
        start_time,
        (Vector2) { start_x_coord, start_y_coord },
        spawn_points.x,
        spawn_points.y);
}

static Enemy* CreateSingleForDebug(EnemyType type, float start_time) {
    EnemyWave wave;
    CreateWave(&wave, 0, type, start_time, 0);
    
    Vector2 position = { GAME_SCREEN_CENTER, ENEMY_LINE_SPAWN_START };

    Enemy enemy;
    InitEnemy(&enemy, position, type, GetEnemyHitpoints(type));
    List_Add(wave.spawns, &enemy);

    List_AddLast(waves, &wave);

    return &enemy;
}

#pragma endregion

int GetWaveEnemies(int wave_no) {
    // Wave table
    if (wave_no == 0) return 0;
	if (wave_no == 1) return 4;
	if (wave_no == 2) return 1;
	if (wave_no > 2 && wave_no <= 9) return GetRandomValue(0, 7);   // Basic, spinners, zig-zag, booster
	if (wave_no > 9 && wave_no <= 14) return GetRandomValue(0, 8);  // + Ghost
	if (wave_no > 14 && wave_no <= 24) return GetRandomValue(0, 9);  // + Stalker
    if (wave_no == 25) return 10;
	return GetRandomValue(0, 10); // + Pidgeon of Prey
}

void GenerateWaves() {
    if (!endless_mode && current_wave_number >= MAX_WAVES)
        return;

    int wave_type = GetWaveEnemies(current_wave_number);
    int modifier = GetRandomValue(0, 1);

    switch(wave_type) {
        // BASIC
        case 0:
            CreateVFormation(wave_id++, ENEMY_BASIC, next_wave_start_time, modifier);
            break;
        case 1:
            CreateLineAtBorders(wave_id++, ENEMY_BASIC, next_wave_start_time, modifier);
            break;
        // SPINNER
        case 2:
            if (GetRandomValue(0, 1))
                CreateVFormation(wave_id++, ENEMY_SPINNER, next_wave_start_time, modifier);
            else
                CreateVFormation(wave_id++, ENEMY_REVERSE_SPINNER, next_wave_start_time, modifier);
            break;
        case 3:
            if (GetRandomValue(0, 1))
                CreateCentralLine(wave_id++, ENEMY_SPINNER, next_wave_start_time, modifier);
            else 
                CreateCentralLine(wave_id++, ENEMY_REVERSE_SPINNER, next_wave_start_time, modifier);
            break;
        // ZIG-ZAG
        case 4:
            CreateCentralLine(wave_id++, ENEMY_ZIGZAG, next_wave_start_time, modifier);
            break;
        case 5:
            CreateVFormation(wave_id++, ENEMY_ZIGZAG, next_wave_start_time, modifier);
            break;
        // BOOSTER
        case 6:
            CreateVFormation(wave_id++, ENEMY_BOOSTER, next_wave_start_time, modifier);
            break;
        case 7:
            CreateCentralLine(wave_id++, ENEMY_BOOSTER, next_wave_start_time, modifier);
            break;
        // GHOST
        case 8:
            CreateGhostTrio(wave_id++, next_wave_start_time);
            break;
        case 9:
            CreateStalker(wave_id++, next_wave_start_time);
            break;
        // PIDGEON OF PREY
        case 10:
            CreatePidgeonOfPrey(wave_id++, next_wave_start_time);
            break;
        default:
            CreateVFormation(wave_id++, ENEMY_BASIC, next_wave_start_time, modifier);
            //CreateSingleForDebug(ENEMY_GHOST, next_wave_start_time);
            break;
    }

    next_wave_start_time += TIME_BETWEEN_WAVES - density;
}

void InitWaves(bool is_endless_mode) {
    total_elapsed_time = 0.0f;
    time_per_event = 0.0f;
    wave_id = 0;

    intensity = 1.0f;
	density = 0.0f;

    current_wave_number = 0;

    endless_mode = is_endless_mode;
    next_wave_start_time = FIRST_WAVE_SPAWN_TIMER;
    last_pidgeon_spawn = 25;

    waves = List_Create(sizeof(EnemyWave));
}

void UpdateWaves(void) {
    total_elapsed_time += GetFrameTime();

    if (waves == NULL)
        return;

    if (waves->size > 0) {
        EnemyWave wave = *(EnemyWave*)List_GetByIndex(waves, 0);

        if (wave.start_time < total_elapsed_time) {
            SpawnEnemies(wave.spawns);
            List_Destroy(wave.spawns);
            List_RemoveFirst(waves);

            if (!endless_mode)
                current_wave_number++;
            
            INTENSITY_UPDATE_EQUATION;
            DENSITY_UPDATE_EQUATION;
        }

        return;
    }

    GenerateWaves();
}

bool AreAllWavesCompleted(void) {
    if (endless_mode) {
        return false;
    }
    return waves != NULL && waves->size == 0 && current_wave_number >= MAX_WAVES && enemies->size == 0;
}
