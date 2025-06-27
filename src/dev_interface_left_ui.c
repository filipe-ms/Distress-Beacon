// dev_interface_left_ui.c
#include "dev_interface_left_ui.h"

#include "pilot.h"
#include "left_ui.h"

#pragma region TOP PILOT
void UI_TriggerTopPilotSpeech(float duration) {
	TriggerTopPilotAnimation(duration);
}

void UI_SetTopPilotText(const char* text) {
	SetTopPilotText(text);
}

void UI_ClearTopPilotText(void) {
	ClearTopPilotText();
}


// Combinação
void UI_TopPilotSpeechAct(const char* text, float duration) {
	SetTopPilotText(text);
	TriggerTopPilotAnimation(duration);
}

#pragma endregion

#pragma region BOTTOM PILOT

void UI_SetBottomPilot(int pilot_id) {
	SetBottomPilotDefault(pilot_id);
}

void UI_TriggerBottomPilotSpeech(float duration) {
	TriggerBottomPilotAnimation(duration);
}

void UI_SetBottomPilotText(const char* text) {
	SetBottomPilotText(text);
}

void UI_ClearBottomPilotText(void) {
	ClearBottomPilotText();
}

void UI_ClearBottomPilot(void) {
	ClearBottomPilot();
}

// Combinação
void UI_BottomPilotSpeechAct(const char* text, float duration) {
	SetBottomPilotText(text);
	TriggerBottomPilotAnimation(duration);
}

#pragma endregion