// dev_interface_left_ui.h
#pragma once

// TOP PILOT
void UI_TriggerTopPilotSpeech(float duration);
void UI_SetTopPilotText(const char* text);
void UI_ClearTopPilotText(void);
void UI_TopPilotSpeechAct(const char* text, float duration);

// BOTTOM PILOT
void UI_SetBottomPilot(int pilot_id);
void UI_TriggerBottomPilotSpeech(float duration);
void UI_SetBottomPilotText(const char* text);
void UI_ClearBottomPilotText(void);
void UI_ClearBottomPilot(void);
void UI_BottomPilotSpeechAct(const char* text, float duration);