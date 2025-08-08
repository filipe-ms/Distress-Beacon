// scene_draw_utils.h
#pragma once

#include "raylib.h"

typedef enum CurrentScreenEffect {
	EFFECT_IDLE,
	EFFECT_FADING_IN,
	EFFECT_PULSING,
	EFFECT_FADING_OUT
} CurrentScreenEffect;

void InitPulseEffect(float pulse_duration, Color tint, float min_alpha, float max_alpha);
void UpdateAndDrawPulseEffect(void);

void InitFadeInEffect(float total_duration, Color tint, float starting_alpha);
bool UpdateAndDrawFadeInEffect(void);

void InitFadeOutEffect(float total_duration, Color tint, float starting_alpha);
bool UpdateAndDrawFadeOutEffect(void);

void InitFadeInThenPulse(float fade_in_duration, Color tint, float fade_in_starting_alpha, float pulse_duration, float pulse_min_alpha, float pulse_max_alpha);

void UpdateAndDrawScreenEffects(void);

float GetCurrentScreenEffectAlpha(void);
int GetCurrentScreenEffectType(void);