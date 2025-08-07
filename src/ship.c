// ship.c

#include "ship.h"
#include "raymath.h"
#include "common.h"
#include "weapon.h"
#include "texture_manager.h"
#include "general_utils.h"
#include "special_effects.h"
#include "input.h"
#include "audio_manager.h"

#define SOURCE_WH 8

#define MAX_THRUSTER_CYCLE 4

#define NEBULA_PARTICLE_A_COUNT 15
#define NEBULA_PARTICLE_B_COUNT 15
#define NEBULA_PARTICLE_C_COUNT 15

Ship ship;

typedef enum OrionDashState {
	DASH_INACTIVE,
	DASH_ACCELERATING,
	DASH_FULL_SPEED,
	DASH_DEACCELERATING,
} OrionDashState;

typedef struct Orion {
	float dash_is_in_use;

	float dash_cooldown;
	float dash_current_cooldown;

	Vector2 dash_speed_modifier;
	Vector2 dash_current_speed_modifier;

	float dash_alpha_modifier;
	float dash_alpha_current_modifier;

	float dash_total_time;
	float dash_current_total_time;

	float dash_acceleration_time;
	float dash_full_speed_time;
	float dash_deacceleration_time;

	float dash_disruption_field_base_damage;
	float dash_disruption_field_damage_tick;
	float dash_disruption_field_current_damage_tick;

	Vector2 dash_disruption_field_area;
	SpecialEffect* dash_disruption_field;

	OrionDashState dash_state;
} Orion;

Orion orion;

typedef enum PuddleJumperPortalAnimationState {
	WORMHOLE_INACTIVE,
	WORMHOLE_SPAWNING,
	WORMHOLE_MOVING,
	WORMHOLE_EXITING,
} PuddleJumperPortalAnimationState;

