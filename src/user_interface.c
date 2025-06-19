// user_interface.c
#include "user_interface.h"
#include "common.h"
#include "player.h"
#include "weapon.h"
#include "draw_utils.h"

#include <stdio.h>

static void DrawMultilineText(const char* text, float start_x, float start_y, int fontSize, Color color, float alpha) {
    const char* str = text;
    float lineHeight = (float)fontSize + 4.0f;

    while (*str) {
        int len = 0;
        while (str[len] != '\n' && str[len] != '\0') {
            len++;
        }

        if (len > 0) {
            const char* line = TextSubtext(str, 0, len); // Função da raylib que faz uma substring
            float lineWidth = (float)MeasureText(line, fontSize);
            float x = start_x;
            DrawText(line, (int)x, (int)start_y, fontSize, Fade(color, alpha));
        }

        str += len;
        if (*str == '\n') {
            str++;
            start_y += lineHeight;
        }
    }
}

static void DrawUIBackground(void) {
	// Posição inicial x, posição inicial y, largura, altura, cor
	DrawRectangle(GAME_SCREEN_WIDTH, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);
}

static void DrawActiveWeapons() {
    DrawOutlinedText("WEAPONS", GAME_SCREEN_WIDTH, 75, 20, WHITE, Fade(RAYWHITE, 0.5f));
	DrawMultilineText(GetActiveWeaponsString(), GAME_SCREEN_WIDTH, 100, 20, BLUE, 1.0f);
}

static void DrawActiveBonuses() {
    int base_height = 250;
    int label_x = GAME_SCREEN_WIDTH;
    int value_x = GAME_SCREEN_WIDTH + 140;

    DrawOutlinedText("UPGRADES", label_x, base_height, 20, WHITE, Fade(RAYWHITE, 0.5f));

    DrawText("Damage", label_x, base_height + 20, 20, RED);
    DrawText(TextFormat("%03.0f%%", GetDamageModifier()), value_x, base_height + 20, 20, RED);

    DrawText("Fire rate", label_x, base_height + 40, 20, YELLOW);
    DrawText(TextFormat("%03.0f%%", GetCooldownModifier()), value_x, base_height + 40, 20, YELLOW);

    DrawText("Size", label_x, base_height + 60, 20, PURPLE);
    DrawText(TextFormat("%03.0f%%", GetSizeModifier()), value_x, base_height + 60, 20, PURPLE);

    DrawText("Bullet Speed", label_x, base_height + 80, 20, LIGHTGRAY);
    DrawText(TextFormat("%03.0f%%", GetSpeedModifier()), value_x, base_height + 80, 20, LIGHTGRAY);
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

    DrawOutlinedText(TextFormat("%04i", GetPlayerScore()), score_position.x, score_position.y, font_size, WHITE, Fade(RAYWHITE, 0.5f));
}

void DrawUserInterface(void) {
	DrawUIBackground();
	DrawExpBar();
	DrawLevelText();
	DrawScore();
	DrawActiveWeapons();
	DrawActiveBonuses();
}