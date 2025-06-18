#pragma once

#include "ship.h"

void InitEnemies(void);
void UpdateEnemies(Ship* ship);
void DrawEnemies(void);
void SpawnEnemies(int amount, int type, int hp);
void SpawnRandomEnemies(int amount, int hp);
void LoadEnemyTextures(void);
void UnloadEnemyTextures(void);