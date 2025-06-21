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
    DrawText("Parabens, voce venceu!", GetScreenWidth() / 2 - MeasureText("Parabens, voce venceu!", 40) / 2, GetScreenHeight() / 2 - 160, 40, WHITE);
    DrawText("(a fonte nao tem acento)", GetScreenWidth() / 2 - MeasureText("(a fonte nao tem acento)", 30) / 2, GetScreenHeight() / 2 - 110, 30, WHITE);
    DrawText("Pressione ENTER para continuar", GetScreenWidth() / 2 - MeasureText("Pressione ENTER para continuar", 24) / 2, GetScreenHeight() / 2 + 20, 24, WHITE);
    EndDrawing();
}
