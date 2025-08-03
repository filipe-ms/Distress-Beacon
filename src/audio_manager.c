// audio_manager.c
#include "audio_manager.h"
#include "list.h"
#include "common.h"
#include "player.h"
#include "scene_manager.h"

#define SPEECH_POOL_SIZE 8

typedef enum {
    AUDIO_TYPE_SFX,
    AUDIO_TYPE_SPEECH,
} AudioType;

typedef struct {
    Sound* sound;
    AudioType type;
} ActiveSound;

typedef struct SpeechState {
    int active_ship_id;
    float timer;
    float elapsed_time;
} SpeechState;

typedef struct PilotAudio {
    Wave wave[2];
    Sound sound_pool[2][SPEECH_POOL_SIZE];
    int sound_pool_index[2];
} PilotAudio;

static PilotAudio nebula_speech;
static PilotAudio orion_speech;
static PilotAudio puddle_speech;
static PilotAudio void_speech;
static PilotAudio aurea_speech;

static SpeechState speech = { 0 };

static List* playing_sounds = NULL;

static Music main_music;
static Music in_game_music;
static Music ending_music;

static Music* playing_music;

// Declarando aqui em cima pra n�o me importar com a ordem que eu chamo embaixo
#pragma region ForwardDeclarations
static void ApplyFadeIn(Wave* wave, float fade_duration_ms);  // Para remover os clicks
static void ApplyFadeOut(Wave* wave, float fade_duration_ms); // Para remover os clicks
static Sound* GetPilotSound(int ship);
static void PlaySoundInternal(Sound* sound, AudioType type);
static bool IsSoundFinished(void* context, void* data);
static void UpdateAudioCleanup(void);
static void ResetSpeech(void);
#pragma endregion ForwardDeclarations

void TriggerPilotSpeech(int ship, float duration) {
    ResetSpeech();
    speech.active_ship_id = ship;
    speech.timer = duration;
}

static void UpdateSpeech(void) {
    if (speech.timer <= 0.0f) return;

    speech.timer -= GetFrameTime();
    if (speech.timer <= 0.0f) {
        ResetSpeech();
        return;
    }

    speech.elapsed_time += GetFrameTime();
    if (speech.elapsed_time >= 0.15f) {
        speech.elapsed_time = 0.0f;

        Sound* new_sound = GetPilotSound(speech.active_ship_id);
        PlaySoundInternal(new_sound, AUDIO_TYPE_SPEECH);
    }
}

static Sound* GetSoundEffect(SoundEffect sound) {
	switch (sound) {
	case NONE: return NULL;
	default: return NULL;
	}
}

void PlaySfx(SoundEffect sound) {
    PlaySoundInternal(GetSoundEffect(sound), AUDIO_TYPE_SFX);
}

void PlayMusic(Scene scene) {
    switch(scene) {
        case START:
        case SELECT_SHIP:
        case TUTORIAL:
        case CREDITS:
        case RANKING:
        case GAME_OVER:
        case WINNER:
        case ENTER_NAME:
        case EXIT:
            playing_music = &main_music;
            break;
        case ENDING_1:
            playing_music = &ending_music;
            break;
        case GAME:
            playing_music = &in_game_music;
            break;
    }

    if (!IsMusicStreamPlaying(*playing_music)) {
        PlayMusicStream(*playing_music);
        SetMusicVolume(*playing_music, 0.1);
        SeekMusicStream(*playing_music, 0);
        
        (*playing_music).looping = true;
    }
}

