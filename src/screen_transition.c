// screen_transition.c
#include "screen_transition.h"

TransitionEffect IniTransitionEffect(float duration, float init_alpha, float alpha_acceleration) {
	TransitionEffect effect;
	effect.duration = duration;
	effect.init_alpha = init_alpha;
	effect.alpha_acceleration = alpha_acceleration;
	effect.is_active = false;
}

float GetAlpha(TransitionEffect* effect) {
	// Retorna 0 se o efeito não estiver ativo ou se o ponteiro for nulo
	// Assim a tela não fica invisível
	if (!effect || !effect->is_active) return 1.0f;
	return effect->init_alpha;
}

void TransitionFadeIn(TransitionEffect* effect) {
	if (!effect || effect->is_active) return;
	effect->is_active = true;
	effect->init_alpha += effect->alpha_acceleration * GetFrameTime();
}