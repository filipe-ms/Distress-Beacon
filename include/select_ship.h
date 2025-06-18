// select_ship.h

#pragma once

#include "raylib.h"

typedef struct SelectMenuShip {
    Texture texture;
    Rectangle sprite_location;
    Rectangle sprite_destination;
    Color color;
    float alpha;
} SelectMenuShip;

void InitSelectMenu(void);
void UpdateShipSelectMenu(void);
void DrawSelectMenu(void);
void LoadSelectMenuTextures(void);
void UnloadSelectMenuTextures(void);

void DrawSelectMenuBackground(void);
