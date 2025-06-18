// user_interface.c
#include "user_interface.h"
#include "common.h"
#include "player.h"

#include <stdio.h>


static void DrawUIBackground(void) {
	// Posição inicial x, posição inicial y, largura, altura, cor
	DrawRectangle(GAME_SCREEN_WIDTH, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);
}

static void DrawExpBar(void) {
	Vector2 exp_bar_position = { GAME_SCREEN_WIDTH, 860 };

	float exp_bar_width = 200;
	float exp_bar_height = 20;

	float fill_percentage = (float)GetPlayerExperience() / GetPlayerExpToLevel();
	int filled_width = (int)(fill_percentage * exp_bar_width);

	DrawRectangle(exp_bar_position.x, exp_bar_position.y, filled_width, exp_bar_height, Fade(PURPLE, 0.6f));
	DrawRectangleLines(exp_bar_position.x, exp_bar_position.y, exp_bar_width, exp_bar_height, Fade(DARKGRAY, 0.6f));
}

static void DrawLevelText(void) {
	Vector2 level_text_position = { GAME_SCREEN_WIDTH, 900 };
	int font_size = 30;

	DrawText(TextFormat("Level %i", GetPlayerLevel()), level_text_position.x, level_text_position.y, font_size, Fade(WHITE, 0.6f));
}

static void DrawScore(void) {
	Vector2 score_position = { GAME_SCREEN_WIDTH, 10 };
	int font_size = 30;

	DrawText(TextFormat("%04i", GetPlayerScore()), score_position.x, score_position.y, font_size, Fade(WHITE, 0.6f));
}

void DrawUserInterface(void) {
	DrawUIBackground();
	DrawExpBar();
	DrawLevelText();
	DrawScore();
}