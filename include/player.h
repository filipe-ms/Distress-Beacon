// player.h
#pragma once

#include "raylib.h"

int GetPlayerShip(void);
int GetPlayerScore(void);
int GetPlayerEnemiesKilled(void);
int GetPlayerLevel(void);
int GetPlayerExperience(void);
int GetPlayerExpToLevel(void);

void SetPlayerShip(int ship_id);
void InitPlayer(void);
bool AddExperience(int exp);
void AddScore(int score);
void AddEnemyKill(void);