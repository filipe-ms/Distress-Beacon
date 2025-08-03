#pragma once

#include "raylib.h"

// Texturas usadas em desenho de naves e projéteis
extern Texture texture_ships;
extern Texture texture_projectiles;
extern Texture texture_ship_assets;
extern Texture texture_custom_ships;
extern Texture texture_special_effects_1;

extern Texture background_stars;
extern Texture background_dark_space;
extern Texture background_stars_and_dust;
extern Texture background_menubg;
extern Texture background_ending;

extern Texture texture_pilots;

extern Texture texture_progress_bars;

extern Texture texture_planet_black_hole;
extern Texture texture_planet_galaxy;
extern Texture texture_planet_moon_1;
extern Texture texture_planet_moon_2;
extern Texture texture_planet_planet_1;
extern Texture texture_planet_planet_2;
extern Texture texture_planet_planet_3;
extern Texture texture_planet_sun;

extern Texture keyboard_buttons;

extern Music main_music;
extern Music in_game_music;
extern Music ending_music;

void LoadAllAssets(void);
void UnloadAllAssets(void);