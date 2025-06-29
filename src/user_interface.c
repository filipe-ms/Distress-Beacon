// user_interface.c
#include "user_interface.h"
#include "common.h"
#include "player.h"
#include "weapon.h"
#include "draw_utils.h"
#include "left_ui.h"
#include "ship.h"
#include "bars.h"
#include "raymath.h"

#include <string.h>
#include <stdio.h>

// Posições aqui no topo para facilitar

static Rectangle GetExpBarPosition() {
    return (Rectangle) { UI_WIDTH + GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.9, 400, 25 };
}

static void DrawRightUIBGandBorder(void) {
    DrawRectangle(UI_WIDTH + GAME_SCREEN_WIDTH, 0, UI_WIDTH, SCREEN_HEIGHT, UI_BG_COLOR);

    int border_width = 5;

    int border_thick = 5;
    int margin_offset = border_thick * 4;

    DrawCenteredPixelBorder(
        UI_RIGHT_CENTER,
        SCREEN_HEIGHT / 2,
        UI_WIDTH - margin_offset,
        SCREEN_HEIGHT - margin_offset,
        border_thick,
        WHITE);
}

#pragma region Draw_Active_Weapons

void DrawAlignedWeaponLevel(const char* label, int value, int pos_x, int pos_y, int level_width, int font_size, Color color) {
    DrawText("Level", pos_x, pos_y, font_size, color);
    char number_text[8];
    sprintf(number_text, "%d", value);

    int number_width = MeasureText(number_text, font_size);
    int number_pos_x = pos_x + level_width - number_width;

    DrawText(number_text, number_pos_x, pos_y, font_size, color);
}

void DrawAlignedWeapon(const char* label, int value, int pos_x, int pos_y, int col_width, int font_size, Color color) {
    DrawText(label, pos_x, pos_y, font_size, color);
    int label_width = MeasureText(label, font_size);
    int level_x = pos_x + col_width - MeasureText("Level 0", font_size);
    DrawAlignedWeaponLevel("Level", value, level_x, pos_y, MeasureText("Level 0", font_size), font_size, color);
}

static void DrawActiveWeapons() {

    int pos_x = UI_RIGHT_CENTER - 200;
    int pos_y = SCREEN_HEIGHT * 0.353;
    int next_line = pos_y;
    int width = 400;
    float border_width = UI_WIDTH * 0.8;
    int font_size = 30;
    float frame_center = (pos_y + font_size * (3) / 2.0f) + 10;

    DrawCenteredRectangle(UI_RIGHT_CENTER, frame_center, border_width - 10, 165, UI_HIGHLIGHT_COLOR);
    DrawCenteredPixelBorder(UI_RIGHT_CENTER, frame_center, border_width, 165, 5, WHITE);

    DrawCenteredOutlinedText("ARSENAL", UI_RIGHT_CENTER, pos_y, font_size, WHITE, Fade(RAYWHITE, 0.5f));
    next_line += font_size;
    if (GetPulseLevel()) {
        DrawAlignedWeapon("Pulse", GetPulseLevel(), pos_x, next_line, width, font_size, WHITE);
        next_line += font_size;
    }
    if (GetPhotonLevel()) {
        DrawAlignedWeapon("Photon", GetPhotonLevel(), pos_x, next_line, width, font_size, WHITE);
        next_line += font_size;
    }
    if (GetShotgunLevel()) {
        DrawAlignedWeapon("Shotgun", GetShotgunLevel(), pos_x, next_line, width, font_size, WHITE);
        next_line += font_size;
    }
    if (GetHomingLevel()) {
        DrawAlignedWeapon("Homing", GetHomingLevel(), pos_x, next_line, width, font_size, WHITE);
    }
    for (int i = GetActiveWeaponsAmount(); i < MAX_WEAPON_SLOTS; i++) {
        DrawAlignedWeapon("Empty", 0, pos_x, next_line, width, font_size, WHITE);
        next_line += font_size;
    }
}

#pragma endregion

void DrawAlignedStat(const char* label, int value, int pos_x, int pos_y, int col_width, int font_size, Color color) {
    char value_text[16];
    sprintf(value_text, "+%d%%", value);
    DrawText(label, pos_x, pos_y, font_size, color);
    int value_width = MeasureText(value_text, font_size);
    DrawText(value_text, pos_x + col_width - value_width, pos_y, font_size, color);
}

static void DrawActiveBonuses() {
    int base_height = SCREEN_HEIGHT * 0.15;
    int start_x = UI_RIGHT_CENTER - 200;
    int width = 400;
	int font_size = 30;
    float border_width = UI_WIDTH * 0.8;
    int num_stats = 5;
    float frame_center = (base_height + font_size * (num_stats + 1) / 2.0f) - 20;

    DrawCenteredRectangle(UI_RIGHT_CENTER, frame_center, border_width - 10, 190, UI_HIGHLIGHT_COLOR);
    DrawCenteredPixelBorder(UI_RIGHT_CENTER, frame_center, border_width, 190, 5, WHITE);

    DrawCenteredOutlinedText("UPGRADES", UI_RIGHT_CENTER, base_height, font_size, WHITE, Fade(RAYWHITE, 0.5f));

    DrawAlignedStat("Damage", GetDamageModifier(), start_x, base_height + font_size * 1, width, font_size, WHITE);
    DrawAlignedStat("Fire rate", GetCooldownModifier(), start_x, base_height + font_size * 2, width, font_size, WHITE);
    DrawAlignedStat("Projectile size", GetSizeModifier(), start_x, base_height + font_size * 3, width, font_size, WHITE);
    DrawAlignedStat("Bullet speed", GetSpeedModifier(), start_x, base_height + font_size * 4, width, font_size, WHITE);
}

