// weapon.h

#pragma once

#include "raylib.h"
#include "ship.h"
#include "list.h"
#include "enemy.h"

typedef enum WeaponType {
	PULSE,
	PHOTON,
	SHOTGUN,
    HOMING,
	WEAPON_COUNT
} WeaponType;

//--------------------------------------------------------------
//
//                         GENERIC
// 
//--------------------------------------------------------------

typedef struct Shoot {
    float damage;
    Vector2 position;
    Vector2 size;
} Shoot;

typedef struct Weapon {
    int id;
    bool is_active;
    
    Rectangle source;
    Vector2 offset;
    Color color;

    float damage;
    Vector2 shoot_speed;
    
    float cooldown_time;
    float cooldown_charge;
} Weapon;

//--------------------------------------------------------------
//
//                         Pulse
// 
//--------------------------------------------------------------

typedef struct PulseShoot {
	Shoot shoot;
    float rotation;
	int shoot_cycle;
} PulseShoot;

typedef struct Pulse {
    Weapon weapon;
    int shoot_cycle;
	List* pulse_shoots;
} Pulse;

extern Pulse pulse;

bool IsPulseActive(void);
void ActivatePulse(void);

//--------------------------------------------------------------
//
//                         Photon
// 
//--------------------------------------------------------------

// Deixei como struct para facilitar no futuro se formos adicionar algum comportamento
typedef struct PhotonShoot {
    Shoot shoot;
} PhotonShoot;

typedef struct Photon {
    Weapon weapon;
    List* photon_shoots;
} Photon;

extern Photon photon;

bool IsPhotonActive(void);
void ActivatePhoton(void);

//--------------------------------------------------------------
//
//                         HOMING
// 
//--------------------------------------------------------------

// Deixei como struct para facilitar no futuro se formos adicionar algum comportamento
typedef struct HomingShoot {
    Shoot shoot;
    float visual_rotation;
    float calc_rotation;
    Vector2 current_velocity;
    Enemy* target;
} HomingShoot;

typedef struct Homing {
    Weapon weapon;
    List* homing_shoots;
} Homing;

extern Homing homing;

bool IsHomingActive(void);
void ActivateHoming(void);

//--------------------------------------------------------------
//
//                         Shotgun
// 
//--------------------------------------------------------------

typedef struct ShotgunShoot {
    Shoot shoot;
    Vector2 speed;
    Rectangle source;
    float lifespan;
    float orientation;
    float alpha;
} ShotgunShoot;

typedef struct Shotgun {
    Weapon weapon;
    float arc;
    List* shotgun_shoots;
} Shotgun;

extern Shotgun shotgun;

bool IsShotgunActive(void);
void ActivateShotgun(void);

// General
void InitWeapon(void);
void UpdateWeapon(Ship* ship);
void DrawWeapon(void);
void LoadWeaponTextures(void);
void UnloadWeaponTextures(void);
bool IsWeaponActive(int reference);
const char* GetActiveWeaponsString(void);

// Power Modifiers
float GetCooldownModifier(void);
float GetDamageModifier(void);
float GetSizeModifier(void);
float GetSpeedModifier(void);

void IncrementCooldownModifier(float value);
void IncrementDamageModifier(float value);
void IncrementSizeModifier(float value);
void IncrementSpeedModifier(float value);