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
    START,
    SELECT_SHIP,
    TUTORIAL,
    GAME,
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