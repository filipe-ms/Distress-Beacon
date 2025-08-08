// draw_utils.h
#pragma once

#include "raylib.h"

// Text
void DrawCenteredText(const char* text, int pos_x, int pos_y, int font_size, Color text_color);
void DrawCenteredMultilineText(const char* text, float pos_x, float pos_y, int fontSize, Color color);
void DrawOutlinedText(const char* text, int pos_x, int pos_y, int font_size, Color front_color, Color back_color);
void DrawCenteredOutlinedText(const char* text, int pos_x, int pos_y, int font_size, Color front_color, Color back_color);

// Borders
void DrawPixelBorder(int pos_x, int pos_y, int width, int height, int border, Color color);
void DrawCenteredPixelBorder(int pos_x, int pos_y, int width, int height, int border, Color color);

// Rectangles
void DrawCenteredRectangle(int pos_x, int pos_y, int width_x, int width_y, Color color);