static void DrawExpBar(void) {
    Rectangle exp_bar = GetExpBarPosition();
    exp_bar.x = UI_RIGHT_CENTER - exp_bar.width/2;

    float scale = 10;
    float fill_percentage = ((float)GetPlayerExperience() / GetPlayerExpToLevel());
    DrawProgressBarRec(PROGRESS_BAR_ROUNDED_EMPTY_WIDE, exp_bar, 1, 1);
    DrawProgressBarRec(PROGRESS_BAR_ROUNDED_BLUE_WIDE, exp_bar, fill_percentage, 1);
}

static void DrawLevel(void) {

    int pos_x = UI_RIGHT_CENTER;
    int pos_y = SCREEN_HEIGHT * 0.93;
    int exp = GetPlayerExperience();
    int to_level = GetPlayerExpToLevel();
	int font_size = 30;

    DrawCenteredText(TextFormat("Level %-6i Exp %i/%i", GetPlayerLevel(), exp, to_level), pos_x, pos_y, font_size, WHITE);
}

static void DrawScore() {
    int pos_x = UI_RIGHT_CENTER;

    int text_pos_y = SCREEN_HEIGHT * 0.05;
    int number_pos_y = SCREEN_HEIGHT * 0.08;
    int border_pos_y = (text_pos_y + number_pos_y) / 2;

    int border_width = UI_WIDTH * 0.8;
    int border_height = 80;

    int bg_width = border_width - 5;
    int bg_height = border_height - 5;

    int font_size = 30;

    DrawCenteredRectangle(pos_x, border_pos_y, bg_width, bg_height, UI_HIGHLIGHT_COLOR);
    DrawCenteredPixelBorder(pos_x, border_pos_y, border_width, border_height, 5, WHITE);
    DrawCenteredOutlinedText("SCORE", pos_x, text_pos_y, font_size, WHITE, Fade(RAYWHITE, 0.5));
    DrawCenteredOutlinedText(TextFormat("%08i", GetPlayerScore()), pos_x, number_pos_y, font_size, WHITE, Fade(RAYWHITE, 0.5f));
}

static float GetSpecialBarAlpha(int bar_pos_x, int bar_pos_y, int bar_width) {
    float min_dist = 50.0f;
    float max_dist = bar_width;

    Vector2 ship_pos = GetShipPosition();
    Vector2 bar_center = { bar_pos_x + bar_width / 2.0f, bar_pos_y };

    float distance = Vector2Distance(ship_pos, bar_center);

    float result = (distance - min_dist) / (max_dist - min_dist);
    
    result = Clamp(result, 0.1f, 1.0f);

    return result;
}


static void DrawShipSpecialCooldown(void) {
    int player_ship = GetPlayerShip();

    float pos_x = GAME_SCREEN_END - GAME_SCREEN_END * 0.2;
    float pos_y = GAME_SCREEN_HEIGHT - GAME_SCREEN_HEIGHT * 0.1;
    float scale = 5;
    float alpha = GetSpecialBarAlpha(pos_x, pos_y, 210);

    // Cooldown
    DrawContainerBar(CONTAINER_BAR_Z_GRAY, pos_x, pos_y, scale, alpha);
    DrawProgressBar(PROGRESS_BAR_Z_GREEN, CONTAINER_BAR_Z_GRAY, pos_x, pos_y, scale, GetShipCooldownPct(player_ship), alpha);

    // Extra Bar 1
    if (player_ship == PUDDLE_JUMPER || player_ship == NEBULA) {
        float offset_y_extra_gauge = 60;
        float gauge_completion = GetExtraGaugePct(player_ship);

        DrawContainerBar(CONTAINER_BAR_Z_GRAY, pos_x, pos_y - offset_y_extra_gauge, scale, alpha);

        if (gauge_completion <= 1) {
            DrawProgressBar(PROGRESS_BAR_Z_RED, CONTAINER_BAR_Z_GRAY, pos_x, pos_y - offset_y_extra_gauge, scale, GetExtraGaugePct(player_ship), alpha);
        } else if (gauge_completion <= 2) {
            DrawProgressBar(PROGRESS_BAR_Z_BLUE, CONTAINER_BAR_Z_GRAY, pos_x, pos_y - offset_y_extra_gauge, scale, 1.0f, alpha);
            DrawProgressBar(PROGRESS_BAR_Z_YELLOW, CONTAINER_BAR_Z_GRAY, pos_x, pos_y - offset_y_extra_gauge, scale, gauge_completion - 1.0f, alpha);
        } else {
            DrawProgressBar(PROGRESS_BAR_Z_YELLOW, CONTAINER_BAR_Z_GRAY, pos_x, pos_y - offset_y_extra_gauge, scale, 1.0f, alpha);
            DrawProgressBar(PROGRESS_BAR_Z_RED, CONTAINER_BAR_Z_GRAY, pos_x, pos_y - offset_y_extra_gauge, scale, gauge_completion - 2.0f, alpha);
        }
    }
}

void DrawUserInterface(void) {

    { // Left
        DrawLeftUIPanel();
    }

    { // Right
        DrawRightUIBGandBorder();
        DrawScore();
        DrawActiveWeapons();
        DrawActiveBonuses();
        DrawExpBar();
        DrawLevel();
    }
    
    { // Game
        DrawShipSpecialCooldown();
    }
}