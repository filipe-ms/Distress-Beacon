// general_utils.c
#include "general_utils.h"
#include "common.h"
#include "raymath.h"
#include <stdio.h>

float ClampWithFlagsF(float value, float min, float max, bool* has_reached_min, bool* has_reached_max) {
    if (value < min) {
        value = min;
        
        if (has_reached_min) {
            *has_reached_min = true;
        }
        
        if (has_reached_max) {
            *has_reached_max = false;
        }
    } else if (value > max) {
        value = max;

        if (has_reached_min) {
            *has_reached_min = false;
        }

        if (has_reached_max) {
            *has_reached_max = true;
        }
    } else {
        if (has_reached_min)
            *has_reached_min = false;

        if (has_reached_max)
            *has_reached_max = false;
    }

    return value;
}

int LerpInt(int start, int end, float elapsed_time, float duration) {
    if (duration <= 0) return end;
    float t = elapsed_time / duration;
    if (t > 1.0f) t = 1.0f;
    return start + (int)(t * (end - start));
}

Color LerpColor(Color start, Color end, float factor)
{
    return (Color) {
        .r = Lerp((float)start.r / 255.0f, (float)end.r / 255.0f, factor) * 255.0f,
        .g = Lerp((float)start.g / 255.0f, (float)end.g / 255.0f, factor) * 255.0f,
        .b = Lerp((float)start.b / 255.0f, (float)end.b / 255.0f, factor) * 255.0f,
        .a = Lerp((float)start.a / 255.0f, (float)end.a / 255.0f, factor) * 255.0f,
    };
}

Vector2 Vector2DivideScalarF(Vector2 vector, float scalar) {
    return (Vector2) { vector.x / scalar, vector.y / scalar };
}

Vector2 Vector2AddScalarF(Vector2 vector, float scalar) {
    return (Vector2) { vector.x + scalar, vector.y + scalar };
}

Vector2 Vector2MultiplyScalarF(Vector2 vector, float scalar) {
    return (Vector2) { vector.x * scalar, vector.y * scalar };
}

Vector2 Vector2SubtractScalarF(Vector2 vector, float scalar) {
    return (Vector2) { vector.x - scalar, vector.y - scalar };
}

bool IsWithinScreenBounds(Vector2 position, Vector2 size) {
    // Adds 10 more pixels just to be extra sure it is true when it is being rendered.
    static const Vector2 vector_ten = { 10, 10 };
    
    Vector2 half_size = Vector2AddScalarF(Vector2DivideScalarF(size, 2.0f), 10);
    
    return position.y < -half_size.y ||
        position.y > (GAME_SCREEN_HEIGHT + half_size.y) ||
        position.x < -half_size.x ||
        position.x > (GAME_SCREEN_END + half_size.y);
}

float WrapAngle(float angle) {
    return fmodf(angle + PI, 2*PI) - PI;
}

float CalculateFacingAngle(Vector2 from, Vector2 to)
{
    Vector2 vect = Vector2Subtract(from, to);
    return atan2f(vect.y, vect.x);
}