// animations.h
#pragma once

#include "raylib.h"

typedef enum HitType {
	SHOCKWAVE,
	EXPLOSION,
	ENERGY,
	CHAOS,
	HIT_TYPE_COUNT
} HitType;

void ConfirmHit(HitType type, Vector2 position);
void UpdateHitConfirmation(void);
void DrawHitConfirmation(void);

void InitHitConfirmation(void);
void UnloadHitConfirmation(void);