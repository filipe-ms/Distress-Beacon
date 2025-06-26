// enemy_wave.h
#pragma once

#include "raylib.h"

void InitWaves(bool is_endless_mode);
void UpdateWaves(void);
bool AreAllWavesCompleted(void);

int GetCurrentWaveNumber(void);