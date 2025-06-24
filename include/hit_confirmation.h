// animations.h
#pragma once

#include "raylib.h"

typedef enum HitType {
	SHOCKWAVE,
	EXPLOSION,
	ENERGY,
	CHAOS,
	HIT_TYPE_COUNT,
	WORMHOLE,
	WORMHOLE_TELEPORT_ANIMATION,
} HitType;

typedef enum ParticleRenderingOrder {
	RENDERING_ORDER_BEFORE_ENEMY = 0,
	RENDERING_ORDER_AFTER_ENEMY = 1,
	RENDERING_ORDER_BEFORE_SHIP = 1,
	RENDERING_ORDER_AFTER_SHIP = 2,
} ParticleRenderingOrder;

void CreateEffect(HitType type, Vector2 position, float duration);
void ConfirmHit(HitType type, Vector2 position);
void UpdateHitConfirmation(void);
void DrawHitConfirmation(ParticleRenderingOrder order);

void InitHitConfirmation(void);
void UnloadHitConfirmation(void);