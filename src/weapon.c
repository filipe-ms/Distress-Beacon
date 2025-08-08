// weapon.c

#include "weapon.h"
#include "common.h"

#include "raymath.h"

#include "enemy.h"
#include "list.h"

#include "special_effects.h"

#include "player.h"
#include "general_utils.h"
#include "texture_manager.h"

#include "audio_manager.h"

#include <string.h>
#include <stdlib.h>

// Weapon power-up values
float cooldown_modifier;
float damage_modifier;
float size_modifier;
float speed_modifier;

static void InitWeaponPowerUpMultipliers(void) {
	cooldown_modifier = 0.0f;
	damage_modifier = 0.0f;
	size_modifier = 0.0f;
	speed_modifier = 0.0f;
}

static float ApplyMultiplier(float multiplier, float variable) {
    return variable + (multiplier/100) * variable;
}

#pragma region POWER_UPS

//--------------------------------------------------------------
//
//                         POWER UPS
// 
//--------------------------------------------------------------

void IncrementCooldownModifier(float value) { cooldown_modifier += value; }
void IncrementDamageModifier(float value) { damage_modifier += value; }
void IncrementSizeModifier(float value) { size_modifier += value; }
void IncrementSpeedModifier(float value) { speed_modifier += value; }

float GetCooldownModifier(void) { return cooldown_modifier; }
float GetDamageModifier(void) { return damage_modifier; }
float GetSizeModifier(void) { return size_modifier; }
float GetSpeedModifier(void) { return speed_modifier; }
#pragma endregion
#pragma region PULSE
//--------------------------------------------------------------
//
//                         PULSE
// 
//--------------------------------------------------------------

Pulse pulse;

int GetPulseLevel(void) { return pulse.weapon.level; }

void PulseLevelUp(void)  { 
    pulse.weapon.level += 1;
    if (pulse.weapon.level == 2) {
        pulse.shoot_cycle = -1;
        pulse.weapon.cooldown_time = 0.4f;
        pulse.weapon.cooldown_charge = 0.4f;
	}
	else if (pulse.weapon.level == 3) {
		pulse.shoot_cycle = 0;
		pulse.weapon.cooldown_time = 0.3f;
		pulse.weapon.cooldown_charge = 0.3f;
	}
}

static void InitPulse(void) {
    pulse.weapon.id = PULSE;
    pulse.weapon.level = 0;

    pulse.weapon.source = (Rectangle){ 40, 0, 8, 8 };
	pulse.weapon.offset = (Vector2){ 0, 0 };
    pulse.weapon.color = WHITE;

    pulse.weapon.damage = 1.0f;
    pulse.weapon.shoot_speed = (Vector2){ 0, -500 };

    pulse.weapon.cooldown_time = 0.5f;
    pulse.weapon.cooldown_charge = 0.5f;
	
    pulse.shoot_cycle = 0;

	pulse.pulse_shoots = List_Create(sizeof(PulseShoot));
}

void InitPulseShootAtCoords(Ship* ship, Vector2 position, float angle_in_deg) {
    PulseShoot shot = { 0 };
    shot.shoot.damage = ApplyMultiplier(damage_modifier, pulse.weapon.damage);
    shot.shoot.size = (Vector2){ ApplyMultiplier(size_modifier, 48.0f), ApplyMultiplier(size_modifier, 48.0f) };

    shot.shoot.position = position;
    shot.rotation = angle_in_deg;
    shot.shoot_speed = Vector2Rotate(pulse.weapon.shoot_speed, angle_in_deg * DEG2RAD);

    List_AddLast(pulse.pulse_shoots, &shot);

    PlaySoundFxWithVolumeAndRandomPitch(&sound1, BASE_WEAPON_SFX_VOLUME, 2.0f, 2.2f);
}

static void InitPulseShoot(Ship* ship) {
    PulseShoot new_pulse_shoot = { 0 };
    new_pulse_shoot.shoot.damage = ApplyMultiplier(damage_modifier, pulse.weapon.damage);
    new_pulse_shoot.shoot.size = (Vector2){ ApplyMultiplier(size_modifier, 48.0f), ApplyMultiplier(size_modifier, 48.0f) };

    Vector2 shoot_spawn_position = {
        ship->position.x,
        ship->position.y - DRAW_WH * 0.75
    };

    new_pulse_shoot.shoot.position = shoot_spawn_position;
    new_pulse_shoot.rotation = pulse.shoot_cycle * 15.0f;
    new_pulse_shoot.shoot_speed = Vector2Rotate(pulse.weapon.shoot_speed, new_pulse_shoot.rotation * DEG2RAD);

    if (pulse.weapon.level == 1) { pulse.shoot_cycle = 0; }
	else if (pulse.weapon.level == 2) { pulse.shoot_cycle *= -1; }
	else if (pulse.weapon.level == 3) {	
        pulse.shoot_cycle++;
        if (pulse.shoot_cycle > 1) pulse.shoot_cycle = -1;
	}

    List_AddLast(pulse.pulse_shoots, &new_pulse_shoot);

    PlaySoundFxWithVolumeAndRandomPitch(&sound1, BASE_WEAPON_SFX_VOLUME, 1.0f, 1.5f);
}

static void PulseShootPositionUpdate(PulseShoot* pulse_shoot) {
	pulse_shoot->shoot.position.x += ApplyMultiplier(speed_modifier, pulse_shoot->shoot_speed.x) * GetFrameTime();
	pulse_shoot->shoot.position.y += ApplyMultiplier(speed_modifier, pulse_shoot->shoot_speed.y) * GetFrameTime();
}

