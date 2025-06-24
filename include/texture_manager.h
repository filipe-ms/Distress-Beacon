#pragma once

#include "raylib.h"

// Texturas usadas em desenho de naves e projéteis
extern Texture texture_ships;
extern Texture texture_projectiles;
extern Texture texture_ship_assets;

extern Texture background_stars;
extern Texture background_dark_space;
extern Texture background_stars_and_dust;
extern Texture background_menubg;

void LoadAllTextures(void);
void UnloadAllTextures(void);