#pragma region LOAD/UNLOAD
void LoadAudioResources(void) {
    main_music = LoadMusicStream("music/Dani Stob - A Journey for Purpose - Loop.wav");
    in_game_music = LoadMusicStream("music/Dani Stob - Unstoppable - Loop.wav");
    ending_music = LoadMusicStream("music/Dani Stob - Beyond The Stars - Loop.wav");

    {
        nebula_speech.wave[0] = LoadWave("sound/speech/nebula/oboe1.wav");
        nebula_speech.wave[1] = LoadWave("sound/speech/nebula/oboe2.wav");

        ApplyFadeIn(&nebula_speech.wave[0], 25.0f);
        ApplyFadeIn(&nebula_speech.wave[1], 25.0f);

        ApplyFadeOut(&nebula_speech.wave[0], 50.0f);
        ApplyFadeOut(&nebula_speech.wave[1], 50.0f);

        nebula_speech.sound_pool_index[0] = 0;
        nebula_speech.sound_pool_index[1] = 0;
    }

    {
		orion_speech.wave[0] = LoadWave("sound/speech/orion/clarinet1.wav");
		orion_speech.wave[1] = LoadWave("sound/speech/orion/clarinet2.wav");

		ApplyFadeIn(&orion_speech.wave[0], 25.0f);
		ApplyFadeIn(&orion_speech.wave[1], 25.0f);

		ApplyFadeOut(&orion_speech.wave[0], 50.0f);
		ApplyFadeOut(&orion_speech.wave[1], 50.0f);

        orion_speech.sound_pool_index[0] = 0;
        orion_speech.sound_pool_index[1] = 0;
	}

    {
        puddle_speech.wave[0] = LoadWave("sound/speech/puddle/string_scratch1.wav");
        puddle_speech.wave[1] = LoadWave("sound/speech/puddle/string_scratch2.wav");

        ApplyFadeIn(&puddle_speech.wave[0], 25.0f);
        ApplyFadeIn(&puddle_speech.wave[1], 25.0f);

        ApplyFadeOut(&puddle_speech.wave[0], 50.0f);
        ApplyFadeOut(&puddle_speech.wave[1], 50.0f);

        puddle_speech.sound_pool_index[0] = 0;
        puddle_speech.sound_pool_index[1] = 0;
    }

    {
		void_speech.wave[0] = LoadWave("sound/speech/void/altosax1.wav");
		void_speech.wave[1] = LoadWave("sound/speech/void/altosax2.wav");

		ApplyFadeIn(&void_speech.wave[0], 25.0f);
		ApplyFadeIn(&void_speech.wave[1], 25.0f);

		ApplyFadeOut(&void_speech.wave[0], 50.0f);
		ApplyFadeOut(&void_speech.wave[1], 50.0f);

		void_speech.sound_pool_index[0] = 0;
		void_speech.sound_pool_index[1] = 0;
    }

    {
		aurea_speech.wave[0] = LoadWave("sound/speech/aurea/tuba1.wav");
		aurea_speech.wave[1] = LoadWave("sound/speech/aurea/tuba2.wav");
		ApplyFadeIn(&aurea_speech.wave[0], 25.0f);
		ApplyFadeIn(&aurea_speech.wave[1], 25.0f);
		ApplyFadeOut(&aurea_speech.wave[0], 50.0f);
		ApplyFadeOut(&aurea_speech.wave[1], 50.0f);
		aurea_speech.sound_pool_index[0] = 0;
		aurea_speech.sound_pool_index[1] = 0;
    }

    // Carrega cada nota no seu respectivo pool de sons
    // Pool � necess�rio para evitar clicks no audio
    for (int i = 0; i < SPEECH_POOL_SIZE; i++) {
        nebula_speech.sound_pool[0][i] = LoadSoundFromWave(nebula_speech.wave[0]);
        nebula_speech.sound_pool[1][i] = LoadSoundFromWave(nebula_speech.wave[1]);

		orion_speech.sound_pool[0][i] = LoadSoundFromWave(orion_speech.wave[0]);
		orion_speech.sound_pool[1][i] = LoadSoundFromWave(orion_speech.wave[1]);

		puddle_speech.sound_pool[0][i] = LoadSoundFromWave(puddle_speech.wave[0]);
		puddle_speech.sound_pool[1][i] = LoadSoundFromWave(puddle_speech.wave[1]);

		void_speech.sound_pool[0][i] = LoadSoundFromWave(void_speech.wave[0]);
		void_speech.sound_pool[1][i] = LoadSoundFromWave(void_speech.wave[1]);

		aurea_speech.sound_pool[0][i] = LoadSoundFromWave(aurea_speech.wave[0]);
		aurea_speech.sound_pool[1][i] = LoadSoundFromWave(aurea_speech.wave[1]);
    }

    // O Wave j� foi copiado para os Sounds.
    UnloadWave(nebula_speech.wave[0]);
    UnloadWave(nebula_speech.wave[1]);

	UnloadWave(orion_speech.wave[0]);
	UnloadWave(orion_speech.wave[1]);

	UnloadWave(puddle_speech.wave[0]);
	UnloadWave(puddle_speech.wave[1]);

	UnloadWave(void_speech.wave[0]);
	UnloadWave(void_speech.wave[1]);

	UnloadWave(aurea_speech.wave[0]);
	UnloadWave(aurea_speech.wave[1]);

    playing_sounds = List_Create(sizeof(ActiveSound));
}

