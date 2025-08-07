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

#define DEBUG_FLAG 0 // Hitboxes e outras coisas

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

#define AUDIO_FLAG 1 // Ativa os sons do jogo

#define ENEMY_DEATH_SFX_VOLUME 0.25f
#define BASE_WEAPON_SFX_VOLUME 0.25f
#define MENU_SFX_VOLUME 0.5f

#define BASE_MUSIC_VOLUME 0.03f


//--------------------------------------------------------------
//
//                             JOGO
//
//--------------------------------------------------------------

#define MAX_WAVES				100		// Número máximo de waves
#define ENEMY_LINE_SPAWN_START	-80
#define MAX_WEAPON_SLOTS		3

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

// Levels e EXP
#define BASE_EXP_TO_LEVEL		100		// EXP base necessária para o primeiro level
#define EXP_TO_LEVEL_INCREMENT	25		// Incremento de EXP necessário por level subsequente
#define EXP_MULTIPLIER			1.0f	// Multiplicador de EXP ganho por inimigo abatido

// Spawns e waves
#define FIRST_WAVE_SPAWN_TIMER	5.0f	// Tempo base entre spawns de inimigos
#define TIME_BETWEEN_WAVES		3.0f	// Tempo base entre spawns de inimigos em waves
#define BASE_WAVE_DURATION		60.0f	// Duração base de cada wave em segundos


// ------------------ Inimigos ------------------

#define INTENSITY_GROWTH 1.0f			// Multiplicador que atua sobre inimigos, afetando HP, número de spawns etc 
#define INTENSITY_SPAWN_FACTOR 0.25f	// Multiplicador que atua sobre o número de inimigos por wave

// HP
#define BASIC_HP_BASE 1.0f

// Específicos
#define GHOST_INVIS_TIMER		3.0f	// Tempo que o ghost fica invisível
#define GHOST_DISTANCE_RANGE	100.0f	// Raio de spawn do ghost ao redor do jogador


// Cálculos

/* Intensity */
#define INTENSITY_UPDATE_EQUATION (intensity = INTENSITY_GROWTH > 0 ? current_wave_number * INTENSITY_GROWTH : 1.0f) // Atualiza a intensity no update

/* DENSITY 
Density só é relevante no modo endless, pois as waves são geradas de 10 em 10.
Para jogo normal, as waves são geradas todas de uma vez. */
#define DENSITY_UPDATE_EQUATION (density = 0.0f) // Atualiza a density no update

/* NEXT WAVE */
#define NEXT_WAVE_TIMER_EQUATION (next_wave_start_time += TIME_BETWEEN_WAVES - density);