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

// Sound Effects
Sound* PlaySoundFx(Sound* sound);
Sound* PlaySoundFxPitch(Sound* sound, float pitch);
Sound* PlaySoundFxRandomPitch(Sound* sound, float minPitch, float maxPitch);
Sound* PlaySoundFxWithVolumeAndRandomPitch(Sound* sound, float volume, float minPitch, float maxPitch);

extern Sound sound1;
extern Sound sound5;
extern Sound sound6;
extern Sound sound11;
extern Sound sound12;
extern Sound sound15;

// Update
void PlayMusic(Scene scene);

void UpdateAudio(void);
void UnloadMusics(void);