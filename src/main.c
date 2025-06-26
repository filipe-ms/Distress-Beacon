/*******************************************************************************************
*
*   Jogo desenvolvido por Luís Filipe Macedo, Anderson Gabriel e Débora Souza
*
*   Esse jogo foi criado usando Raylib (www.raylib.com)
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

// Nossos próprios includes
#include "common.h"

#include "scene_manager.h"

#include "player.h"
#include "enemy.h"
#include "weapon.h"
#include "power_ups.h"

#include "main_menu.h"
#include "select_ship.h"

#include "tutorial.h"

#include "special_effects.h"
#include "list.h"

#include "background.h"
#include "enemy_projectile.h"

#include "texture_manager.h"

static Scene starting_scene = START;

void LoadGame(void);
void UnloadGame(void);

//--------------------------------------------------------------
//
//                           MAIN
// 
//--------------------------------------------------------------

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Distress Beacon"); // Inicializa a janela do jogo

    SetTargetFPS(240);

    LoadGame();     // Inicializa o diretório de recursos/assets e as texturas
    InitEnemies();
    InitSceneManager(starting_scene);

    LoopScene();

    UnloadGame();   // Free

    CloseWindow();  // Fecha a janela

    return 0;
}

// Inicializa as texturas no jogo
void LoadGame() {
    SearchAndSetResourceDir("resources");
    LoadAllTextures();
    InitEnemySourceRects();
    LoadTutorialTextures();
    EnemyProjectile_Load();
}

// Unload game variables
void UnloadGame(void)
{
    LoadAllTextures();
    UnloadEnemyList();
    UnloadTutorialTextures();
    UnloadPowerUpTextures();
	UnloadEffects();
	List_FreeAll();
}