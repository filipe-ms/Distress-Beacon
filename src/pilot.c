#include "pilot.h"
#include "texture_manager.h"
#include "common.h"

typedef struct Pilot {
	int type;
	int current_frame;
	float time_elapsed;
} Pilot;

static Pilot pilot = { 0 };

static Rectangle GetPilotPortraitSource(int ship) {
	int source_x = 8 * pilot.current_frame;
	switch (ship) {
	case ORION:         return (Rectangle) { source_x, 64, 8, 8 };
	case AUREA:         return (Rectangle) { source_x, 16, 8, 8 };
	case NEBULA:        return (Rectangle) { source_x, 48, 8, 8 };
	case PUDDLE_JUMPER: return (Rectangle) { source_x, 40, 8, 8 };
	case VOID:          return (Rectangle) { source_x, 72, 8, 8 };
	default:                  return (Rectangle) { source_x, 80, 0, 0 };
	}
}

static float GetFrameDurationForSpeech(int current_frame) {
	switch (current_frame) {
	//case 0: return 0.2f;
	//case 1: return 0.2f;
	//case 2: return 0.2f;
	//case 3: return 0.2f;
	//case 4: return 0.2f;
	default: return 0.2f;
	}
}

void DrawPilotStaticPortrait(int ship, Vector2 position, float scale, Color tint) {
	Rectangle source = GetPilotPortraitSource(ship);
	Vector2 origin = { 8 * scale / 2, 8 * scale / 2 };
	DrawTexturePro(texture_pilots, source, (Rectangle) { position.x, position.y, 8 * scale, 8 * scale }, origin, 0.0f, tint);
}

void DrawPilotDuringSpeech(int ship, int pos_x, int pos_y, float scale, Color tint) {
	if (pilot.type != ship) {
		pilot.type = ship;
		pilot.current_frame = 0;
		pilot.time_elapsed = 0.0f;
	}

	pilot.time_elapsed += GetFrameTime();
	
	if (pilot.time_elapsed >= GetFrameDurationForSpeech(pilot.current_frame)) {
		pilot.current_frame = (pilot.current_frame +1) % 5;
		pilot.time_elapsed = 0.0f;
	}

	Rectangle source = GetPilotPortraitSource(ship);
	Vector2 origin = { 8 * scale / 2.0f, 8 * scale / 2.0f };

	DrawTexturePro(texture_pilots, source, (Rectangle) { pos_x, pos_y, 8 * scale, 8 * scale }, origin, 0.0f, tint);
}