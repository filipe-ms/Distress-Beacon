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

#define DEBUG_FLAG 1 // Hitboxes e outras coisas

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
//                             SONS
//
//--------------------------------------------------------------

#define AUDIO_FLAG 1; // Ativa os sons do jogo

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
#define MAX_WAVES 50                // Número máximo de waves

#define ENEMY_LINE_SPAWN_START -80

#define MAX_WEAPON_SLOTS 3

//--------------------------------------------------------------
//
//                            DESENHO
//
//--------------------------------------------------------------

#define DRAW_WH 48 // Largura/Altura padrão de desenho das naves e inimigos simples
#define UI_BG_COLOR (Color){ 13, 13, 23, 255 }
#define UI_HIGHLIGHT_COLOR (Color){ 20, 20, 40, 255 }

//--------------------------------------------------------------
//
//                            OUTROS
//
//--------------------------------------------------------------

#define NONE -1

// Pilotos jogáveis
#define AUREA 0
#define ORION 1
#define NEBULA 2
#define PUDDLE_JUMPER 3
#define VOID 4

#define PLAYABLE_SHIPS 5

// Pilotos não jogáveis
#define DUCK 5
#define ROBOT 6
#define ALIEN 7
#define PIRATE 8
#define INVERSE 9
#define INTERFERENCE 10

// Número de pilotos. Não é o número de jogáveis!
#define TOTAL_PILOT_NUMBER 11

// Direções
#define CENTER 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

//--------------------------------------------------------------
//
//                        BALANCEAMENTO
//
//--------------------------------------------------------------

#define GHOST_INVIS_TIMER 3.0f