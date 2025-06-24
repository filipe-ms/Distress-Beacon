// ship.c

#include "ship.h"
#include "raymath.h"
#include "common.h"
#include "weapon.h"
#include "texture_manager.h"

#define SOURCE_WH 8

#define MAX_THRUSTER_CYCLE 4

Ship ship;

typedef struct Orion {
	float dash_cooldown;
	float dash_recharge;
	float double_tap_timer;
	Vector2 dash_position;
	bool slide_flag;
	float slide_timer;
	int direction;
	int last_direction_pressed;
} Orion;

Orion orion;

static void InitShipSpecifics(Ship* ship, int id) {
	switch (id) {
	case AUREA:
	case ORION:
		orion.dash_cooldown = 5.0f;
		orion.dash_recharge = 0.0f;
		orion.double_tap_timer = 0.3f;
		orion.slide_flag = false;
		orion.slide_timer = 0.0f;
		orion.dash_position = (Vector2){ 0.0f, 0.0f };
		orion.direction = CENTER;
		orion.last_direction_pressed = -1;
	case NEBULA:
	default: break;
	}
	ship->speed = (Vector2){ 360.0f, 360.0f };
}

void InitShip(Ship* ship, int id) {
	ship->id = id;
	ship->position.x = (GAME_SCREEN_START + GAME_SCREEN_END) / 2;
	ship->position.y = SCREEN_HEIGHT - 100;
	ship->draw_size = (Vector2){ DRAW_WH, DRAW_WH };
	ship->direction = CENTER;
	ship->thruster_cycle = 0;
	ship->animation_cycle = 0.0f;
	ship->color = WHITE;
	ship->alpha = 1.0f;
	ship->is_alive = true;
	InitShipSpecifics(ship, id);
}

static void UpdateAurea() { }

static void UpdateOrion(Ship* ship) {

	// Cooldown
	if (orion.dash_recharge > 0) {
		orion.dash_recharge -= GetFrameTime();
		if (orion.dash_recharge < 0) orion.dash_recharge = 0;
	}

	if (orion.double_tap_timer > 0) {
		orion.double_tap_timer -= GetFrameTime();
		if (orion.double_tap_timer <= 0) orion.last_direction_pressed = -1;
	}

	// Slide behavior
	if (orion.slide_flag) {
		orion.slide_timer -= GetFrameTime();
		if (orion.slide_timer > 0) {
			float slide_speed = 2000.0f;
			switch (orion.direction) {
			case LEFT:  orion.dash_position.x -= slide_speed * GetFrameTime(); break;
			case RIGHT: orion.dash_position.x += slide_speed * GetFrameTime(); break;
			case UP:    orion.dash_position.y -= slide_speed * GetFrameTime(); break;
			case DOWN:  orion.dash_position.y += slide_speed * GetFrameTime(); break;
			}
			ship->position = orion.dash_position;
			return;
		}
		else {
			orion.slide_flag = false;
			orion.dash_recharge = orion.dash_cooldown;
			orion.slide_timer = 0;
		}
	}

	int current_direction = -1;
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) current_direction = RIGHT;
	else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) current_direction = LEFT;
	else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) current_direction = UP;
	else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) current_direction = DOWN;

	if (current_direction != -1) {
		if (orion.double_tap_timer > 0 && 
			orion.last_direction_pressed == current_direction &&
			orion.dash_recharge <= 0) {
			
			orion.slide_flag = true;
			orion.dash_position = ship->position;
			orion.slide_timer = 0.1f;
			orion.direction = current_direction;
			orion.double_tap_timer = 0.0f;
		}
		else {
			orion.last_direction_pressed = current_direction;
			orion.double_tap_timer = 0.3f;
		}
	}
}

static void UpdateNebula() { }

static void UpdatePuddleJumper() { }

