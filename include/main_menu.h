// main_menu.h

#pragma once

#include "raylib.h"

typedef enum {
    MENU_OPTION_START,
    MENU_OPTION_RANKING,
    MENU_OPTION_CREDITS,
    MENU_OPTION_EXIT,
    MENU_OPTION_COUNT
} MainMenuOption;

void InitMainMenu();
void UpdateMainMenu();
void DrawMainMenu();
