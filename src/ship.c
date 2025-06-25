// ship.c

#include "ship.h"
#include "raymath.h"
#include "common.h"
#include "weapon.h"
#include "texture_manager.h"
#include "general_utils.h"
#include "hit_confirmation.h"

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

typedef struct PuddleJumper {
	Vector2 wormhole_portal_offset;
	float wormhole_cooldown;
	float wormhole_duration;

	float wormhole_current_cooldown;
	float wormhole_current_duration;

	Vector2 wormhole_in_position;
	Vector2 wormhole_out_position;

	bool wormhole_is_recharged;
} PuddleJumper;

PuddleJumper puddle_jumper;

typedef enum AureaDroneState {
	DRONE_INITIALIZED,
	DRONE_DISPATCHED,
	DRONE_ROTATING,
	DRONE_FIRING,
	DRONE_FINALIZING,
} AureaDroneState;

typedef struct Aurea {
	int drone_shots; 

	float drone_cooldown;
	float drone_firing_duration;
	float drone_dispatch_duration;
	Vector2 drone_max_range;
	
	float drone_current_cooldown;
	float drone_firing_current_duration;
	float drone_dispatch_current_duration;
	int drone_fired_shots;

	Vector2 drone_left_cast_position;
	Vector2 drone_right_cast_position;

	SpecialEffect* drone_left;
	SpecialEffect* drone_right;

	SpecialEffect* drone_left_thruster;
	SpecialEffect* drone_right_thruster;

	AureaDroneState state;
} Aurea;

Aurea aurea;

