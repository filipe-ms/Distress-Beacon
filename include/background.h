// background.h
#pragma once

#include "raylib.h"

typedef enum BackgroundTexture {
	BACKGROUND_MAIN_MENU,
	BACKGROUND_GAME,
	BACKGROUND_SELECT_SHIP,
	BACKGROUND_COUNT
} BackgroundTexture;

void InitBackground(BackgroundTexture texture, Color tint, float scale, float alpha, float speed);
void UnloadBackgroundTexture(void);
void UpdateBackground(void);
void DrawBackground(void);
void UnloadBackgroundTexture(void);
void SetBackgroundScrollSpeed(float speed);
float GetBackgroundScrollSpeed(void);