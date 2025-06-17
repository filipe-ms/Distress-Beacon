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
    VOID,
    START,
    SELECT_SHIP,
    TUTORIAL,
    GAME,
    WINNER,
    GAME_OVER,
    ENTER_NAME,
    RANKING,
    EXIT
} Scene;

void InitSceneManager(Scene scene);
void ChangeScene(Scene scene);
void ChangeSceneArgs(Scene scene, int id);
void LoopScene(void);