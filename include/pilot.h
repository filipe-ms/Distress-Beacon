#pragma once

#include "raylib.h"

// Refatora��o:
// Basta chamar essa fun��o com uma dura��o quando tomar um tiro ou
// quando quiser que o piloto fale por qualquer motivo.
// A gente pode colocar um switch aqui e colocar outras anima��es tb
// tipo c�mera chacoalhando etc.
void TriggerPilotAnimation(float duration);


// Desenha o piloto na tela
void DrawPilot(int ship, int pos_x, int pos_y, float scale, Color tint);
