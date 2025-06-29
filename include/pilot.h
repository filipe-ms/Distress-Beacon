#pragma once

#include "raylib.h"

void SetTopPilot(int pilot_id, int pos_x, int pos_y, float scale, Color color);
void SetTopPilotDefault(void);
void TriggerTopPilotAnimation(float duration);
int GetTopPilotId(void);

void SetBottomPilot(int pilot_id, int pos_x, int pos_y, float scale, Color color);
void SetBottomPilotDefault(int pilot_id);
void TriggerBottomPilotAnimation(float duration);
int GetBottomPilotId(void);
void ClearBottomPilot(void);

void UpdatePilot(void);
void DrawPilot(void);