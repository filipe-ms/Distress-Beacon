// draw_utils.c
#include "draw_utils.h"

void DrawOutlinedText(const char* text, int pos_x, int pos_y, int font_size, Color front_color, Color back_color) {
	DrawText(text, pos_x - 1, pos_y - 1, font_size, back_color);
	DrawText(text, pos_x + 1, pos_y + 1, font_size, front_color);
}