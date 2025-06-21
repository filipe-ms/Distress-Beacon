// user_interface.c
#include "user_interface.h"
#include "common.h"
#include "player.h"
#include "weapon.h"
#include "draw_utils.h"

#include <stdio.h>

// Posições aqui no topo para facilitar

static Vector2 GetScoreTextPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.05 };
}

static Vector2 GetScoreNumberPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, SCREEN_HEIGHT* 0.08 };
}

static Vector2 GetActiveBonusesStartingPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.2 };
}

static Vector2 GetActiveWeaponsPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.5 };
}

static Rectangle GetExpBarPosition() {
    return (Rectangle) { UI_WIDTH + GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.85, 360, 20 };
}

static Vector2 GetLevelTextPosition() {
    return (Vector2) { UI_WIDTH + GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.9 };
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
            DrawText(line, (int)x + UI_WIDTH/2 - lineWidth/2, (int)start_y, fontSize, Fade(color, alpha));
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
    int font_size = 30;
    int text_mid = MeasureText("WEAPONS", font_size) / 2;
    DrawOutlinedText("WEAPONS", UI_RIGHT_CENTER - text_mid, (int)draw_pos.y, font_size, WHITE, Fade(RAYWHITE, 0.5f));
	DrawMultilineText(GetActiveWeaponsString(), (int)draw_pos.x, (int)draw_pos.y + 50, font_size, BLUE, 1.0f);
}

static void DrawActiveBonuses() {
    Vector2 start_pos = GetActiveBonusesStartingPosition();
    int base_height = start_pos.y;
    int label_x = UI_RIGHT_CENTER - 200;
    int value_x = UI_RIGHT_CENTER + 100;
	int font_size = 30;

    int upgrades_size = MeasureText("UPGRADES", font_size);
    DrawOutlinedText("UPGRADES", UI_RIGHT_CENTER - upgrades_size/2, base_height, font_size, WHITE, Fade(RAYWHITE, 0.5f));

    DrawText("Damage", label_x, base_height + font_size *2, font_size, RED);
    DrawText(TextFormat("%03.0f%%", GetDamageModifier()), value_x, base_height + font_size*2, font_size, RED);

    DrawText("Fire rate", label_x, base_height + font_size*3, font_size, YELLOW);
    DrawText(TextFormat("%03.0f%%", GetCooldownModifier()), value_x, base_height + font_size*3, font_size, YELLOW);

    DrawText("Size", label_x, base_height + font_size*4, font_size, PURPLE);
    DrawText(TextFormat("%03.0f%%", GetSizeModifier()), value_x, base_height + font_size*4, font_size, PURPLE);

    DrawText("Bullet Speed", label_x, base_height + font_size*5, font_size, LIGHTGRAY);
    DrawText(TextFormat("%03.0f%%", GetSpeedModifier()), value_x, base_height + font_size*5, font_size, LIGHTGRAY);

    DrawText("Shield", label_x, base_height + font_size * 6, font_size, SKYBLUE);
    DrawText(TextFormat("%d", GetShieldCapacity()), value_x, base_height + font_size * 6, font_size, SKYBLUE);
}

static void DrawExpBar(void) {
    Rectangle exp_bar = GetExpBarPosition();
    exp_bar.x = UI_RIGHT_CENTER - exp_bar.width/2;
    Rectangle fill = exp_bar;

    float fill_percentage = (float)GetPlayerExperience() / GetPlayerExpToLevel();
    fill.width = fill_percentage * exp_bar.width;


    DrawRectangleRounded(fill, 0.5f, 10, PURPLE);
    DrawRectangleRoundedLines(exp_bar, 0.5f, 10, Fade(DARKGRAY, 0.6f));
}


static void DrawLevelText(void) {
	Vector2 level_text = GetLevelTextPosition();
	int font_size = 30;

    int text_mid = MeasureText(TextFormat("Level %i", GetPlayerLevel()), font_size)/2;

	DrawText(TextFormat("Level %i", GetPlayerLevel()), level_text.x + UI_WIDTH/2 - text_mid, level_text.y, font_size, Fade(WHITE, 1.0f));
}

static void DrawScoreText(void) {
	Vector2 score_position = GetScoreTextPosition();
	int font_size = 30;
	int text_mid = MeasureText("SCORE", font_size) / 2;
    DrawOutlinedText("SCORE", score_position.x + UI_WIDTH / 2 - text_mid, score_position.y, font_size, WHITE, Fade(RAYWHITE, 0.5f));
}

static void DrawScoreNumber(void) {
	Vector2 score_position = GetScoreNumberPosition();
	int font_size = 30;
    int text_mid = MeasureText(TextFormat("%08i", GetPlayerScore()), font_size) / 2;
    DrawOutlinedText(TextFormat("%08i", GetPlayerScore()), score_position.x + UI_WIDTH/2 - text_mid, score_position.y, font_size, Fade(WHITE, 0.9f), Fade(RAYWHITE, 0.5f));
}

void DrawUserInterface(void) {

    { // Left
        DrawLeftUIBackground();
    }

    { // Right
        DrawRightUIBackground();
        DrawScoreText();
        DrawScoreNumber();
        DrawActiveWeapons();
        DrawActiveBonuses();
        DrawExpBar();
        DrawLevelText();
    }
}