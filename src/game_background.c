// game_background.c
#include "game_background.h"
#include "background.h"
#include "raymath.h"

Background background;

void InitGameBackground(void) {
	background.texture = LoadTexture("purplebg.png");
    background.position_x = 0.0f;
    background.position_y = -1200.0f;
    background.color = (Color){ 255, 255, 255, 170 };
    background.alpha = 0.0f;
}

void UpdateGameBackground(void) {
    if (background.alpha < 0.7f) background.alpha += 0.2f * GetFrameTime();
    background.position_y = fmodf(background.position_y + 75 * GetFrameTime(), 2400.0f);
}

void DrawGameBackground(void) {
	TraceLog(LOG_INFO, "BG Alpha: %f", background.alpha);
    DrawTexture(background.texture, (int)background.position_x, (int)background.position_y, Fade(background.color, background.alpha));
    DrawTexture(background.texture, (int)background.position_x, (int)background.position_y - 2400, Fade(background.color, background.alpha));
}

void UnloadGameBackground(void) {
    UnloadTexture(background.texture);
}
