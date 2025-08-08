#include "game_over.h"
#include "raylib.h"
#include "scene_manager.h"
#include "common.h"
#include "select_ship.h"
#include "player.h"
#include "ranking.h"
#include "draw_utils.h"
#include "scene_draw_effects.h"
#include "timer.h"
#include "input.h"

static bool enter_pressed;

void InitGameOver(void) {
    LoadRanking();
	InitFadeInEffect(3.0f, BLACK, 0.8f);
    enter_pressed = false;
}

void UpdateGameOver(void) {
    if (IsConfirmButtonPressed()) {
        enter_pressed = true;
        InitTimer(2.0f);
        InitFadeOutEffect(2.1f, BLACK, GetCurrentScreenEffectAlpha());
    }

    if (enter_pressed && UpdateTimer()) {
        int current_score = GetPlayerScore();

        if (playerCount < MAX_ENTRIES || current_score > entries[MAX_ENTRIES - 1].score) {
            ChangeScene(ENTER_NAME);
        }
        else {
            ChangeScene(RANKING);
        }
    }
}

void DrawGameOver(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    int font_size = 80;
    DrawOutlinedText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", font_size) / 2, SCREEN_HEIGHT / 2 - font_size, font_size, RAYWHITE, Fade(RED, 0.5f));
	UpdateAndDrawScreenEffects();
    EndDrawing();
}
