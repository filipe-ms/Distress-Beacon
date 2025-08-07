// game.c
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
#include "special_effects.h"
#include "audio_manager.h"
#include "background.h"
#include "enemy_projectile.h"
#include "timer.h"
#include "input.h"
#include "scene_draw_effects.h"


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
bool player_has_died;

void InitGame(void) {
    // Flags
    pause = false;
    pause_flag = false;
    victory = false;
    level_up_flag = false;
	player_has_died = false;

    // Other inits
    InitEffects();
    InitShip(&ship, GetPlayerShip());
    InitWeapon();
    InitPlayer();
    InitEnemies();
    EnemyProjectile_Init();
    InitPowerUps();
    InitWaves(false);
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
        case VOID:
            PrismLevelUp();
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
    UpdateEffects();

    if (player_has_died) {
        
        if (GetElapsedTime() > 0.5f && ship.should_render) {
            ship.should_render = false;
            CreateManagedEffect(EXPLOSION, ship.position);
        }
        if (UpdateTimer()) {
            ChangeScene(GAME_OVER);
        }
    } else {
        if (IsPauseButtonPressed() && !pause_flag) {
            pause = !pause;
            pause_flag = true;
        }
        if (IsPauseButtonReleased()) {
            pause_flag = false;
        }

        if (!pause)
        {
            if (level_up_flag) {
                UpdateLevelUpSelectMenu(&level_up_flag);
            } else {
                int player_level = GetPlayerLevel();

			    UpdateBackground();
                
                UpdateWaves();
                UpdateEnemies(&ship);
                EnemyProjectile_Update(&ship);
                UpdateShip(&ship);
                UpdateWeapon(&ship);
                CleanupEnemies();

                if (!ship.is_alive) {
				    InitTimer(1.0f);
				    player_has_died = true;
                }

                if (GetPlayerLevel() > player_level) {
                    PowerRandomizer();
                    level_up_flag = true;
                }
            }
        }

	    if (AreAllWavesCompleted() && !victory) {
            const float sound_time = 9.0f;
		    victory = true;

            InitTimer(sound_time);
            StopMusic();
            PlaySoundFxWithVolumeAndRandomPitch(&sound28, 0.8, 1, 1);
            InitFadeOutEffect(sound_time, BLACK, 0);
	    }

        if (victory && UpdateTimer()) {
            ChangeSceneArgs(ENDING_1, GetPlayerScore());
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

    UpdateAndDrawScreenEffects();

    if (victory) DrawText("VOCÊ VENCEU!",   (int)GAME_SCREEN_CENTER - MeasureText("VOCÊ VENCEU!", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, WHITE);
    if (pause) DrawText("JOGO PAUSADO", (int)GAME_SCREEN_CENTER - MeasureText("JOGO PAUSADO", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, GRAY);

    DrawUserInterface(); // Desenha por último, está agora em outro plano
    EndDrawing();
}