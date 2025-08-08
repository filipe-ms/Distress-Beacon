// power_ups.h

#pragma once

#include "raylib.h"

typedef enum PowerUpType {
    // Efeitos de Power-up
    SHOOT_COOLDOWN,
    SHOOT_DAMAGE,
    SHOOT_SIZE,
    SHOOT_SPEED,

    // Armas
    WEAPON_PULSE,
    WEAPON_PHOTON,
    WEAPON_SHOTGUN,
    WEAPON_HOMING,
    WEAPON_PRISM,

    // Shield
    SHIELD,

    // Esse aqui sempre vai ser o número total de power ups
    POWERUP_COUNT
} PowerUpType;

typedef struct PowerUpCard {
    PowerUpType type;
    float value;

    char* name;
    char* type_string;
    char* description;

    Texture* texture;

    char description_buffer[128];

} PowerUpCard;

void InitPowerUps(void);
void UnloadPowerUpTextures(void);
void PowerRandomizer(void);
void UpdateLevelUpSelectMenu(bool* flag);
void DrawLevelUpSelectMenu(bool flag);