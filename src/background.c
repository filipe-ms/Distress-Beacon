// background.c
#include "background.h"
#include "raymath.h"

typedef struct Background {
    float position_x;
    float position_y;
    Color color;
	float scale;
	float alpha;
	float speed;
} Background;

Texture texture;
Background background;

static void LoadSelectedTexture(BackgroundTexture background_texture) {
	if (texture.id != 0) UnloadTexture(texture);
	switch (background_texture) {
	case BACKGROUND_MAIN_MENU:
		texture = LoadTexture("menubg.png");
		break;
	case BACKGROUND_GAME:
		texture = LoadTexture("purplebg.png");
		break;
	case BACKGROUND_SELECT_SHIP:
		texture = LoadTexture("shipselectbg.png");
		break;
	default:
		texture = LoadTexture("menubg.png");
		break;
	}
}

void UnloadBackgroundTexture(void) {
	if (texture.id != 0) {
		UnloadTexture(texture);
	}
}

void InitBackground(BackgroundTexture texture, Color tint, float scale, float alpha, float speed) {
	LoadSelectedTexture(texture);
	background.position_x = 0.0f;
	background.position_y = 0.0f;
	background.color = tint;
	background.scale = scale;
	background.alpha = alpha;
	background.speed = speed;
}

void SetBackgroundScrollSpeed(float speed) {
	background.speed = speed;
}

float GetBackgroundScrollSpeed(void) {
	return background.speed;
}

void UpdateBackground() {
	background.position_y = fmodf(background.position_y + background.speed * GetFrameTime(), texture.height * background.scale);
}

void DrawBackground() {
	Color color = Fade(background.color, background.alpha);
	Vector2 position1 = { background.position_x, background.position_y };
	Vector2 position2 = { background.position_x, background.position_y - texture.height * background.scale };
	DrawTextureEx(texture, position1, 0.0f, background.scale, color);
	DrawTextureEx(texture, position2, 0.0f, background.scale, color);
}