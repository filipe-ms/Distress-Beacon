#include "winner.h"
#include "scene_manager.h"
#include "raylib.h"
#include "select_ship.h"
#include "ranking.h"
#include "player.h"
#include "stdio.h"

static bool returnToMenu = false;

void InitWinner(void) {
    returnToMenu = false;
    LoadRanking();
}

void UpdateWinner(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        returnToMenu = true;

        int current_score = GetPlayerScore();

        if (playerCount < MAX_ENTRIES || current_score > entries[MAX_ENTRIES - 1].score) {
            ChangeScene(ENTER_NAME);
        }
        else {
            ChangeScene(RANKING);
        }
    }
}

void DrawWinner(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Parabéns, você venceu!", GetScreenWidth() / 2 - MeasureText("Parabéns, você venceu!", 40) / 2, GetScreenHeight() / 2 - 160, 40, WHITE);
    DrawText("Pressione ENTER para continuar", GetScreenWidth() / 2 - MeasureText("Pressione ENTER para continuar", 24) / 2, GetScreenHeight() / 2 + 20, 24, WHITE);
    EndDrawing();
}