void UnloadAudioResources(void) {
    for (int i = 0; i < SPEECH_POOL_SIZE; i++) {
        UnloadSound(nebula_speech.sound_pool[0][i]);
        UnloadSound(nebula_speech.sound_pool[1][i]);
		UnloadSound(orion_speech.sound_pool[0][i]);
		UnloadSound(orion_speech.sound_pool[1][i]);
		UnloadSound(puddle_speech.sound_pool[0][i]);
		UnloadSound(puddle_speech.sound_pool[1][i]);
		UnloadSound(void_speech.sound_pool[0][i]);
		UnloadSound(void_speech.sound_pool[1][i]);
		UnloadSound(aurea_speech.sound_pool[0][i]);
		UnloadSound(aurea_speech.sound_pool[1][i]);
    }
}

#pragma endregion LOAD/UNLOAD

#pragma region STATIC

static void ApplyFadeIn(Wave* wave, float fade_duration_ms) {
    if (!wave || !wave->data || fade_duration_ms <= 0) return;

    // Calcula quantos frames de amostra o fade deve durar.
    unsigned int fade_frame_count = (unsigned int)(((float)wave->sampleRate * fade_duration_ms) / 1000.0f);
    if (fade_frame_count == 0) return;
    if (fade_frame_count > wave->frameCount) fade_frame_count = wave->frameCount;

    // O n�mero total de amostras a serem processadas (frames * canais).
    unsigned int sample_count_to_fade = fade_frame_count * wave->channels;

    // Processa as amostras com base no tamanho (bits). 16-bit � o mais comum.
    if (wave->sampleSize == 16) {
        short* samples = (short*)wave->data;
        for (unsigned int i = 0; i < sample_count_to_fade; i++) {
            float multiplier = (float)i / (float)(sample_count_to_fade - 1);
            samples[i] = (short)((float)samples[i] * multiplier);
        }
    }
    // Suporte para float de 32-bit.
    else if (wave->sampleSize == 32) {
        float* samples = (float*)wave->data;
        for (unsigned int i = 0; i < sample_count_to_fade; i++) {
            float multiplier = (float)i / (float)(sample_count_to_fade - 1);
            samples[i] *= multiplier;
        }
    }
}

static void ApplyFadeOut(Wave* wave, float fade_duration_ms) {
    if (!wave || !wave->data || fade_duration_ms <= 0) return;

    unsigned int fade_frame_count = (unsigned int)(((float)wave->sampleRate * fade_duration_ms) / 1000.0f);
    if (fade_frame_count == 0) return;
    if (fade_frame_count > wave->frameCount) fade_frame_count = wave->frameCount;

    unsigned int fade_sample_count = fade_frame_count * wave->channels;
    unsigned int total_sample_count = wave->frameCount * wave->channels;
    unsigned int start_sample = total_sample_count - fade_sample_count;

    if (wave->sampleSize == 16) {
        short* samples = (short*)wave->data;
        for (unsigned int i = 0; i < fade_sample_count; i++) {
            float multiplier = 1.0f - ((float)i / (float)(fade_sample_count - 1));
            samples[start_sample + i] = (short)((float)samples[start_sample + i] * multiplier);
        }
    }
    else if (wave->sampleSize == 32) {
        float* samples = (float*)wave->data;
        for (unsigned int i = 0; i < fade_sample_count; i++) {
            float multiplier = 1.0f - ((float)i / (float)(fade_sample_count - 1));
            samples[start_sample + i] *= multiplier;
        }
    }
}