static int CheckIfOutOfBonds(void* context, PulseShoot* pulse_shoot) {
	return IsWithinScreenBounds(pulse_shoot->shoot.position, pulse_shoot->shoot.size);
}

static void UpdatePulse(Ship* ship) {
    if (!pulse.weapon.level) return;
	pulse.weapon.cooldown_charge -= ApplyMultiplier(cooldown_modifier, GetFrameTime());

    if (pulse.weapon.cooldown_charge <= 0 && ship->is_able_to_act) {
		pulse.weapon.cooldown_charge = pulse.weapon.cooldown_time;
		InitPulseShoot(ship);
    }

    List_ForEach(pulse.pulse_shoots, (Function)PulseShootPositionUpdate);
	List_RemoveWithFn(pulse.pulse_shoots, NULL, (MatchFunction)CheckIfOutOfBonds);
}

static void PulseShootDraw(PulseShoot* pulse_shoot) {
    Rectangle pulse_rect_draw = {
        pulse_shoot->shoot.position.x,
        pulse_shoot->shoot.position.y,
        pulse_shoot->shoot.size.x,
        pulse_shoot->shoot.size.y
    };

    Vector2 draw_origin = { pulse_shoot->shoot.size.x / 2.0f, pulse_shoot->shoot.size.y / 2.0f };

    if (DEBUG_FLAG) {
        DrawCircleV(pulse_shoot->shoot.position, 20, Fade(RED, 0.5f));
    }

    DrawTexturePro(texture_projectiles, pulse.weapon.source, pulse_rect_draw, draw_origin, pulse_shoot->rotation, WHITE);
}

static void DrawPulseShoot() {
	List_ForEach(pulse.pulse_shoots, (Function)PulseShootDraw);
}
#pragma endregion
#pragma region PHOTON
//--------------------------------------------------------------
//
//                         PHOTON
// 
//--------------------------------------------------------------

Photon photon;

int GetPhotonLevel(void) { return photon.weapon.level; }
void PhotonLevelUp(void) {
    photon.weapon.level += 1;
}

static void InitPhoton(void) {
    photon.weapon.id = PHOTON;
    photon.weapon.level = 0;
    photon.weapon.source = (Rectangle){ 0, 8, 8, 8 };
    photon.weapon.offset = (Vector2){ 0, 0 };
    photon.weapon.damage = 1.0f;
    photon.weapon.shoot_speed = (Vector2){ 0, -720 };
    photon.weapon.cooldown_time = 0.5f;
    photon.weapon.cooldown_charge = 0.0f;
    photon.weapon.color = RED;

    photon.photon_shoots = List_Create(sizeof(PhotonShoot));
}

static void InitPhotonShoot(Ship* ship) {
    PhotonShoot new_photon_shoot = { 0 };
    new_photon_shoot.shoot.damage = ApplyMultiplier(damage_modifier, photon.weapon.damage);
    new_photon_shoot.shoot.size = (Vector2){ ApplyMultiplier(size_modifier, 36.0f), ApplyMultiplier(size_modifier, 36.0f) };
    new_photon_shoot.alpha = 1.0f;
    if (photon.weapon.level == 1) {
        new_photon_shoot.shoot.position = (Vector2){ ship->position.x, ship->position.y };
        List_AddLast(photon.photon_shoots, &new_photon_shoot);
        
    }
    else if (photon.weapon.level == 2) {
        new_photon_shoot.shoot.position = (Vector2){ ship->position.x + new_photon_shoot.shoot.size.x / 2, ship->position.y };
        List_AddLast(photon.photon_shoots, &new_photon_shoot);
        new_photon_shoot.shoot.position = (Vector2){ ship->position.x - new_photon_shoot.shoot.size.x / 2, ship->position.y };
        List_AddLast(photon.photon_shoots, &new_photon_shoot);
    }
    else if (photon.weapon.level == 3) {
        new_photon_shoot.shoot.position = (Vector2){ ship->position.x, ship->position.y - new_photon_shoot.shoot.size.y };
        List_AddLast(photon.photon_shoots, &new_photon_shoot);
		new_photon_shoot.shoot.position = (Vector2){ ship->position.x + new_photon_shoot.shoot.size.x, ship->position.y };
		List_AddLast(photon.photon_shoots, &new_photon_shoot);
		new_photon_shoot.shoot.position = (Vector2){ ship->position.x - new_photon_shoot.shoot.size.x, ship->position.y };
		List_AddLast(photon.photon_shoots, &new_photon_shoot);
    }

    PlaySoundFxWithVolumeAndRandomPitch(&sound4, BASE_WEAPON_SFX_VOLUME, 1.8f, 2.2f);
}

static void PhotonShootPositionUpdate(PhotonShoot* photon_shoot) {
    photon_shoot->shoot.position.y += ApplyMultiplier(speed_modifier, photon.weapon.shoot_speed.y) * GetFrameTime();
}

static int CheckPhotonShootOutOfBounds(void* context, PhotonShoot* item) {
    PhotonShoot* photon_shoot = (PhotonShoot*)item;
    if (photon_shoot->shoot.position.y < -80) {
        return 1;
    }
    return 0;
}

