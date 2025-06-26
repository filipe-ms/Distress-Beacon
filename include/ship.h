// ship.h

#pragma once

#include "common.h"
#include "player.h"

#include "raylib.h"

typedef struct Ship {
	int id;                 // ID da nave (Aurea, Orion, Nebula)
    Vector2 position;       // O local em que vai ser desenhado
	Vector2 draw_size;      // Tamanho do desenho da nave
    int direction;          // Para que lado está andando
    int thruster_cycle;     // Em que sprite o thruster está
	float animation_cycle;  // Ciclo de animação da nave
    Color color;            // Cor
    float alpha;            // Transparência
	Vector2 speed;          // Velocidade da nave
    bool isInvincible;
    float invincibilityTimer;
    float is_alive;
} Ship;

extern Ship ship;

void InitShip(Ship* ship, int id);
void UpdateShip(Ship* ship);
void Ship_TakeDamage(Ship* ship);
void DrawShip(Ship* ship);
void UnloadShip();
