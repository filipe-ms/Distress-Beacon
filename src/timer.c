// timer.c
#include "timer.h"

typedef struct Timer {
	float duration;
	float elapsed_time;
} Timer;

Timer timer;

void InitTimer(float duration) {
	timer.duration = duration;
	timer.elapsed_time = 0.0f;
}

bool UpdateTimer(void) {
	timer.elapsed_time += GetFrameTime();
	return timer.elapsed_time > timer.duration;
}

float GetElapsedTime(void) {
	return timer.elapsed_time;
}