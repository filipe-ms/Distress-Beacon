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
    int level;
    
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
    Vector2 shoot_speed;
} PulseShoot;

typedef struct Pulse {
    Weapon weapon;
    int shoot_cycle;
	List* pulse_shoots;
} Pulse;

extern Pulse pulse;

int GetPulseLevel(void);
void PulseLevelUp(void);
void InitPulseShootAtCoords(Ship* ship, Vector2 position, float angle_in_deg);

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

int GetPhotonLevel(void);
void PhotonLevelUp(void);

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
    bool has_locked_on_any_target;
} HomingShoot;

typedef struct Homing {
    Weapon weapon;
    List* homing_shoots;
} Homing;

extern Homing homing;

int GetHomingLevel(void);
void HomingLevelUp(void);

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
    int min_shells;
    int max_shells;
    float arc;
    List* shotgun_shoots;
} Shotgun;

extern Shotgun shotgun;

int IsShotgunActive(void);
void ShotgunLevelUp(void);


typedef struct Shield {
    int capacity;
    bool is_active;
} Shield;

extern Shield shield;

// Shield Apply
bool IsShieldActive(void);
void ActivateShield(void);
void DeactivateShield(void);
int GetShieldCapacity(void);

// General
void InitWeapon(void);
void UpdateWeapon(Ship* ship);
void DrawWeapon(void);
int GetWeaponLevel(int reference);
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