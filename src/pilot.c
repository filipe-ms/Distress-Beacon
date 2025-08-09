#include "pilot.h"
#include "texture_manager.h"
#include "common.h"
#include "audio_manager.h"
#include "player.h"

typedef struct Pilot {
	int pilot_id;
    int current_frame;
    float elapsed_time;
    float total_animation_time;

	int pos_x;
	int pos_y;
	float scale;
	Color color;
} Pilot;

typedef struct {
    Rectangle base_rect;
    int frame_count;
    float frame_duration;
} PilotSpriteData;

static const PilotSpriteData PILOT_DATA[TOTAL_PILOT_NUMBER] = {
   // Jogáveis
   [ORION]              = {{ 0, 64, 8, 8 }, 5, 0.15f},
   [AUREA]              = {{ 0, 16, 8, 8 }, 5, 0.15f},
   [NEBULA]             = {{ 0, 48, 8, 8 }, 5, 0.15f},
   [PUDDLE_JUMPER]      = {{ 0, 40, 8, 8 }, 5, 0.15f},
   [VOID]               = {{ 0, 72, 8, 8 }, 5, 0.15f},

   // Não jogáveis
   [DUCK]               = {{ 0, 0, 8, 8 }, 5, 0.15f},
   [ROBOT]              = {{ 0, 8, 8, 8 }, 5, 0.15f},
   [ALIEN]              = {{ 0, 24, 8, 8 }, 5, 0.15f},
   [PIRATE]             = {{ 0, 32, 8, 8 }, 5, 0.15f},
   [INVERSE]            = {{ 0, 56, 8, 8 }, 5, 0.15f},
   [INTERFERENCE]       = {{ 0, 80, 8, 8 }, 5, 0.15f}
};

static Pilot top_pilot = { -1, 0, 0, 0 };
static Pilot bottom_pilot = { -1, 0, 0, 0 };


#pragma region TOP_PILOT

int GetTopPilotId(void) { return top_pilot.pilot_id; }

void SetTopPilot(int pilot_id, int pos_x, int pos_y, float scale, Color color) {
    if (pilot_id < 0 || pilot_id >= PLAYABLE_SHIPS) {
        top_pilot.pilot_id = INTERFERENCE;
	} else {
		top_pilot.pilot_id = pilot_id;
	}
	top_pilot.current_frame = 0;
	top_pilot.elapsed_time = 0.0f;
	top_pilot.total_animation_time = 0.0f;

	top_pilot.pos_x = pos_x;
	top_pilot.pos_y = pos_y;
	top_pilot.scale = scale;
	top_pilot.color = color;
}

void SetTopPilotDefault(void) {
	top_pilot.pilot_id = GetPlayerShip();
	int scale = 16;
	top_pilot.current_frame = 0;
	top_pilot.elapsed_time = 0.0f;
	top_pilot.total_animation_time = 0.0f;
	top_pilot.pos_x = UI_LEFT_CENTER;
	top_pilot.pos_y = SCREEN_HEIGHT * 0.18f - scale / 2;
	top_pilot.scale = scale;
	top_pilot.color = WHITE;
}

void TriggerTopPilotAnimation(float duration) {
    top_pilot.total_animation_time = duration;
    top_pilot.current_frame = 0;
    top_pilot.elapsed_time = 0.0f;
    TriggerPilotSpeech(top_pilot.pilot_id, duration);
}

static void UpdateTopPilot() {
	if (top_pilot.total_animation_time > 0) {
		top_pilot.total_animation_time -= GetFrameTime();
		top_pilot.elapsed_time += GetFrameTime();
		PilotSpriteData* sprite = &PILOT_DATA[top_pilot.pilot_id];
		if (top_pilot.elapsed_time >= sprite->frame_duration) {
			top_pilot.elapsed_time = 0.0f;
			top_pilot.current_frame = (top_pilot.current_frame + 1) % sprite->frame_count;
		}
	}
	else {
		top_pilot.current_frame = 0;
		top_pilot.elapsed_time = 0.0f;
	}
}

