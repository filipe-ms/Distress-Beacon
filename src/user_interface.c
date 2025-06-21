// user_interface.c
#include "user_interface.h"
#include "common.h"
#include "player.h"
#include "weapon.h"
#include "draw_utils.h"

#include <stdio.h>

// Posições aqui no topo para facilitar
static Vector2 GetActiveBonusesStartingPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, 250 };
}
static Vector2 GetExpBarPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, 860 };
}

static Vector2 GetLevelTextPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, 900 };
}

static Vector2 GetScorePosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, 10 };
}

static Vector2 GetActiveWeaponsPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, 75 };
}

static void DrawLeftUIBackground() {
    DrawRectangle(0, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);
}

static void DrawRightUIBackground(void) {
    // Posição inicial x, posição inicial y, largura, altura, cor
    DrawRectangle(UI_WIDTH + GAME_SCREEN_WIDTH, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);
}


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



static void DrawActiveWeapons() {
    Vector2 draw_pos = GetActiveWeaponsPosition();
    DrawOutlinedText("WEAPONS", (int)draw_pos.x, (int)draw_pos.y, 20, WHITE, Fade(RAYWHITE, 0.5f));
	DrawMultilineText(GetActiveWeaponsString(), (int)draw_pos.x, (int)draw_pos.y + 25, 20, BLUE, 1.0f);
}

static void DrawActiveBonuses() {
    Vector2 start_pos = GetActiveBonusesStartingPosition();
    int base_height = start_pos.y;
    int label_x = start_pos.x;
    int value_x = start_pos.x + 140;

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
    Vector2 exp_bar_position = GetExpBarPosition();

	float exp_bar_width = 200;
	float exp_bar_height = 20;

	float fill_percentage = (float)GetPlayerExperience() / GetPlayerExpToLevel();
	int filled_width = (int)(fill_percentage * exp_bar_width);

	DrawRectangle(exp_bar_position.x, exp_bar_position.y, filled_width, exp_bar_height, Fade(PURPLE, 0.6f));
	DrawRectangleLines(exp_bar_position.x, exp_bar_position.y, exp_bar_width, exp_bar_height, Fade(DARKGRAY, 0.6f));
}

static void DrawLevelText(void) {
	Vector2 level_text_position = GetLevelTextPosition();
	int font_size = 30;

	DrawText(TextFormat("Level %i", GetPlayerLevel()), level_text_position.x, level_text_position.y, font_size, Fade(WHITE, 0.6f));
}

static void DrawScore(void) {
	Vector2 score_position = GetScorePosition();
	int font_size = 30;

    DrawOutlinedText(TextFormat("%04i", GetPlayerScore()), score_position.x, score_position.y, font_size, WHITE, Fade(RAYWHITE, 0.5f));
}

void DrawUserInterface(void) {

    { // Left
        DrawLeftUIBackground();
    }

    { // Right
        DrawRightUIBackground();
        DrawExpBar();
        DrawLevelText();
        DrawScore();
        DrawActiveWeapons();
        DrawActiveBonuses();
    }
}