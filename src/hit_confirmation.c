// animations.c
#include "hit_confirmation.h"
#include "list.h"
#include "texture_manager.h"

#define HIT_ANIM_FRAMES 4

const Rectangle SHOCKWAVE_FRAME[] = {
	{ 40, 32, 16, 16 }, // Frame 1
	{ 56, 32, 16, 16 }, // Frame 2
	{ 72, 32, 16, 16 }, // Frame 3
	{ 88, 32, 16, 16 }  // Frame 4
};

const Rectangle EXPLOSION_FRAME[] = {
	{ 72, 48, 8, 8 },   // Frame 1
	{ 80, 48, 8, 8 },   // Frame 2
	{ 88, 48, 8, 8 },   // Frame 3
	{ 96, 48, 8, 8 }    // Frame 4
};

const Rectangle ENERGY_FRAME[] = {
	{72, 56, 8, 8},   // Frame 1
	{80, 56, 8, 8},   // Frame 2
	{88, 56, 8, 8},   // Frame 3
	{96, 56, 8, 8}    // Frame 4
};

const Rectangle CHAOS_FRAME[] = {
	{40, 48, 8, 8},   // Frame 1
	{48, 48, 8, 8},   // Frame 2
	{56, 48, 8, 8},   // Frame 3
	{64, 48, 8, 8}    // Frame 4
};


typedef struct HitConfirmation {
	HitType type;
	Rectangle source;
	Vector2 position;
	Vector2 size;
	float duration;
	int frame;
} HitConfirmation;

List* hit_confirmations;

void InitHitConfirmation(void) {
	texture_ship_assets = LoadTexture("playerassets.png");
	hit_confirmations = List_Create(sizeof(HitConfirmation));
}
void UnloadHitConfirmation(void) {
	UnloadTexture(texture_ship_assets);
}
static Rectangle GetShockwaveSource(int frame) {
	if (frame >= 1 && frame <= HIT_ANIM_FRAMES) {
		return SHOCKWAVE_FRAME[frame];
	}
	return (Rectangle) { 0 };
}
static void UpdateShockwaveFrameTime(HitConfirmation* hit) {
	hit->duration -= GetFrameTime();
	if (hit->duration <= 0.0f) {
		hit->duration = 0.01f * hit->frame;
		hit->frame++;
		hit->source = GetShockwaveSource(hit->frame);
	}
}
static HitConfirmation InitShockwaveHitConfirmation(Vector2 position) {
	HitConfirmation hit = { 0 };
	hit.type = SHOCKWAVE;
	hit.source = SHOCKWAVE_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 96, 96 };
	hit.duration = 0.01f;
	hit.frame = 0;

	return hit;
}

static Rectangle GetExplosionSource(int frame) {
	if (frame >= 1 && frame <= HIT_ANIM_FRAMES) {
		return EXPLOSION_FRAME[frame];
	}
	return (Rectangle) { 0 };
}

static void UpdateExplosionFrameTime(HitConfirmation* hit) {
	hit->duration -= GetFrameTime();
	if (hit->duration <= 0.0f) {
		hit->duration = 0.02f * hit->frame;
		hit->frame++;
		hit->source = GetExplosionSource(hit->frame);
	}
}

static HitConfirmation InitExplosionHitConfirmation(Vector2 position) {
	HitConfirmation hit = { 0 };
	hit.type = EXPLOSION;
	hit.source = EXPLOSION_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 64, 64 };
	hit.duration = 0.02f;
	hit.frame = 0;
	return hit;
}

static Rectangle GetEnergySource(int frame) {
	if (frame >= 1 && frame <= HIT_ANIM_FRAMES) {
		return ENERGY_FRAME[frame];
	}
	return (Rectangle) { 0 };
}

static void UpdateEnergyFrameTime(HitConfirmation* hit) {
	hit->duration -= GetFrameTime();
	if (hit->duration <= 0.0f) {
		hit->duration = 0.02f * hit->frame;
		hit->frame++;
		hit->source = GetEnergySource(hit->frame);
	}
}

static HitConfirmation InitEnergyHitConfirmation(Vector2 position) {
	HitConfirmation hit = { 0 };
	hit.type = ENERGY;
	hit.source = ENERGY_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 64, 64 };
	hit.duration = 0.01f;
	hit.frame = 0;
	return hit;
}

static Rectangle GetChaosSource(int frame) {
	if (frame >= 1 && frame <= HIT_ANIM_FRAMES) {
		return CHAOS_FRAME[frame];
	}
	return (Rectangle) { 0 };
}

static void UpdateChaosFrameTime(HitConfirmation* hit) {
	hit->duration -= GetFrameTime();
	if (hit->duration <= 0.0f) {
		hit->duration = 0.02f * hit->frame;
		hit->frame++;
		hit->source = GetChaosSource(hit->frame);
	}
}

static HitConfirmation InitChaosHitConfirmation(Vector2 position) {
	HitConfirmation hit = { 0 };
	hit.type = CHAOS;
	hit.source = CHAOS_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 64, 64 };
	hit.duration = 0.02f;
	hit.frame = 0;
	return hit;
}


static bool IsHitConfirmationFinished(void* context, HitConfirmation* hit) {
	return hit->frame > HIT_ANIM_FRAMES;
}


void ConfirmHit(HitType type, Vector2 position) {
	HitConfirmation hit;
	switch (type) {
	case SHOCKWAVE: hit = InitShockwaveHitConfirmation(position); break;
	case EXPLOSION:	hit = InitExplosionHitConfirmation(position); break;
	case ENERGY:	hit = InitEnergyHitConfirmation(position); break;
	case CHAOS:	    hit = InitChaosHitConfirmation(position); break;
	default:		hit = InitShockwaveHitConfirmation(position); break;
	}
	List_Add(hit_confirmations, &hit);
}

void UpdateFrameTime(HitConfirmation* hit) {
	switch (hit->type) {
	case SHOCKWAVE: UpdateShockwaveFrameTime(hit); break;
	case EXPLOSION: UpdateExplosionFrameTime(hit); break;
	case ENERGY:    UpdateEnergyFrameTime(hit); break;
	case CHAOS:     UpdateChaosFrameTime(hit); break;
	default:        UpdateShockwaveFrameTime(hit); break;
	}
}

void UpdateHitConfirmation(void) {
	List_ForEach(hit_confirmations, (Function)UpdateFrameTime);
	List_RemoveWithFn(hit_confirmations, NULL, (MatchFunction)IsHitConfirmationFinished);
}

static void DrawAnimation(HitConfirmation* hit) {
	Rectangle dest = {
		hit->position.x - hit->size.x / 2,
		hit->position.y - hit->size.y / 2,
		hit->size.x,
		hit->size.y
	};
	DrawTexturePro(texture_ship_assets, hit->source, dest, (Vector2) { 0 }, 0.0f, Fade(WHITE, 0.5f));
}

void DrawHitConfirmation(void) {
	List_ForEach(hit_confirmations, (Function)DrawAnimation);
}

