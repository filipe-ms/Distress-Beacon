#include "game.h"
#include "enemy.h"
#include "common.h"
#include "weapon.h"
#include "user_interface.h"
#include "power_ups.h"
#include "scene_manager.h"
#include "ship.h"
#include "game_background.h"
#include "list.h"
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

void InitGame(int ship_id) {
    // Flags
    pause = false;
    pause_flag = false;
    victory = false;
    level_up_flag = false;
    
    // Other inits
    InitShip(&ship, ship_id);
    InitWeapon();
    InitPlayer();
    InitEnemies();
    InitPowerUps();
	InitGameBackground();
    InitWaves();

    if (ship_id == 0) { // AUREA
        ActivatePulse();
    }
    else if (ship_id == 1) { // ORION
        ActivatePhoton();
    }
    else if (ship_id == 2) { // NEBULA
        ActivateShotgun();
    }
    else { // Default
        ActivatePulse();
    }
}

//--------------------------------------------------------------
//
//                      ENEMY KILLS
// 
//--------------------------------------------------------------

void CheckCollision(Shoot* shoot) {
}

// Function to check and handle bullet and enemy collisions
void CheckBulletAndEnemyCollision() {

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
            CheckBulletAndEnemyCollision();
            UpdateShip(&ship);
            UpdateWeapon(&ship);
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
	
	DrawUserInterface();
    DrawEnemies();
    DrawWeapon(&ship);
    DrawShip(&ship);
    DrawLevelUpSelectMenu(level_up_flag);
    DrawWaves();
    
    if (victory) DrawText("YOU WIN", SCREEN_WIDTH / 2 - MeasureText("YOU WIN", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, WHITE);
    if (pause) DrawText("GAME PAUSED", SCREEN_WIDTH / 2 - MeasureText("GAME PAUSED", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, GRAY);
    
    EndDrawing();
}

//--------------------------------------------------------------
//
//                     LOAD | UNLOAD
// 
//--------------------------------------------------------------

void LoadGameResources(void) {
}

void UnloadGameResources(void) {
	UnloadGameBackground();
}