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

typedef struct Wave {
    int wave_id;
    float start_time;
    int modifier;
    float intensity;
    List* spawns;
} Wave;

#pragma region spawners

static Wave* CreateWave(Wave* wave, int id, EnemyType type, float start_time, int modifier, float intensity) {
    wave->wave_id = id;
    wave->start_time = start_time;
    wave->modifier = (modifier % 2);
    wave->intensity = intensity;
    wave->spawns = List_Create(sizeof(Enemy));
}

static void CreateLineAtBorders(int id, EnemyType type, float start_time, int modifier, float intensity) {
    Wave wave;
    CreateWave(&wave, id, type, start_time, modifier, intensity);
    
    int starting_x;
    int step;
    
    if (wave.modifier == 0) {
        starting_x = GAME_SCREEN_START + 20;
        step = 40;
    } else {
        starting_x = GAME_SCREEN_END - 20;
        step = -40;
    }

    for(int i = 0; i < 5 + intensity / 2; i++) {
        Vector2 position = { starting_x + i * step, ENEMY_LINE_SPAWN_START };

        Enemy enemy;
        ActivateEnemy(&enemy, position, type, 3 + intensity / 2);
        List_Add(wave.spawns, &enemy);
    }

    List_AddLast(waves, &wave);
}

static int CreateVFormation(int id, EnemyType type, float start_time, int modifier, float intensity) {
    Wave wave;
    CreateWave(&wave, id, type, start_time, modifier, intensity);
    

    int enemy_count = 5 + (int)(intensity / 2.0f);
    int rows = (int)(enemy_count / 2.0f);
    
    float center_x = GAME_SCREEN_CENTER;
    float starting_y = ENEMY_LINE_SPAWN_START;
    
    float vertical_spacing = 50.0f;
    float horizontal_spacing = 50.0f;
    
    float start_y = ENEMY_LINE_SPAWN_START;

    Enemy middle_enemy;
    ActivateEnemy(
        &middle_enemy,
        (Vector2){ center_x, starting_y - wave.modifier * rows * horizontal_spacing },
        type, 3 + intensity / 2);
    List_Add(wave.spawns, &middle_enemy);
    
    Vector2 left_pos = { center_x, starting_y - wave.modifier * rows * horizontal_spacing };
    Vector2 right_pos = { center_x, starting_y - wave.modifier * rows * horizontal_spacing };
    
    for (int i = 0; i < rows; i++) {
        left_pos.x -= vertical_spacing;
        left_pos.y -= horizontal_spacing * ((modifier == 0) ? 1 : -1);

        Enemy left_enemy;
        ActivateEnemy(&left_enemy, left_pos, type, 3 + intensity / 2);
        List_Add(wave.spawns, &left_enemy);


        right_pos.x += vertical_spacing;
        right_pos.y -= horizontal_spacing * ((modifier == 0) ? 1 : -1);

        Enemy right_enemy;
        ActivateEnemy(&right_enemy, right_pos, type, 3 + intensity / 2);
        List_Add(wave.spawns, &right_enemy);
    }
    
    List_AddLast(waves, &wave);
}

static void CreateCentralLine(int id, EnemyType type, float start_time, int modifier, float intensity) {
    Wave wave;
    CreateWave(&wave, id, type, start_time, modifier, intensity);
    
    int starting_x;
    int step;

    int screen_area = GAME_SCREEN_END - GAME_SCREEN_START;
    
    for(int i = 0; i < 3 + intensity / 3; i++) {
        Vector2 position = (modifier == 0) ?
            (Vector2) {
                GAME_SCREEN_CENTER,
                ENEMY_LINE_SPAWN_START * i
            } 
            :
            (Vector2) {
                GAME_SCREEN_START + screen_area * (float)GetRandomValue(0, RAND_MAX) / (float)RAND_MAX,
                ENEMY_LINE_SPAWN_START
            };

        Enemy enemy;
        ActivateEnemy(&enemy, position, type, 3 + intensity / 2);
        List_Add(wave.spawns, &enemy);
    }

    List_AddLast(waves, &wave);
}

