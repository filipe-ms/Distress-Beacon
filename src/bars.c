#include "bars.h"
#include "texture_manager.h"

static Rectangle GetContainerSourceRect(ProgressBarContainerType type) {
	switch (type) {
	case CONTAINER_BAR_Z_GRAY:			return (Rectangle) { 0, 0,  48, 11 };
	case CONTAINER_BAR_Z_BLACK:			return (Rectangle) { 0, 12, 48, 11 };
	case CONTAINER_BAR_Z_COPPER:		return (Rectangle) { 0, 23, 48, 11 };

	case CONTAINER_BAR_HEX_WHITE:		return (Rectangle) { 0, 34, 48, 11 };
	case CONTAINER_BAR_HEX_BLACK:		return (Rectangle) { 0, 45, 48, 11 };
	case CONTAINER_BAR_HEX_COPPER:		return (Rectangle) { 0, 56, 48, 11 };

	case CONTAINER_BAR_ROUNDED_GRAY:	return (Rectangle) { 0, 67, 48, 11 };
	case CONTAINER_BAR_ROUNDED_BLACK:	return (Rectangle) { 0, 78, 48, 11 };
	case CONTAINER_BAR_ROUNDED_COPPER:	return (Rectangle) { 0, 89, 48, 11 };

	case CONTAINER_BAR_CAPSULE_WHITE:	return (Rectangle) { 0, 100, 48, 11 };
	case CONTAINER_BAR_CAPSULE_BLACK:	return (Rectangle) { 0, 111, 48, 11 };
	case CONTAINER_BAR_CAPSULE_COPPER:	return (Rectangle) { 0, 122, 48, 11 };
	
	default: return (Rectangle) { 0, 0, 48, 11 };
	}
}

static Rectangle GetEmptyBarSource(ProgressBarType type) {
	int bar_type = type / 5;
	switch (bar_type) {
	case 0:	return (Rectangle) { 90,  0, 42, 5 };
	case 1:	return (Rectangle) { 90,  5, 34, 5 };
	case 2:	return (Rectangle) { 90, 10, 32, 5 };
	case 3:	return (Rectangle) { 90, 15, 34, 5 };
	
	default: return (Rectangle) { 90, 0, 42, 5 };
	}
}

static Rectangle GetProgressBarRectangle(ProgressBarType type) {
	switch (type) {
	case PROGRESS_BAR_Z_EMPTY:			return (Rectangle) { 90, 0,  42, 5 };
	case PROGRESS_BAR_Z_RED:			return (Rectangle) { 48, 0,  42, 5 };
	case PROGRESS_BAR_Z_YELLOW:			return (Rectangle) { 48, 5,  42, 5 };
	case PROGRESS_BAR_Z_BLUE:			return (Rectangle) { 48, 10, 42, 5 };
	case PROGRESS_BAR_Z_GREEN:			return (Rectangle) { 48, 15, 42, 5 };

	case PROGRESS_BAR_HEX_EMPTY:		return (Rectangle) { 90, 5, 34, 5 };
	case PROGRESS_BAR_HEX_RED:			return (Rectangle) { 48, 20, 34, 5 };
	case PROGRESS_BAR_HEX_YELLOW:		return (Rectangle) { 48, 25, 34, 5 };
	case PROGRESS_BAR_HEX_BLUE:			return (Rectangle) { 48, 30, 34, 5 };
	case PROGRESS_BAR_HEX_GREEN:		return (Rectangle) { 48, 35, 34, 5 };

	case PROGRESS_BAR_ROUNDED_EMPTY:	return (Rectangle) { 90, 10, 32, 5 };
	case PROGRESS_BAR_ROUNDED_RED:		return (Rectangle) { 48, 40, 32, 5 };
	case PROGRESS_BAR_ROUNDED_YELLOW:	return (Rectangle) { 48, 45, 32, 5 };
	case PROGRESS_BAR_ROUNDED_BLUE:		return (Rectangle) { 48, 50, 32, 5 };
	case PROGRESS_BAR_ROUNDED_GREEN:	return (Rectangle) { 48, 55, 32, 5 };

	case PROGRESS_BAR_CAPSULE_EMPTY:	return (Rectangle) { 90, 15, 34, 5 };
	case PROGRESS_BAR_CAPSULE_RED:		return (Rectangle) { 48, 60, 34, 5 };
	case PROGRESS_BAR_CAPSULE_YELLOW:	return (Rectangle) { 48, 64, 34, 5 };
	case PROGRESS_BAR_CAPSULE_BLUE:		return (Rectangle) { 48, 68, 34, 5 };
	case PROGRESS_BAR_CAPSULE_GREEN:	return (Rectangle) { 48, 72, 34, 5 };

	case PROGRESS_BAR_ROUNDED_EMPTY_WIDE:	return (Rectangle) { 48, 76, 83, 5 };
	case PROGRESS_BAR_ROUNDED_BLUE_WIDE:	return (Rectangle) { 48, 81, 83, 5 };

	default: return (Rectangle) { 48, 0, 42, 5 };
	}
}

static Rectangle GetDestRec(int pos_x, int pos_y, int width, int height) {
	return (Rectangle) { (float)pos_x, (float)pos_y, (float)width, (float)height };
}

void DrawContainerBar(ProgressBarContainerType type, int pos_x, int pos_y, float scale, float alpha) {
	Rectangle source = GetContainerSourceRect(type);
	Rectangle dest = GetDestRec(pos_x, pos_y, source.width * scale, source.height * scale);
	DrawTexturePro(texture_progress_bars, source, dest, (Vector2) { 0, 0 }, 0.0f, Fade(WHITE, alpha));
}

void DrawProgressBar(ProgressBarType type, ProgressBarContainerType container, int pos_x, int pos_y, float scale, float percentage, float alpha) {
	Rectangle empty_source = GetEmptyBarSource(type);
	Rectangle dest = GetDestRec(pos_x, pos_y, empty_source.width * scale, empty_source.height * scale);

	if (container/3 == 0) dest.x += 3 * scale;
	else if (container/3 == 1) dest.x += 4 * scale;
	else if (container/3 == 2) dest.x += 3 * scale;

	if (container < 5) dest.y += 3 * scale;
	else dest.y += 1 * scale;

	DrawTexturePro(texture_progress_bars, empty_source, dest, (Vector2) { 0, 0 }, 0.0f, Fade(WHITE, alpha*0.6f));

	Rectangle fill_source = GetProgressBarRectangle(type);
	float original_width = fill_source.width;

	fill_source.width = original_width * percentage;

	Rectangle fill_dest = dest;
	fill_dest.width = dest.width * percentage;

	DrawTexturePro(texture_progress_bars, fill_source, fill_dest, (Vector2) { 0, 0 }, 0.0f, Fade(WHITE, alpha));
}

void DrawProgressBarEx(ProgressBarType type, int pos_x, int pos_y, int width, int height, float fill, float alpha) {
	Rectangle source = GetProgressBarRectangle(type);
	source.width = source.width * fill;

	Rectangle dest = GetDestRec(pos_x, pos_y, width, height);
	DrawTexturePro(texture_progress_bars, source, dest, (Vector2) { 0, 0 }, 0.0f, Fade(WHITE, alpha));
}

void DrawProgressBarRec(ProgressBarType type, Rectangle rec, float fill, float alpha) {
	Rectangle source = GetProgressBarRectangle(type);
	source.width = source.width * fill;

	Rectangle dest = rec;
	dest.width = rec.width * fill;

	DrawTexturePro(texture_progress_bars, source, dest, (Vector2) { 0, 0 }, 0.0f, Fade(WHITE, alpha));
}

