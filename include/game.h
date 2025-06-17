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

void InitGame(int ship_id);  // Initialize game
void UpdateGame(void);       // Update game (one frame)
void DrawGame(void);         // Draw game (one frame)

// LOAD | UNLOAD
void LoadGameTextures(void);
void UnloadGameTextures(void);