static void InitShipSpecifics(Ship* ship, int id) {
	switch (id) {
	case AUREA:
		aurea.drone_shots = 5;
		aurea.drone_cooldown = aurea.drone_current_cooldown = 10.0f;
		aurea.drone_firing_duration = 5.0f;
		aurea.drone_dispatch_duration = 2.0f;
		aurea.drone_max_range = (Vector2) { 0, -200 };
		aurea.state = DRONE_INITIALIZED;
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
	case PUDDLE_JUMPER:
		puddle_jumper.wormhole_cooldown = 10.0f;
		puddle_jumper.wormhole_portal_offset = (Vector2) { 0, -400 };
		puddle_jumper.wormhole_duration = 5.0f;
		puddle_jumper.wormhole_current_duration = 0.0f;
		puddle_jumper.wormhole_current_cooldown = 10.0f;
		puddle_jumper.wormhole_is_recharged = true;
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

static void UpdateAurea() { 
	bool is_ability_ready = false;

	aurea.drone_cooldown = ClampWithFlagsF(
		aurea.drone_current_cooldown + GetFrameTime(),
		0, aurea.drone_cooldown,
		NULL, &is_ability_ready);

	// Drone animations
	switch(aurea.state) {
		case DRONE_INITIALIZED:
			if (!IsKeyPressed(KEY_Z) || !is_ability_ready)
				break;
			// When it activates
			Vector2 left_offset = (Vector2) { -20, 0 };
			Vector2 right_offset = (Vector2) { 20, 0 };
	
			aurea.drone_left_cast_position = Vector2Add(ship.position, left_offset);
			aurea.drone_right_cast_position = Vector2Add(ship.position, right_offset);
	
			aurea.drone_left = CreateUnmanagedEffect(DRONE, aurea.drone_left_cast_position, aurea.drone_dispatch_duration);
			aurea.drone_right = CreateUnmanagedEffect(DRONE, aurea.drone_right_cast_position, aurea.drone_dispatch_duration);
	
			aurea.drone_left_thruster = CreateUnmanagedEffect(DRONE_THRUSTER, aurea.drone_left_cast_position, 0);
			aurea.drone_right_thruster = CreateUnmanagedEffect(DRONE_THRUSTER, aurea.drone_right_cast_position, 0);

			aurea.drone_dispatch_current_duration = 0;
			aurea.drone_current_cooldown = 0;
			aurea.state = DRONE_DISPATCHED;
			break;

		case DRONE_DISPATCHED:
			bool drones_reached_limit = false;
			aurea.drone_dispatch_current_duration = ClampWithFlagsF(
				aurea.drone_dispatch_current_duration + GetFrameTime(), 
				0, aurea.drone_dispatch_duration, 
				NULL, &drones_reached_limit);
			
			float sin_ease = sinf((aurea.drone_dispatch_current_duration / aurea.drone_dispatch_duration) * (PI / 2)) * aurea.drone_dispatch_duration;

			Vector2 thruster_offset = (Vector2) { 0, 24 };

			aurea.drone_left->position = Vector2Add(aurea.drone_left_cast_position,
				Vector2MultiplyScalarF(
					aurea.drone_max_range, sin_ease));

			aurea.drone_left_thruster->position = Vector2Add(aurea.drone_left->position, thruster_offset);

			aurea.drone_right->position = Vector2Add(aurea.drone_right_cast_position,
				Vector2MultiplyScalarF(
					aurea.drone_max_range, sin_ease));

			aurea.drone_right_thruster->position = Vector2Add(aurea.drone_right->position, thruster_offset);

			if (drones_reached_limit) {
				aurea.state = DRONE_FIRING;
				aurea.drone_fired_shots = 0;

				DestroyEffect(aurea.drone_left_thruster);
				DestroyEffect(aurea.drone_right_thruster);
				aurea.drone_left_thruster = aurea.drone_right_thruster = NULL;
			}
			break;

		case DRONE_FIRING:
			InitPulseShootAtCoords(&ship, aurea.drone_left->position, aurea.drone_left->rotation + 180);
			InitPulseShootAtCoords(&ship, aurea.drone_right->position, aurea.drone_right->rotation + 180);
			aurea.drone_fired_shots++;
			aurea.state = DRONE_ROTATING;

			if (aurea.drone_fired_shots > aurea.drone_shots) {
				aurea.state = DRONE_FINALIZING;
				break;
			}
			
			break;

		case DRONE_ROTATING:
			bool has_reached_turning_angle = false;

			aurea.drone_firing_current_duration = ClampWithFlagsF(
				aurea.drone_firing_current_duration + GetFrameTime(),
				0, ((float)aurea.drone_fired_shots / (float)aurea.drone_shots),
				NULL, &has_reached_turning_angle);

			aurea.drone_left->rotation = -aurea.drone_firing_current_duration * 180 + 180;
			aurea.drone_right->rotation = aurea.drone_firing_current_duration * 180 + 180;

			if (has_reached_turning_angle) {
				aurea.state = DRONE_FIRING;
			}
			break;
			
		case DRONE_FINALIZING:		
			CreateManagedEffect(DRONE_EXPLOSION, aurea.drone_left->position);
			CreateManagedEffect(DRONE_EXPLOSION, aurea.drone_right->position);

			DestroyEffect(aurea.drone_left);
			DestroyEffect(aurea.drone_right);

			aurea.drone_left = aurea.drone_right = NULL;
			aurea.state = DRONE_INITIALIZED;
			break;
	}
}

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

static void UpdatePuddleJumper() {
	// Cooldown
	bool is_ability_ready = false;

	puddle_jumper.wormhole_current_cooldown = ClampWithFlagsF(
		puddle_jumper.wormhole_current_cooldown + GetFrameTime(),
		0, puddle_jumper.wormhole_cooldown,
		NULL, &is_ability_ready);

	if (is_ability_ready){
		puddle_jumper.wormhole_is_recharged = true;
	}

	// Wormhole expiration time logic
	bool is_duration_expired = false;

	puddle_jumper.wormhole_current_duration = ClampWithFlagsF(
		puddle_jumper.wormhole_current_duration + GetFrameTime(),
		0, puddle_jumper.wormhole_duration, NULL, &is_duration_expired);

	if (IsKeyPressed(KEY_Z)) {
		// When it activates
		if (puddle_jumper.wormhole_is_recharged) {
			puddle_jumper.wormhole_in_position = ship.position;
			puddle_jumper.wormhole_out_position = Vector2Add(ship.position, puddle_jumper.wormhole_portal_offset);	
			puddle_jumper.wormhole_current_duration = 0;
			puddle_jumper.wormhole_current_cooldown = 0;
			puddle_jumper.wormhole_is_recharged = false;

			if (puddle_jumper.wormhole_out_position.y <= 0) {
				puddle_jumper.wormhole_out_position.y = ship.position.y - puddle_jumper.wormhole_portal_offset.y;
			}
			
			CreateManagedEffectDuration(WORMHOLE, puddle_jumper.wormhole_in_position, puddle_jumper.wormhole_duration);
			CreateManagedEffectDuration(WORMHOLE, puddle_jumper.wormhole_out_position, puddle_jumper.wormhole_duration);
			return;
		}
		
		// Jump-in logic
		if (!is_duration_expired) {
			bool is_intersecting_portal_in = CheckCollisionCircles(ship.position, ship.draw_size.x, puddle_jumper.wormhole_in_position, 40);
			bool is_intersecting_portal_out = CheckCollisionCircles(ship.position, ship.draw_size.x, puddle_jumper.wormhole_out_position, 40);
		
			if (is_intersecting_portal_in) {
				ship.position = puddle_jumper.wormhole_out_position;
				CreateManagedEffectDuration(WORMHOLE_TELEPORT_ANIMATION, puddle_jumper.wormhole_out_position, 0.5f);
			}
	
			if (is_intersecting_portal_out) {
				ship.position = puddle_jumper.wormhole_in_position;
				CreateManagedEffectDuration(WORMHOLE_TELEPORT_ANIMATION, puddle_jumper.wormhole_in_position, 0.5f);
			}
		}
	}
}

static void UpdateVoid() {}

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
		case VOID:
			UpdateVoid();
			break;
		default:
			return;
	}

	WallBehavior(&ship->position);
}

void Ship_TakeDamage(Ship *ship)
{
	// --- VERIFICA SE A NAVE ESTÁ INVENCÍVEL ---
	if (ship->isInvincible) {
		return; // Se estiver, ignora a colisão e continua o loop
	}

	if (IsShieldActive())
	{
		shield.capacity--;
		
		// --- ATIVA A INVENCIBILIDADE APÓS O HIT ---
		ship->isInvincible = true;
		ship->invincibilityTimer = 1.0f; // Duração de 1 segundo
	
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

static void DrawVoid(Ship* ship, Rectangle draw_pos) {
	Rectangle void_sprite = { 8, 32, SOURCE_WH, SOURCE_WH };

	if (ship->direction == LEFT) {
		void_sprite.x -= SOURCE_WH;
	}
	else if (ship->direction == RIGHT) {
		void_sprite.x += SOURCE_WH;
	}

	Vector2 origin = { 0 };
	float rotation = 0.0f;

	DrawTexturePro(texture_ships, void_sprite, draw_pos, origin, rotation, Fade(ship->color, ship->alpha));

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
	case VOID:
		DrawVoid(ship, destination);
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