#include "texture_manager.h"

Texture texture_ships;
Texture texture_projectiles;
Texture texture_ship_assets;
Texture texture_custom_ships;
Texture texture_special_effects_1;

Texture background_stars;
Texture background_dark_space;
Texture background_stars_and_dust;
Texture background_menubg;
Texture background_ending;

Texture texture_pilots;
Texture texture_progress_bars;

Texture texture_planet_black_hole;
Texture texture_planet_galaxy;
Texture texture_planet_moon_1;
Texture texture_planet_moon_2;
Texture texture_planet_planet_1;
Texture texture_planet_planet_2;
Texture texture_planet_planet_3;
Texture texture_planet_sun;

Texture keyboard_buttons;

void LoadAllTextures(void) {
	texture_ships = LoadTexture("ships.png");
	texture_custom_ships = LoadTexture("custom_ships.png");
	texture_projectiles = LoadTexture("weapons.png");
	texture_ship_assets = LoadTexture("playerassets.png");
	texture_special_effects_1 = LoadTexture("sfx-1.png");

	background_stars = LoadTexture("background/stars.png");
	background_dark_space = LoadTexture("background/dark_space.png");
	background_stars_and_dust = LoadTexture("background/stars_and_dust.png");
	background_ending = LoadTexture("background/ending.png");

	texture_pilots = LoadTexture("pilots.png");

	texture_progress_bars = LoadTexture("progress_bars.png");

	texture_planet_black_hole = LoadTexture("ending_1/BlackHole.png");
	texture_planet_galaxy = LoadTexture("ending_1/Galaxy.png");
	texture_planet_moon_1 = LoadTexture("ending_1/Moon1.png");
	texture_planet_moon_2 = LoadTexture("ending_1/Moon2.png");
	texture_planet_planet_1 = LoadTexture("ending_1/Planet1.png");
	texture_planet_planet_2 = LoadTexture("ending_1/Planet2.png");
	texture_planet_planet_3 = LoadTexture("ending_1/Planet3.png");
	texture_planet_sun = LoadTexture("ending_1/Sun.png");	
	
	keyboard_buttons = LoadTexture("keys/keyboard.png");
}

void UnloadAllTextures(void) {
	UnloadTexture(texture_ships);
	UnloadTexture(texture_projectiles);
	UnloadTexture(texture_ship_assets);
	UnloadTexture(texture_custom_ships);
	UnloadTexture(texture_special_effects_1);

	UnloadTexture(background_stars);
	UnloadTexture(background_dark_space);
	UnloadTexture(background_stars_and_dust);
	UnloadTexture(background_ending);

	UnloadTexture(texture_pilots);
	UnloadTexture(texture_progress_bars);

	UnloadTexture(texture_planet_black_hole);
	UnloadTexture(texture_planet_galaxy);
	UnloadTexture(texture_planet_moon_1);
	UnloadTexture(texture_planet_moon_2);
	UnloadTexture(texture_planet_planet_1);
	UnloadTexture(texture_planet_planet_2);
	UnloadTexture(texture_planet_planet_3);
	UnloadTexture(texture_planet_sun);
	
	UnloadTexture(keyboard_buttons);
}