typedef struct PuddleJumper {
	float wormhole_enter_is_in_use;
	float wormhole_spawn_is_in_use;

	float wormhole_spawn_cooldown;
	float wormhole_spawn_current_cooldown;
	
	float wormhole_enter_cooldown;
	float wormhole_enter_current_cooldown;

	float wormhole_scale_time;
	float wormhole_current_in_scale_time;
	float wormhole_current_out_scale_time;
	
	bool wormhole_in_active;
	bool wormhole_out_active;

	float wormhole_moving_time;
	float wormhole_current_moving_time;

	float wormhole_base_damage;
	
	SpecialEffect* wormhole_in_effect;
	SpecialEffect* wormhole_out_effect;
	SpecialEffect* wormhole_puddle_jumper;
	SpecialEffect* wormhole_tether;

	PuddleJumperPortalAnimationState wormhole_state;

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

typedef struct NebulaParticleTracker {
	SpecialEffect* effect;
	float angle;
	float motion_current_time;
	float motion_time;
} NebulaParticleTracker;

typedef struct Nebula {
	float blaster_cooldown;
	float blaster_current_cooldown;

	float blaster_charge_time_a;
	float blaster_charge_time_b;
	float blaster_charge_time_c;

	float blaster_base_damage_a;
	float blaster_base_damage_b;
	float blaster_base_damage_c;

	float blaster_current_charge_time;

	Vector2 particle_initial_offset;

	SpecialEffect* charge_energy_field;

	NebulaParticleTracker a_particles[NEBULA_PARTICLE_A_COUNT];
	NebulaParticleTracker b_particles[NEBULA_PARTICLE_B_COUNT];
	NebulaParticleTracker c_particles[NEBULA_PARTICLE_C_COUNT];
} Nebula;

Nebula nebula;

#define EVENT_HORIZON_SIZE 300.0f
#define EVENT_HORIZON_PULSE_PERIOD 1.0f
#define EVENT_HORIZON_MAX_PULSES 5

typedef enum VoidSpecialState {
	VOID_SPECIAL_INACTIVE,
	VOID_SPECIAL_ACTIVE,
	VOID_SPECIAL_ENDING,
} VoidSpecialState;

typedef struct Void {
	float event_horizon_cooldown;
	float event_horizon_current_cooldown;

	float event_horizon_duration;
	int event_horizon_pulse_count;

	float event_horizon_base_damage;
	float event_horizon_current_damage;

	bool is_skill_ready;

	//SpecialEffect* event_horizon_effect;
	VoidSpecialState event_horizon_state;

	Vector2 event_horizon_center;
} Void;

Void ship_void;

Vector2 GetShipPosition(void) {
	return ship.position;
}

float GetShipCooldownPct(int ship) {
	switch (ship) {
	case AUREA:
		return aurea.drone_current_cooldown / aurea.drone_cooldown;
	case ORION:
		return (orion.dash_is_in_use ? 0 : orion.dash_current_cooldown / orion.dash_cooldown);
	case NEBULA:
		return nebula.blaster_current_cooldown / nebula.blaster_cooldown;
	case PUDDLE_JUMPER:
		return (puddle_jumper.wormhole_enter_is_in_use ? 0 : puddle_jumper.wormhole_enter_current_cooldown / puddle_jumper.wormhole_enter_cooldown);
	case VOID:
		return (ship_void.event_horizon_state > 0 ? 1 : 1.0 - (ship_void.event_horizon_current_cooldown / ship_void.event_horizon_cooldown));

	default:
		return 0.0f;
	}
}

float GetExtraGaugePct(int ship) {
	switch (ship) {
		case PUDDLE_JUMPER:
			return (puddle_jumper.wormhole_spawn_is_in_use ? 0 : puddle_jumper.wormhole_spawn_current_cooldown / puddle_jumper.wormhole_spawn_cooldown);
		case NEBULA: {
			// Level 1
			float gauge_1_completion = fminf(Normalize(nebula.blaster_current_charge_time, 0, nebula.blaster_charge_time_a), 1.0f);

			if (gauge_1_completion < 1)
				return gauge_1_completion;

			// Level 2
			float gauge_2_completion = fminf(Normalize(nebula.blaster_current_charge_time, nebula.blaster_charge_time_a, nebula.blaster_charge_time_b), 1.0f);

			if (gauge_2_completion < 1)
				return 1 + gauge_2_completion;

			// Level 3
			float gauge_3_completion = fminf(Normalize(nebula.blaster_current_charge_time, nebula.blaster_charge_time_b, nebula.blaster_charge_time_c), 1.0f);

			return 2 + gauge_3_completion;
		}
		default:
			return 0.0f;
	}
}

static void InitNebulaParticle(EffectType effect, int particle_count, NebulaParticleTracker* particle_arr, float particle_motion_time) {
	for(int i = 0; i < particle_count; i++) {
		particle_arr[i] = (NebulaParticleTracker) {
			.effect = CreateUnmanagedEffect(effect, (Vector2){ 0 }, 0),
			.motion_current_time = particle_motion_time * ((float)(i + 1) / (float)particle_count),
			.motion_time = particle_motion_time,
			.angle = (float)GetRandomValue(0, 3600) / 10.0f * DEG2RAD,
		};

		particle_arr[i].effect->color = Fade(WHITE, 0);
		particle_arr[i].effect->rotation = (float)GetRandomValue(0, 3600) / 10.0f * DEG2RAD;
		particle_arr[i].effect->position = Vector2Rotate(
			Vector2MultiplyScalarF(nebula.particle_initial_offset, particle_arr[i].motion_current_time),
			particle_arr[i].angle);
	}
}

static void InitShipSpecifics(Ship* ship, int id) {
	switch (id) {
	case AUREA:
		aurea.drone_shots = 8;
		aurea.drone_cooldown = 10.0f;
		aurea.drone_current_cooldown = 0;
		aurea.drone_firing_duration = 5.0f;
		aurea.drone_dispatch_duration = 2.0f;
		aurea.drone_max_range = (Vector2) { 0, -200 };
		aurea.state = DRONE_INITIALIZED;
		break;
	case ORION:
		orion.dash_is_in_use = false;
		
		orion.dash_cooldown = 15.0f;
		orion.dash_current_cooldown = 0;
		
		orion.dash_alpha_modifier = 0.8f;
		orion.dash_speed_modifier = (Vector2){ 300, 300 };
		orion.dash_alpha_current_modifier;
	
		orion.dash_acceleration_time = 2.0f;
		orion.dash_full_speed_time = 6.0f;
		orion.dash_deacceleration_time = 2.0f;
		orion.dash_total_time = orion.dash_acceleration_time + orion.dash_full_speed_time + orion.dash_deacceleration_time;
	
		orion.dash_state = DASH_INACTIVE;
		orion.dash_disruption_field = NULL;
		orion.dash_disruption_field_base_damage = 0.5f;
		orion.dash_disruption_field_area = (Vector2) { 450, 450 };
		orion.dash_disruption_field_damage_tick = 0.5f;
		break;
	case NEBULA:
	{
		static const a_particle_motion_time = 1.0f;
		static const b_particle_motion_time = 2.0f;
		static const c_particle_motion_time = 3.0f;

		nebula.blaster_cooldown = 5.0f;

		nebula.blaster_charge_time_a = 6.0f;
		nebula.blaster_charge_time_b = 12.0f;
		nebula.blaster_charge_time_c = 18.0f;

		nebula.blaster_base_damage_a = 5;
		nebula.blaster_base_damage_b = 10;
		nebula.blaster_base_damage_c = 15;

		nebula.particle_initial_offset = (Vector2) { 0, 120 };

		InitNebulaParticle(NEBULA_PARTICLE_A, NEBULA_PARTICLE_A_COUNT, &nebula.a_particles, a_particle_motion_time);
		InitNebulaParticle(NEBULA_PARTICLE_B, NEBULA_PARTICLE_B_COUNT, &nebula.b_particles, b_particle_motion_time);
		InitNebulaParticle(NEBULA_PARTICLE_C, NEBULA_PARTICLE_C_COUNT, &nebula.c_particles, c_particle_motion_time);

		nebula.charge_energy_field = CreateUnmanagedEffect(NEBULA_ENERGY_FIELD, (Vector2) { 0 }, 0);
		break;
	}
	case PUDDLE_JUMPER:
	{
		puddle_jumper.wormhole_enter_is_in_use = false;
		puddle_jumper.wormhole_spawn_is_in_use = false;

		puddle_jumper.wormhole_base_damage = 20.0f;
		puddle_jumper.wormhole_spawn_cooldown = 10.0f;

		puddle_jumper.wormhole_scale_time = 0.25f;

		puddle_jumper.wormhole_in_effect = CreateUnmanagedEffect(WORMHOLE, Vector2Zero(), 0);
		puddle_jumper.wormhole_in_effect->size = (Vector2){ 0 };
		puddle_jumper.wormhole_in_effect->order = RENDERING_ORDER_AFTER_SHIP;
		
		puddle_jumper.wormhole_out_effect = CreateUnmanagedEffect(WORMHOLE, Vector2Zero(), 0);
		puddle_jumper.wormhole_out_effect->size = (Vector2){ 0 };

		puddle_jumper.wormhole_puddle_jumper = CreateUnmanagedEffect(WORMHOLE_PUDDLE_JUMPER_SHIP, Vector2Zero(), 0);
		puddle_jumper.wormhole_puddle_jumper->size = (Vector2){ 0 };

		puddle_jumper.wormhole_tether = CreateUnmanagedEffect(WORMHOLE_TETHER, Vector2Zero(), 0);
		puddle_jumper.wormhole_tether->size = (Vector2){ 0 };

		puddle_jumper.wormhole_in_active = false;
		puddle_jumper.wormhole_out_active = false;

		puddle_jumper.wormhole_current_in_scale_time = 0.0f;
		puddle_jumper.wormhole_current_out_scale_time = 0.0f;

		puddle_jumper.wormhole_enter_cooldown = 15.0f;

		puddle_jumper.wormhole_moving_time = 0.25f;

		puddle_jumper.wormhole_state = WORMHOLE_INACTIVE;

		puddle_jumper.wormhole_spawn_current_cooldown = 0.0f;
		puddle_jumper.wormhole_enter_current_cooldown = 0.0f;
		break;
	}

	case VOID:
		ship_void.event_horizon_cooldown = 30.0f;
		ship_void.event_horizon_current_cooldown = 0.0f;
		ship_void.event_horizon_duration = 0.0f;
		ship_void.event_horizon_base_damage = 5.0f;
		ship_void.event_horizon_current_damage = 5.0f;

		ship_void.is_skill_ready = false;

		ship_void.event_horizon_state = VOID_SPECIAL_INACTIVE;
		ship_void.event_horizon_center = (Vector2){ 0 };
		break;

	default: break;
	}
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
	ship->speed = (Vector2){ 360.0f, 360.0f };
	ship->speed_dash_modifier = (Vector2){ 0 };
	ship->should_render = true;
	ship->is_able_to_act = true;
	ship->is_tutorial = false;
	InitShipSpecifics(ship, id);
}

static void UpdateAurea(Ship* ship) { 
	bool is_ability_ready = false;

	aurea.drone_current_cooldown = ClampWithFlagsF(
		aurea.drone_current_cooldown + GetFrameTime(),
		0, aurea.drone_cooldown,
		NULL, &is_ability_ready);

	// Drone animations
	switch(aurea.state) {
		case DRONE_INITIALIZED:
			if (!IsActionButtonPressed() || !is_ability_ready)
				break;
			// When it activates
			Vector2 left_offset = (Vector2) { -20, 0 };
			Vector2 right_offset = (Vector2) { 20, 0 };
	
			aurea.drone_left_cast_position = Vector2Add(ship->position, left_offset);
			aurea.drone_right_cast_position = Vector2Add(ship->position, right_offset);
	
			aurea.drone_left = CreateUnmanagedEffect(DRONE, aurea.drone_left_cast_position, aurea.drone_dispatch_duration);
			aurea.drone_right = CreateUnmanagedEffect(DRONE, aurea.drone_right_cast_position, aurea.drone_dispatch_duration);
	
			aurea.drone_left_thruster = CreateUnmanagedEffect(DRONE_THRUSTER, aurea.drone_left_cast_position, 0);
			aurea.drone_right_thruster = CreateUnmanagedEffect(DRONE_THRUSTER, aurea.drone_right_cast_position, 0);

			aurea.drone_dispatch_current_duration = 0;
			aurea.drone_current_cooldown = 0;
			aurea.state = DRONE_DISPATCHED;

			PlaySoundFxWithVolumeAndRandomPitch(&sound10, 1.0f, 1.2f, 1.8f);
			PlaySoundFxWithVolumeAndRandomPitch(&sound10, 1.0f, 1.6f, 2.0f);

			break;

		case DRONE_DISPATCHED:
		{
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
		}
		case DRONE_FIRING:
			InitPulseShootAtCoords(ship, aurea.drone_left->position, aurea.drone_left->rotation + 180);
			InitPulseShootAtCoords(ship, aurea.drone_right->position, aurea.drone_right->rotation + 180);
			aurea.drone_fired_shots++;
			aurea.state = DRONE_ROTATING;

			if (aurea.drone_fired_shots > aurea.drone_shots) {
				aurea.state = DRONE_FINALIZING;
				break;
			}
			
			break;

		case DRONE_ROTATING:
		{
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
		}
		case DRONE_FINALIZING:		
			CreateManagedEffect(DRONE_EXPLOSION, aurea.drone_left->position);
			CreateManagedEffect(DRONE_EXPLOSION, aurea.drone_right->position);

			DestroyEffect(aurea.drone_left);
			DestroyEffect(aurea.drone_right);

			PlaySoundFxWithVolumeAndRandomPitch(&sound2, 1.0f, 1.0f, 1.4f);
			PlaySoundFxWithVolumeAndRandomPitch(&sound2, 1.0f, 1.4f, 1.8f);

			aurea.drone_left = aurea.drone_right = NULL;
			aurea.state = DRONE_INITIALIZED;
			break;
	}
}

static void UpdateOrion(Ship* ship) {
	static const float disruption_field_maximum_transparency = 0.5f;

	// Cooldown
	float frame_time = GetFrameTime();
	
	bool has_changed_state = false;

	if (orion.dash_disruption_field) {
		orion.dash_disruption_field->position = ship->position;
	}

	switch(orion.dash_state) {
		case DASH_INACTIVE:
		{
			bool is_dash_ready = false;
	
			orion.dash_current_cooldown = ClampWithFlagsF(
				orion.dash_current_cooldown + frame_time,
				0, orion.dash_cooldown,
				NULL, &is_dash_ready);

			if (is_dash_ready && IsActionButtonPressed()) {
				orion.dash_is_in_use = true;

				orion.dash_state = DASH_ACCELERATING;
				// Create disruption field for a bit over than the total time
				orion.dash_disruption_field = CreateManagedEffectDuration(ORION_DISRUPTION_FIELD, (Vector2) { 0 }, orion.dash_total_time + 1.0f);
				orion.dash_disruption_field->size = orion.dash_disruption_field_area;

				PlaySoundFxWithVolumeAndRandomPitch(&sound17, 1.0f, 1.0f, 1.0f);
				PlaySoundFxWithVolumeAndRandomPitch(&sound18, 1.0f, 3.0f, 3.5f);
			}

			break;
		}
		case DASH_ACCELERATING:
		{
			// Time
			orion.dash_current_total_time = ClampWithFlagsF(
				orion.dash_current_total_time + frame_time,
				0, orion.dash_acceleration_time,
				NULL, &has_changed_state);
				
			// Speed
			float factor = Normalize(orion.dash_current_total_time,
				0, orion.dash_acceleration_time);

			ship->speed_dash_modifier = Vector2MultiplyScalarF(orion.dash_speed_modifier, factor);

			// Alpha
			ship->alpha = 1.0f - orion.dash_alpha_modifier * factor;

			orion.dash_disruption_field->color = Fade(WHITE, factor * disruption_field_maximum_transparency);

			if (has_changed_state) {
				orion.dash_state = DASH_FULL_SPEED;
				orion.dash_disruption_field_current_damage_tick = 0;

				PlaySoundFxWithVolumeAndRandomPitch(&sound18, 1.0f, 3.0f, 3.5f);
			}

			break;
		}			
		case DASH_FULL_SPEED:
		{
			// Time
			orion.dash_current_total_time = ClampWithFlagsF(
				orion.dash_current_total_time + frame_time,
				0, orion.dash_acceleration_time + orion.dash_full_speed_time,
				NULL, &has_changed_state);
				
				bool should_tick = false;
				
				orion.dash_disruption_field_current_damage_tick = ClampWithFlagsF(
				orion.dash_disruption_field_current_damage_tick + frame_time,
				0, orion.dash_disruption_field_damage_tick, false, &should_tick);

			if (should_tick) {
				// Dividing the area by 2.4 to better suit the animation range
				DashDisruptionFieldTick(orion.dash_disruption_field_area.x / 2.4f, orion.dash_disruption_field_base_damage);
				orion.dash_disruption_field_current_damage_tick = 0;

				PlaySoundFxWithVolumeAndRandomPitch(&sound18, 1.0f, 3.0f, 3.5f);
			}
			
			if (has_changed_state) {
				orion.dash_state = DASH_DEACCELERATING;

				PlaySoundFxWithVolumeAndRandomPitch(&sound17, 1.0f, 1.0f, 1.0f);
			}
			
			break;
		}
		case DASH_DEACCELERATING:
		{
			// Time
			orion.dash_current_total_time = ClampWithFlagsF(
				orion.dash_current_total_time + frame_time,
				0, orion.dash_total_time,
				NULL, &has_changed_state);

			// Speed
			float factor = 1.0f - Normalize(orion.dash_current_total_time,
				orion.dash_full_speed_time + orion.dash_acceleration_time,
				orion.dash_total_time);

			ship->speed_dash_modifier = Vector2MultiplyScalarF(orion.dash_speed_modifier, factor);

			// Alpha
			ship->alpha = 1.0f - orion.dash_alpha_modifier * factor;

			orion.dash_disruption_field->color = Fade(WHITE, factor * disruption_field_maximum_transparency);
			
			if (has_changed_state) {
				ship->speed_dash_modifier = Vector2Zero();
				ship->alpha = 1.0f;
				orion.dash_current_total_time = 0.0f;

				orion.dash_state = DASH_INACTIVE;
				orion.dash_disruption_field->color = Fade(WHITE, 0);
				orion.dash_disruption_field = NULL;

				orion.dash_current_cooldown = 0.0f;
				orion.dash_is_in_use = false;
			}
			
			break;
		}
	}
}

static void UpdateNebula_UpdateParticle(NebulaParticleTracker* particle_arr, int particle_count, float min_time, float cur_time, float max_time) {
	float frame_time = GetFrameTime();

	for(int i = 0; i < particle_count; i++) {

		// Motion Time
		particle_arr[i].motion_current_time = fmaxf(0, particle_arr[i].motion_current_time - frame_time);

		// Transparency
		float transparency_by_time = Clamp((cur_time - min_time) / (max_time - min_time), 0, 1);
		float transparency_by_distance = 1.0f - particle_arr[i].motion_current_time / particle_arr[i].motion_time;

		particle_arr[i].effect->color = Fade(WHITE, transparency_by_time * transparency_by_distance);

		// Rotation
		particle_arr[i].effect->rotation += GetFrameTime() * 360;

		// Position
		Vector2 offset_vector = Vector2Rotate(
			Vector2MultiplyScalarF(
				nebula.particle_initial_offset,
				particle_arr[i].motion_current_time
			),
			particle_arr[i].angle
		);

		particle_arr[i].effect->position = Vector2Add(ship.position, offset_vector);

		// Reset (if necessary)
		if (particle_arr[i].motion_current_time <= 0) {
			particle_arr[i].motion_current_time = particle_arr[i].motion_time;
			particle_arr[i].angle = (float)GetRandomValue(0, 3600) / 10.0f * DEG2RAD;
		}
	}
}

static void UpdateNebula(Ship* ship) {
	static const Color a_color = { .r = 162, .g = 255, .b = 243, .a = 255 };
	static const Color b_color = { .r = 243, .g = 97, .b = 255, .a = 255 };
	static const Color c_color = { .r = 255, .g = 162, .b = 0, .a = 255 };

	float frame_time = GetFrameTime();
	bool is_skill_ready = false;

	// COOLDOWN
	nebula.blaster_current_cooldown = ClampWithFlagsF(
		nebula.blaster_current_cooldown + frame_time, 0, nebula.blaster_cooldown,
		NULL, &is_skill_ready);
	
	float time_factor = is_skill_ready ? 1 : -5;
	float prev_charge_time = nebula.blaster_current_charge_time;

	bool is_charge_complete = false;
	float current_charge_time = nebula.blaster_current_charge_time = ClampWithFlagsF(
		nebula.blaster_current_charge_time + frame_time * time_factor,
		0, nebula.blaster_charge_time_c, NULL, &is_charge_complete); 

	// Updating particles
	UpdateNebula_UpdateParticle(&nebula.a_particles, NEBULA_PARTICLE_A_COUNT, 0, current_charge_time, nebula.blaster_charge_time_a);
	UpdateNebula_UpdateParticle(&nebula.b_particles, NEBULA_PARTICLE_B_COUNT, nebula.blaster_charge_time_a, current_charge_time, nebula.blaster_charge_time_b);
	UpdateNebula_UpdateParticle(&nebula.c_particles, NEBULA_PARTICLE_C_COUNT, nebula.blaster_charge_time_b, current_charge_time, nebula.blaster_charge_time_c);

	// Updating energy field
	bool is_absorbing_a_particles = current_charge_time >= 0;
	bool is_absorbing_b_particles = current_charge_time >= nebula.blaster_charge_time_a;
	bool is_absorbing_c_particles = current_charge_time >= nebula.blaster_charge_time_b;

	nebula.charge_energy_field->position = ship->position;

	if (is_absorbing_c_particles) {
		nebula.charge_energy_field->color = c_color;
	} else if (is_absorbing_b_particles) {
		float normalized = Clamp(
			Normalize(current_charge_time, nebula.blaster_charge_time_a, nebula.blaster_charge_time_b), 0, 1
		);
		nebula.charge_energy_field->color = LerpColor(b_color, c_color, normalized);
	} else if (is_absorbing_a_particles) {
		float normalized = Clamp(
			Normalize(current_charge_time, 0, nebula.blaster_charge_time_a), 0, 1
		);
		nebula.charge_energy_field->color = LerpColor(a_color, b_color, normalized);
	}

	nebula.charge_energy_field->color = Fade(nebula.charge_energy_field->color, current_charge_time / nebula.blaster_charge_time_c);

	// Is cooldown ready
	if (!is_skill_ready || !IsKeyDown(KEY_Z)) {
		return;
	}
	
	bool is_level_3 = is_charge_complete;
	bool is_level_2 = is_absorbing_b_particles;

	if (is_level_3) {
		InitBlasterShoot(ship, 2, nebula.blaster_base_damage_c);
	} else if (is_level_2) {
		InitBlasterShoot(ship, 1, nebula.blaster_base_damage_b);
	} else {
		InitBlasterShoot(ship, 0, nebula.blaster_base_damage_a);
	}

	nebula.blaster_current_cooldown = 0;
}

static void UpdatePuddleJumper(Ship* ship) {
	// Cooldown
	bool is_wormhole_spawn_ready = false;
	bool is_wormhole_in_ready = false;
	bool is_wormhole_out_ready = false;

	float frame_time = GetFrameTime();

	puddle_jumper.wormhole_spawn_current_cooldown = ClampWithFlagsF(
		puddle_jumper.wormhole_spawn_current_cooldown + frame_time,
		0, puddle_jumper.wormhole_spawn_cooldown,
		NULL, &is_wormhole_out_ready);

	// In-Timer
	float in_portal_opening_factor = puddle_jumper.wormhole_current_in_scale_time / puddle_jumper.wormhole_scale_time;

	puddle_jumper.wormhole_current_in_scale_time = ClampWithFlagsF(
		puddle_jumper.wormhole_current_in_scale_time + frame_time * (puddle_jumper.wormhole_in_active ? 1 : -1), 0, puddle_jumper.wormhole_scale_time,
		NULL, &is_wormhole_in_ready);
	puddle_jumper.wormhole_in_effect->size = Vector2MultiplyScalarF(
		puddle_jumper.wormhole_in_effect->original_size, in_portal_opening_factor);

	// Out-Timer
	puddle_jumper.wormhole_current_out_scale_time = Clamp(
		puddle_jumper.wormhole_current_out_scale_time + frame_time * (puddle_jumper.wormhole_out_active ? 1 : -1), 0, puddle_jumper.wormhole_scale_time);
	puddle_jumper.wormhole_out_effect->size = Vector2MultiplyScalarF(
		puddle_jumper.wormhole_out_effect->original_size, puddle_jumper.wormhole_current_out_scale_time / puddle_jumper.wormhole_scale_time);

	// Tether scale
	puddle_jumper.wormhole_tether->size.y = in_portal_opening_factor * 54;

	// Wormhole Spawn Logic
	if (is_wormhole_out_ready && IsActionButton2Pressed()) {
		// When it activates
		PlaySoundFxWithVolumeAndRandomPitch(&sound11, 1.0f, 1.0f, 1.0f);

		puddle_jumper.wormhole_out_active = true;
		puddle_jumper.wormhole_spawn_current_cooldown = 0;

		puddle_jumper.wormhole_current_out_scale_time = 0;
		puddle_jumper.wormhole_out_effect->position = ship->position;		
		return;
	}

	// Wormhole enter animation
	switch(puddle_jumper.wormhole_state) {
		case WORMHOLE_INACTIVE:
		{
			bool is_skill_ready = false;

			puddle_jumper.wormhole_enter_current_cooldown = ClampWithFlagsF(
				puddle_jumper.wormhole_enter_current_cooldown + frame_time,
				0, puddle_jumper.wormhole_enter_cooldown,
				NULL, &is_skill_ready);

			if (puddle_jumper.wormhole_out_active && is_skill_ready && IsActionButtonPressed()) {
				puddle_jumper.wormhole_enter_is_in_use = true;
				puddle_jumper.wormhole_spawn_is_in_use = true;

				ship->is_able_to_act = false;

				// Opening IN wormhole
				PlaySoundFxWithVolumeAndRandomPitch(&sound11, 1.0f, 1.0f, 1.0f);
				PlaySoundFxWithVolumeAndRandomPitch(&sound12, 1.0f, 1.5f, 1.5f);

				puddle_jumper.wormhole_in_active = true;
				puddle_jumper.wormhole_in_effect->position = ship->position;

				// Switching state
				puddle_jumper.wormhole_state = WORMHOLE_SPAWNING;
				
				// Starting movement timer
				puddle_jumper.wormhole_current_moving_time = 0.0f;

				// Prepare fake ship to fly (flying animation)
				puddle_jumper.wormhole_puddle_jumper->size = puddle_jumper.wormhole_puddle_jumper->original_size;
				puddle_jumper.wormhole_puddle_jumper->rotation = CalculateFacingAngle(
					puddle_jumper.wormhole_in_effect->position,
					puddle_jumper.wormhole_out_effect->position) * RAD2DEG - 90;
				
				float distance = Vector2Distance(puddle_jumper.wormhole_in_effect->position, puddle_jumper.wormhole_out_effect->position);
				puddle_jumper.wormhole_tether->position = Vector2DivideScalarF(Vector2Add(puddle_jumper.wormhole_in_effect->position, puddle_jumper.wormhole_out_effect->position), 2.0f);
				puddle_jumper.wormhole_tether->rotation = puddle_jumper.wormhole_puddle_jumper->rotation + 90;
				puddle_jumper.wormhole_tether->size.x = distance;
			}
			break;
		}
		case WORMHOLE_SPAWNING:
		{
			// Tether asset
			if (is_wormhole_in_ready) {
				ship->should_render = false;
				puddle_jumper.wormhole_state = WORMHOLE_MOVING;
				CreateManagedEffectDuration(WORMHOLE_TELEPORT_ANIMATION, ship->position, 0.5f);
			}
			break;
		}
		case WORMHOLE_MOVING:
		{
			bool is_movement_done = false;

			puddle_jumper.wormhole_current_moving_time = ClampWithFlagsF(puddle_jumper.wormhole_current_moving_time + frame_time,
				0, puddle_jumper.wormhole_moving_time, NULL, &is_movement_done);

			// Moving ship animation
			float factor = puddle_jumper.wormhole_current_moving_time / puddle_jumper.wormhole_moving_time;
			float distance = Vector2Distance(puddle_jumper.wormhole_in_effect->position, puddle_jumper.wormhole_out_effect->position);
			Vector2 rotated_vector = Vector2Rotate((Vector2) { 0, -1 }, puddle_jumper.wormhole_puddle_jumper->rotation * DEG2RAD);
			Vector2 scaled_vector = Vector2MultiplyScalarF(rotated_vector, factor * distance);

			puddle_jumper.wormhole_puddle_jumper->position = Vector2Add(ship->position, scaled_vector);

			WormholePassThroughDamage(ship, puddle_jumper.wormhole_puddle_jumper->position, puddle_jumper.wormhole_base_damage);

			if (is_movement_done) {
				puddle_jumper.wormhole_state = WORMHOLE_EXITING;
			}

			break;
		}
		case WORMHOLE_EXITING:
			ship->should_render = true;
			ship->is_able_to_act = true;
			puddle_jumper.wormhole_in_active = false;
			puddle_jumper.wormhole_out_active = false;
			ship->position = puddle_jumper.wormhole_out_effect->position;
			CreateManagedEffectDuration(WORMHOLE_TELEPORT_ANIMATION, ship->position, 0.5f);
			ship->color = WHITE;
			puddle_jumper.wormhole_state = WORMHOLE_INACTIVE;
			puddle_jumper.wormhole_puddle_jumper->size = (Vector2) {0, 0};
			WormholeClearList();

			puddle_jumper.wormhole_enter_is_in_use = false;
			puddle_jumper.wormhole_spawn_is_in_use = false;
			puddle_jumper.wormhole_enter_current_cooldown = 0.0f;
			puddle_jumper.wormhole_spawn_current_cooldown = 0.0f;
			break;

		default:
			break;
	}
}

static void UpdateVoid(Ship* ship) {
	if (!ship) return;

	if (!ship_void.is_skill_ready && !ship_void.event_horizon_state) {
		ship_void.event_horizon_current_cooldown -= GetFrameTime();
		if (ship_void.event_horizon_current_cooldown <= 0) {
			ship_void.is_skill_ready = true;
			ship_void.event_horizon_current_cooldown = 0;
		}
		else {
			ship_void.is_skill_ready = false;
		}
	}

	if (ship_void.is_skill_ready &&
		IsActionButtonPressed() &&
		!ship_void.event_horizon_state)
	{
		// Ativação
		ship_void.event_horizon_center.x = ship->position.x;
		ship_void.event_horizon_center.y = ship->position.y - EVENT_HORIZON_SIZE;
		ship_void.event_horizon_state = VOID_SPECIAL_ACTIVE;
		ship_void.event_horizon_current_damage = ship_void.event_horizon_base_damage + GetPlayerLevel();
		ship_void.event_horizon_pulse_count = 0;
		ship_void.event_horizon_duration = 0.0f; // Reseta o timer do pulso

		CreateManagedEffect(VOID_EVENT_HORIZON, ship_void.event_horizon_center);

		EventHorizonTick(ship_void.event_horizon_center, EVENT_HORIZON_SIZE / 2, ship_void.event_horizon_current_damage);
		ship_void.event_horizon_pulse_count++;

		PlaySoundFxWithVolumeAndRandomPitch(&sound15, 0.4f, 1.0f, 1.0f);
	}

	else if (ship_void.event_horizon_state == VOID_SPECIAL_ACTIVE) {
		ship_void.event_horizon_duration += GetFrameTime();

		if (ship_void.event_horizon_duration >= EVENT_HORIZON_PULSE_PERIOD) {
			ship_void.event_horizon_duration = 0.0f;

			EventHorizonTick(ship_void.event_horizon_center, EVENT_HORIZON_SIZE / 2, ship_void.event_horizon_current_damage);
			ship_void.event_horizon_pulse_count++;

			if (ship_void.event_horizon_pulse_count == EVENT_HORIZON_MAX_PULSES) {
				ship_void.event_horizon_state = VOID_SPECIAL_INACTIVE;
				ship_void.event_horizon_duration = 0.0f;
				ship_void.event_horizon_current_cooldown = ship_void.event_horizon_cooldown;
				ship_void.is_skill_ready = false;
			}
		}
	}
}


static void WallBehavior(Vector2* position) {
	position->x = Clamp(position->x, GAME_SCREEN_START + DRAW_WH / 2, GAME_SCREEN_END - (DRAW_WH / 2));
	position->y = Clamp(position->y, DRAW_WH / 2, SCREEN_HEIGHT - DRAW_WH / 2);
}

void UpdateShip(Ship* ship) {
	float movement_x = (ship->speed.x + ship->speed_dash_modifier.x) * GetFrameTime();
	float movement_y = (ship->speed.y + ship->speed_dash_modifier.y) * GetFrameTime();

	if (!ship->is_tutorial && ship->is_able_to_act) {
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
	}

	ship->animation_cycle += GetFrameTime();

	if (ship->animation_cycle > 0.25f) {
		ship->animation_cycle -= 0.25f;
		ship->thruster_cycle = (ship->thruster_cycle + 1) % MAX_THRUSTER_CYCLE;
	}

	if(!ship->is_tutorial) {
		switch(ship->id) {
			case AUREA:
				UpdateAurea(ship);
				break;
			case ORION:
				UpdateOrion(ship);
				break;
			case NEBULA:
				UpdateNebula(ship);
				break;
			case PUDDLE_JUMPER:
				UpdatePuddleJumper(ship);
				break;
			case VOID:
				UpdateVoid(ship);
				break;
			default:
				return;
		}
	}

	if (!ship->is_tutorial) {
		WallBehavior(&ship->position);
	}

	if (ship->invincibilityTimer > 0) {

		ship->invincibilityTimer -= GetFrameTime();

		if (ship->invincibilityTimer <= 0) {
			ship->isInvincible = false;
			ship->invincibilityTimer = 0;
		}
	}
}

bool Ship_TakeDamage(Ship* ship)
{
	// se for orion e estiver invisível -> não tem dano (;
	if (ship->id == ORION && orion.dash_state != DASH_INACTIVE)
		return true;

	if (ship->id == PUDDLE_JUMPER && puddle_jumper.wormhole_state != WORMHOLE_INACTIVE)
		return true;

	// --- VERIFICA SE A NAVE ESTÁ INVENCÍVEL ---
	if (ship->isInvincible) {
		return true; // Se estiver, ignora a colisão e continua viva
	}

	if (IsShieldActive())
	{
		shield.capacity--;

		// --- ATIVA A INVENCIBILIDADE APÓS O HIT ---
		ship->isInvincible = true;
		ship->invincibilityTimer = 1.0f; // Duração de 1 segundo

		if (shield.capacity <= 0) {
			DeactivateShield();
		}

		return true;
	}

	ship->is_alive = false;
	return false;
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

	if (ship->direction == LEFT) {
		nebula_sprite.x -= 8;
		right_thruster_position.x -= 12;
		center_thruster_position.x -= 6;
	}
	else if (ship->direction == RIGHT) {
		nebula_sprite.x += 8;
		left_thruster_position.x += 12;
		center_thruster_position.x += 6;
	}

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
	if (!ship->should_render)
		return;

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
}

void UnloadShip(Ship* ship) {
	if (ship->id == NEBULA) {
		for(int i = 0; i < NEBULA_PARTICLE_A_COUNT; i++) {
			DestroyEffect(nebula.a_particles[i].effect);
		}

		for(int i = 0; i < NEBULA_PARTICLE_B_COUNT; i++) {
			DestroyEffect(nebula.b_particles[i].effect);
		}

		for(int i = 0; i < NEBULA_PARTICLE_C_COUNT; i++) {
			DestroyEffect(nebula.c_particles[i].effect);
		}

		DestroyEffect(nebula.charge_energy_field);
	}

	if (ship->id == PUDDLE_JUMPER) {
		DestroyEffect(puddle_jumper.wormhole_in_effect);
		DestroyEffect(puddle_jumper.wormhole_out_effect);
	}
}