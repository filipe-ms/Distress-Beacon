// weapon.c

#include "weapon.h"
#include "common.h"

#include "raymath.h"

#include "enemy.h"

#include <string.h>
#include <stdlib.h>

// Power up textures
Texture weapon_texture;

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

//--------------------------------------------------------------
//
//                         PULSE
// 
//--------------------------------------------------------------

Pulse pulse;

bool IsPulseActive(void) { return pulse.weapon.is_active; }
void ActivatePulse(void) { pulse.weapon.is_active = true; }

static void InitPulse(void) {
    pulse.weapon.id = PULSE;
    pulse.weapon.is_active = false;

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

static void InitPulseShoot(Ship* ship) {
    PulseShoot new_pulse_shoot = { 0 };
    new_pulse_shoot.shoot.damage = ApplyMultiplier(damage_modifier, pulse.weapon.damage);
    new_pulse_shoot.shoot.size = (Vector2){ ApplyMultiplier(size_modifier, 48.0f), ApplyMultiplier(size_modifier, 48.0f) };

    Vector2 shoot_spawn_position = {
        ship->position.x,
        ship->position.y - DRAW_WH*0.75
    };

    new_pulse_shoot.shoot.position = shoot_spawn_position;
    new_pulse_shoot.rotation = pulse.shoot_cycle * 15.0f;

    pulse.shoot_cycle++;
    if (pulse.shoot_cycle > 1) pulse.shoot_cycle = -1;

    List_AddLast(pulse.pulse_shoots, &new_pulse_shoot);
}

static void PulseShootPositionUpdate(PulseShoot* pulse_shoot) {
    Vector2 resulting_speed = Vector2Rotate(pulse.weapon.shoot_speed, pulse_shoot->rotation * DEG2RAD);
	pulse_shoot->shoot.position.x += ApplyMultiplier(speed_modifier, resulting_speed.x) * GetFrameTime();
	pulse_shoot->shoot.position.y += ApplyMultiplier(speed_modifier, resulting_speed.y) * GetFrameTime();
}

static int CheckIfOutOfBonds(void* context, PulseShoot* pulse_shoot) {
	if (pulse_shoot->shoot.position.y < -80) {
        return true;
	}
    return false;
}

static void UpdatePulse(Ship* ship) {
    if (!pulse.weapon.is_active) return;
	pulse.weapon.cooldown_charge -= ApplyMultiplier(cooldown_modifier, GetFrameTime());

    if (pulse.weapon.cooldown_charge <= 0) {
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

    DrawTexturePro(weapon_texture, pulse.weapon.source, pulse_rect_draw, draw_origin, pulse_shoot->rotation, WHITE);
}


static void DrawPulseShoot() {
	List_ForEach(pulse.pulse_shoots, (Function)PulseShootDraw);
}

//--------------------------------------------------------------
//
//                         PHOTON
// 
//--------------------------------------------------------------

Photon photon;

bool IsPhotonActive(void) { return photon.weapon.is_active; }
void ActivatePhoton(void) { photon.weapon.is_active = true; }

static void InitPhoton(void) {
    photon.weapon.id = PHOTON;
    photon.weapon.is_active = false;
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
    new_photon_shoot.shoot.position = (Vector2){ ship->position.x, ship->position.y };
    List_AddLast(photon.photon_shoots, &new_photon_shoot);
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
    if (!photon.weapon.is_active) return;

    photon.weapon.cooldown_charge -= ApplyMultiplier(cooldown_modifier, GetFrameTime());

    if (photon.weapon.cooldown_charge <= 0) {
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

    DrawTexturePro(weapon_texture, photon.weapon.source, destRec, origin, 0, WHITE);
}

static void DrawPhoton(void) {
    List_ForEach(photon.photon_shoots, (Function)DrawPhotonShoot);
}

//--------------------------------------------------------------
//
//                         HOMING
// 
//--------------------------------------------------------------

Homing homing;

const Vector2 homing_shoot_speed_base = { .x = 0, .y = -300 };

bool IsHomingActive(void) { return homing.weapon.is_active; }
void ActivateHoming(void) { homing.weapon.is_active = true; }

static void InitHoming(void) {
    homing.weapon.id = HOMING;
    homing.weapon.is_active = false;
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
    new_homing_shoot.current_velocity = (Vector2) { ApplyMultiplier(speed_modifier, homing_shoot_speed_base.x), ApplyMultiplier(speed_modifier, homing_shoot_speed_base.y) };
    List_AddLast(homing.homing_shoots, &new_homing_shoot);
}

static void HomingShootPositionUpdate(HomingShoot* homing_shoot) {
    // Pick the closest enemy
    
    if (homing_shoot->target == NULL) {
        Enemy* closest_enemy = NULL;
        float closest_distance = 0;
        Vector2 shootPos = homing_shoot->shoot.position;
        
        bool hasInitialized = false;
        
        
        for(int i = 0; i < MAX_ENEMY_NUMBER; i++) {
            Enemy* enemy = &enemies[i];
            Vector2 enemyPos = { enemy->position.x, enemy->position.y };
            
            float distance = Vector2Distance(enemyPos, shootPos);
            
            if (!hasInitialized || distance < closest_distance) { 
                closest_distance = distance;
                closest_enemy = enemy;
                hasInitialized = true;
            }
        }

        homing_shoot->target = closest_enemy;
    }
    
    if (homing_shoot->target == NULL || !homing_shoot->target->active) {   
        homing_shoot->shoot.position.y += ApplyMultiplier(speed_modifier, homing_shoot->current_velocity.y) * GetFrameTime();
        homing_shoot->shoot.position.x += ApplyMultiplier(speed_modifier, homing_shoot->current_velocity.x) * GetFrameTime();
        return;
    }

    // 1. Get base speed (magnitude)
    float base_speed = Vector2Length(homing_shoot_speed_base);

    // 2. Calculate PROPER direction vector (target -> projectile)
    Vector2 target_position = (Vector2) { homing_shoot->target->position.x, homing_shoot->target->position.y };
    Vector2 direction = Vector2Subtract(target_position, homing_shoot->shoot.position);
    Vector2 normalized_direction = Vector2Normalize(direction);

    // 3. Calculate desired angle (atan2 returns -π to π)
    float desired_angle = atan2f(normalized_direction.y, normalized_direction.x);

    // 4. Get current angle (convert to radians)
    float current_angle = homing_shoot->calc_rotation;

    // 5. Calculate angular difference (-π to π)
    float angle_diff = fmodf(desired_angle - current_angle + PI, 2*PI) - PI;

    // 6. Apply gradual adjustment
    float max_adjustment = 2.0f * GetFrameTime(); // Radians per frame
    float adjustment = Clamp(angle_diff, -max_adjustment, max_adjustment);
    float new_angle = current_angle + adjustment;

    // 7. Calculate new velocity vector
    Vector2 new_velocity = {
        cosf(new_angle) * base_speed,
        sinf(new_angle) * base_speed
    };

    homing_shoot->current_velocity = new_velocity;

    // 8. Update projectile state
    homing_shoot->shoot.position.x += ApplyMultiplier(speed_modifier, new_velocity.x) * GetFrameTime();
    homing_shoot->shoot.position.y += ApplyMultiplier(speed_modifier, new_velocity.y) * GetFrameTime();

    // 9. Set rotation (convert back to degrees)
    homing_shoot->visual_rotation = new_angle * RAD2DEG + 90;
    homing_shoot->calc_rotation = new_angle;
}

static int CheckHomingShootOutOfBounds(void* context, HomingShoot* item) {
    HomingShoot* homing_shoot = (HomingShoot*)item;
    if (homing_shoot->shoot.position.y < -80) {
        return 1;
    }
    return 0;
}

static void UpdateHoming(Ship* ship) {
    if (!homing.weapon.is_active) return;

    homing.weapon.cooldown_charge -= ApplyMultiplier(cooldown_modifier, GetFrameTime());

    if (homing.weapon.cooldown_charge <= 0) {
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
    }
	
    if (homing_shoot->target != NULL) {
        DrawLine(
            homing_shoot->shoot.position.x,
            homing_shoot->shoot.position.y,
            homing_shoot->target->position.x,
            homing_shoot->target->position.y,
            RED);
    }

    DrawTexturePro(weapon_texture, homing.weapon.source, destRec, origin, homing_shoot->visual_rotation, WHITE);
}

static void DrawHoming(void) {
    List_ForEach(homing.homing_shoots, (Function)DrawHomingShoot);
}

//--------------------------------------------------------------
//
//                         SHOTGUN
// 
//--------------------------------------------------------------

Shotgun shotgun;

bool IsShotgunActive(void) { return shotgun.weapon.is_active; }

void ActivateShotgun(void) { shotgun.weapon.is_active = true; }

static void InitShotgun(void) {
    shotgun.weapon.id = SHOTGUN;
    shotgun.weapon.is_active = false;
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
    int shells = GetRandomValue(3, 6);
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

        new_shotgun_shoot.lifespan = (float)(GetRandomValue(1, 3) / 10.0f);
        new_shotgun_shoot.alpha = 1.0f;

        if (Vector2Length(new_shotgun_shoot.speed) > 1300) {
            new_shotgun_shoot.source = (Rectangle){ 16, 0, 8, 8 };
        } else {
            new_shotgun_shoot.source = (Rectangle){ 24, 0, 8, 8 };
        }

        List_AddLast(shotgun.shotgun_shoots, &new_shotgun_shoot);
    }
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
    if (!shotgun.weapon.is_active) return;

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

    DrawTexturePro(weapon_texture, shotgun_shoot->source, destRec, origin, shotgun_shoot->orientation, Fade(WHITE, shotgun_shoot->alpha));
}

static void DrawShotgun(void) {
    List_ForEach(shotgun.shotgun_shoots, (Function)DrawShotgunShoot);
}

//--------------------------------------------------------------
//
//                         OTHERS
// 
//--------------------------------------------------------------

bool IsWeaponActive(int reference) {
    switch (reference) {
    case PULSE:   return IsPulseActive();
    case PHOTON:  return IsPhotonActive();
    case SHOTGUN: return IsShotgunActive();
    case HOMING:  return IsHomingActive();
    default:      return false;
    }
}

const char* GetActiveWeaponsString(void) {
    static char active_weapons[256];  // Buffer p/ não precisar mallocar e dar free depois
    active_weapons[0] = '\0';

    if (IsPulseActive())   strcat(active_weapons, "Pulse\n");
    if (IsPhotonActive())  strcat(active_weapons, "Photon\n");
    if (IsShotgunActive()) strcat(active_weapons, "Shotgun\n");
    if (IsHomingActive())  strcat(active_weapons, "Homing\n");

    if (strlen(active_weapons) == 0) {
        strcpy(active_weapons, "None");
    }
    else {
        int len = strlen(active_weapons);
        if (active_weapons[len - 1] == '\n') active_weapons[len - 1] = '\0';
    }

    return active_weapons;
}

static void InitAllWeapons(void) {
    InitPulse();
    InitPhoton();
    InitShotgun();
    InitHoming();
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
}

void DrawWeapon(void) {
    DrawPulseShoot();
    DrawPhoton();
    DrawShotgun();
    DrawHoming();
}

void LoadWeaponTextures(void) {
    weapon_texture = LoadTexture("weapons.png");
}

void UnloadWeaponTextures(void) {
    UnloadTexture(weapon_texture);
}