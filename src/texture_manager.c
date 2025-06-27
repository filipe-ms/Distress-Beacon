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

Texture texture_pilots;

void LoadAllTextures(void) {
	texture_ships = LoadTexture("ships.png");
	texture_custom_ships = LoadTexture("custom_ships.png");
	texture_projectiles = LoadTexture("weapons.png");
	texture_ship_assets = LoadTexture("playerassets.png");
	texture_special_effects_1 = LoadTexture("sfx-1.png");

	background_stars = LoadTexture("background/stars.png");
	background_dark_space = LoadTexture("background/dark_space.png");
	background_stars_and_dust = LoadTexture("background/stars_and_dust.png");

	texture_pilots = LoadTexture("pilots.png");
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

	UnloadTexture(texture_pilots);
}