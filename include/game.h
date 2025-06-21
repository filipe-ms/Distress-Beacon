//--------------------------------------------------------------
//
//							game.h
// 
//--------------------------------------------------------------
// 
//			Toda a lógica de jogo acontece aqui.
// 
//--------------------------------------------------------------


#pragma once

#include "player.h"
#include "weapon.h"

#include "raylib.h"

void InitGame(void);  // Initialize game
void UpdateGame(void);       // Update game (one frame)
void DrawGame(void);         // Draw game (one frame)