static void WallBehavior(Vector2* position) {
	position->x = Clamp(position->x, GAME_SCREEN_START + DRAW_WH / 2, GAME_SCREEN_END - (DRAW_WH / 2));
	position->y = Clamp(position->y, DRAW_WH / 2, SCREEN_HEIGHT - DRAW_WH / 2);
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

	switch(ship->id) {
		case AUREA:
			UpdateAurea();
			break;
		case ORION:
			UpdateOrion(ship);
			break;
		case NEBULA:
			UpdateNebula();
			break;
		case PUDDLE_JUMPER:
			UpdatePuddleJumper();
			break;
		default:
			return;
	}

	WallBehavior(&ship->position);
}

void Ship_TakeDamage(Ship *ship)
{
	// --- VERIFICA SE A NAVE EST� INVENC�VEL ---
	if (ship->isInvincible) {
		return; // Se estiver, ignora a colis�o e continua o loop
	}

	if (IsShieldActive())
	{
		shield.capacity--;
		
		// --- ATIVA A INVENCIBILIDADE AP�S O HIT ---
		ship->isInvincible = true;
		ship->invincibilityTimer = 1.0f; // Dura��o de 1 segundo
	
		if (shield.capacity <= 0) {
			DeactivateShield();
			return;
		}
	}
	
	ship->is_alive = false;
}

static void DrawPuddleJumper(Ship* ship, Rectangle draw_pos) {
	Rectangle puddle_jumper_sprite = { 8, 16, SOURCE_WH, SOURCE_WH }; // Sprite base, andando p/ cima
	Rectangle left_thruster_sprite = { 40, 0, 8, 8 };		// Primeiro thruster da sheet
	Rectangle right_thruster_sprite = { 56, 0, 8, 8 };		// Referência do segundo thruster
	
	Rectangle left_thruster_position = { draw_pos.x - 18, draw_pos.y + DRAW_WH, ship->draw_size.x, ship->draw_size.y };
	Rectangle right_thruster_position = { draw_pos.x + 12, draw_pos.y + DRAW_WH, ship->draw_size.x, ship->draw_size.y };

	if (ship->direction == LEFT) {
		puddle_jumper_sprite.x -= SOURCE_WH;
		left_thruster_position.x -= 4;
		right_thruster_position.x -= 10;
	} else if (ship->direction == RIGHT) {
		puddle_jumper_sprite.x += SOURCE_WH;
		left_thruster_position.x += 10;
		right_thruster_position.x += 4;
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

	DrawTexturePro(texture_ships, puddle_jumper_sprite, draw_pos, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(texture_ship_assets, left_thruster_sprite, left_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(texture_ship_assets, right_thruster_sprite, right_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));

	if (DEBUG_FLAG) DrawCircleV(ship->position, 20, Fade(BLUE, 0.5f));
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

	DrawTexturePro(texture_ships, aurea_sprite, draw_pos, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(texture_ship_assets, left_thruster_sprite, left_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(texture_ship_assets, right_thruster_sprite, right_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));

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

	DrawTexturePro(texture_ships, orion_sprite, draw_pos, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(texture_ship_assets, thruster_sprite, thruster_position, origin, rotation, Fade(ship->color, ship->alpha));

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

	DrawTexturePro(texture_ships, nebula_sprite, draw_pos, origin, 0.0f, Fade(ship->color, ship->alpha));
	DrawTexturePro(texture_ship_assets, left_thruster_sprite, left_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(texture_ship_assets, center_thruster_sprite, center_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));
	DrawTexturePro(texture_ship_assets, right_thruster_sprite, right_thruster_position, origin, rotation, Fade(ship->color, ship->alpha));

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
	case NEBULA:
		DrawNebula(ship, destination);
		break;
	case PUDDLE_JUMPER:
		DrawPuddleJumper(ship, destination);
		break;
	}

	if (ship->isInvincible) {
		ship->invincibilityTimer -= GetFrameTime();

		TraceLog(LOG_INFO, "tempo que falta eh %.2f", ship->invincibilityTimer);

		if (ship->invincibilityTimer <= 0) {
			ship->isInvincible = false;

			TraceLog(LOG_INFO, "vulneravel");
		}
	}
}