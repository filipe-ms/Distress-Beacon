//--------------------------------------------------------------
//
//                      draw_object.h
// 
//--------------------------------------------------------------
// 
// Esse header serve para desenhar os assets.
// Separei dos outros lugares pra facilitar o desenho.
// 
//--------------------------------------------------------------

#pragma once

#define CENTER 0
#define LEFT 1
#define RIGHT 2

#define AUREA 0
#define ORION 1
#define NEBULA 2

#include "raylib.h"
#include "ship.h"

// Específicos

// Aurea
typedef struct Aurea {
    Rectangle destination;  // O local em que vai ser desenhado
    int direction;        // Para que lado está andando
    int thruster_cycle;     // Em que ciclo o thruster está
    Color color;            // Cor
    float alpha;            // Transparência
} Aurea;

void DrawAurea(Aurea* aurea);

// Orion
typedef struct Orion {
    Rectangle destination;  // O local em que vai ser desenhado
    int direction;        // Para que lado está andando
    int thruster_cycle;     // Em que ciclo o thruster está
    Color color;            // Cor
    float alpha;            // Transparência
} Orion;

void DrawOrion(Orion *orion);

// Nebula
typedef struct Nebula {
    Rectangle destination;  // O local em que vai ser desenhado
    int direction;        // Para que lado está andando
    int thruster_cycle;     // Em que ciclo o thruster está
    Color color;            // Cor
    float alpha;            // Transparência
} Nebula;

void DrawNebula(Nebula* nebula);



void LoadObjectTextures(void);
void UnloadObjectTextures(void);
