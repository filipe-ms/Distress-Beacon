//--------------------------------------------------------------
//
//                          common.h
//
//--------------------------------------------------------------
//
//              Constantes de várias partes do jogo
//
//--------------------------------------------------------------

#pragma once

#define DEBUG_FLAG 0

//--------------------------------------------------------------
//
//                             TELA
//
//--------------------------------------------------------------

#define SCREEN_WIDTH 1920	// Largura da tela
#define SCREEN_HEIGHT 1080	// Altura da tela


#define UI_WIDTH 525		// screen_width - game_screen_width / 2

#define GAME_SCREEN_WIDTH 870
#define GAME_SCREEN_HEIGHT (SCREEN_HEIGHT)
#define GAME_SCREEN_START (UI_WIDTH)	// Começa no ponto em que a UI da esquerda acaba
#define GAME_SCREEN_END (UI_WIDTH + GAME_SCREEN_WIDTH)
#define GAME_SCREEN_CENTER ((GAME_SCREEN_START + GAME_SCREEN_END) /2)
#define UI_LEFT_CENTER (525/2)
#define UI_RIGHT_CENTER (GAME_SCREEN_END + UI_LEFT_CENTER)

//--------------------------------------------------------------
//
//                             JOGO
//
//--------------------------------------------------------------

#define BASE_EXP_TO_LEVEL 100       // EXP base necessária para o primeiro level
#define EXP_TO_LEVEL_INCREMENT 10   // Incremento de EXP necessário por level subsequente

#define BASE_ENEMY_SPAWN_TIMER 5.0f // Tempo base entre spawns de inimigos
#define BASE_WAVE_DURATION 60.0f    // Duração base de cada wave em segundos

#define MAX_ENEMY_NUMBER 100        // Número máximo de inimigos
#define MAX_WAVES 3                 // Número máximo de waves

//--------------------------------------------------------------
//
//                            DESENHO
//
//--------------------------------------------------------------

#define DRAW_WH 48 // Largura/Altura padrão de desenho

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