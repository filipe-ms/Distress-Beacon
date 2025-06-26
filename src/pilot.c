#include "pilot.h"
#include "texture_manager.h"
#include "common.h"

typedef struct Pilot {
    int current_frame;
    float elapsed_time;
    float total_animation_time;
} Pilot;

typedef struct {
    Rectangle base_rect;
    int frame_count;
    float frame_duration;
} PilotSpriteData;

static const PilotSpriteData PILOT_DATA[PLAYABLE_SHIPS] = {
    [ORION]         = {{ 0, 64, 8, 8 }, 5, 0.15f},
    [AUREA]         = {{ 0, 16, 8, 8 }, 5, 0.2f},
    [NEBULA]        = {{ 0, 48, 8, 8 }, 5, 0.2f},
    [PUDDLE_JUMPER] = {{ 0, 40, 8, 8 }, 5, 0.2f},
    [VOID]          = {{ 0, 72, 8, 8 }, 5, 0.2f},
};

static Pilot pilot = { 0 };

void TriggerPilotAnimation(float duration) {
    pilot.total_animation_time = duration;
    pilot.current_frame = 0;
    pilot.elapsed_time = 0.0f;
}

void DrawPilot(int ship, int pos_x, int pos_y, float scale, Color tint) {
    if (ship < 0 || ship >= PLAYABLE_SHIPS) return;

    int frame_to_draw = 0;
    const PilotSpriteData* data = &PILOT_DATA[ship];

    if (pilot.total_animation_time > 0) {
        pilot.total_animation_time -= GetFrameTime();
        pilot.elapsed_time += GetFrameTime();

        if (pilot.elapsed_time >= data->frame_duration) {
            pilot.elapsed_time = 0.0f;
            pilot.current_frame = (pilot.current_frame + 1) % data->frame_count;
        }

        frame_to_draw = pilot.current_frame;

        if (pilot.total_animation_time <= 0) {
            pilot.total_animation_time = 0;
            pilot.current_frame = 0;
        }
    }

    Rectangle source = data->base_rect;
    source.x += frame_to_draw * source.width;

    float size = source.width * scale;
    Rectangle dest = { pos_x, pos_y, size, size };
    Vector2 origin = { size / 2.0f, size / 2.0f };

    DrawTexturePro(texture_pilots, source, dest, origin, 0.0f, tint);
}