static void PlaySoundInternal(Sound* sound, AudioType type) {
    if (!sound || !playing_sounds) return;

    PlaySound(*sound);
    ActiveSound active_sound = { .sound = sound, .type = type };
    List_Add(playing_sounds, &active_sound);
}

static bool IsSoundFinished(void* context, void* data) {
    (void)context;
    ActiveSound* active_sound = (ActiveSound*)data;
    return !IsSoundPlaying(*(active_sound->sound));
}

static void UpdateAudioCleanup(void) {
    if (!playing_sounds) return;
    List_RemoveWithFn(playing_sounds, NULL, IsSoundFinished);
}

static void ResetSpeech(void) {
    speech.timer = 0.0f;
    speech.elapsed_time = 0.0f;
    speech.active_ship_id = -1;
}

static Sound* GetPilotSound(int ship) {
    switch (ship) {
    // Avan�a o �ndice do pool e retorna o ponteiro para o som
    case ORION: {
		int note_index = GetRandomValue(0, 1);
		int pool_idx = orion_speech.sound_pool_index[note_index];
		orion_speech.sound_pool_index[note_index] = (pool_idx + 1) % SPEECH_POOL_SIZE;
		return &orion_speech.sound_pool[note_index][pool_idx];
    }
	case PUDDLE_JUMPER: {
		int note_index = GetRandomValue(0, 1);
		int pool_idx = puddle_speech.sound_pool_index[note_index];
		puddle_speech.sound_pool_index[note_index] = (pool_idx + 1) % SPEECH_POOL_SIZE;
		return &puddle_speech.sound_pool[note_index][pool_idx];
	}
	case NEBULA: {
        int note_index = GetRandomValue(0, 1);
        int pool_idx = nebula_speech.sound_pool_index[note_index];
        nebula_speech.sound_pool_index[note_index] = (pool_idx + 1) % SPEECH_POOL_SIZE;
        return &nebula_speech.sound_pool[note_index][pool_idx];
	}
	case VOID: {
		int note_index = GetRandomValue(0, 1);
		int pool_idx = void_speech.sound_pool_index[note_index];
		void_speech.sound_pool_index[note_index] = (pool_idx + 1) % SPEECH_POOL_SIZE;
		return &void_speech.sound_pool[note_index][pool_idx];
	}
    case AUREA: {
		int note_index = GetRandomValue(0, 1);
		int pool_idx = aurea_speech.sound_pool_index[note_index];
		aurea_speech.sound_pool_index[note_index] = (pool_idx + 1) % SPEECH_POOL_SIZE;
		return &aurea_speech.sound_pool[note_index][pool_idx];
    }
    default: {
        int note_index = GetRandomValue(0, 1);
        int pool_idx = orion_speech.sound_pool_index[note_index];
        orion_speech.sound_pool_index[note_index] = (pool_idx + 1) % SPEECH_POOL_SIZE;
        return &orion_speech.sound_pool[note_index][pool_idx];
    }
    }
}

#pragma endregion STATIC

void UpdateAudio(void) {
    UpdateAudioCleanup();
    UpdateSpeech();
    
    if (playing_music != NULL)
        UpdateMusicStream(*playing_music);
}

void UnloadMusics(void) {
	UnloadMusicStream(main_music);
	UnloadMusicStream(in_game_music);
	UnloadMusicStream(ending_music);
}