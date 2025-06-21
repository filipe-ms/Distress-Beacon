#pragma once

#include "common.h"
#include "ship.h"
#include "raylib.h"

bool CheckForEnemyCollisions(Ship* ship);

float ClampWithFlagsF(float value, float min, float max, bool* has_reached_min, bool* has_reached_max);