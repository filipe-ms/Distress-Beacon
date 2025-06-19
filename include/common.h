//--------------------------------------------------------------
//
//                          common.h
//
//--------------------------------------------------------------
//
//              Constantes de v�rias partes do jogo
//
//--------------------------------------------------------------

#pragma once

#define DEBUG_FLAG 1

//--------------------------------------------------------------
//
//                             TELA
//
//--------------------------------------------------------------

#define SCREEN_WIDTH 1100     // Largura
#define SCREEN_HEIGHT 960     // Altura

#define GAME_SCREEN_WIDTH 700 // A soma disso com UI_WIDTH tem que ser igual a SCREEN_WIDTH
#define UI_WIDTH 400

//--------------------------------------------------------------
//
//                             JOGO
//
//--------------------------------------------------------------

#define BASE_EXP_TO_LEVEL 100       // EXP base necess�ria para o primeiro level
#define EXP_TO_LEVEL_INCREMENT 10   // Incremento de EXP necess�rio por level subsequente

#define BASE_ENEMY_SPAWN_TIMER 5.0f // Tempo base entre spawns de inimigos
#define BASE_WAVE_DURATION 60.0f    // Dura��o base de cada wave em segundos

#define MAX_ENEMY_NUMBER 100        // N�mero m�ximo de inimigos
#define MAX_WAVES 3                 // N�mero m�ximo de waves

//--------------------------------------------------------------
//
//                            DESENHO
//
//--------------------------------------------------------------

#define DRAW_WH 48 // Largura/Altura padr�o de desenho

//--------------------------------------------------------------
//
//                            OUTROS
//
//--------------------------------------------------------------

#define AUREA 0
#define ORION 1
#define NEBULA 2
#define PUDDLE_JUMPER 3

#define PLAYABLE_SHIPS 4

#define CENTER 0
#define LEFT 1
#define RIGHT 2