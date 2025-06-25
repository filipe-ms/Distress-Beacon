// animations.h
#pragma once

#include "raylib.h"

typedef enum EffectType {
	SHOCKWAVE,
	EXPLOSION,
	ENERGY,
	CHAOS,
	HIT_TYPE_COUNT,
	WORMHOLE,
	WORMHOLE_TELEPORT_ANIMATION,
	DRONE,
	DRONE_EXPLOSION,
	DRONE_THRUSTER,
} EffectType;

typedef enum ParticleRenderingOrder {
	RENDERING_ORDER_BEFORE_ENEMY = 0,
	RENDERING_ORDER_AFTER_ENEMY = 1,
	RENDERING_ORDER_BEFORE_SHIP = 1,
	RENDERING_ORDER_AFTER_SHIP = 2,
} ParticleRenderingOrder;

typedef struct SpecialEffect {
	EffectType type;
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
} SpecialEffect;

void DestroyEffect(SpecialEffect* effect);

SpecialEffect* CreateManagedEffectDuration(EffectType type, Vector2 position, float duration);
SpecialEffect* CreateUnmanagedEffect(EffectType type, Vector2 position, float duration);
void CreateManagedEffect(EffectType type, Vector2 position);

void UpdateEffects(void);
void DrawEffects(ParticleRenderingOrder order);

void InitEffects(void);
void UnloadEffects(void);