static Enemy* CreateSingle(int id, EnemyType type, float start_time, int modifier, float intensity) {
    Wave wave;
    CreateWave(&wave, id, type, start_time, modifier, intensity);
    
    Vector2 position = { GAME_SCREEN_CENTER, ENEMY_LINE_SPAWN_START };

    Enemy enemy;
    ActivateEnemy(&enemy, position, type, 5);
    List_Add(wave.spawns, &enemy);

    List_AddLast(waves, &wave);

    return &enemy;
}

static void CreateGhost(int id, float start_time, int modifier, float intensity) {
    Enemy* enemy = CreateSingle(id, ENEMY_GHOST, start_time, modifier, intensity);
    enemy->position.y = -500.0f;
    enemy->position.x = GAME_SCREEN_CENTER;
}

static Enemy* CreateSingleForDebug(EnemyType type, float start_time) {
    Wave wave;
    CreateWave(&wave, 0, type, start_time, 0, 0);
    
    Vector2 position = { GAME_SCREEN_CENTER, ENEMY_LINE_SPAWN_START };

    Enemy enemy;
    ActivateEnemy(&enemy, position, type, 5);
    List_Add(wave.spawns, &enemy);

    List_AddLast(waves, &wave);

    return &enemy;
}

#pragma endregion

void GenerateWaves(int level) {
    float intensity = (float)level / 2.0f;
    float start_time = 5;

    for(int i = 0; i < level + 1; i++) {
        int wave_type = GetRandomValue(99, 100);
        int modifier = GetRandomValue(0, 1);

        switch(wave_type) {
            // BASIC
            case 0:
                CreateLineAtBorders(wave_id++, ENEMY_BASIC, start_time, modifier, intensity);
                break;
            case 1:
                CreateVFormation(wave_id++, ENEMY_BASIC, start_time, modifier, intensity);
                break;
            // SPINNER
            case 2:
                CreateVFormation(wave_id++, ENEMY_SPINNER, start_time, modifier, intensity);
                break;
            case 3:
                CreateCentralLine(wave_id++, ENEMY_SPINNER, start_time, modifier, intensity);
                break;
            // ZIG-ZAG
            case 4:
                CreateCentralLine(wave_id++, ENEMY_ZIGZAG, start_time, modifier, intensity);
                break;
            case 5:
                CreateVFormation(wave_id++, ENEMY_ZIGZAG, start_time, modifier, intensity);
                break;
            // BOOSTER
            case 6:
                CreateVFormation(wave_id++, ENEMY_BOOSTER, start_time, modifier, intensity);
                break;
            case 7:
                CreateCentralLine(wave_id++, ENEMY_BOOSTER, start_time, modifier, intensity);
                break;
            // GHOST
            case 8:
                CreateGhost(wave_id++, start_time, modifier, intensity);
                break;
            default: 
                CreateSingleForDebug(ENEMY_BOSS_PIDGEON_OF_PREY, start_time);
                break;
        }

        start_time += 8;
    }
}

void InitWaves(int level) {
    total_elapsed_time = 0.0f;
    time_per_event = 0.0f;
    wave_id = 0;

    waves = List_Create(sizeof(Wave));

    GenerateWaves(level);
}

void UpdateWaves(void) {
    float dt = GetFrameTime();
    total_elapsed_time += dt;

    if (waves != NULL && waves->size > 0) {
        Wave wave = *(Wave*)List_GetByIndex(waves, 0);

        if (wave.start_time < total_elapsed_time) {
            SpawnEnemies(wave.spawns);
            List_Destroy(wave.spawns);
            List_RemoveFirst(waves);
        }
    }
}

bool AreAllWavesCompleted(void) {
    return waves != NULL && waves->size == 0;
}