static void UpdatePhoton(Ship* ship) {
    if (!photon.weapon.level) return;

    photon.weapon.cooldown_charge -= ApplyMultiplier(cooldown_modifier, GetFrameTime());

    if (photon.weapon.cooldown_charge <= 0 && ship->is_able_to_act) {
        InitPhotonShoot(ship);
        photon.weapon.cooldown_charge = photon.weapon.cooldown_time;
    }

    List_ForEach(photon.photon_shoots, (Function)PhotonShootPositionUpdate);
    List_RemoveWithFn(photon.photon_shoots, NULL, CheckPhotonShootOutOfBounds);
}

static void DrawPhotonShoot(PhotonShoot* photon_shoot) {
    Rectangle destRec = {
        photon_shoot->shoot.position.x - photon_shoot->shoot.size.x / 2.0f,
        photon_shoot->shoot.position.y - photon_shoot->shoot.size.y / 2.0f,
        photon_shoot->shoot.size.x,
		photon_shoot->shoot.size.y
    };
    Vector2 origin = { 0,0 };
    if (DEBUG_FLAG) {
        Vector2 center = { photon_shoot->shoot.position.x, photon_shoot->shoot.position.y };
        DrawCircleV(center, 20, Fade(RED, 0.5f));
    }	

    DrawTexturePro(texture_projectiles, photon.weapon.source, destRec, origin, 0, Fade(WHITE, photon_shoot->alpha));
}

static void DrawPhoton(void) {
    List_ForEach(photon.photon_shoots, (Function)DrawPhotonShoot);
}
#pragma endregion

// Dash do Orion
void DashDisruptionFieldTick(float radius, float base_damage) {
    float damage = ApplyMultiplier(damage_modifier, base_damage);

    for (Node* enemyNode = enemies->head; enemyNode != NULL; enemyNode = enemyNode->next) {
        Enemy* enemy = (Enemy*)enemyNode->data;

        if (CheckCollisionCircles(ship.position, radius, enemy->position, enemy->size.x / 2.0f)) {
            enemy->hp -= damage;
            CreateManagedEffect(EXPLOSION, enemy->position);
        }
    }
}

#pragma region HOMING
//--------------------------------------------------------------
//
//                         HOMING
// 
//--------------------------------------------------------------

Homing homing;

const Vector2 homing_shoot_speed_base = { .x = 0, .y = -300 };

int GetHomingLevel(void) { return homing.weapon.level; }
void HomingLevelUp(void) { 
    homing.weapon.level += 1;
    homing.weapon.damage += 0.25f;
}

static void InitHoming(void) {
    homing.weapon.id = HOMING;
    homing.weapon.level = 0;
    homing.weapon.source = (Rectangle){ 8 * 5, 8 * 1, 8, 8 };
    homing.weapon.offset = (Vector2){ 0, 0 };
    homing.weapon.damage = 0.5f;
    homing.weapon.shoot_speed = homing_shoot_speed_base;
    homing.weapon.cooldown_time = 0.4f;
    homing.weapon.cooldown_charge = 0.0f;
    homing.weapon.color = YELLOW;

    homing.homing_shoots = List_Create(sizeof(HomingShoot));
}

static void InitHomingShoot(Ship* ship) {
    HomingShoot new_homing_shoot = { 0 };
    new_homing_shoot.shoot.damage = ApplyMultiplier(damage_modifier, homing.weapon.damage);
    new_homing_shoot.shoot.size = (Vector2){ ApplyMultiplier(size_modifier, 18), ApplyMultiplier(size_modifier, 18) };
    new_homing_shoot.shoot.position = (Vector2){ ship->position.x, ship->position.y };
    new_homing_shoot.visual_rotation = 0.0f;
    new_homing_shoot.calc_rotation = -90.0f * DEG2RAD;
    new_homing_shoot.target = NULL;
    new_homing_shoot.has_locked_on_any_target = false;
    new_homing_shoot.current_velocity = (Vector2) { ApplyMultiplier(speed_modifier, homing_shoot_speed_base.x), ApplyMultiplier(speed_modifier, homing_shoot_speed_base.y) };
    List_AddLast(homing.homing_shoots, &new_homing_shoot);

    PlaySoundFxWithVolumeAndRandomPitch(&sound14, BASE_WEAPON_SFX_VOLUME - 0.10f, 2.6f, 3.0f);
}

