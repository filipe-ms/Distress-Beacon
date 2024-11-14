/*******************************************************************************************
*
*   raylib - classic game: space invaders
*
*   Sample game developed by Ian Eito, Albert Martos and Ramon Santamaria
*
*   This game has been created using raylib v1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

//--------------------------------------------------------------
//
//                         IMPORTS
// 
//--------------------------------------------------------------

// Raylib
#include "raylib.h"
#include "resource_dir.h"

// Outras libs
#include "math.h"

// Nossos pr�prios includes
#include "common.h"
#include "draw_object.h"

#include "scene_manager.h"

#include "player.h"
#include "enemy.h"
#include "weapon.h"
#include "power_ups.h"

#include "main_menu.h"
#include "select_ship.h"

#include "tutorial.h"
#include "game.h"

static Scene starting_scene = SELECT_SHIP;

void LoadGame(void);
void UnloadGame(void);

//--------------------------------------------------------------
//
//                           MAIN
// 
//--------------------------------------------------------------

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders"); // Inicializa a janela do jogo

    SetTargetFPS(240);

    LoadGame();         // Inicializa o diret�rio de recursos/assets e as texturas
    
    InitSceneManager(starting_scene);

    LoopScene();

    UnloadGame();           // Free

    CloseWindow();          // Fecha a janela

    return 0;
}

// Inicializa as texturas no jogo
void LoadGame() {
    SearchAndSetResourceDir("resources");

    LoadMenuBackgroundTexture();
    LoadSelectMenuTextures();
    LoadObjectTextures();
    LoadEnemyTextures();
    LoadGameTextures();
    LoadTutorialTextures();
    LoadWeaponTextures();
    LoadPowerUpTextures();
}


// Unload game variables
void UnloadGame(void)
{
    UnloadObjectTextures();
    UnloadEnemyTextures();
    UnloadSelectMenuTextures();
    UnloadMenuBackgroundTexture();
    UnloadTutorialTextures();
    UnloadWeaponTextures();
    UnloadPowerUpTextures();
}