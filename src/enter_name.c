// enter_name.c
#include "enter_name.h"
#include "ranking.h"
#include "raylib.h"
#include "player.h"
#include "scene_manager.h"
#include "winner.h"
#include "input.h"

#include <string.h>

#define MAX_NAME_LENGTH 20

static char playerName[MAX_NAME_LENGTH + 1] = "";
static int charIndex = 0;
char message[50];
bool message_flag = false;


void InitEnterName(void) {
    memset(playerName, 0, sizeof(playerName));
    charIndex = 0;
    message_flag = false;
}

void UpdateEnterName(void) {
    int key = GetCharPressed();

    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (key != ',') && (charIndex < MAX_NAME_LENGTH)) {
            playerName[charIndex] = (char)key;
            charIndex++;
            playerName[charIndex] = '\0';
        }
        message_flag = false;
        key = GetCharPressed();
    }

    if (IsReturnButtonPressed() && charIndex > 0) {
        charIndex--;
        playerName[charIndex] = '\0';
        message_flag = false;
    }

    if (IsConfirmButtonPressed()) {
        if (charIndex == 0) {
            message_flag = true;
            strcpy(message, "O nome nao pode estar vazio");
        }
        else {
            AddToRanking(playerName, GetPlayerShip(), GetPlayerScore());
            ChangeScene(RANKING);
        }
    }
}


void DrawEnterName(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Voce esta no ranking! Escreva seu nome:", 100, 100, 30, WHITE);
    DrawText(playerName, 100, 150, 30, WHITE);
    if (message_flag) {
        DrawText(message, 100, 200, 30, RED);
    }
    EndDrawing();
}
