#pragma once

#include "raylib.h"
#include "common.h"
#include "raymath.h"

#include <stdio.h>

float ClampWithFlagsF(float value, float min, float max, bool* has_reached_min, bool* has_reached_max);
int LerpInt(int start, int end, float elapsed_time, float duration);

Vector2 Vector2DivideScalarF(Vector2 vector, float scalar);
Vector2 Vector2AddScalarF(Vector2 vector, float scalar);
Vector2 Vector2MultiplyScalarF(Vector2 vector, float scalar);
Vector2 Vector2SubtractScalarF(Vector2 vector, float scalar);
bool IsWithinScreenBounds(Vector2 position, Vector2 size);

float WrapAngle(float angle);
float CalculateFacingAngle(Vector2 from, Vector2 to);