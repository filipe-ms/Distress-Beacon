// left_ui.c
#include "left_ui.h"

#include "common.h"
#include "draw_utils.h"
#include "ship_references.h"
#include "pilot.h"
#include "player.h"

#include <string.h>

char top_pilot_text_buffer[256] = { 0 };
char bottom_pilot_text_buffer[256] = { 0 };

#pragma region DRAW BG AND BG BORDERS
static void DrawLeftUIBackground() {
	DrawRectangle(0, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);
}

static void DrawLeftUIBorders() {
	int border_width = 5;

	int border_thick = 5;
	int margin_offset = border_thick * 4;

	DrawCenteredPixelBorder(
		UI_LEFT_CENTER, 
		SCREEN_HEIGHT / 2, 
		UI_WIDTH - margin_offset, 
		SCREEN_HEIGHT - margin_offset, 
		border_thick, 
		WHITE);
}

static void DrawBGandBorders() {
	DrawLeftUIBackground();
	DrawLeftUIBorders();
}

#pragma endregion

#pragma region DRAW PILOT

static void DrawTopPilotName() {
	DrawCenteredText(
		GetPilotName(GetTopPilotId()),
		UI_LEFT_CENTER,
		(int)(SCREEN_HEIGHT * 0.05f),
		40,
		GOLD);
}

static void DrawBottomPilotName() {
	DrawCenteredText(
		GetPilotName(GetBottomPilotId()),
		UI_LEFT_CENTER,
		SCREEN_HEIGHT * 0.5f,
		40,
		GOLD
	);
}

static void DrawPilotHead() {
	DrawPilot();
}

static void DrawPilotPanel() {
	DrawTopPilotName();
	DrawBottomPilotName();
	DrawPilotHead();
}



#pragma endregion

#pragma region DRAW TEXT

static void DrawTopTextBorder() {
	int border_thick = 5;
	int margin_offset = border_thick * 4;
	DrawCenteredPixelBorder(
		UI_LEFT_CENTER, 
		SCREEN_HEIGHT * 0.35, 
		UI_WIDTH - border_thick * 16, 
		SCREEN_HEIGHT * 0.17, 
		border_thick, 
		WHITE);
}

static void DrawTopPilotText() {
	DrawCenteredMultilineText(
		top_pilot_text_buffer,
		UI_LEFT_CENTER, 
		SCREEN_HEIGHT * 0.35,
		30, 
		WHITE);
}

static void DrawBottomTextBorder() {
	int border_thick = 5;
	int margin_offset = border_thick * 4;
	DrawCenteredPixelBorder(
		UI_LEFT_CENTER,
		SCREEN_HEIGHT * 0.8,
		UI_WIDTH - border_thick * 16,
		SCREEN_HEIGHT * 0.17,
		border_thick,
		WHITE);
}

static void DrawBottomPilotText() {
	DrawCenteredMultilineText(
		bottom_pilot_text_buffer,
		UI_LEFT_CENTER,
		SCREEN_HEIGHT * 0.8,
		30,
		WHITE);
}

static void DrawTopPilotTextPanel() {
	DrawTopPilotText();
	if(top_pilot_text_buffer[0] != '\0') DrawTopTextBorder();
}

static void DrawBottomPilotTextPanel() {
	DrawBottomPilotText();
	if (bottom_pilot_text_buffer[0] != '\0') DrawBottomTextBorder();

}

#pragma endregion

void SetTopPilotText(const char* text) {
	if (text != NULL) {
		strncpy(top_pilot_text_buffer, text, sizeof(top_pilot_text_buffer) - 1);
		top_pilot_text_buffer[sizeof(top_pilot_text_buffer) - 1] = '\0';
	}
	else {
		top_pilot_text_buffer[0] = '\0';
	}
}

void ClearTopPilotText(void) {
	top_pilot_text_buffer[0] = '\0';
}

void SetBottomPilotText(const char* text) {
	if (text != NULL) {
		strncpy(bottom_pilot_text_buffer, text, sizeof(bottom_pilot_text_buffer) - 1);
		bottom_pilot_text_buffer[sizeof(bottom_pilot_text_buffer) - 1] = '\0';
	}
	else {
		bottom_pilot_text_buffer[0] = '\0';
	}
}

void ClearBottomPilotText(void) {
	bottom_pilot_text_buffer[0] = '\0';
}

void InitLeftUIPanel(void) {
	SetTopPilotDefault();
	SetBottomPilotDefault(NONE);
	TriggerTopPilotAnimation(5.0f);
}

void DrawLeftUIPanel(void) {
	DrawBGandBorders();
	DrawPilotPanel();
	DrawTopPilotTextPanel();
	DrawBottomPilotTextPanel();
}