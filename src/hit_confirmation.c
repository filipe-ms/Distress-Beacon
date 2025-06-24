// animations.c
#include "hit_confirmation.h"
#include "list.h"
#include "texture_manager.h"
#include "general_utils.h"
#include "raymath.h"
#include "math.h"

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
	Vector2 original_size;
	Vector2 size;
	float rotation;
	float duration;
	float max_duration;
	int current_frame;
	int ending_frame;
	Color color;
	Texture2D* texture;
	ParticleRenderingOrder order;
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
		hit->duration = 0.01f * hit->current_frame;
		hit->current_frame++;
		hit->source = GetShockwaveSource(hit->current_frame);
	}
}
static HitConfirmation InitShockwaveHitConfirmation(Vector2 position) {
	HitConfirmation hit = { 0 };
	hit.type = SHOCKWAVE;
	hit.source = SHOCKWAVE_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 96, 96 };
	hit.duration = 0.01f;
	hit.current_frame = 0;
	hit.rotation = 0;
	hit.ending_frame = HIT_ANIM_FRAMES;
	hit.texture = &texture_ship_assets;
	hit.color = Fade(WHITE, 0.5f);
	hit.order = RENDERING_ORDER_AFTER_ENEMY;
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
		hit->duration = 0.02f * hit->current_frame;
		hit->current_frame++;
		hit->source = GetExplosionSource(hit->current_frame);
	}
}

static HitConfirmation InitExplosionHitConfirmation(Vector2 position) {
	HitConfirmation hit = { 0 };
	hit.type = EXPLOSION;
	hit.source = EXPLOSION_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 64, 64 };
	hit.duration = 0.02f;
	hit.current_frame = 0;
	hit.rotation = 0;
	hit.ending_frame = HIT_ANIM_FRAMES;
	hit.texture = &texture_ship_assets;
	hit.color = Fade(WHITE, 0.5f);
	hit.order = RENDERING_ORDER_AFTER_ENEMY;
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
		hit->duration = 0.02f * hit->current_frame;
		hit->current_frame++;
		hit->source = GetEnergySource(hit->current_frame);
	}
}

static HitConfirmation InitEnergyHitConfirmation(Vector2 position) {
	HitConfirmation hit = { 0 };
	hit.type = ENERGY;
	hit.source = ENERGY_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 64, 64 };
	hit.duration = 0.01f;
	hit.current_frame = 0;
	hit.rotation = 0;
	hit.ending_frame = HIT_ANIM_FRAMES;
	hit.texture = &texture_ship_assets;
	hit.color = Fade(WHITE, 0.5f);
	hit.order = RENDERING_ORDER_AFTER_ENEMY;
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
		hit->duration = 0.02f * hit->current_frame;
		hit->current_frame++;
		hit->source = GetChaosSource(hit->current_frame);
	}
}

static HitConfirmation InitChaosHitConfirmation(Vector2 position) {
	HitConfirmation hit = { 0 };
	hit.type = CHAOS;
	hit.source = CHAOS_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 64, 64 };
	hit.duration = 0.02f;
	hit.current_frame = 0;
	hit.rotation = 0;
	hit.ending_frame = HIT_ANIM_FRAMES;
	hit.texture = &texture_ship_assets;
	hit.color = Fade(WHITE, 0.5f);
	hit.order = RENDERING_ORDER_AFTER_ENEMY;
	return hit;
}

static void UpdateWormhole(HitConfirmation* hit) {
	hit->duration += GetFrameTime();

	//during the first second, expand
	if (hit->duration < 1) {
		hit->size = Vector2MultiplyScalarF(hit->original_size, fminf(hit->duration, 1));
	} else if (hit->duration + 1 > hit->max_duration) {
		hit->size = Vector2MultiplyScalarF(hit->original_size, fmaxf(hit->max_duration - hit->duration, 0));
	} else {
		hit->size = hit->original_size;
	}

	hit->rotation += RAD2DEG * 2 * PI * GetFrameTime();
}

