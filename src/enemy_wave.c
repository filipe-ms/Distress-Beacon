#include "enemy_wave.h"
#include "raylib.h"
#include "common.h"
#include "enemy.h"

int current_wave_index;
int next_spawn_event_index;
float wave_timer;
float time_per_event;
float message_alpha;
bool message_is_fading_out;
bool is_active;
bool all_waves_completed;

typedef struct SpawnEvent {
    int specific_enemy_type;
    int specific_enemy_count;
    int random_enemy_count;
    int enemy_health;
} SpawnEvent;

typedef struct Wave {
    const char* name;
    float total_duration;
    int spawn_events;
    SpawnEvent events[10];
} Wave;

Wave waves[MAX_WAVES] = {
    // Wave 0: "FIRST WAVE"
    {
        "FIRST WAVE",
        60.0f,
        4,
        {
            {0, 6, 0, 3},
            {0, 5, 2, 3},
            {1, 4, 4, 3},
            {2, 3, 5, 3}
        }
    },

    // Wave 1: "SECOND WAVE"
    {
        "SECOND WAVE",
        50.0f,
        5,
        {
            {1, 4, 4, 4},
            {2, 4, 4, 4},
            {1, 0, 8, 4},
            {2, 0, 10, 4},
            {2, 0, 12, 4}
        }
    },

    // Wave 2: "THIRD WAVE"
    {
        "THIRD WAVE",
        40.0f,
        4,
        {
            {2, 3, 5, 5},
            {2, 0, 8, 5},
            {2, 0, 10, 5},
            {2, 0, 12, 5}
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

    if (currentWave->spawn_events > 0) {
        time_per_event = currentWave->total_duration / (float)currentWave->spawn_events;
    }
    else {
        time_per_event = 0;
    }

    message_alpha = 1.0f;
    message_is_fading_out = true;
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

    StartNextWave();
}

void UpdateWaves(void) {
    if (!is_active) return;

    float dt = GetFrameTime();
    wave_timer -= dt;

    const Wave* currentWave = &waves[current_wave_index];

    // Lógica de Fade-out da mensagem
    if (message_is_fading_out) {
        if (wave_timer < currentWave->total_duration - 2.5f) {
            message_alpha -= 0.8f * dt; // Fade-out
            if (message_alpha < 0.0f) {
                message_alpha = 0.0f;
            }
        }
    }

    if (wave_timer <= 0) {
        StartNextWave();
        return;
    }

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
        DrawText(text, GAME_SCREEN_START / 2 - text_width / 2, SCREEN_HEIGHT / 2 - 100, 40, Fade(WHITE, message_alpha));
    }
}

bool AreAllWavesCompleted(void) {
    return all_waves_completed;
}
