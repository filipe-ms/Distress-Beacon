#include "game.h"
#include "enemy.h"
#include "common.h"
#include "weapon.h"
#include "user_interface.h"
#include "power_ups.h"
#include "scene_manager.h"
#include "ship.h"
#include "player.h"
#include "enemy_wave.h"
#include "raymath.h"
#include "hit_confirmation.h"
#include "background.h"
#include "enemy_projectile.h"
#include "timer.h"

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
    EnemyProjectile_Init();
    InitPowerUps();
    InitWaves(false);
	InitEffects();
	InitEnemySourceRects();
	InitBackground(BACKGROUND_GAME, Fade(GRAY, 0.7f), STRETCH_TO_SCREEN, 1.0f, 100.0f);

    switch (GetPlayerShip()) {
        case AUREA:
            PulseLevelUp();
            break;
        case ORION:
            PhotonLevelUp();
            break;
        case NEBULA:
            ShotgunLevelUp();
            break;
        case PUDDLE_JUMPER:
            HomingLevelUp();
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
			UpdateBackground();
            UpdateEffects();
            UpdateWaves();
            UpdateEnemies(&ship);
            EnemyProjectile_Update(&ship);
            UpdateShip(&ship);
            int player_level = GetPlayerLevel();
            UpdateWeapon(&ship);
            if (!ship.is_alive) ChangeScene(GAME_OVER);
            if (GetPlayerLevel() > player_level) {
                PowerRandomizer();
                level_up_flag = true;
            }
        }
    }

	if (AreAllWavesCompleted() && !victory) {
		victory = true;
		ChangeSceneArgs(WINNER, GetPlayerScore());
        InitTimer(10.0f);
	}

    if (victory && UpdateTimer()) {
        ChangeScene(WINNER);
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
    DrawBackground();
    DrawEffects(RENDERING_ORDER_BEFORE_ENEMY);
    DrawEnemies();
    EnemyProjectile_Draw();
    DrawWeapon();
    DrawEffects(RENDERING_ORDER_BEFORE_SHIP);
    DrawShip(&ship);
    DrawEffects(RENDERING_ORDER_AFTER_SHIP);
    DrawLevelUpSelectMenu(level_up_flag);

    if (IsShieldActive()) {
        DrawCircleV(ship.position, (DRAW_WH / 2.0f) + 10.0f, Fade(SKYBLUE, 0.5f));
    }

    if (victory) DrawText("YOU WIN",   (int)GAME_SCREEN_CENTER - MeasureText("YOU WIN", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, WHITE);
    if (pause) DrawText("GAME PAUSED", (int)GAME_SCREEN_CENTER - MeasureText("GAME PAUSED", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, GRAY);

    DrawUserInterface(); // Desenha por último, está agora em outro plano
    EndDrawing();
}