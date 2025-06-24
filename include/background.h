// background.h
#pragma once

#include "raylib.h"

typedef enum BackgroundTexture {
	BACKGROUND_STARS,
	BACKGROUND_GAME,
	BACKGROUND_SELECT_SHIP,
	BACKGROUND_COUNT
} BackgroundTexture;

typedef enum BackgroundScaleOption {
	STRETCH_TO_SCREEN = -1,
} BackgroundScaleOption;

void InitBackground(BackgroundTexture texture, Color tint, float scale, float alpha, float speed);
void UpdateBackground(void);
void DrawBackground(void);
void SetBackgroundScrollSpeed(float speed);
float GetBackgroundScrollSpeed(void);