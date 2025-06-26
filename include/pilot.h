#pragma once

#include "raylib.h"

// Refatoração:
// Basta chamar essa função com uma duração quando tomar um tiro ou
// quando quiser que o piloto fale por qualquer motivo.
// A gente pode colocar um switch aqui e colocar outras animações tb
// tipo câmera chacoalhando etc.
void TriggerPilotAnimation(float duration);


// Desenha o piloto na tela
void DrawPilot(int ship, int pos_x, int pos_y, float scale, Color tint);
