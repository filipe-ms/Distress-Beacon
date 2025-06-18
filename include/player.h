// player.h
#pragma once

#include "raylib.h"

int GetPlayerScore(void);
int GetPlayerEnemiesKilled(void);
int GetPlayerLevel(void);
int GetPlayerExperience(void);
int GetPlayerExpToLevel(void);

void InitPlayer(void);
bool AddExperience(int exp);
void AddScore(int score);
void AddEnemyKill(void);