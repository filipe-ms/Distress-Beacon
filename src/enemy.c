#include "enemy.h"
#include "common.h"
#include "behavior.h"
#include "raylib.h"

// --- Definições Internas do Módulo ---

// Tipos de inimigos, usados internamente.
typedef enum {
    BASIC,
    ZIGZAG,
    BOOSTER,
    WALLER,
    BOSS
} EnemyType;

// Estrutura que define um único inimigo.
typedef struct {
    bool active;
    Rectangle position;
    Vector2 speed;
    int type;
    float hp;
    float exp;
    float move_time;
    bool action_flag;
    Color color;
} Enemy;

// Estrutura que encapsula TODO o estado do módulo de inimigos (Singleton).
static struct {
    Enemy enemies[MAX_ENEMY_NUMBER];
    Texture2D texture;
    Rectangle source_rects[5]; // Retângulos de origem na spritesheet para cada tipo.
} state;


// --- Funções Auxiliares (Internas) ---

// Configura e ativa um inimigo a partir do pool.
// Esta função corrige a lógica de spawn que estava com problemas.
static void ActivateEnemy(Enemy* enemy, int type, int hp) {
    enemy->active = true;
    enemy->type = type;
    enemy->hp = (float)hp;
    enemy->color = WHITE;
    enemy->move_time = 0.0f;
    enemy->action_flag = false;

    // Posição de spawn aleatória fora do topo da tela.
    enemy->position.x = (float)GetRandomValue(0, (int)(GetScreenWidth() - enemy->position.width));
    enemy->position.y = (float)GetRandomValue(-200, -50);

    // Reseta a velocidade
    enemy->speed = (Vector2){ 0, 120.0f };

    // Define a experiência baseada no tipo.
    switch (type) {
    case ZIGZAG:  enemy->exp = 20.0f; break;
    case BOOSTER: enemy->exp = 25.0f; break;
    case WALLER:  enemy->exp = 15.0f; break;
    default:      enemy->exp = 10.0f; break;
    }
}


// --- Funções Públicas ---

void InitEnemies(void) {
    // Inicializa o pool de inimigos como inativos.
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        state.enemies[i].active = false;
        state.enemies[i].position.width = 48;
        state.enemies[i].position.height = 48;
    }
}

void SpawnEnemies(int amount, int type, int hp) {
    int spawned_count = 0;
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        if (spawned_count >= amount) break; // Sai se já spawnou a quantidade pedida.

        if (!state.enemies[i].active) {
            ActivateEnemy(&state.enemies[i], type, hp);
            spawned_count++;
        }
    }
}

void SpawnRandomEnemies(int amount, int hp) {
    int spawned_count = 0;
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        if (spawned_count >= amount) break;

        if (!state.enemies[i].active) {
            int random_type = GetRandomValue(BASIC, WALLER); // Spawna um dos 4 tipos básicos.
            ActivateEnemy(&state.enemies[i], random_type, hp);
            spawned_count++;
        }
    }
}

void UpdateEnemies(Ship* ship) {
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        if (!state.enemies[i].active) continue;

        Enemy* e = &state.enemies[i]; // Ponteiro para o inimigo atual para facilitar a leitura.

        // Aplica o comportamento correspondente ao tipo.
        switch (e->type) {
        case BASIC:   BehaviorBasic(&e->position, 120.0f); break;
        case ZIGZAG:  BehaviorZigZag(&e->position, 120.0f, &e->speed.x, &e->move_time, &e->action_flag); break;
        case BOOSTER: BehaviorBooster(&e->position, ship, &e->move_time, &e->action_flag); break;
        case WALLER:  BehaviorWaller(&e->position, e->speed.x); break;
        default:      BehaviorBasic(&e->position, 120.0f); break;
        }

        // Mantém o inimigo dentro das bordas laterais da tela.
        if (e->position.x <= 0) {
            e->position.x = 0;
            if (e->type == ZIGZAG) e->speed.x *= -1;
        }
        else if (e->position.x >= GetScreenWidth() - e->position.width) {
            e->position.x = GetScreenWidth() - e->position.width;
            if (e->type == ZIGZAG) e->speed.x *= -1;
        }

        // Desativa o inimigo se ele sair pela parte inferior da tela.
        if (e->position.y > GetScreenHeight() + 20) {
            e->active = false;
        }
    }
}

void DrawEnemies(void) {
    Vector2 origin = { 0, 0 }; // Origem para a função DrawTexturePro.
    for (int i = 0; i < MAX_ENEMY_NUMBER; i++) {
        if (state.enemies[i].active) {
            DrawTexturePro(
                state.texture,
                state.source_rects[state.enemies[i].type],
                state.enemies[i].position,
                origin,
                0,
                state.enemies[i].color
            );
        }
    }
}

void LoadEnemyTextures(void) {
    state.texture = LoadTexture("ships.png");

    // Define os retângulos de origem para cada tipo de inimigo.
    state.source_rects[BASIC] = (Rectangle){ 32, 0, 8, 8 };
    state.source_rects[ZIGZAG] = (Rectangle){ 32, 24, 8, 8 };
    state.source_rects[BOOSTER] = (Rectangle){ 48, 24, 8, 8 };
    state.source_rects[WALLER] = (Rectangle){ 32, 8, 8, 8 };
    state.source_rects[BOSS] = (Rectangle){ 16, 8, 8, 8 };
}

void UnloadEnemyTextures(void) {
    UnloadTexture(state.texture);
}
