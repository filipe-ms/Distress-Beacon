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

const Rectangle ORION_DISRUPTION_FIELD_FRAME[] = {
	{24 * 16, 24 * 3, 24, 24},   // Frame 1
	{24 * 17, 24 * 3, 24, 24},   // Frame 2
	{24 * 18, 24 * 3, 24, 24},   // Frame 3
	{24 * 19, 24 * 3, 24, 24},   // Frame 4
	{24 * 20, 24 * 3, 24, 24},   // Frame 5
	{24 * 21, 24 * 3, 24, 24},   // Frame 6
	{24 * 22, 24 * 3, 24, 24},   // Frame 7
	{24 * 23, 24 * 3, 24, 24},   // Frame 8
};

const Rectangle VOID_EVENT_HORIZON_FRAME[] = {
	{24 * 0, 24 * 6, 24, 24},   // Frame 1
	{24 * 1, 24 * 6, 24, 24},   // Frame 2
	{24 * 2, 24 * 6, 24, 24},   // Frame 3
	{24 * 3, 24 * 6, 24, 24},   // Frame 4
	{24 * 4, 24 * 6, 24, 24},   // Frame 5
	{24 * 5, 24 * 6, 24, 24},   // Frame 6
	{24 * 6, 24 * 6, 24, 24},   // Frame 7
	{24 * 7, 24 * 6, 24, 24},   // Frame 8
};

List* managed_effects;
List* unmanaged_effects;

void InitEffects(void) {
	texture_ship_assets = LoadTexture("playerassets.png");

	if (managed_effects) List_Destroy(managed_effects);
	managed_effects = List_Create(sizeof(SpecialEffect));

	if (unmanaged_effects) List_Destroy(unmanaged_effects);
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
	hit.reproduction_type = REPRODUCTION_ONCE;
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
	hit.reproduction_type = REPRODUCTION_ONCE;
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
	hit.reproduction_type = REPRODUCTION_ONCE;
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
	hit.reproduction_type = REPRODUCTION_ONCE;
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
	hit.original_size = (Vector2){ 8 * 12, 8 * 12 };
	hit.duration = 0;
	hit.max_duration = duration;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_projectiles;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_BEFORE_SHIP;
	hit.reproduction_type = REPRODUCTION_TIMED_LOOP;
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
	hit.reproduction_type = REPRODUCTION_TIMED_LOOP;
	return hit;
}

