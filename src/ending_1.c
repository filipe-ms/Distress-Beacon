// select_ship.c

#include "common.h"
#include "background.h"
#include "scene_draw_effects.h"
#include "raymath.h"
#include "timer.h"
#include "ship.h"
#include "pilot.h"
#include "draw_utils.h"
#include "ship_references.h"
#include "input.h"
#include "left_ui.h"

void Ending1Scene_Init() {
    InitLeftUIPanel();
    SetTopPilotDefault();
}

void Ending1Scene_Update() {
    UpdatePilot();
}

void Ending1Scene_Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawBackground();

    DrawLeftUIPanel();
    
    // Right UI Pannel
    DrawRectangle(GAME_SCREEN_END, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);

    EndDrawing();
}