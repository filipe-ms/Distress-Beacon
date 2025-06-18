#pragma once

#include "common.h"
#include "ship.h"
#include "raylib.h"

void WallBehavior(Vector2* position);
bool CheckForAllCollisions(Ship* ship);