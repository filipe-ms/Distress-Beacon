// audio_manager.h
#pragma once

#include "raylib.h"

// Load/Unload
void LoadAudioResources(void);
void UnloadAudioResources(void);

// Pilot
void TriggerPilotSpeech(int ship, float duration);

// SFX
typedef enum {
    NONE
} SoundEffect;

void PlaySfx(SoundEffect sound);

// Update
void UpdateAudio(void);