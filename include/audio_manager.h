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
extern Sound sound2;
extern Sound sound4;
extern Sound sound5;
extern Sound sound6;
extern Sound sound7;
extern Sound sound8;
extern Sound sound9;
extern Sound sound10;
extern Sound sound11;
extern Sound sound12;
extern Sound sound14;
extern Sound sound15;
extern Sound sound16;
extern Sound sound17;
extern Sound sound18;
extern Sound sound19;
extern Sound sound20;
extern Sound sound21;
extern Sound sound22;
extern Sound sound23;
extern Sound sound24;
extern Sound sound25;
extern Sound sound26;
extern Sound sound27;

// Update
void PlayMusic(Scene scene);

void UpdateAudio(void);
void UnloadMusics(void);

void PlayExplosionSound(void);