// screen_transition.h
#pragma once

#include "raylib.h"

typedef struct TransitionEffect {
	float duration;
	float init_alpha;
	float alpha_acceleration;
	bool is_active;
} TransitionEffect;

TransitionEffect IniTransitionEffect(float duration, float init_alpha, float alpha_acceleration);
void TransitionFadeIn(TransitionEffect* effect);
void TransitionFadeOut(TransitionEffect* effect);