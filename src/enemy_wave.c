#include "enemy_wave.h"
#include "raylib.h"
#include "common.h"
#include "enemy.h"

// Variáveis globais para o estado das ondas
int current_wave_index;
int next_spawn_event_index;
float wave_timer;
float time_per_event;
float message_alpha;
bool message_is_fading_out;
bool is_active;
bool all_waves_completed;

// Estrutura para um evento de spawn de inimigos
typedef struct SpawnEvent {
    int specific_enemy_type;
    int specific_enemy_count;
    int random_enemy_count;
    int enemy_health;
} SpawnEvent;

// Estrutura para uma onda de inimigos
typedef struct Wave {
    const char* name;
    float total_duration;
    int spawn_events;
    // O tamanho do array deve ser suficiente para a maior onda
    SpawnEvent events[10];
} Wave;

// --- CORREÇÃO: Array de ondas inicializado globalmente (estilo C89) ---
// Motivo: Para evitar erros em compiladores mais antigos que não suportam
// a sintaxe de inicialização do C99 (compound literals e designated initializers).
// Os dados agora são definidos no momento da compilação.
Wave waves[MAX_WAVES] = {
    // Wave 0: "FIRST WAVE"
    {
        "FIRST WAVE",       // .name
        60.0f,              // .total_duration
        4,                  // .spawn_events
        {                   // .events
            {0, 6, 0, 3},   // event 0
            {0, 5, 2, 3},   // event 1
            {1, 4, 4, 3},   // event 2
            {2, 3, 5, 3}    // event 3
            // O restante dos 10 eventos são inicializados com 0
        }
    },

    // Wave 1: "SECOND WAVE"
    {
        "SECOND WAVE",      // .name
        50.0f,              // .total_duration
        5,                  // .spawn_events
        {                   // .events
            {1, 4, 4, 4},   // event 0
            {2, 4, 4, 4},   // event 1
            {-1, 0, 8, 4},  // event 2
            {-1, 0, 10, 4}, // event 3
            {-1, 0, 12, 4}  // event 4
        }
    },

    // Wave 2: "THIRD WAVE"
    {
        "THIRD WAVE",       // .name
        40.0f,              // .total_duration
        4,                  // .spawn_events
        {                   // .events
            {2, 3, 5, 5},   // event 0
            {-1, 0, 8, 5},  // event 1
            {-1, 0, 10, 5}, // event 2
            {-1, 0, 12, 5}  // event 3
        }
    }
};


static void StartNextWave(void) {
    current_wave_index++;
    if (current_wave_index >= MAX_WAVES) {
        all_waves_completed = true;
        is_active = false;
        return;
    }

    const Wave* currentWave = &waves[current_wave_index];
    wave_timer = currentWave->total_duration;
    next_spawn_event_index = 0;

    // Intervalo de tempo entre cada evento de spawn
    if (currentWave->spawn_events > 0) {
        time_per_event = currentWave->total_duration / (float)currentWave->spawn_events;
    }
    else {
        time_per_event = 0;
    }

    // Reseta o efeito de fade da mensagem da onda
    message_alpha = 1.0f; // Começa visível
    message_is_fading_out = true; // Começa a fazer fade-out após um tempo
}

void InitWaves(void) {
    current_wave_index = -1;
    next_spawn_event_index = 0;
    wave_timer = 0.0f;
    time_per_event = 0.0f;
    message_alpha = 0.0f;
    message_is_fading_out = false;
    is_active = true;
    all_waves_completed = false;

    // --- CORREÇÃO: A chamada para carregar dados foi removida ---
    // Os dados agora são carregados na inicialização global do array 'waves'.
    StartNextWave();
}

void UpdateWaves(void) {
    if (!is_active) return;

    float dt = GetFrameTime();
    wave_timer -= dt;

    const Wave* currentWave = &waves[current_wave_index];

    // Lógica de Fade-out da mensagem da onda
    if (message_is_fading_out) {
        // A mensagem permanece visível por 2.5 segundos e então começa o fade-out
        if (wave_timer < currentWave->total_duration - 2.5f) {
            message_alpha -= 0.8f * dt; // Fade-out
            if (message_alpha < 0.0f) {
                message_alpha = 0.0f;
            }
        }
    }

    if (wave_timer <= 0) {
        // Verifica se ainda há inimigos antes de iniciar a próxima onda (opcional, mas recomendado)
        // if (GetActiveEnemiesCount() == 0) {
        StartNextWave();
        // }
        return;
    }

    // Checa pelos eventos de spawn
    if (next_spawn_event_index < currentWave->spawn_events) {
        float next_event_trigger_time = currentWave->total_duration - ((next_spawn_event_index + 1) * time_per_event);

        if (wave_timer <= next_event_trigger_time) {
            const SpawnEvent* event = &currentWave->events[next_spawn_event_index];

            if (event->specific_enemy_count > 0 && event->specific_enemy_type != -1) {
                SpawnEnemies(event->specific_enemy_count, event->specific_enemy_type, event->enemy_health);
            }
            if (event->random_enemy_count > 0) {
                SpawnRandomEnemies(event->random_enemy_count, event->enemy_health);
            }

            next_spawn_event_index++;
        }
    }
}

void DrawWaves(void) {
    if (all_waves_completed || message_alpha <= 0.0f) return;

    if (current_wave_index < MAX_WAVES) {
        const char* text = waves[current_wave_index].name;
        int text_width = MeasureText(text, 40);
        DrawText(text, GetScreenWidth() / 2 - text_width / 2, GetScreenHeight() / 2 - 100, 40, Fade(WHITE, message_alpha));
    }
}

bool AreAllWavesCompleted(void) {
    return all_waves_completed;
}