static HitConfirmation InitWormhole(Vector2 position, float duration) {
	HitConfirmation hit = { 0 };
	hit.type = WORMHOLE;
	hit.source = (Rectangle) { 24, 8, 8, 8 };
	hit.position = position;
	hit.size = (Vector2){ 0 };
	hit.original_size = (Vector2){ 8 * 10, 8 * 10 };
	hit.duration = 0;
	hit.max_duration = duration;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_projectiles;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_BEFORE_SHIP;
	return hit;
}

static void UpdateWormholeTeleportAnimation(HitConfirmation* hit) {
	hit->duration += GetFrameTime();
	hit->rotation += PI * GetFrameTime();
	hit->size = Vector2MultiplyScalarF(hit->original_size, sinf((hit->duration / hit->max_duration) * PI));
	hit->color = Fade(WHITE, 1 - (hit->duration / hit->max_duration));
}

static HitConfirmation InitWormholeTeleportAnimation(Vector2 position, float duration) {
	HitConfirmation hit = { 0 };
	hit.type = WORMHOLE_TELEPORT_ANIMATION;
	hit.source = (Rectangle) { 16, 8, 8, 8 };
	hit.position = position;
	hit.size = (Vector2){ 0 };
	hit.original_size = (Vector2){ 8 * 8, 8 * 8 };
	hit.duration = 0;
	hit.max_duration = duration;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_projectiles;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_SHIP;
	return hit;
}

static bool IsHitConfirmationFinished(void* context, HitConfirmation* hit) {
	bool has_ended = hit->current_frame > hit->ending_frame ||
		hit->current_frame == hit->ending_frame && hit->duration >= hit->max_duration;

	return has_ended;
}

void CreateEffect(HitType type, Vector2 position, float duration) {
	HitConfirmation hit;
	switch (type) {
		case WORMHOLE:
			hit = InitWormhole(position, duration);
			break;
		case WORMHOLE_TELEPORT_ANIMATION:
			hit = InitWormholeTeleportAnimation(position, duration);
			break;
	}
	List_Add(hit_confirmations, &hit);
}

void ConfirmHit(HitType type, Vector2 position) {
	HitConfirmation hit;
	switch (type) {
	case SHOCKWAVE:			hit = InitShockwaveHitConfirmation(position); break;
	case EXPLOSION:			hit = InitExplosionHitConfirmation(position); break;
	case ENERGY:			hit = InitEnergyHitConfirmation(position); break;
	case CHAOS:				hit = InitChaosHitConfirmation(position); break;
	default:				hit = InitShockwaveHitConfirmation(position); break;
	}
	List_Add(hit_confirmations, &hit);
}

void UpdateFrameTime(HitConfirmation* hit) {
	switch (hit->type) {
	case SHOCKWAVE:						UpdateShockwaveFrameTime(hit); break;
	case EXPLOSION:						UpdateExplosionFrameTime(hit); break;
	case ENERGY:						UpdateEnergyFrameTime(hit); break;
	case CHAOS:							UpdateChaosFrameTime(hit); break;
	case WORMHOLE:						UpdateWormhole(hit); break;
	case WORMHOLE_TELEPORT_ANIMATION:	UpdateWormholeTeleportAnimation(hit); break;
	default:							UpdateShockwaveFrameTime(hit); break;
	}
}

void UpdateHitConfirmation(void) {
	List_ForEach(hit_confirmations, (Function)UpdateFrameTime);
	List_RemoveWithFn(hit_confirmations, NULL, (MatchFunction)IsHitConfirmationFinished);
}

static void DrawAnimation(ParticleRenderingOrder* order, HitConfirmation* hit) {
	if (hit->order != *order) {
		return;
	}

	Rectangle dest = {
		hit->position.x,
		hit->position.y,
		hit->size.x,
		hit->size.y
	};

	Vector2 origin = (Vector2) { hit->size.x / 2, hit->size.y / 2 };

	DrawTexturePro(*hit->texture, hit->source, dest, origin, hit->rotation, hit->color);
}

void DrawHitConfirmation(ParticleRenderingOrder order) {
	List_ForEachCtx(hit_confirmations, &order, (Function)DrawAnimation);
}

