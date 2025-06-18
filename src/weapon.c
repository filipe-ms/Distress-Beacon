// weapon.c

#include "weapon.h"
#include "common.h"

#include "raymath.h"

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
	cooldown_modifier = 1.0f;
	damage_modifier = 1.0f;
	size_modifier = 1.0f;
	speed_modifier = 1.0f;
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
	pulse.weapon.offset = (Vector2){ DRAW_WH/2, DRAW_WH * 1.25 };
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
    new_pulse_shoot.shoot.damage = pulse.weapon.damage * damage_modifier;
    new_pulse_shoot.shoot.size = (Vector2){ 48 * size_modifier, 48 * size_modifier };

    new_pulse_shoot.shoot.position = Vector2Subtract(ship->position, pulse.weapon.offset);
    new_pulse_shoot.rotation = pulse.shoot_cycle * 15.0f;

    pulse.shoot_cycle++;
    if (pulse.shoot_cycle > 1) pulse.shoot_cycle = -1;

    List_AddLast(pulse.pulse_shoots, &new_pulse_shoot);
}

static void PulseShootPositionUpdate(PulseShoot* pulse_shoot) {
    Vector2 resulting_speed = Vector2Rotate(pulse.weapon.shoot_speed, pulse_shoot->rotation * DEG2RAD);
	pulse_shoot->shoot.position.x += resulting_speed.x * speed_modifier * GetFrameTime();
	pulse_shoot->shoot.position.y += resulting_speed.y * speed_modifier * GetFrameTime();
}

static int CheckIfOutOfBonds(void* context, PulseShoot* pulse_shoot) {
	if (pulse_shoot->shoot.position.y < -80) {
        return true;
	}
    return false;
}

static void UpdatePulse(Ship* ship) {
    if (!pulse.weapon.is_active) return;
	pulse.weapon.cooldown_charge -= cooldown_modifier * GetFrameTime();

    if (pulse.weapon.cooldown_charge <= 0) {
		pulse.weapon.cooldown_charge = pulse.weapon.cooldown_time;
		InitPulseShoot(ship);
    }

    List_ForEach(pulse.pulse_shoots, (Function)PulseShootPositionUpdate);
	List_RemoveWithFn(pulse.pulse_shoots, NULL, (MatchFunction)CheckIfOutOfBonds);
}

static void PulseShootDraw(PulseShoot* pulse_shoot) {
    Rectangle pulse_rect_draw = {
        pulse_shoot->shoot.position.x + pulse_shoot->shoot.size.x/2,
        pulse_shoot->shoot.position.y + pulse_shoot->shoot.size.y/2,
        pulse_shoot->shoot.size.x,
        pulse_shoot->shoot.size.y
    };

    Vector2 draw_origin = { pulse_rect_draw.width / 2.0f, pulse_rect_draw.height / 2.0f };

    if (DEBUG_FLAG) {
        Vector2 center = { pulse_shoot->shoot.position.x + pulse_shoot->shoot.size.x / 2, pulse_shoot->shoot.position.y + pulse_shoot->shoot.size.y / 2 };
        DrawCircleV(center, 20, Fade(RED, 0.5f));
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
    new_photon_shoot.shoot.damage = photon.weapon.damage + damage_modifier;
    new_photon_shoot.shoot.size = (Vector2){ 36 * size_modifier, 36 * size_modifier };
    new_photon_shoot.shoot.position = (Vector2){ ship->position.x, ship->position.y };
    List_AddLast(photon.photon_shoots, &new_photon_shoot);
}

static void PhotonShootPositionUpdate(PhotonShoot* photon_shoot) {
    photon_shoot->shoot.position.y += photon.weapon.shoot_speed.y * speed_modifier * GetFrameTime();
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

    photon.weapon.cooldown_charge -= cooldown_modifier * GetFrameTime();

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
        new_shotgun_shoot.shoot.damage = shotgun.weapon.damage * damage_modifier;
        new_shotgun_shoot.shoot.size = (Vector2){ 48 * size_modifier, 48 * size_modifier };

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
    shotgun_shoot->shoot.position.x += shotgun_shoot->speed.x * GetFrameTime();
    shotgun_shoot->shoot.position.y += shotgun_shoot->speed.y * GetFrameTime();

    shotgun_shoot->lifespan -= GetFrameTime();
    shotgun_shoot->alpha = shotgun_shoot->lifespan / 0.3f; // Normalize alpha
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

    shotgun.weapon.cooldown_charge -= cooldown_modifier * GetFrameTime();

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
    default:     return false;
    }
}

const char* GetActiveWeaponsString(void) {
    static char active_weapons[256];  // Buffer p/ não precisar mallocar e dar free depois
    active_weapons[0] = '\0';

    if (IsPulseActive()) strcat(active_weapons, "Pulse\n");
    if (IsPhotonActive()) strcat(active_weapons, "Photon\n");
    if (IsShotgunActive()) strcat(active_weapons, "Shotgun\n");

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
}

void InitWeapon(void) {
    InitWeaponPowerUpMultipliers();
    InitAllWeapons();
}

void UpdateWeapon(Ship* ship) {
    UpdatePulse(ship);
    UpdatePhoton(ship);
    UpdateShotgun(ship);
}

void DrawWeapon(void) {
    DrawPulseShoot();
    DrawPhoton();
    DrawShotgun();
}

void LoadWeaponTextures(void) {
    weapon_texture = LoadTexture("weapons.png");
}

void UnloadWeaponTextures(void) {
    UnloadTexture(weapon_texture);
}