static void HomingShootPositionUpdate(HomingShoot* homing_shoot) {
    // Pick the closest enemy
    if (homing_shoot->target == NULL && !homing_shoot->has_locked_on_any_target) {
        Enemy* closest_enemy = NULL;
        float closest_distance = 0;
        Vector2 shootPos = homing_shoot->shoot.position;
        
        bool hasInitialized = false;
        
        for(Node* enemyNode = enemies->head; enemyNode != NULL; enemyNode = enemyNode->next) {
            Enemy* enemy = (Enemy*)enemyNode->data;

            if (!enemy->is_targetable)
                continue;

            Vector2 enemyPos = { enemy->position.x, enemy->position.y };
            
            float distance = Vector2Distance(enemyPos, shootPos);
            
            if (enemy->is_on_screen) {
                if (!hasInitialized || distance < closest_distance) { 
                    closest_distance = distance;
                    closest_enemy = enemy;
                    hasInitialized = true;
                }
            }
        }

        homing_shoot->target = closest_enemy;
        homing_shoot->has_locked_on_any_target = true;
    }
    
    if (homing_shoot->target == NULL || !homing_shoot->target->is_on_screen) {
        homing_shoot->shoot.position.y += ApplyMultiplier(speed_modifier, homing_shoot->current_velocity.y) * GetFrameTime();
        homing_shoot->shoot.position.x += ApplyMultiplier(speed_modifier, homing_shoot->current_velocity.x) * GetFrameTime();
        return;
    }

    float base_speed = Vector2Length(homing_shoot_speed_base);

    Vector2 target_position = (Vector2) { homing_shoot->target->position.x, homing_shoot->target->position.y };
    Vector2 direction = Vector2Subtract(target_position, homing_shoot->shoot.position);
    Vector2 normalized_direction = Vector2Normalize(direction);

    float desired_angle = atan2f(normalized_direction.y, normalized_direction.x);
    float current_angle = homing_shoot->calc_rotation;
    float angle_diff = WrapAngle(desired_angle - current_angle);

    float max_adjustment = 2.0f * GetFrameTime(); // Radians per frame
    float adjustment = Clamp(angle_diff, -max_adjustment, max_adjustment);
    float new_angle = current_angle + adjustment;

    Vector2 new_velocity = {
        cosf(new_angle) * base_speed,
        sinf(new_angle) * base_speed
    };

    homing_shoot->current_velocity = new_velocity;
    homing_shoot->shoot.position.x += ApplyMultiplier(speed_modifier, new_velocity.x) * GetFrameTime();
    homing_shoot->shoot.position.y += ApplyMultiplier(speed_modifier, new_velocity.y) * GetFrameTime();
    homing_shoot->visual_rotation = new_angle * RAD2DEG + 90;
    homing_shoot->calc_rotation = new_angle;
}

static bool CheckHomingShootOutOfBounds(void* context, HomingShoot* item) {
    HomingShoot* homing_shoot = (HomingShoot*)item;
    return IsWithinScreenBounds(homing_shoot->shoot.position, homing_shoot->shoot.size);
}

static void UpdateHoming(Ship* ship) {
    if (!homing.weapon.level) return;

    homing.weapon.cooldown_charge -= ApplyMultiplier(cooldown_modifier, GetFrameTime());

    if (homing.weapon.cooldown_charge <= 0 && ship->is_able_to_act) {
        InitHomingShoot(ship);
        homing.weapon.cooldown_charge = homing.weapon.cooldown_time;
    }

    List_ForEach(homing.homing_shoots, (Function)HomingShootPositionUpdate);
    List_RemoveWithFn(homing.homing_shoots, NULL, CheckHomingShootOutOfBounds);
}

static void DrawHomingShoot(HomingShoot* homing_shoot) {
    Rectangle destRec = {
        homing_shoot->shoot.position.x,
        homing_shoot->shoot.position.y,
        homing_shoot->shoot.size.x,
		homing_shoot->shoot.size.y
    };
    Vector2 origin = { homing_shoot->shoot.size.x / 2.0f, homing_shoot->shoot.size.y / 2.0f };

    if (DEBUG_FLAG) {
        Vector2 center = { homing_shoot->shoot.position.x, homing_shoot->shoot.position.y };
        DrawCircleV(center, homing_shoot->shoot.size.x / 2.0f, Fade(RED, 0.5f));
        
        if (homing_shoot->target != NULL && homing_shoot->target->is_on_screen) {
            DrawLine(
                homing_shoot->shoot.position.x,
                homing_shoot->shoot.position.y,
                homing_shoot->target->position.x,
                homing_shoot->target->position.y,
                RED);
        }
    }

    DrawTexturePro(texture_projectiles, homing.weapon.source, destRec, origin, homing_shoot->visual_rotation, WHITE);
}

static void DrawHoming(void) {
    List_ForEach(homing.homing_shoots, (Function)DrawHomingShoot);
}
#pragma endregion
#pragma region SHOTGUN
//--------------------------------------------------------------
//
//                         SHOTGUN
// 
//--------------------------------------------------------------

Shotgun shotgun;

int GetShotgunLevel(void) { return shotgun.weapon.level; }

void ShotgunLevelUp(void) { 
    shotgun.weapon.level += 1;
    shotgun.min_shells += 1;
    shotgun.arc -= 5.0f;
}

static void InitShotgun(void) {
    shotgun.weapon.id = SHOTGUN;
    shotgun.weapon.level = 0;
	shotgun.min_shells = 3;
	shotgun.max_shells = 6;
    shotgun.weapon.offset = (Vector2){ DRAW_WH/2, DRAW_WH * 1.25 };
    shotgun.weapon.color = WHITE;
    shotgun.weapon.damage = 0.5f;
    shotgun.weapon.cooldown_time = 1.0f;
    shotgun.weapon.cooldown_charge = 0;
    shotgun.weapon.shoot_speed = (Vector2){ 0, -1000 };
    shotgun.arc = 45.0f;
    shotgun.shotgun_shoots = List_Create(sizeof(ShotgunShoot));
}

