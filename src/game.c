#include "game.h"
#include "enemy.h"
#include "common.h"
#include "weapon.h"
#include "user_interface.h"
#include "power_ups.h"
#include "scene_manager.h"
#include "ship.h"
#include "game_background.h"
#include "game_behavior.h"
#include "player.h"
#include "enemy_wave.h"
#include "raymath.h"

// Waves
#define FIRST_WAVE 0
#define SECOND_WAVE 1
#define THIRD_WAVE 2
#define VICTORY 3

// FLAGS
bool pause;
bool pause_flag;
bool victory;
bool level_up_flag;

Ship ship;

void InitGame(void) {
    // Flags
    pause = false;
    pause_flag = false;
    victory = false;
    level_up_flag = false;
    
    // Other inits
    InitShip(&ship, GetPlayerShip());
    InitWeapon();
    InitPlayer();
    InitEnemies();
    InitPowerUps();
	InitGameBackground();
    InitWaves();

    switch (GetPlayerShip()) {
        case AUREA:
            ActivatePulse();
            break;
        case ORION:
            ActivatePhoton();
            break;
        case NEBULA:
            ActivateShotgun();
            break;
        case PUDDLE_JUMPER:
            ActivateHoming();
            break;
    }
}


//--------------------------------------------------------------
//
//                         UPDATE
// 
//--------------------------------------------------------------



void UpdateGame(void)
{
    if (IsKeyPressed('P') && !pause_flag) {
        pause = !pause;
        pause_flag = true;
    }
    if (IsKeyReleased('P')) {
        pause_flag = false;
    }

    if (!pause)
    {
        if (level_up_flag) {
            UpdateLevelUpSelectMenu(&level_up_flag);
        } else {
            UpdateGameBackground();
            UpdateWaves();
            UpdateEnemies(&ship);
            UpdateShip(&ship);
            UpdateWeapon(&ship);
			int player_level = GetPlayerLevel();
            if (CheckForAllCollisions(&ship)) ChangeScene(GAME_OVER);
            if (GetPlayerLevel() > player_level) {
                PowerRandomizer();
                level_up_flag = true;
            }
        }
    }
}


//--------------------------------------------------------------
//
//                         DRAW
// 
//--------------------------------------------------------------


void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(BLACK);
	DrawGameBackground();
	
    DrawEnemies();
    DrawWeapon();
    DrawShip(&ship);
    DrawLevelUpSelectMenu(level_up_flag);
    DrawWaves();
    
    if (victory) DrawText("YOU WIN", GAME_SCREEN_WIDTH / 2 - MeasureText("YOU WIN", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, WHITE);
    if (pause) DrawText("GAME PAUSED", GAME_SCREEN_WIDTH / 2 - MeasureText("GAME PAUSED", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, GRAY);
    
    DrawUserInterface(); // Desenha por �ltimo, est� agora em outro plano
    EndDrawing();
}

//--------------------------------------------------------------
//
//                     LOAD | UNLOAD
// 
//--------------------------------------------------------------

void UnloadGameResources(void) {
	UnloadGameBackground();
}