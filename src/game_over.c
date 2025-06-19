#include "game_over.h"
#include "raylib.h"
#include "scene_manager.h"
#include "common.h"
#include "select_ship.h"
#include "player.h"
#include "ranking.h"
#include "draw_utils.h"

float game_over_alpha = 1.0f;
bool game_over_enter_transition = true;
bool game_over_leave_transition = false;
float game_over_hold_timer = 0.0f;
const float GAME_OVER_HOLD_DURATION = 2.0f;


void InitGameOver(void) {
    game_over_alpha = 1.0f;
    game_over_enter_transition = true;
    game_over_leave_transition = false;
    game_over_hold_timer = 0.0f;
    LoadRanking();
}

void UpdateGameOver(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        game_over_leave_transition = true;
    }

    if (game_over_enter_transition && !game_over_leave_transition) {
        if (game_over_hold_timer < GAME_OVER_HOLD_DURATION) {
            game_over_hold_timer += GetFrameTime();
        }
        else {
            game_over_alpha -= 0.5f * GetFrameTime();
            if (game_over_alpha <= 0.5f) {
                game_over_alpha = 0.5f;
                game_over_enter_transition = false;
            }
        }
    }
    else if (game_over_leave_transition) {
        game_over_alpha += 0.5f * GetFrameTime();
        if (game_over_alpha >= 2.5f) {
            game_over_alpha = 2.5f;
            game_over_leave_transition = false;

            int current_score = GetPlayerScore();

            if (playerCount < MAX_ENTRIES || current_score > entries[MAX_ENTRIES - 1].score) {
                ChangeScene(ENTER_NAME);
            }
            else {
                ChangeScene(RANKING);
            }
        }
    }
}

void DrawGameOver(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    int font_size = 80;
    DrawOutlinedText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", font_size) / 2, SCREEN_HEIGHT / 2 - font_size, font_size, RAYWHITE, Fade(RED, 0.5f));
    DrawSelectMenuBackground();
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, game_over_alpha));
    EndDrawing();
}
