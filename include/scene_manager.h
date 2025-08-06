//--------------------------------------------------------------
//
//                      scene_manager.h
// 
//--------------------------------------------------------------
// 
// 
//--------------------------------------------------------------

#pragma once

#include "raylib.h"

typedef enum {
	INTRO,
    START,
    SELECT_SHIP,
    TUTORIAL,
    GAME,
    CREDITS,
    WINNER,
    GAME_OVER,
    ENDING_1,
    ENTER_NAME,
    RANKING,
    EXIT
} Scene;

void InitSceneManager(Scene scene);
void ChangeScene(Scene scene);
void ChangeSceneArgs(Scene scene, int id);
void LoopScene(void);