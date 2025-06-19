// ship.c

#include "ship.h"
#include "raymath.h"
#include "common.h"
#include "weapon.h"
#include "game_behavior.h"

#define SOURCE_WH 8

#define MAX_THRUSTER_CYCLE 4

Texture ships;
Texture thrusters;

static Vector2 GetInitShipSpeed(int id) {
	switch (id) {
	case AUREA:
	case ORION:
	case NEBULA:
	default: return (Vector2) { 360.0f, 360.0f };
	}
}

void InitShip(Ship* ship, int id) {
	ship->id = id;
	ship->position.x = GAME_SCREEN_WIDTH/2;
	ship->position.y = SCREEN_HEIGHT - 100;
	ship->draw_size = (Vector2){ DRAW_WH, DRAW_WH };
	ship->direction = CENTER;
	ship->thruster_cycle = 0;
	ship->animation_cycle = 0.0f;
	ship->color = WHITE;
	ship->alpha = 1.0f;
	ship->speed = GetInitShipSpeed(id);
}
void UpdateShip(Ship* ship) {
	float movement_x = ship->speed.x * GetFrameTime();
	float movement_y = ship->speed.y * GetFrameTime();

	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
		ship->position.x += movement_x;
		ship->direction = RIGHT;
	} else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
		ship->position.x -= movement_x;
		ship->direction = LEFT;
	}
	else ship->direction = CENTER;

	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) ship->position.y -= movement_y;
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) ship->position.y += movement_y;

	ship->animation_cycle += GetFrameTime();

	if (ship->animation_cycle > 0.25f) {
		ship->animation_cycle -= 0.25f;
		ship->thruster_cycle = (ship->thruster_cycle + 1) % MAX_THRUSTER_CYCLE;
	}

	WallBehavior(&ship->position);
}

static void DrawAurea(Ship* ship, Rectangle draw_pos) {
	Rectangle aurea_sprite = { 8, 8, SOURCE_WH, SOURCE_WH }; // Sprite base, andando p/ cima
	Rectangle left_thruster_sprite = { 40, 0, 8, 8 };		// Primeiro thruster da sheet
	Rectangle right_thruster_sprite = { 56, 0, 8, 8 };		// Referência do segundo thruster
	
	Rectangle left_thruster_position = { draw_pos.x - 18, draw_pos.y + DRAW_WH, ship->draw_size.x, ship->draw_size.y };
	Rectangle right_thruster_position = { draw_pos.x + 12, draw_pos.y + DRAW_WH, ship->draw_size.x, ship->draw_size.y };

	if (ship->direction == LEFT) {
		aurea_sprite.x -= SOURCE_WH;
		left_thruster_position.x -= 6;
		right_thruster_position.x -= 12;
	} else if (ship->direction == RIGHT) {
		aurea_sprite.x += SOURCE_WH;
		left_thruster_position.x += 12;
		right_thruster_position.x += 6;
	}

	if (ship->thruster_cycle == 1) {
		left_thruster_sprite.x = 48;
		right_thruster_sprite.x = 64;
	}
	else if (ship->thruster_cycle == 2) {
		left_thruster_sprite.x = 56;
		right_thruster_sprite.x = 48;
	}
	else if (ship->thruster_cycle == 3) {
		left_thruster_sprite.x = 64;
		right_thruster_sprite.x = 40;
	}

	Vector2 origin = { 0 };
	float rotation = 0.0f;

	DrawTexturePro(ships, aurea_sprite, draw_pos, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(thrusters, left_thruster_sprite, left_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(thrusters, right_thruster_sprite, right_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));

	if (DEBUG_FLAG) DrawCircleV(ship->position, 20, Fade(BLUE, 0.5f));
}

static void DrawOrion(Ship* ship, Rectangle draw_pos) {
	Rectangle orion_sprite = { 8, 0, SOURCE_WH, SOURCE_WH };
	Rectangle thruster_sprite = { 40, 8, SOURCE_WH, SOURCE_WH };

	Rectangle thruster_position = draw_pos;
	thruster_position.y += 42;

	if (ship->direction == LEFT) {
		orion_sprite.x -= SOURCE_WH;
		thruster_position.x -= 6;
	} else if (ship->direction == RIGHT) {
		orion_sprite.x += SOURCE_WH;
		thruster_position.x += 6;
	}

	thruster_sprite.x = 40 + ship->thruster_cycle * 8;

	Vector2 origin = { 0 };
	float rotation = 0.0f;

	DrawTexturePro(ships, orion_sprite, draw_pos, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(thrusters, thruster_sprite, thruster_position, origin, rotation, Fade(ship->color, ship->alpha));

	if (DEBUG_FLAG) DrawCircleV(ship->position, 20, Fade(BLUE, 0.5f));
}

static void DrawNebula(Ship* ship, Rectangle draw_pos) {
	Rectangle nebula_sprite = { 8, 24, 8, 8 };

	Rectangle left_thruster_sprite = { 80, 0, 8, 8 };
	Rectangle center_thruster_sprite = { 96, 8, 8, 8 };
	Rectangle right_thruster_sprite = { 96, 0, 8, 8 };

	Rectangle left_thruster_position = { draw_pos.x - 24, draw_pos.y + 36, ship->draw_size.x, ship->draw_size.y };
	Rectangle center_thruster_position = { draw_pos.x, draw_pos.y + 48, ship->draw_size.x, ship->draw_size.y };
	Rectangle right_thruster_position = { draw_pos.x + 18, draw_pos.y + 36, ship->draw_size.x, ship->draw_size.y };

	if (ship->thruster_cycle == 1) {
		left_thruster_sprite.x = 88;
		right_thruster_sprite.x = 72;
	}
	else if (ship->thruster_cycle == 2) {
		left_thruster_sprite.x = 96;
		right_thruster_sprite.x = 88;
	}
	else if (ship->thruster_cycle == 3) {
		left_thruster_sprite.x = 72;
		right_thruster_sprite.x = 80;
	}

	center_thruster_sprite.x -= 8 * ship->thruster_cycle;

	Vector2 origin = { 0 };
	float rotation = 0.0f;

	DrawTexturePro(ships, nebula_sprite, draw_pos, origin, 0.0f, Fade(ship->color, ship->alpha));
	DrawTexturePro(thrusters, left_thruster_sprite, left_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(thrusters, center_thruster_sprite, center_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(thrusters, right_thruster_sprite, right_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));

	if (DEBUG_FLAG) DrawCircleV(ship->position, 20, Fade(BLUE, 0.5f));

}

void DrawShip(Ship* ship) {
	Rectangle destination = {
		ship->position.x - DRAW_WH / 2,
		ship->position.y - DRAW_WH / 2,
		ship->draw_size.x, 
		ship->draw_size.y
	};

	switch (ship->id) {
	case AUREA:
		DrawAurea(ship, destination);
		break;
	case ORION:
		DrawOrion(ship, destination);
		break;
	default:
		DrawNebula(ship, destination);
		break;
	}
}
void LoadShipTextures(void) {
    ships = LoadTexture("ships.png");
    thrusters = LoadTexture("playerassets.png");
}
void UnloadShipTextures(void) {
    UnloadTexture(ships);
    UnloadTexture(thrusters);
}