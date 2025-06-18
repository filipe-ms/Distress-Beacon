// background.h
#pragma once

#include "raylib.h"

typedef struct Background {
    Texture texture;
    float position_x;
    float position_y;
    Color color;
    float alpha;
} Background;