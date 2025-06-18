//--------------------------------------------------------------
//
//                          common.h
// 
//--------------------------------------------------------------
// 
//      Utilizado para objetos, variáveis e afins 
//            comuns a várias partes do jogo.
// 
//--------------------------------------------------------------

#pragma once


//--------------------------------------------------------------
//
//                          DEBUG
// 
//--------------------------------------------------------------

#define DEBUG_FLAG 1

//--------------------------------------------------------------
//
//                          TELA
// 
//--------------------------------------------------------------


#define SCREEN_WIDTH 1100   // Largura
#define SCREEN_HEIGHT 960  // Altura

#define GAME_SCREEN_WIDTH 700 // A soma disso com UI_WIDTH tem que ser igual a SCREEN_WIDTH
#define UI_WIDTH 400


//--------------------------------------------------------------
//
//                          PERFORMANCE
// 
//--------------------------------------------------------------


#define MAX_ENEMY_NUMBER 100


//--------------------------------------------------------------
//
//                          JOGO
// 
//--------------------------------------------------------------


#define STARTING_POS_X (SCREEN_WIDTH / 2)
#define STARTING_POS_Y (SCREEN_HEIGHT - 100)

#define BASE_EXP_TO_LEVEL 10
#define EXP_TO_LEVEL_INCREMENT 10

#define BASE_ENEMY_SPAWN_TIMER 5.0f
#define BASE_WAVE_DURATION 60.0f

#define MAX_WAVES 3

//--------------------------------------------------------------
//
//                          DESENHO
// 
//--------------------------------------------------------------


#define DRAW_WH 48