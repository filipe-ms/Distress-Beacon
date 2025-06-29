#pragma once

#include "raylib.h"

typedef enum {
	CONTAINER_BAR_NONE = -1,
    CONTAINER_BAR_Z_GRAY,
    CONTAINER_BAR_Z_BLACK,
    CONTAINER_BAR_Z_COPPER,

    CONTAINER_BAR_HEX_WHITE,
    CONTAINER_BAR_HEX_BLACK,
    CONTAINER_BAR_HEX_COPPER,

    CONTAINER_BAR_ROUNDED_GRAY,
    CONTAINER_BAR_ROUNDED_BLACK,
    CONTAINER_BAR_ROUNDED_COPPER,

    CONTAINER_BAR_CAPSULE_WHITE,
    CONTAINER_BAR_CAPSULE_BLACK,
    CONTAINER_BAR_CAPSULE_COPPER,

    CONTAINER_BAR_COUNT
} ProgressBarContainerType;

typedef enum {
	PROGRESS_BAR_Z_EMPTY,
	PROGRESS_BAR_Z_RED,
	PROGRESS_BAR_Z_YELLOW,
	PROGRESS_BAR_Z_BLUE,
	PROGRESS_BAR_Z_GREEN,

	PROGRESS_BAR_HEX_EMPTY,
	PROGRESS_BAR_HEX_RED,
	PROGRESS_BAR_HEX_YELLOW,
	PROGRESS_BAR_HEX_BLUE,
	PROGRESS_BAR_HEX_GREEN,

	PROGRESS_BAR_ROUNDED_EMPTY,
	PROGRESS_BAR_ROUNDED_RED,
	PROGRESS_BAR_ROUNDED_YELLOW,
	PROGRESS_BAR_ROUNDED_BLUE,
	PROGRESS_BAR_ROUNDED_GREEN,

	PROGRESS_BAR_CAPSULE_EMPTY,
	PROGRESS_BAR_CAPSULE_RED,
	PROGRESS_BAR_CAPSULE_YELLOW,
	PROGRESS_BAR_CAPSULE_BLUE,
	PROGRESS_BAR_CAPSULE_GREEN,

	PROGRESS_BAR_ROUNDED_EMPTY_WIDE,
	PROGRESS_BAR_ROUNDED_BLUE_WIDE,

	PROGRESS_BAR_COUNT
} ProgressBarType;

void DrawContainerBar(ProgressBarContainerType type, int pos_x, int pos_y, float scale, float alpha);
void DrawProgressBar(ProgressBarType type, ProgressBarContainerType container, int pos_x, int pos_y, float scale, float percentage, float alpha);
void DrawProgressBarEx(ProgressBarType type, int pos_x, int pos_y, int width, int height, float fill, float alpha);
void DrawProgressBarRec(ProgressBarType type, Rectangle rec, float fill, float alpha);