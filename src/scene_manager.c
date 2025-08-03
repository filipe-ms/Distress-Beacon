// scene_manager.c

#include "scene_manager.h"
#include "select_ship.h"
#include "main_menu.h"
#include "game.h"
#include "tutorial.h"
#include "ranking.h"
#include "enter_name.h"
#include "winner.h"
#include "game_over.h"
#include "audio_manager.h"
#include "credits.h"

Scene current_scene;
Scene main_menu;

void InitSceneManager(Scene scene) {
    ChangeScene(scene);
}

void ChangeScene(Scene scene) {
    ChangeSceneArgs(scene, 0);
}

void ChangeSceneArgs(Scene scene, int id) {
    current_scene = scene;

    switch (current_scene) {
    case START:
        InitMainMenu();
        break;

    case RANKING:
        InitRanking();
        break;

    case SELECT_SHIP:
        InitSelectMenu();
        break;

    case TUTORIAL:
        InitTutorial();
        break;

    case CREDITS:
        InitCredits();

    case GAME:
        InitGame();
        break;

    case ENTER_NAME:
        InitEnterName();
        break;

    case WINNER:
        InitWinner();
        break;

	case GAME_OVER:
		InitGameOver();
		break;

    case ENDING_1:
        Ending1Scene_Init();
        break;

    }
}


void UpdateCurrentScene(void) {

    switch (current_scene) {

    case START:
        UpdateMainMenu();
        break;

	case RANKING:
		UpdateRanking();
		break;

    case SELECT_SHIP:
        UpdateShipSelectMenu();
        break;

    case TUTORIAL:
        UpdateTutorial();
        break;

    case CREDITS:
		UpdateCredits();
		break;

    case GAME:
        UpdateGame();
        break;

    case ENTER_NAME:
        UpdateEnterName();
        break;

    case WINNER:
        UpdateWinner();
        break;

    case GAME_OVER:
        UpdateGameOver();
        break;
        
    case ENDING_1:
        Ending1Scene_Update();
        break;
    }

	UpdateAudio();
}

void DrawCurrentScene(void) {

    switch (current_scene) {

    case START:
        DrawMainMenu();
        break;

    case RANKING:
        DrawRanking();
        break;

    case SELECT_SHIP:
        DrawSelectMenu();
        break;

    case TUTORIAL:
        DrawTutorial();
        break;

	case CREDITS:
		DrawCredits();
		break;

    case GAME:
        DrawGame();
        break;

	case ENTER_NAME:
        DrawEnterName();
        break;

    case WINNER:
        DrawWinner();
        break;

    case GAME_OVER:
        DrawGameOver();
        break;
                
    case ENDING_1:
        Ending1Scene_Draw();
        break;
    }
}

void LoopScene() {
    while (!WindowShouldClose() && current_scene != EXIT) {
        UpdateCurrentScene();
        DrawCurrentScene();
    }
}