static void DrawTopPilot() {
	PilotSpriteData* sprite = &PILOT_DATA[top_pilot.pilot_id];

	Rectangle source = sprite->base_rect;
	source.x += top_pilot.current_frame * source.width;

	float size = source.width * top_pilot.scale;

	Rectangle dest = { top_pilot.pos_x, top_pilot.pos_y, size, size };
	Vector2 origin = { size / 2.0f, size / 2.0f };

	DrawTexturePro(texture_pilots, source, dest, origin, 0.0f, top_pilot.color);
}

#pragma endregion TOP_PILOT

#pragma region BOTTOM_PILOT

int GetBottomPilotId(void) { return bottom_pilot.pilot_id; }

void SetBottomPilot(int pilot_id, int pos_x, int pos_y, float scale, Color color) {
	if (pilot_id < NONE || pilot_id >= TOTAL_PILOT_NUMBER) {
		bottom_pilot.pilot_id = INTERFERENCE;
	}
	else {
		bottom_pilot.pilot_id = pilot_id;
	}
	bottom_pilot.current_frame = 0;
	bottom_pilot.elapsed_time = 0.0f;
	bottom_pilot.total_animation_time = 0.0f;

	bottom_pilot.pos_x = pos_x;
	bottom_pilot.pos_y = pos_y;
	bottom_pilot.scale = scale;
	bottom_pilot.color = color;
}

void SetBottomPilotDefault(int pilot_id) {
	if (pilot_id < NONE || pilot_id >= TOTAL_PILOT_NUMBER) {
		bottom_pilot.pilot_id = INTERFERENCE;
	}
	else {
		bottom_pilot.pilot_id = pilot_id;
	}

	int scale = 16;

	bottom_pilot.current_frame = 0;
	bottom_pilot.elapsed_time = 0.0f;
	bottom_pilot.total_animation_time = 0.0f;
	bottom_pilot.pos_x = UI_LEFT_CENTER;
	bottom_pilot.pos_y = SCREEN_HEIGHT * 0.62f - scale / 2;
	bottom_pilot.scale = scale;
	bottom_pilot.color = WHITE;
}

void ClearBottomPilot() {
	bottom_pilot.pilot_id = -1;
	bottom_pilot.current_frame = 0;
	bottom_pilot.elapsed_time = 0.0f;
	bottom_pilot.total_animation_time = 0.0f;
}

void TriggerBottomPilotAnimation(float duration) {
	bottom_pilot.total_animation_time = duration;
	bottom_pilot.current_frame = 0;
	bottom_pilot.elapsed_time = 0.0f;
	TriggerPilotSpeech(bottom_pilot.pilot_id, duration);
}

static void UpdateBottomPilot() {
	if (bottom_pilot.total_animation_time > 0 || bottom_pilot.pilot_id == INTERFERENCE) {
		bottom_pilot.total_animation_time -= GetFrameTime();
		bottom_pilot.elapsed_time += GetFrameTime();
		PilotSpriteData* sprite = &PILOT_DATA[bottom_pilot.pilot_id];
		if (bottom_pilot.elapsed_time >= sprite->frame_duration) {
			bottom_pilot.elapsed_time = 0.0f;
			bottom_pilot.current_frame = (bottom_pilot.current_frame + 1) % sprite->frame_count;
		}
	}
	else {
		bottom_pilot.current_frame = 0;
		bottom_pilot.elapsed_time = 0.0f;
	}
}

void DrawBottomPilot() {
	if (bottom_pilot.pilot_id == NONE) return;

	PilotSpriteData* sprite = &PILOT_DATA[bottom_pilot.pilot_id];
	Rectangle source = sprite->base_rect;
	source.x += bottom_pilot.current_frame * source.width;
	float size = source.width * bottom_pilot.scale;
	Rectangle dest = { bottom_pilot.pos_x, bottom_pilot.pos_y, size, size };
	Vector2 origin = { size / 2.0f, size / 2.0f };
	DrawTexturePro(texture_pilots, source, dest, origin, 0.0f, bottom_pilot.color);
}

#pragma endregion BOTTOM_PILOT

void UpdatePilot(void) {
	UpdateTopPilot();
	UpdateBottomPilot();
}

void DrawPilot(void) {
	DrawTopPilot();
	DrawBottomPilot();
}