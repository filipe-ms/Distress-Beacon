#include "ranking.h"
#include "raylib.h"
#include "scene_manager.h"
#include "select_ship.h"
#include "common.h"
#include "player.h"
#include "draw_utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


RankingEntry entries[MAX_ENTRIES];

int playerCount = 0;

bool enter_transition = true;
bool leave_transition = false;

float rec_alpha = 1.0f;

const char* GetShipName(int shipType) {
    switch (shipType) {
    case 0: return "Aurea";
    case 1: return "Orion";
    case 2: return "Nebula";
    case 3: return "Puddle Jumper";
    default: return "Unknown";
    }
}
void LoadRanking(void) {
    FILE* file = fopen("ranking.txt", "r");
    if (!file) {
        playerCount = 0;
        return;
    }

    playerCount = 0;
    char line[128];
    while (fgets(line, sizeof(line), file) && playerCount < MAX_ENTRIES) {
        char* name = strtok(line, ",");
        char* shipName = strtok(NULL, ",");
        char* scoreStr = strtok(NULL, "\n");

        if (name && shipName && scoreStr) {
            strncpy(entries[playerCount].name, name, MAX_NAME_LENGTH);
            entries[playerCount].name[MAX_NAME_LENGTH] = '\0';

            strncpy(entries[playerCount].shipName, shipName, MAX_NAME_LENGTH);
            entries[playerCount].shipName[MAX_NAME_LENGTH] = '\0';

            entries[playerCount].score = atoi(scoreStr);
            playerCount++;
        }
    }
    fclose(file);
}
void SaveRanking(void) {
    FILE* file = fopen("ranking.txt", "w");
    if (file) {
        for (int i = 0; i < playerCount; i++) {
            fprintf(file, "%s,%s,%d\n", entries[i].name, entries[i].shipName, entries[i].score);
        }
        fclose(file);
    }
}
void InitRanking() {
    enter_transition = true;
    leave_transition = false;
    rec_alpha = 1.0f;

    for (int i = 0; i < MAX_ENTRIES; i++) {
        strcpy(entries[i].name, "");
        strcpy(entries[i].shipName, "");
        entries[i].score = -1;
    }
    LoadRanking();
}
void AddToRanking(const char* name, int shipType, int score) {
    LoadRanking();

    RankingEntry newEntry;
    strncpy(newEntry.name, name, MAX_NAME_LENGTH);
    newEntry.name[MAX_NAME_LENGTH] = '\0';
    newEntry.score = score;
    strncpy(newEntry.shipName, GetShipName(shipType), MAX_NAME_LENGTH);
    newEntry.shipName[MAX_NAME_LENGTH] = '\0';


    int insertIndex = -1;
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (newEntry.score > entries[i].score) {
            insertIndex = i;
            break;
        }
    }

    if (insertIndex != -1) {
        for (int j = MAX_ENTRIES - 1; j > insertIndex; j--) {
            entries[j] = entries[j - 1];
        }
        entries[insertIndex] = newEntry;
        if (playerCount < MAX_ENTRIES) {
            playerCount++;
        }
    }
    else if (playerCount < MAX_ENTRIES) {
        entries[playerCount] = newEntry;
        playerCount++;
    }

    SaveRanking();
}
void UpdateRanking() {
    if (IsKeyPressed(KEY_ENTER)) {
        if (enter_transition) {
            enter_transition = false;
            rec_alpha = 0;
        }
        else {
            leave_transition = true;
        }
    }
    if (enter_transition) {
        rec_alpha -= 0.5f * GetFrameTime();
        if (rec_alpha <= 0) {
            rec_alpha = 0;
            enter_transition = false;
        }
    } else if (leave_transition) {
        rec_alpha += 0.5f * GetFrameTime();
        if (rec_alpha >= 1) {
            rec_alpha = 1;
            ChangeScene(START);
        }
    }
}

static int StringEqualsIgnoreCase(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return 0;
        }
        s1++;
        s2++;
    }
    return (*s1 == *s2);
}

static Color GetShipColor(const char* shipName) {
    if (StringEqualsIgnoreCase(shipName, "Aurea")) return YELLOW;
    else if (StringEqualsIgnoreCase(shipName, "Orion")) return WHITE;
    else if (StringEqualsIgnoreCase(shipName, "Nebula")) return PURPLE;
    else if (StringEqualsIgnoreCase(shipName, "Puddle Jumper")) return GREEN;

    return WHITE;
}

void DrawRanking(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    int col1 = SCREEN_WIDTH * 0.2;
    int col2 = SCREEN_WIDTH * 0.4;
    int col3 = SCREEN_WIDTH * 0.6;
    int col4 = SCREEN_WIDTH * 0.8;

    int pos_y = SCREEN_HEIGHT * 0.05;
    int header_font_size = 70;
    int entry_font_size = 34;
    char* ranking_header = "Melhores jogadores";

    int k = SCREEN_WIDTH / 2 - MeasureText(ranking_header, header_font_size) / 2;

    DrawOutlinedText(ranking_header, k, pos_y, header_font_size, WHITE, Fade(RAYWHITE, 0.5f));

    int start_y = pos_y + header_font_size + entry_font_size;
    int line_spacing = entry_font_size + entry_font_size / 2;

    for (int i = 0; i < playerCount; i++) {
        int line_y = start_y + i * line_spacing;

        char posStr[8];
        char nameStr[64];
        char shipStr[64];
        char scoreStr[16];

        sprintf(posStr, "%d.", i + 1);
        sprintf(nameStr, "%s", entries[i].name);
        sprintf(shipStr, "%s", entries[i].shipName);
        sprintf(scoreStr, "%d", entries[i].score);

        int pos_x = col1 - MeasureText(posStr, entry_font_size) / 2;
        int name_x = col2 - MeasureText(nameStr, entry_font_size) / 2;
        int ship_x = col3 - MeasureText(shipStr, entry_font_size) / 2;
        int score_x = col4 - MeasureText(scoreStr, entry_font_size) / 2;

        Color ship_color = GetShipColor(entries[i].shipName);

        DrawText(posStr, pos_x, line_y, entry_font_size, WHITE);
        DrawText(nameStr, name_x, line_y, entry_font_size, WHITE);
        DrawText(shipStr, ship_x, line_y, entry_font_size, ship_color);
        DrawText(scoreStr, score_x, line_y, entry_font_size, WHITE);
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, rec_alpha));
    EndDrawing();
}