static void InitShotgunShoot(Ship* ship) {
    int shells = GetRandomValue(shotgun.min_shells, shotgun.max_shells);
    for (int i = 0; i < shells; i++) {
        ShotgunShoot new_shotgun_shoot = { 0 };
        new_shotgun_shoot.shoot.damage = ApplyMultiplier(damage_modifier, shotgun.weapon.damage);
        new_shotgun_shoot.shoot.size = (Vector2){ ApplyMultiplier(size_modifier, 48), ApplyMultiplier(size_modifier, 48) };

        float angle = (float)GetRandomValue((int)(-shotgun.arc), (int)(shotgun.arc));
        new_shotgun_shoot.orientation = angle;

        Vector2 rotated_offset = Vector2Rotate(shotgun.weapon.offset, angle * DEG2RAD);
        new_shotgun_shoot.shoot.position = Vector2Subtract(ship->position, rotated_offset);

        Vector2 base_speed = shotgun.weapon.shoot_speed;
        base_speed.y += GetRandomValue(0, -500);
        new_shotgun_shoot.speed = Vector2Rotate(base_speed, angle * DEG2RAD);

        new_shotgun_shoot.lifespan = (float)(GetRandomValue(shotgun.weapon.level, 2 + shotgun.weapon.level) / 10.0f);
        new_shotgun_shoot.alpha = 1.0f;

        if (Vector2Length(new_shotgun_shoot.speed) > 1300) {
            new_shotgun_shoot.source = (Rectangle){ 16, 0, 8, 8 };
        } else {
            new_shotgun_shoot.source = (Rectangle){ 24, 0, 8, 8 };
        }

        List_AddLast(shotgun.shotgun_shoots, &new_shotgun_shoot);
    }

    PlaySoundFxWithVolumeAndRandomPitch(&sound16, BASE_WEAPON_SFX_VOLUME, 3.0f, 3.5f);
}

static void UpdateShotgunShoot(ShotgunShoot* shotgun_shoot) {
    shotgun_shoot->shoot.position.x += ApplyMultiplier(speed_modifier, shotgun_shoot->speed.x) * GetFrameTime();
    shotgun_shoot->shoot.position.y += ApplyMultiplier(speed_modifier, shotgun_shoot->speed.y) * GetFrameTime();

    shotgun_shoot->lifespan -= GetFrameTime();
    shotgun_shoot->alpha = shotgun_shoot->lifespan / 0.3f;
    if (shotgun_shoot->alpha < 0) shotgun_shoot->alpha = 0;

}

static int CheckShotgunShootExpired(void* context, ShotgunShoot* item) {
    ShotgunShoot* shotgun_shoot = (ShotgunShoot*)item;
    if (shotgun_shoot->lifespan <= 0 || shotgun_shoot->shoot.position.y < -80) {
        return 1;
    }
    return 0;
}

static void UpdateShotgun(Ship* ship) {
    if (!shotgun.weapon.level) return;

    shotgun.weapon.cooldown_charge -= ApplyMultiplier(cooldown_modifier, GetFrameTime());

    if (shotgun.weapon.cooldown_charge <= 0) {
        InitShotgunShoot(ship);
        shotgun.weapon.cooldown_charge = shotgun.weapon.cooldown_time;
    }

    List_ForEach(shotgun.shotgun_shoots, (Function)UpdateShotgunShoot);
    List_RemoveWithFn(shotgun.shotgun_shoots, NULL, CheckShotgunShootExpired);
}

static void DrawShotgunShoot(ShotgunShoot* shotgun_shoot) {
    Rectangle destRec = {
        shotgun_shoot->shoot.position.x + shotgun_shoot->shoot.size.x / 2.0f,
        shotgun_shoot->shoot.position.y + shotgun_shoot->shoot.size.x / 2.0f,
        shotgun_shoot->shoot.size.x,
        shotgun_shoot->shoot.size.y,
    };

    Vector2 origin = {
        shotgun_shoot->shoot.size.x / 2.0f,
        shotgun_shoot->shoot.size.y / 2.0f
    };

	if (DEBUG_FLAG) {
		Vector2 center = { shotgun_shoot->shoot.position.x + shotgun_shoot->shoot.size.x / 2, shotgun_shoot->shoot.position.y + shotgun_shoot->shoot.size.y / 2 };
		DrawCircleV(center, 20, Fade(RED, 0.5f));
	}

    DrawTexturePro(texture_projectiles, shotgun_shoot->source, destRec, origin, shotgun_shoot->orientation, Fade(WHITE, shotgun_shoot->alpha));
}

static void DrawShotgun(void) {
    List_ForEach(shotgun.shotgun_shoots, (Function)DrawShotgunShoot);
}
#pragma endregion
#pragma region BLAST
List* blaster_shoots;

void InitBlasterShoot(Ship* ship, int level, float base_damage) {
    BlasterShoot shot = { 0 };
    shot.shoot.damage = ApplyMultiplier(damage_modifier, base_damage);
    shot.shoot.size = Vector2Add((Vector2){ 100, 100 }, Vector2MultiplyScalarF(Vector2One(), level * 20));

    shot.shoot.position = ship->position;
    shot.rotation = -45;
    shot.speed = (Vector2) { 0, -900 };
    shot.level = level;
    shot.targets = List_Create(sizeof(int));

    if (level == 0) {
        shot.source = (Rectangle){ 8 * 1, 8 * 5, 8, 8 };
        PlaySoundFxWithVolumeAndRandomPitch(&sound7, BASE_WEAPON_SFX_VOLUME, 1.5f, 1.5f);
    } else if (level == 1) {
        shot.source = (Rectangle){ 8 * 2, 8 * 5, 8, 8 };
        PlaySoundFxWithVolumeAndRandomPitch(&sound8, BASE_WEAPON_SFX_VOLUME, 1.5f, 1.5f);
    } else if (level == 2) {
        shot.source = (Rectangle){ 8 * 1, 8 * 7, 8, 8 };
        PlaySoundFxWithVolumeAndRandomPitch(&sound9, BASE_WEAPON_SFX_VOLUME, 1.5f, 1.5f);
    }

    List_AddLast(blaster_shoots, &shot);
}

