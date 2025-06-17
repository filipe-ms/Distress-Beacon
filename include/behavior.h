#pragma once

#include "raylib.h"
#include "ship.h"

void BehaviorBasic(Rectangle* position, float speed);
void BehaviorZigZag(Rectangle* position, float speed_y, float* speed_x, float* move_time, bool* action_flag);
void BehaviorWaller(Rectangle* position, float speed_x);
void BehaviorBooster(Rectangle* position, const struct Ship* target, float* move_time, bool* action_flag);