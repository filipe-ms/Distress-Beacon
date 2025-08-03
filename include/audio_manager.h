// audio_manager.h
#pragma once

#include "raylib.h"
#include "scene_manager.h"

// Load/Unload
void LoadAudioResources(void);
void UnloadAudioResources(void);

// Pilot
void TriggerPilotSpeech(int ship, float duration);

// SFX
typedef enum {
    NO_SFX
} SoundEffect;

void PlaySfx(SoundEffect sound);

// Update
void PlayMusic(Scene scene);

void UpdateAudio(void);
void UnloadMusics(void);