static void DrawBlasterShoot(BlasterShoot* shot) {
    Rectangle destRec = {
        shot->shoot.position.x,
        shot->shoot.position.y,
        shot->shoot.size.x,
        shot->shoot.size.y,
    };

    Vector2 origin = {
        shot->shoot.size.x / 2.0f,
        shot->shoot.size.y / 2.0f
    };

	if (DEBUG_FLAG) {
        DrawCircle(shot->shoot.position.x, shot->shoot.position.y, shot->shoot.size.x / 1.50f, PINK);
        DrawCircle(shot->shoot.position.x, shot->shoot.position.y, shot->shoot.size.x / 1.75f, RED);
        DrawCircle(shot->shoot.position.x, shot->shoot.position.y, shot->shoot.size.x / 2.00f, GREEN);
	}

    
    DrawTexturePro(texture_projectiles, shot->source, destRec, origin, shot->rotation, WHITE);
}

static void DrawBlasterShots(void) {
    List_ForEach(blaster_shoots, (Function)DrawBlasterShoot);
}


static bool BlasterShoot_CheckForHits(Enemy* enemy, BlasterShoot* shoot) {
    Vector2 enemy_pos = { enemy->position.x, enemy->position.y };
    
    if (!enemy->is_targetable)
    return false;

    Node* node = shoot->targets->head;
    for(; node != NULL; node = node->next) {
        int* curr_enemy_id = node->data;
        if (enemy->id == *curr_enemy_id) {
            return false;
        }
    }
    
    float size;

    if (shoot->level == 0) {
        size = shoot->shoot.size.x / 2.0f;
    } else if (shoot->level == 1) {
        size = shoot->shoot.size.x / 1.75f;
    } else {
        size = shoot->shoot.size.x / 1.50f;
    }

    if (CheckCollisionCircles(enemy_pos, enemy->size.x / 2.0f, shoot->shoot.position, size)) {
        enemy->hp -= shoot->shoot.damage;
        CreateManagedEffect(EXPLOSION, enemy_pos);
        
        List_Add(shoot->targets, &enemy->id);

        return true;
    }
    
    return false;
}

static int CheckBlasterOutOfBonds(void* context, BlasterShoot* shot) {
    if (shot->shoot.position.y < -80) {
        List_Destroy(shot->targets);
        return true;
    }
    return false;
}

static void BlasterShootPositionUpdate(BlasterShoot* shoot) {
	shoot->shoot.position.x += ApplyMultiplier(speed_modifier, shoot->speed.x) * GetFrameTime();
	shoot->shoot.position.y += ApplyMultiplier(speed_modifier, shoot->speed.y) * GetFrameTime();
}

static void UpdateBlaster(Ship* ship) {
    List_ForEach(blaster_shoots, (Function)BlasterShootPositionUpdate);
	List_RemoveWithFn(blaster_shoots, NULL, (MatchFunction)CheckBlasterOutOfBonds);
}

#pragma endregion
#pragma region WORMHOLE
List* wormhole_enemy_list;

void WormholeClearList(void) {
    List_Destroy(wormhole_enemy_list);
    wormhole_enemy_list = List_Create(sizeof(int));
}

void WormholePassThroughDamage(Ship* ship, Vector2 position, float base_damage) {
    for(Node* enemyNode = enemies->head; enemyNode != NULL; enemyNode = enemyNode->next) {
        Enemy* enemy = (Enemy*)enemyNode->data;

        Node* node = wormhole_enemy_list->head;

        for(; node != NULL; node = node->next) {
            int* curr_enemy_id = node->data;

            if (enemy->id == *curr_enemy_id) {
                return false;
            }
        }

        if (CheckCollisionCircles(position, ship->draw_size.x /2.0f, enemy->position, enemy->size.x / 2.0f)) {
            float damage = ApplyMultiplier(damage_modifier, base_damage);
            List_Add(wormhole_enemy_list, &enemy->id);
            
            enemy->hp -= damage;
            CreateManagedEffect(EXPLOSION, enemy->position);
        }
    }
}

static bool CheckEnemyCollisionWithPlayer(Vector2* ship_pos, Vector2* enemy_pos) {
    float ship_radius = DRAW_WH / 2.0f;
    float enemy_radius = DRAW_WH / 2.0f;

    if (DEBUG_FLAG) {
        DrawCircleV(*ship_pos, ship_radius, Fade(RED, 0.5f));
        DrawCircleV(*enemy_pos, ship_radius, Fade(RED, 0.5f));
    }

    return CheckCollisionCircles(*ship_pos, ship_radius, *enemy_pos, enemy_radius);
}

#pragma endregion
#pragma region PRISM
//--------------------------------------------------------------
//
//                         PRISM
// 
//--------------------------------------------------------------

Prism prism;

int GetPrismLevel(void) { return prism.weapon.level; }
void PrismLevelUp(void) { 
    prism.weapon.level += 1;
    prism.weapon.cooldown_time -= 0.05f;
    prism.weapon.damage += 0.15f;
}

static void InitPrism(void) {
    prism.weapon.id = PRISM;
    prism.weapon.level = 0;

    prism.weapon.source = (Rectangle){ 0, 0, 0, 0 };
    prism.weapon.offset = (Vector2){ 0, 0 };
    prism.weapon.color = BLUE;

    prism.weapon.damage = 0.25f;
    prism.weapon.shoot_speed = (Vector2){ 0, 0 };

    prism.weapon.cooldown_time = 0.75f;
    prism.weapon.cooldown_charge = 0.0f;

    prism.prism_shoot.segments = NULL;
    prism.state = PRISM_INACTIVE;
}

