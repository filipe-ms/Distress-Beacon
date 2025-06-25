// draw_utils.h
#pragma once

#include "raylib.h"

void DrawOutlinedText(const char* text, int pos_x, int pos_y, int font_size, Color front_color, Color back_color);
void DrawCenteredOutlinedText(const char* text, int pos_x, int pos_y, int font_size, Color front_color, Color back_color);
void DrawCenteredText(const char* text, int pos_x, int pos_y, int font_size, Color text_color);
void DrawCenteredMultilineText(const char* text, float pos_x, float pos_y, int fontSize, Color color);
void DrawCenteredRectangle(int pos_x, int pos_y, int width_x, int width_y, Color color);

