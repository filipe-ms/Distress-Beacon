// draw_utils.c
#include "draw_utils.h"
#include "common.h"

void DrawOutlinedText(const char* text, int pos_x, int pos_y, int font_size, Color front_color, Color back_color) {
	DrawText(text, pos_x - 1, pos_y - 1, font_size, back_color);
	DrawText(text, pos_x + 1, pos_y + 1, font_size, front_color);
}

void DrawCenteredOutlinedText(const char* text, int pos_x, int pos_y, int font_size, Color front_color, Color back_color) {
    int text_width = MeasureText(text, font_size);
    int text_height = font_size;

    int x = pos_x - text_width / 2;
    int y = pos_y - text_height / 2;

    DrawOutlinedText(text, x, y, font_size, front_color, back_color);
}

void DrawCenteredText(const char* text, int pos_x, int pos_y, int font_size, Color text_color) {
	int text_center = MeasureText(text, font_size)/2;
	DrawText(text, pos_x - text_center, pos_y, font_size, text_color);
}

void DrawCenteredMultilineText(const char* text, float pos_x, float pos_y, int fontSize, Color color) {
    const char* str = text;
    float lineHeight = (float)fontSize + 4.0f;

    int numLines = 1;
    for (const char* s = text; *s; s++) {
        if (*s == '\n') numLines++;
    }

    float totalHeight = numLines * lineHeight;
    float start_y = pos_y - totalHeight / 2.0f;

    while (*str) {
        int len = 0;
        while (str[len] != '\n' && str[len] != '\0') {
            len++;
        }

        if (len > 0) {
            const char* line = TextSubtext(str, 0, len);
            float lineWidth = (float)MeasureText(line, fontSize);
            float x = pos_x - lineWidth / 2.0f;
            DrawText(line, (int)x, (int)start_y, fontSize, color);
        }

        str += len;
        if (*str == '\n') {
            str++;
            start_y += lineHeight;
        }
    }
}

void DrawCenteredRectangle(int pos_x, int pos_y, int width_x, int width_y, Color color) {
	int half_width = width_x / 2;
	DrawRectangle(pos_x - half_width, pos_y, width_x, width_y, color);
}

void DrawPixelBorder(int pos_x, int pos_y, int width, int height, int border_width, Color color) {
    Vector2 left_border_pos = { 0 };
}