static void ApplyPrismHit(Enemy* target, float damage) {
    if (!target) return;

    target->hp -= damage;
    CreateManagedEffect(ENERGY, target->position);
}

static Enemy* FindInitialPrismTarget(Ship* ship) {
    Enemy* first_enemy = NULL;
    float min_dist_sq = -1.0f;

    for (Node* enemyNode = enemies->head; enemyNode != NULL; enemyNode = enemyNode->next) {
        Enemy* enemy = (Enemy*)enemyNode->data;
        if (!enemy->is_targetable || !enemy->is_on_screen) continue;

        if (fabs(enemy->position.x - ship->position.x) < enemy->size.x && enemy->position.y < ship->position.y) {
            float dist_sq = Vector2DistanceSqr(ship->position, enemy->position);
            if (first_enemy == NULL || dist_sq < min_dist_sq) {
                min_dist_sq = dist_sq;
                first_enemy = enemy;
            }
        }
    }
    return first_enemy;
}

static Enemy* FindNextPrismTarget(Vector2 origin, List* hit_enemies_list) {
    Enemy* next_closest_enemy = NULL;
    float next_min_dist_sq = -1.0f;

    for (Node* enemyNode = enemies->head; enemyNode != NULL; enemyNode = enemyNode->next) {
        Enemy* enemy = (Enemy*)enemyNode->data;
        if (!enemy->is_targetable || !enemy->is_on_screen) continue;

        bool already_hit = false;
        for (Node* hitNode = hit_enemies_list->head; hitNode != NULL; hitNode = hitNode->next) {
            if (*(Enemy**)hitNode->data == enemy) {
                already_hit = true;
                break;
            }
        }
        if (already_hit) continue;

        float dist_sq = Vector2DistanceSqr(origin, enemy->position);
        if (next_closest_enemy == NULL || dist_sq < next_min_dist_sq) {
            next_min_dist_sq = dist_sq;
            next_closest_enemy = enemy;
        }
    }
    return next_closest_enemy;
}

static void InitPrismShoot(Ship* ship) {

    prism.prism_shoot.shoot.damage = ApplyMultiplier(damage_modifier, prism.weapon.damage);
    prism.prism_shoot.shoot.size = (Vector2){ 1.0f, 1.0f }; // Fixo
    prism.prism_shoot.alpha = 1.0f;

    if (prism.prism_shoot.segments != NULL) List_Destroy(prism.prism_shoot.segments);
    prism.prism_shoot.segments = List_Create(sizeof(Vector2));
    List_AddLast(prism.prism_shoot.segments, &ship->position);

    Enemy* current_target = FindInitialPrismTarget(ship);

    if (current_target == NULL) {
        Vector2 end_point = { ship->position.x, -20.0f };
        List_AddLast(prism.prism_shoot.segments, &end_point);
        prism.state = PRISM_ACTIVE;
        return;
    }

    List* hit_enemies_this_shot = List_Create(sizeof(Enemy*));
    Vector2 current_origin = ship->position;
    prism.prism_shoot.bounces = prism.weapon.level;

    while (current_target != NULL && prism.prism_shoot.bounces >= 0) {

        Vector2 hit_position = current_target->position;

        ApplyPrismHit(current_target, prism.prism_shoot.shoot.damage);

        List_AddLast(prism.prism_shoot.segments, &hit_position);
        List_Add(hit_enemies_this_shot, &current_target);
        current_origin = hit_position;

        prism.prism_shoot.bounces--;

        if (prism.prism_shoot.bounces < 0) break;

        current_target = FindNextPrismTarget(current_origin, hit_enemies_this_shot);
    }

    List_Destroy(hit_enemies_this_shot);
    prism.state = PRISM_ACTIVE;
}

static void UpdatePrism(Ship* ship) {
    if (!prism.weapon.level) return;

    if (prism.state == PRISM_ACTIVE) {
        prism.prism_shoot.alpha -= 5.0f * GetFrameTime();
        if (prism.prism_shoot.alpha <= 0) {
            prism.state = PRISM_INACTIVE;
            prism.prism_shoot.alpha = 0;
        }
    }

    prism.weapon.cooldown_charge -= ApplyMultiplier(cooldown_modifier, GetFrameTime());

    if (prism.weapon.cooldown_charge <= 0) {
        InitPrismShoot(ship);
        PlaySoundFxWithVolumeAndRandomPitch(&sound1, BASE_WEAPON_SFX_VOLUME, 2.0f, 2.5f);
        prism.weapon.cooldown_charge = prism.weapon.cooldown_time;
    }
}

static void DrawPrism(void) {
    if (prism.state != PRISM_ACTIVE) return;

    for (int i = 0; i < prism.prism_shoot.segments->size - 1; i++) {
        Vector2* start = (Vector2*)List_GetByIndex(prism.prism_shoot.segments, i);
        Vector2* end = (Vector2*)List_GetByIndex(prism.prism_shoot.segments, i + 1);

        if (start && end) {
            if (DEBUG_FLAG) {
                DrawCircleV(*start, 20, Fade(RED, 0.5f));
                DrawCircleV(*end, 20, Fade(RED, 0.5f));
            }
            //DrawLineV(*start, *end, Fade(prism.weapon.color, prism.prism_shoot.alpha));
			  DrawLineEx(*start, *end, prism.weapon.level, Fade(prism.weapon.color, prism.prism_shoot.alpha));
        }
    }
}

#pragma endregion

