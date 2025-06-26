// animations.c
#include "special_effects.h"
#include "list.h"
#include "texture_manager.h"
#include "general_utils.h"
#include "raymath.h"

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

const Rectangle DRONE_THRUSTER_FRAME[] = {
	{ 8 * 5, 8 * 1, 8, 8 },
	{ 8 * 6, 8 * 1, 8, 8 },
	{ 8 * 7, 8 * 1, 8, 8 },
	{ 8 * 8, 8 * 1, 8, 8 },
};

List* managed_effects;

List* unmanaged_effects;

void InitEffects(void) {
	texture_ship_assets = LoadTexture("playerassets.png");
	managed_effects = List_Create(sizeof(SpecialEffect));
	unmanaged_effects = List_Create(sizeof(SpecialEffect));
}
void UnloadEffects(void) {
	UnloadTexture(texture_ship_assets);
}
static Rectangle GetShockwaveSource(int frame) {
	if (frame >= 1 && frame <= HIT_ANIM_FRAMES) {
		return SHOCKWAVE_FRAME[frame];
	}
	return (Rectangle) { 0 };
}
static void UpdateShockwaveFrameTime(SpecialEffect* hit) {
	hit->duration -= GetFrameTime();
	if (hit->duration <= 0.0f) {
		hit->duration = 0.01f * hit->current_frame;
		hit->current_frame++;
		hit->source = GetShockwaveSource(hit->current_frame);
	}
}
static SpecialEffect InitShockwaveHitConfirmation(Vector2 position) {
	SpecialEffect hit = { 0 };
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

static void UpdateExplosionFrameTime(SpecialEffect* hit) {
	hit->duration -= GetFrameTime();
	if (hit->duration <= 0.0f) {
		hit->duration = 0.02f * hit->current_frame;
		hit->current_frame++;
		hit->source = GetExplosionSource(hit->current_frame);
	}
}

static SpecialEffect InitExplosionHitConfirmation(Vector2 position) {
	SpecialEffect hit = { 0 };
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

static void UpdateEnergyFrameTime(SpecialEffect* hit) {
	hit->duration -= GetFrameTime();
	if (hit->duration <= 0.0f) {
		hit->duration = 0.02f * hit->current_frame;
		hit->current_frame++;
		hit->source = GetEnergySource(hit->current_frame);
	}
}

static SpecialEffect InitEnergyHitConfirmation(Vector2 position) {
	SpecialEffect hit = { 0 };
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

static void UpdateChaosFrameTime(SpecialEffect* hit) {
	hit->duration -= GetFrameTime();
	if (hit->duration <= 0.0f) {
		hit->duration = 0.02f * hit->current_frame;
		hit->current_frame++;
		hit->source = GetChaosSource(hit->current_frame);
	}
}

static SpecialEffect InitChaosHitConfirmation(Vector2 position) {
	SpecialEffect hit = { 0 };
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

static void UpdateWormholeAnimation(SpecialEffect* hit) {
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

static SpecialEffect InitWormhole(Vector2 position, float duration) {
	SpecialEffect hit = { 0 };
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

static void UpdateWormholeTeleportAnimation(SpecialEffect* hit) {
	hit->duration += GetFrameTime();
	hit->rotation += PI * GetFrameTime();
	hit->size = Vector2MultiplyScalarF(hit->original_size, sinf((hit->duration / hit->max_duration) * PI));
	hit->color = Fade(WHITE, 1 - (hit->duration / hit->max_duration));
}

static SpecialEffect InitWormholeTeleportAnimation(Vector2 position, float duration) {
	SpecialEffect hit = { 0 };
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

static bool IsHitConfirmationFinished(void* context, SpecialEffect* hit) {
	bool has_ended = hit->current_frame > hit->ending_frame ||
		hit->current_frame == hit->ending_frame && hit->duration >= hit->max_duration;

	return has_ended;
}

static SpecialEffect InitDrone(Vector2 position, float duration) {
	SpecialEffect hit = { 0 };
	hit.type = DRONE;
	hit.source = (Rectangle) { 8 * 7, 8 * 1, 8, 8 };
	hit.position = position;
	hit.size = hit.original_size = (Vector2){ 8 * 5, 8 * 5 };
	hit.duration = 0;
	hit.max_duration = duration;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 180;
	hit.texture = &texture_ships;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_SHIP;
	return hit;
}

static SpecialEffect InitDroneThruster(Vector2 position) {
	SpecialEffect hit = { 0 };
	hit.type = DRONE_THRUSTER;
	hit.source = DRONE_THRUSTER_FRAME[0];
	hit.position = position;
	hit.size = (Vector2){ 32, 32 };
	hit.duration = 0.02f;
	hit.current_frame = 0;
	hit.texture = &texture_ship_assets;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_ENEMY;
	return hit;
}

static void UpdateThrusterAnimation(SpecialEffect* hit) {
	hit->duration += GetFrameTime();
	hit->current_frame = (int)(fmodf(hit->duration, 0.8f) / 0.2f);
	hit->source = DRONE_THRUSTER_FRAME[hit->current_frame];
}

static SpecialEffect InitNebulaParticleA(Vector2 position) {
	SpecialEffect hit = { 0 };
	hit.type = NEBULA_PARTICLE_A;
	hit.source = (Rectangle) { 8 * 9, 8 * 7, 8, 8 };
	hit.position = position;
	hit.size = hit.original_size = (Vector2){ 8 * 2, 8 * 2 };
	hit.duration = 0;
	hit.max_duration = 0;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_ship_assets;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_SHIP;
	return hit;
}

static SpecialEffect InitNebulaParticleB(Vector2 position) {
	SpecialEffect hit = { 0 };
	hit.type = NEBULA_PARTICLE_B;
	hit.source = (Rectangle) { 8 * 5, 8 * 6, 8, 8 };
	hit.position = position;
	hit.size = hit.original_size = (Vector2){ 8 * 2, 8 * 2 };
	hit.duration = 0;
	hit.max_duration = 0;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_ship_assets;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_SHIP;
	return hit;
}

static SpecialEffect InitNebulaParticleC(Vector2 position) {
	SpecialEffect hit = { 0 };
	hit.type = NEBULA_PARTICLE_C;
	hit.source = (Rectangle) { 8 * 9, 8 * 6, 8, 8 };
	hit.position = position;
	hit.size = hit.original_size = (Vector2){ 8 * 2, 8 * 2 };
	hit.duration = 0;
	hit.max_duration = 0;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_ship_assets;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_SHIP;
	return hit;
}

static SpecialEffect InitNebulaEnergyField(Vector2 position) {
	SpecialEffect hit = { 0 };
	hit.type = NEBULA_ENERGY_FIELD;
	hit.source = (Rectangle) { 8 * 11, 8 * 4, 16, 16 };
	hit.position = position;
	hit.size = hit.original_size = (Vector2){ 8 * 12, 8 * 12 };
	hit.duration = 0;
	hit.max_duration = 0;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_ship_assets;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_SHIP;
	return hit;
}

void DestroyEffect(SpecialEffect* effect) {
	List_Remove(unmanaged_effects, effect);
}

SpecialEffect* CreateManagedEffectDuration(EffectType type, Vector2 position, float duration) {
	SpecialEffect effect;
	switch (type) {
		case WORMHOLE:                    effect = InitWormhole(position, duration); break;
		case WORMHOLE_TELEPORT_ANIMATION: effect = InitWormholeTeleportAnimation(position, duration); break;
	}
	List_Add(managed_effects, &effect);
	return (SpecialEffect*)List_GetByIndex(managed_effects, 0);
}

SpecialEffect* CreateUnmanagedEffect(EffectType type, Vector2 position, float duration) {
	SpecialEffect effect;
	switch (type) {
		case DRONE:					effect = InitDrone(position, duration); break;
		case DRONE_THRUSTER:		effect = InitDroneThruster(position); break;
		case NEBULA_PARTICLE_A:		effect = InitNebulaParticleA(position); break;
		case NEBULA_PARTICLE_B:		effect = InitNebulaParticleB(position); break;
		case NEBULA_PARTICLE_C:		effect = InitNebulaParticleC(position); break;
		case NEBULA_ENERGY_FIELD:	effect = InitNebulaEnergyField(position); break;
	}
	List_Add(unmanaged_effects, &effect);
	return (SpecialEffect*)List_GetByIndex(unmanaged_effects, 0);
}

void CreateManagedEffect(EffectType type, Vector2 position) {
	SpecialEffect hit;
	switch (type) {
	case SHOCKWAVE:			hit = InitShockwaveHitConfirmation(position); break;
	case EXPLOSION:			hit = InitExplosionHitConfirmation(position); break;
	case ENERGY:			hit = InitEnergyHitConfirmation(position); break;
	case CHAOS:				hit = InitChaosHitConfirmation(position); break;
	case DRONE_EXPLOSION:   hit = InitExplosionHitConfirmation(position); hit.color = WHITE; break;
	default:				hit = InitShockwaveHitConfirmation(position); break;
	}
	List_Add(managed_effects, &hit);
}

void UpdateFrameTime(SpecialEffect* hit) {
	switch (hit->type) {
	case SHOCKWAVE:						UpdateShockwaveFrameTime(hit); break;
	case EXPLOSION:						UpdateExplosionFrameTime(hit); break;
	case ENERGY:						UpdateEnergyFrameTime(hit); break;
	case CHAOS:							UpdateChaosFrameTime(hit); break;
	case WORMHOLE:						UpdateWormholeAnimation(hit); break;
	case WORMHOLE_TELEPORT_ANIMATION:	UpdateWormholeTeleportAnimation(hit); break;
	case DRONE_THRUSTER:				UpdateThrusterAnimation(hit); break;
	default:
		break;
	}
}

void UpdateEffects(void) {
	List_ForEach(managed_effects, (Function)UpdateFrameTime);
	List_RemoveWithFn(managed_effects, NULL, (MatchFunction)IsHitConfirmationFinished);

	List_ForEach(unmanaged_effects, (Function)UpdateFrameTime);
}

static void DrawAnimation(ParticleRenderingOrder* order, SpecialEffect* hit) {
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

void DrawEffects(ParticleRenderingOrder order) {
	List_ForEachCtx(managed_effects, &order, (Function)DrawAnimation);
	List_ForEachCtx(unmanaged_effects, &order, (Function)DrawAnimation);
}

