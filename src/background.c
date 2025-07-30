// background.c
#include "background.h"
#include "common.h"
#include "raymath.h"
#include "texture_manager.h"

typedef struct Background {
	Texture* texture;
    float position_x;
    float position_y;
    Color color;
	float scale;
	float alpha;
	float speed;
} Background;

Background background;

static void LoadSelectedTexture(BackgroundTexture background_texture) {
	switch (background_texture) {
	case BACKGROUND_STARS:
		background.texture = &background_stars;
		break;
	case BACKGROUND_GAME:
		background.texture = &background_dark_space;
		break;
	case BACKGROUND_SELECT_SHIP:
		background.texture = &background_stars_and_dust;
		break;
	case BACKGROUND_ENDING:
		background.texture = &background_ending;
		break;
	default:
		background.texture = &background_menubg;
		break;
	}
}

static float GetStretchScale(void) {
	float width_scale = (float)SCREEN_WIDTH / background.texture->width;
	float height_scale = (float)SCREEN_HEIGHT / background.texture->height;
	return fmaxf(width_scale, height_scale);
}

void InitBackground(BackgroundTexture bg_texture, Color tint, float scale, float alpha, float speed) {
	LoadSelectedTexture(bg_texture);
	background.position_x = 0.0f;
	background.position_y = 0.0f;
	background.color = tint;

	if ((int)scale == STRETCH_TO_SCREEN) background.scale = GetStretchScale();
	else background.scale = scale;

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
	background.position_y = fmodf(background.position_y + background.speed * GetFrameTime(), background.texture->height * background.scale);
}

void DrawBackground() {
	Color color = Fade(background.color, background.alpha);
	Vector2 position1 = { background.position_x, background.position_y };
	Vector2 position2 = { background.position_x, background.position_y - background.texture->height * background.scale };
	DrawTextureEx(*background.texture, position1, 0.0f, background.scale, color);
	DrawTextureEx(*background.texture, position2, 0.0f, background.scale, color);
}