void EventHorizonTick(Vector2 position, float radius, float base_damage) {
    float damage = ApplyMultiplier(damage_modifier, base_damage);

    for (Node* enemyNode = enemies->head; enemyNode != NULL; enemyNode = enemyNode->next) {
        Enemy* enemy = (Enemy*)enemyNode->data;

        if (CheckCollisionCircles(position, radius, enemy->position, enemy->size.x / 2.0f)) {
            enemy->hp -= damage;
            CreateManagedEffect(CHAOS, enemy->position);
        }
    }
}

//--------------------------------------------------------------
//
//                         SHIELD
// 
//--------------------------------------------------------------

Shield shield;
bool IsShieldActive(void) { return shield.is_active; }

void ActivateShield(void) {
    shield.is_active = true;
    shield.capacity = 3;

}
void DeactivateShield(void) {
    shield.is_active = false;
    shield.capacity = 0;
}

int GetShieldCapacity(void) {
    return shield.capacity;
}

//--------------------------------------------------------------
//
//                         OTHERS
// 
//--------------------------------------------------------------

static bool CheckForHits(Enemy* enemy, Shoot* shoot) {
    Vector2 enemy_pos = { enemy->position.x, enemy->position.y };

    if (!enemy->is_targetable)
        return false;

    if (CheckCollisionCircles(enemy_pos, enemy->size.x / 2.0f, shoot->position, shoot->size.x / 2.0f)) {
        enemy->hp -= shoot->damage;
        CreateManagedEffect(EXPLOSION, enemy_pos);

        return true;
    }

    return false;
}

static void UnbindHomingProjectiles() {
    for(Node* enemyNode = enemies->head; enemyNode != NULL; enemyNode = enemyNode->next) {
        Enemy* enemy = (Enemy*)enemyNode->data;

        if (enemy->hp > 0) {
            continue;
        }

        for(Node* homingNode = homing.homing_shoots->head; homingNode != NULL; homingNode = homingNode->next) {
            HomingShoot* shoot = (HomingShoot*)homingNode->data;

            if (shoot->target == enemy) {
                shoot->target = NULL;
            }
        }
    }
}

bool CheckForProjectileCollisions(Ship* ship) {
    int enemy_count = enemies->size;
    for(Node* enemyNode = enemies->head; enemyNode != NULL; enemyNode = enemyNode->next) {
        Enemy* enemy = (Enemy*)enemyNode->data;

        if (!enemy->is_on_screen) continue;

        List_RemoveWithFn(pulse.pulse_shoots, enemy, (MatchFunction)CheckForHits);
        List_RemoveWithFn(photon.photon_shoots, enemy, (MatchFunction)CheckForHits);
        List_RemoveWithFn(shotgun.shotgun_shoots, enemy, (MatchFunction)CheckForHits);
        List_RemoveWithFn(homing.homing_shoots, enemy, (MatchFunction)CheckForHits);
        List_ForEachCtx(blaster_shoots, enemy, BlasterShoot_CheckForHits);
    }

    List_ForEach(enemies, UnbindHomingProjectiles);

    return false;
}

int GetWeaponLevel(int reference) {
    switch (reference) {
    case PULSE:   return GetPulseLevel();
    case PHOTON:  return GetPhotonLevel();
    case SHOTGUN: return GetShotgunLevel();
    case HOMING:  return GetHomingLevel();
	case PRISM:   return GetPrismLevel();
    default:      return false;
    }
}

const char* GetActiveWeaponsString(void) {
    static char active_weapons[256];  // Buffer p/ não precisar mallocar e dar free depois
    active_weapons[0] = '\0';

    if (GetPulseLevel())   strcat(active_weapons, "Pulse\n");
    if (GetPhotonLevel())  strcat(active_weapons, "Photon\n");
    if (GetShotgunLevel()) strcat(active_weapons, "Shotgun\n");
    if (GetHomingLevel())  strcat(active_weapons, "Homing\n");
	if (GetPrismLevel())   strcat(active_weapons, "Prism\n");

    if (strlen(active_weapons) == 0) {
        strcpy(active_weapons, "None");
    }
    else {
        int len = strlen(active_weapons);
        if (active_weapons[len - 1] == '\n') active_weapons[len - 1] = '\0';
    }

    return active_weapons;
}

int GetActiveWeaponsAmount(void) {
    int owned_weapons = 0;
    if (GetPulseLevel()) owned_weapons++;
    if (GetPhotonLevel()) owned_weapons++;
    if (GetShotgunLevel()) owned_weapons++;
    if (GetHomingLevel()) owned_weapons++;
	if (GetPrismLevel()) owned_weapons++;
    return owned_weapons;
}

static void InitAllWeapons(void) {
    InitPulse();
    InitPhoton();
    InitShotgun();
    InitHoming();
    InitPrism();

    blaster_shoots = List_Create(sizeof(BlasterShoot));
    wormhole_enemy_list = List_Create(sizeof(int));
}

void InitWeapon(void) {
    InitWeaponPowerUpMultipliers();
    InitAllWeapons();
}

void UpdateWeapon(Ship* ship) {
    UpdatePulse(ship);
    UpdatePhoton(ship);
    UpdateShotgun(ship);
    UpdateHoming(ship);
	UpdatePrism(ship);

    UpdateBlaster(ship);

    CheckForProjectileCollisions(ship);
}

void DrawWeapon(void) {
    DrawPulseShoot();
    DrawPhoton();
    DrawShotgun();
    DrawHoming();
	DrawPrism();
    DrawBlasterShots();
}