static bool IsHitConfirmationFinished(void* context, SpecialEffect* hit) {
	bool has_ended = false;
	
	if (hit->reproduction_type == REPRODUCTION_ONCE) {
		has_ended = hit->current_frame > hit->ending_frame + 1;
	} else {
		has_ended = hit->duration >= hit->max_duration;
	}

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

static SpecialEffect InitOrionDisruptionField(Vector2 position, float duration) {
	SpecialEffect hit = { 0 };
	hit.type = ORION_DISRUPTION_FIELD;
	hit.source = ORION_DISRUPTION_FIELD_FRAME[0];
	hit.position = position;
	hit.size = hit.original_size = (Vector2){ 24, 24 };
	hit.duration = 0;
	hit.max_duration = duration;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_special_effects_1;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_ENEMY;
	hit.reproduction_type = REPRODUCTION_TIMED_LOOP;
	return hit;
}

static SpecialEffect InitWormholePuddleJumper(Vector2 position) {
	SpecialEffect hit = { 0 };
	hit.type = WORMHOLE_PUDDLE_JUMPER_SHIP;
	hit.source = (Rectangle) { 8 * 1, 8 * 2, 8, 8 };
	hit.position = position;
	hit.size = hit.original_size = (Vector2){ 48, 48 };
	hit.duration = 0;
	hit.max_duration = 0;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_ships;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_AFTER_ENEMY;
	return hit;
}

static SpecialEffect InitWormholeTether(Vector2 position) {
	SpecialEffect hit = { 0 };
	hit.type = WORMHOLE_TETHER;
	hit.source = (Rectangle) { 8 * 1, 8 * 8 + 2, 4, 4 };
	hit.position = position;
	hit.size = hit.original_size = (Vector2){ 48, 48 };
	hit.duration = 0;
	hit.max_duration = 0;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_projectiles;
	hit.color = WHITE;
	hit.order = RENDERING_ORDER_BEFORE_SHIP;
	return hit;
}

static SpecialEffect InitVoidEventHorizon(Vector2 position) {
	SpecialEffect hit = { 0 };
	hit.type = VOID_EVENT_HORIZON;
	hit.source = (Rectangle){ 24 * 0, 24 * 6, 24, 24 };
	hit.position = position;
	hit.original_size = (Vector2){ 24, 24 };
	hit.size = (Vector2){ 300, 300 };
	hit.max_duration = 5.0f;
	hit.duration = 0.0f;
	hit.current_frame = 0;
	hit.ending_frame = 0;
	hit.rotation = 0;
	hit.texture = &texture_special_effects_1;
	hit.color = Fade(WHITE, 0.5f);
	hit.order = RENDERING_ORDER_AFTER_ENEMY;
	hit.reproduction_type = REPRODUCTION_TIMED_LOOP;
	return hit;
}

static void UpdateVoidEventHorizon(SpecialEffect* hit) {
	if (!hit) return;

	hit->duration += GetFrameTime();
	const int totalFrames = 8;
	const float frameTime = 0.2f;
	const float pulseCycle = 1.0f;

	if (hit->duration <= 4.0f) {
		hit->current_frame = (int)(fmodf(hit->duration, frameTime * totalFrames) / frameTime);

		float t = fmodf(hit->duration, pulseCycle);
		float pulseAlpha = 0.3f + 0.7f * (1.0f - t / pulseCycle);
		hit->color = Fade(WHITE, pulseAlpha);

		if (hit->current_frame >= 0 && hit->current_frame < totalFrames) {
			hit->source = VOID_EVENT_HORIZON_FRAME[hit->current_frame];
		}
	}
	else if (hit->duration <= 5.0f) {
		float t = fmodf(4.0f, pulseCycle);
		float alphaAtDecayStart = 0.3f + 0.7f * (1.0f - t / pulseCycle);

		float decayTime = hit->duration - 4.0f;
		float decayDuration = 1.0f;

		float pulseAlpha = alphaAtDecayStart * (1.0f - (decayTime / decayDuration));
		hit->color = Fade(WHITE, pulseAlpha);
	}
	else {
		hit->color = Fade(WHITE, 0.0f);
	}
}

static void UpdateOrionDisruptionField(SpecialEffect* hit) {
	hit->duration += GetFrameTime();
	hit->current_frame = (int)(fmodf(hit->duration, 0.2f * 8.0f) / 0.2f);
	TraceLog(LOG_WARNING, "Current Frame: %d | Duration: %f ", hit->current_frame, hit->duration);
	hit->source = ORION_DISRUPTION_FIELD_FRAME[hit->current_frame];
}

SpecialEffect* CreateManagedEffectDuration(EffectType type, Vector2 position, float duration) {
	SpecialEffect effect;
	switch (type) {
		case WORMHOLE_TELEPORT_ANIMATION: effect = InitWormholeTeleportAnimation(position, duration); break;
		case ORION_DISRUPTION_FIELD:	  effect = InitOrionDisruptionField(position, duration); break;
	}
	List_Add(managed_effects, &effect);
	return (SpecialEffect*)List_GetByIndex(managed_effects, 0);
}

static SpecialEffect InitPlanet(Vector2 position, EffectType type) {
	Texture* texture;

	switch (type) {
		case PLANET_BLACK_HOLE:
			texture = &texture_planet_black_hole;
			break;

		case PLANET_GALAXY:
			texture = &texture_planet_galaxy;
			break;
		
		case PLANET_MOON_1:
			texture = &texture_planet_moon_1;
			break;

		case PLANET_MOON_2:
			texture = &texture_planet_moon_2;
			break;
			
		case PLANET_1:
			texture = &texture_planet_planet_1;
			break;

		case PLANET_2:
			texture = &texture_planet_planet_2;
			break;

		case PLANET_3:
			texture = &texture_planet_planet_3;
			break;

		case PLANET_SUN:
			texture = &texture_planet_sun;
			break;
	}

	SpecialEffect effect = { 0 };
	effect.type = type;
	effect.source = (Rectangle) { 0, 0, texture->width / 20, texture->height / 20 };
	effect.position = position;
	effect.size = effect.original_size = (Vector2){ texture->width / 20, texture->height / 20 };
	effect.duration = 0;
	effect.max_duration = 0;
	effect.current_frame = 0;
	effect.ending_frame = 0;
	effect.rotation = 0;
	effect.texture = texture;
	effect.color = WHITE;
	effect.order = RENDERING_ORDER_BEFORE_SHIP;

	return effect;
}

SpecialEffect* CreateUnmanagedEffect(EffectType type, Vector2 position, float duration) {
	SpecialEffect effect;
	switch (type) {
		case WORMHOLE:						effect = InitWormhole(position, duration); break;
		case DRONE:							effect = InitDrone(position, duration); break;
		case DRONE_THRUSTER:				effect = InitDroneThruster(position); break;
		case NEBULA_PARTICLE_A:				effect = InitNebulaParticleA(position); break;
		case NEBULA_PARTICLE_B:				effect = InitNebulaParticleB(position); break;
		case NEBULA_PARTICLE_C:				effect = InitNebulaParticleC(position); break;
		case NEBULA_ENERGY_FIELD:			effect = InitNebulaEnergyField(position); break;
		case WORMHOLE_PUDDLE_JUMPER_SHIP:	effect = InitWormholePuddleJumper(position); break;
		case WORMHOLE_TETHER:				effect = InitWormholeTether(position); break;
		
		case PLANET_BLACK_HOLE:
		case PLANET_GALAXY:
		case PLANET_MOON_1:
		case PLANET_MOON_2:
		case PLANET_1:
		case PLANET_2:
		case PLANET_3:
		case PLANET_SUN:
			effect = InitPlanet(position, type); break;
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
	case VOID_EVENT_HORIZON:hit = InitVoidEventHorizon(position); break;
	default:				hit = InitShockwaveHitConfirmation(position); break;
	}
	List_Add(managed_effects, &hit);
}

static void UpdatePlanetAnimation(SpecialEffect* effect) {
	effect->duration += GetFrameTime();
	effect->current_frame = fmodf((effect->duration * 10.0f), 400.0f);

	int width = effect->texture->width / 20.0f;
	int height = effect->texture->height / 20.0f;

	effect->source = (Rectangle) {
		(int)fmodf(effect->current_frame, 20) * width,
		(int)(effect->current_frame / 20) * height,
		width,
		height,
	};
}

void UpdateFrameTime(SpecialEffect* effect) {
	switch (effect->type) {
	case SHOCKWAVE:						UpdateShockwaveFrameTime(effect); break;
	case EXPLOSION:						UpdateExplosionFrameTime(effect); break;
	case ENERGY:						UpdateEnergyFrameTime(effect); break;
	case CHAOS:							UpdateChaosFrameTime(effect); break;
	case WORMHOLE:						UpdateWormholeAnimation(effect); break;
	case WORMHOLE_TELEPORT_ANIMATION:	UpdateWormholeTeleportAnimation(effect); break;
	case DRONE_THRUSTER:				UpdateThrusterAnimation(effect); break;
	case ORION_DISRUPTION_FIELD:	  	UpdateOrionDisruptionField(effect); break;
	case VOID_EVENT_HORIZON:			UpdateVoidEventHorizon(hit); break;
	
	case PLANET_BLACK_HOLE:
	case PLANET_GALAXY:
	case PLANET_MOON_1:
	case PLANET_MOON_2:
	case PLANET_1:
	case PLANET_2:
	case PLANET_3:
	case PLANET_SUN:
		UpdatePlanetAnimation(effect); break;
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

