// ship.c

#include "ship.h"
#include "raymath.h"
#include "common.h"
#include "weapon.h"
#include "game_behavior.h"

#define AUREA 0
#define ORION 1
#define NEBULA 2

#define CENTER 0
#define LEFT 1
#define RIGHT 2

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
	ship->draw_size = (Vector2){ DRAW_WH };
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

	if (IsKeyDown(KEY_RIGHT)) {
		ship->position.x += movement_x;
		ship->direction = RIGHT;
	} else if (IsKeyDown(KEY_LEFT)) {
		ship->position.x -= movement_x;
		ship->direction = LEFT;
	}
	else ship->direction = CENTER;

	if (IsKeyDown(KEY_UP)) ship->position.y -= movement_y;
	if (IsKeyDown(KEY_DOWN)) ship->position.y += movement_y;

	ship->animation_cycle += GetFrameTime();

	if (ship->animation_cycle > 0.25f) {
		ship->animation_cycle -= 0.25f;
		ship->thruster_cycle = (ship->thruster_cycle + 1) % MAX_THRUSTER_CYCLE;
	}

	WallBehavior(&ship->position);
}
static void DrawAurea(Ship* ship, Rectangle draw_pos) {
	Rectangle ship_sprite = { 8, 8, SOURCE_WH, SOURCE_WH }; // Sprite base, andando p/ cima
	Rectangle left_thruster_sprite = { 40, 0, 8, 8 };		// Primeiro thruster da sheet
	Rectangle right_thruster_sprite = { 56, 0, 8, 8 };		// Referência do segundo thruster
	
	Rectangle left_thruster_position = { draw_pos.x - 18, draw_pos.y + DRAW_WH, DRAW_WH, DRAW_WH };
	Rectangle right_thruster_position = { draw_pos.x + 12, draw_pos.y + DRAW_WH, DRAW_WH, DRAW_WH };

	if (ship->direction == LEFT) {
		ship_sprite.x -= SOURCE_WH;
		left_thruster_position.x -= 6;
		right_thruster_position.x -= 12;
	} else if (ship->direction == RIGHT) {
		ship_sprite.x += SOURCE_WH;
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

	if (DEBUG_FLAG) {
		DrawCircleV(ship->position, 20, Fade(BLUE, 0.5f));
	}

	DrawTexturePro(ships, ship_sprite, draw_pos, (Vector2) { 0 }, 0.0f, Fade(ship->color, ship->alpha));
	DrawTexturePro(thrusters, left_thruster_sprite, left_thruster_position, (Vector2) { 0 }, 0.0f, Fade(ship->color, ship->alpha));
	DrawTexturePro(thrusters, right_thruster_sprite, right_thruster_position, (Vector2) { 0 }, 0.0f, Fade(ship->color, ship->alpha));
}
void DrawShip(Ship* ship) {
	Rectangle destination = {
		ship->position.x - DRAW_WH / 2,
		ship->position.y - DRAW_WH / 2,
		DRAW_WH,
		DRAW_WH
	};

	switch (ship->id) {
	case AUREA:
		DrawAurea(ship, destination);
		break;
	default:
		DrawAurea(ship, destination); // Para outros tipos de naves, por enquanto, desenha como Aurea
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