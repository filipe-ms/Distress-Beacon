// behavior.c

#include "behavior.h"
#include "ship.h"
#include "common.h"

#include "raylib.h"
#include "raymath.h"

void BehaviorBasic(Rectangle* position, float speed) {
    position->y += speed * GetFrameTime();
}

void BehaviorZigZag(Rectangle* position, float speed_y, float* speed_x, float* move_time, bool* action_flag) {
    if (*action_flag == false) {
        *move_time = (float)GetRandomValue(1, 2);
        *speed_x = (GetRandomValue(0, 1) == 0) ? 100 : -100;
        *action_flag = true;
    }

    *move_time -= GetFrameTime();
    if (*move_time <= 0) {
        *move_time = (float)GetRandomValue(1, 2);
        *speed_x *= -1;
    }

    float boost = (float)(GetRandomValue(1, 5));

    position->y += speed_y * GetFrameTime();
    position->x += *speed_x * boost * GetFrameTime();
}

void BehaviorWaller(Rectangle* position, float speed_x) {
    position->x += speed_x * GetFrameTime();
}

void BehaviorBooster(Rectangle* position, const struct Ship* target, float* move_time, bool* action_flag) {
    float initial_y_target = 100.0f;
    float seek_speed = 100.0f;
    float boost_speed = 1000.0f;

    if (position->y < initial_y_target) {
        position->y += 100 * GetFrameTime();
    }
    else {
        if (*action_flag == false) {
            *move_time = 2.0f;
            *action_flag = true;
        }

        if (*move_time > 0) {
            *move_time -= GetFrameTime();
            if (position->x < target->position.x) {
                position->x += seek_speed * GetFrameTime();
                if (position->x > target->position.x) position->x = target->position.x;
            }
            else if (position->x > target->position.x) {
                position->x -= seek_speed * GetFrameTime();
                if (position->x < target->position.x) position->x = target->position.x;
            }
        }
        else {
            position->y += boost_speed * GetFrameTime();
        }
    }
}