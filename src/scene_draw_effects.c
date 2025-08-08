// scene_draw_utils.c
#include "scene_draw_effects.h"
#include "common.h"
#include "raymath.h"

typedef struct ScreenPulseEffect {
	// Internal
	Rectangle area;
	float elapsed_time;
	float current_alpha;

	// User set	
	float pulse_duration;
	float min_rect_alpha;
	float max_rect_alpha;
	Color tint;
} ScreenPulseEffect;

typedef struct ScreenFadeInEffect {
	// Internal
	Rectangle area;
	float elapsed_time;
	float starting_alpha;

	// User set
	float total_duration;
	Color tint;
	float current_alpha;
} ScreenFadeInEffect;

typedef struct ScreenFadeOutEffect {
	// Internal
	Rectangle area;
	float elapsed_time;
	float current_alpha;
	
	// User set
	float total_duration;
	Color tint;
	float starting_alpha;
} ScreenFadeOutEffect;

static ScreenPulseEffect pulse_effect;
static ScreenFadeInEffect fade_in_effect;
static ScreenFadeOutEffect fade_out_effect;
static bool transition_check;

static CurrentScreenEffect current_effect; // Usado para efeitos compostos

void InitPulseEffect(float pulse_duration, Color tint, float min_rect_alpha, float max_rect_alpha) {
	pulse_effect.area = (Rectangle){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	pulse_effect.elapsed_time = 0.0f;
	pulse_effect.pulse_duration = pulse_duration;
	
	pulse_effect.min_rect_alpha = min_rect_alpha;
	pulse_effect.max_rect_alpha = max_rect_alpha;
	pulse_effect.current_alpha = min_rect_alpha;
	pulse_effect.tint = tint;
	transition_check = false;
}

void UpdateAndDrawPulseEffect(void) {
	pulse_effect.elapsed_time += GetFrameTime();
	float progress = fmod(pulse_effect.elapsed_time / pulse_effect.pulse_duration, 1.0f);
	float wave = sinf(progress * PI);
	pulse_effect.current_alpha = Lerp(pulse_effect.min_rect_alpha, pulse_effect.max_rect_alpha, wave);

	Color color = Fade(pulse_effect.tint, pulse_effect.current_alpha);
	DrawRectangleRec(pulse_effect.area, color);
}

void InitFadeInEffect(float total_duration, Color tint, float starting_rect_alpha) {
	fade_in_effect.area = (Rectangle){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	fade_in_effect.elapsed_time = 0.0f;
	fade_in_effect.total_duration = total_duration;
	fade_in_effect.tint = tint;
	fade_in_effect.starting_alpha = starting_rect_alpha;
	fade_in_effect.current_alpha = starting_rect_alpha;
	current_effect = EFFECT_FADING_IN;
	transition_check = false;
}

bool UpdateAndDrawFadeInEffect(void) {
	fade_in_effect.elapsed_time += GetFrameTime();
	current_effect = EFFECT_FADING_IN;

	if (fade_in_effect.elapsed_time >= fade_in_effect.total_duration) {
		fade_in_effect.current_alpha = 0.0f;
		current_effect = EFFECT_IDLE;
		return true;
	}

	float progress = fade_in_effect.elapsed_time / fade_in_effect.total_duration;
	progress = Clamp(progress, 0.0f, 1.0f);
	fade_in_effect.current_alpha = Lerp(fade_in_effect.starting_alpha, 0.0f, progress);
	Color color = Fade(fade_in_effect.tint, fade_in_effect.current_alpha);
	DrawRectangleRec(fade_in_effect.area, color);
	return false;
}

void InitFadeOutEffect(float total_duration, Color tint, float starting_alpha) {
	fade_out_effect.area = (Rectangle){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	fade_out_effect.elapsed_time = 0.0f;
	fade_out_effect.total_duration = total_duration;
	fade_out_effect.tint = tint;
	fade_out_effect.starting_alpha = starting_alpha;
	fade_out_effect.current_alpha = starting_alpha;
	current_effect = EFFECT_FADING_OUT;
	transition_check = false;
}

bool UpdateAndDrawFadeOutEffect(void) {
	fade_out_effect.elapsed_time += GetFrameTime();
	current_effect = EFFECT_FADING_OUT;

	if (fade_out_effect.elapsed_time >= fade_out_effect.total_duration) {
		fade_out_effect.current_alpha = 1.0f;
		DrawRectangleRec(fade_out_effect.area, Fade(fade_out_effect.tint, 1.0f));
		current_effect = EFFECT_IDLE;
		return true;
	}

	float progress = fade_out_effect.elapsed_time / fade_out_effect.total_duration;
	progress = Clamp(progress, 0.0f, 1.0f);
	fade_out_effect.current_alpha = Lerp(fade_out_effect.starting_alpha, 1.0f, progress);
	Color color = Fade(fade_out_effect.tint, fade_out_effect.current_alpha);
	DrawRectangleRec(fade_out_effect.area, color);
	return false;
}

float GetCurrentScreenEffectAlpha(void) {
	if (current_effect == EFFECT_FADING_IN) {
		return fade_in_effect.current_alpha;
	}
	else if (current_effect == EFFECT_PULSING) {
		return pulse_effect.current_alpha;
	}
	else if (current_effect == EFFECT_FADING_OUT) {
		return fade_out_effect.current_alpha;
	}
	return 0.0f;
}

void InitFadeInThenPulse(float fade_in_duration, Color tint, float fade_in_starting_alpha, float pulse_duration, float pulse_min_alpha, float pulse_max_alpha) {
	InitFadeInEffect(fade_in_duration, tint, fade_in_starting_alpha);
	InitPulseEffect(pulse_duration, tint, pulse_min_alpha, pulse_max_alpha);
	transition_check = true;

	current_effect = EFFECT_FADING_IN;
}

void UpdateAndDrawScreenEffects(void) {
	if (current_effect == EFFECT_FADING_IN) {
		if (UpdateAndDrawFadeInEffect()) {
			if (transition_check == true) {
				current_effect = EFFECT_PULSING;
				transition_check = false;
			}
		}
	}
	else if (current_effect == EFFECT_PULSING) {
		UpdateAndDrawPulseEffect();
	}
	else if (current_effect == EFFECT_FADING_OUT) {
		if (UpdateAndDrawFadeOutEffect()) {
			current_effect = EFFECT_IDLE;
		}
	}
}

int GetCurrentScreenEffectType(void) {
	return current_effect;
}