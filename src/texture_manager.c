#include "texture_manager.h"

Texture texture_ships;
Texture texture_projectiles;
Texture texture_ship_assets;
Texture texture_custom_ships;

Texture background_stars;
Texture background_dark_space;
Texture background_stars_and_dust;
Texture background_menubg;

void LoadAllTextures(void) {
	texture_ships = LoadTexture("ships.png");
	texture_custom_ships = LoadTexture("custom_ships.png");
	texture_projectiles = LoadTexture("weapons.png");
	texture_ship_assets = LoadTexture("playerassets.png");

	background_stars = LoadTexture("background/stars.png");
	background_dark_space = LoadTexture("background/dark_space.png");
	background_stars_and_dust = LoadTexture("background/stars_and_dust.png");
	background_menubg = LoadTexture("background/menubg.png");
}
void UnloadAllTextures(void) {
	UnloadTexture(texture_ships);
	UnloadTexture(texture_projectiles);
	UnloadTexture(texture_ship_assets);
	UnloadTexture(texture_custom_ships);

	UnloadTexture(background_stars);
	UnloadTexture(background_dark_space);
	UnloadTexture(background_stars_and_dust);
	UnloadTexture(background_menubg);
}