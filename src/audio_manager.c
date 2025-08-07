// audio_manager.c
#include "audio_manager.h"
#include "list.h"
#include "common.h"
#include "player.h"
#include "scene_manager.h"
#include "stdlib.h"
#include "raymath.h"

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

static Music intro_music;
static Music main_music;
static Music in_game_music;
static Music ending_music;

static Music* playing_music;

// Sound effects
static List* audio_samples = NULL;

Sound sound1;
Sound sound2;
Sound sound4;
Sound sound5;
Sound sound6;
Sound sound7;
Sound sound8;
Sound sound9;
Sound sound10;
Sound sound11;
Sound sound12;
Sound sound14;
Sound sound15;
Sound sound16;
Sound sound17;
Sound sound18;
Sound sound19;
Sound sound20;
Sound sound21;
Sound sound22;
Sound sound23;
Sound sound24;
Sound sound25;
Sound sound26;
Sound sound27;


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

Sound* PlaySoundFx(Sound* sound) {
    Sound* sound_alias = (Sound*)malloc(sizeof(Sound));
    if (!sound_alias) return NULL;

    *sound_alias = LoadSoundAlias(*sound);
    List_Add(audio_samples, &sound_alias);
    PlaySound(*sound_alias);

    return sound_alias;
}

Sound* PlaySoundFxPitch(Sound* sound, float pitch) {
    // Allocate memory for the Sound alias on the heap
    Sound* sound_alias = PlaySoundFx(sound);
    SetSoundPitch(*sound_alias, pitch);
    SetSoundVolume(*sound_alias, 0.3);
    return sound_alias;
}

Sound* PlaySoundFxRandomPitch(Sound* sound, float minPitch, float maxPitch) {
    float randomValue = GetRandomValue(1, 1000) / 1000.0f;
    return PlaySoundFxPitch(sound, Lerp(minPitch, maxPitch, randomValue));
}

Sound* PlaySoundFxWithVolumeAndRandomPitch(Sound* sound, float volume, float minPitch, float maxPitch) {
    Sound* new_sound = PlaySoundFxRandomPitch(sound, minPitch, maxPitch);
    SetSoundVolume(*new_sound, volume);
    return new_sound;
}


void PlaySfx(SoundEffect sound) {
    PlaySoundInternal(GetSoundEffect(sound), AUDIO_TYPE_SFX);
}

void PlayMusic(Scene scene) {
    switch(scene) {
        case INTRO:
            playing_music = &intro_music;
            break;
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
        SetMusicVolume(*playing_music, BASE_MUSIC_VOLUME);
        SeekMusicStream(*playing_music, 0);
        
        (*playing_music).looping = true;
    }
}

#pragma region LOAD/UNLOAD
void LoadAudioResources(void) {

    if (audio_samples) {
        List_Destroy(audio_samples);
    }

    audio_samples = List_Create(sizeof(Sound*));

    // Music
    intro_music = LoadMusicStream("music/Dani Stob - A New Begining - Full.wav");
    main_music = LoadMusicStream("music/Dani Stob - A Journey for Purpose - Loop.wav");
    in_game_music = LoadMusicStream("music/Dani Stob - Unstoppable - Loop.wav");
    ending_music = LoadMusicStream("music/Dani Stob - Beyond The Stars - Loop.wav");

    // Sounds
    sound1 = LoadSound("sound/effect/laser-45816.mp3"); // Projéteis de AUREA e VOID
    sound2 = LoadSound("sound/effect/explosion-47163.mp3");
    sound4 = LoadSound("sound/effect/laser-45816.mp3");
    sound5 = LoadSound("sound/effect/laser-bolt-89300.mp3");
    sound6 = LoadSound("sound/effect/lasergun-152375.mp3"); // Ainda sem uso
    sound7 = LoadSound("sound/effect/sci-fi-weapon-shoot-firing-plasma-ku-05-233818.mp3");
    sound8 = LoadSound("sound/effect/sci-fi-weapon-shoot-firing-pulse-dn-05-233832.mp3");
    sound9 = LoadSound("sound/effect/sci-fi-weapon-shoot-firing-pulse-tm-04-233827.mp3");
    sound10 = LoadSound("sound/effect/swish-swoosh-woosh-sfx-48-357150.mp3");
    sound11 = LoadSound("sound/effect/teleport-14639.mp3"); // Wormhole (Abrindo)
    sound12 = LoadSound("sound/effect/teleport-90137.mp3"); // Wormhole (Atravessando)
    
    sound14 = LoadSound("sound/effect/warp-306033.mp3"); // Puddle Jumper SHOOT
    sound15 = LoadSound("sound/effect/warp-sfx-6897.mp3");
    sound16 = LoadSound("sound/effect/futuristic-smg-sound-effect-100378.mp3");
    sound17 = LoadSound("sound/effect/scifi-anime-whoosh-45-205264.mp3");
    sound18 = LoadSound("sound/effect/designed-fire-winds-swoosh-04-116788.mp3");
    sound19 = LoadSound("sound/effect/explosion-pas-61639.mp3");
    sound20 = LoadSound("sound/effect/distant-explosion-90743.mp3");

    sound21 = LoadSound("sound/effect/sample_confirm_success02_kofi_by_miraclei-360154.mp3");
    sound22 = LoadSound("sound/effect/transition-futuristic-ufo-121421.mp3");
    sound23 = LoadSound("sound/effect/game-start-6104.mp3");
    sound24 = LoadSound("sound/effect/sci-fi-launch-5-351240.mp3");
    sound25 = LoadSound("sound/effect/slow-whoosh-118247.mp3");
    sound26 = LoadSound("sound/effect/space-ship-bridge-loop-104525.mp3");
    sound27 = LoadSound("sound/effect/sci-fi-alarm-95054.mp3");

    // Speech

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

    UnloadSound(sound1);
    UnloadSound(sound2);
    UnloadSound(sound4);
    UnloadSound(sound5);
    UnloadSound(sound6);
    UnloadSound(sound7);
    UnloadSound(sound8);
    UnloadSound(sound9);
    UnloadSound(sound10);
    UnloadSound(sound11);
    UnloadSound(sound12);
    UnloadSound(sound14);
    UnloadSound(sound15);
    UnloadSound(sound16);
    UnloadSound(sound17);
    UnloadSound(sound18);
    UnloadSound(sound19);
    UnloadSound(sound20);

    UnloadSound(sound21);
    UnloadSound(sound22);
    UnloadSound(sound23);
    UnloadSound(sound24);

    UnloadSound(sound25);
    UnloadSound(sound26);
    UnloadSound(sound27);
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

static bool IsSoundFXFinished(void* context, void* data) {
    // The data in the list is a pointer to the Sound pointer
    Sound** sound_ptr_ptr = (Sound**)data;
    Sound* sound_ptr = *sound_ptr_ptr;

    if (!IsSoundPlaying(*sound_ptr)) {
        UnloadSoundAlias(*sound_ptr);
        free(sound_ptr);
        return true;
    }

    return false;
}

static void UpdateAudioCleanup(void) {
    if (playing_sounds) {
        List_RemoveWithFn(playing_sounds, NULL, IsSoundFinished);
    }

    if (audio_samples) {
        List_RemoveWithFn(audio_samples, NULL, IsSoundFXFinished);
    }
}

static void ResetSpeech(void) {
    speech.timer = 0.0f;
    speech.elapsed_time = 0.0f;
    speech.active_ship_id = -1;
}

static Sound* GetSpeechFromPilot(PilotAudio* speech) {
    int note_index = GetRandomValue(0, 1);
    int pool_idx = speech->sound_pool_index[note_index];
    speech->sound_pool_index[note_index] = (pool_idx + 1) % SPEECH_POOL_SIZE;
    return &speech->sound_pool[note_index][pool_idx];
}

static Sound* GetPilotSound(int ship) {
    switch (ship) {
    case ORION:         return GetSpeechFromPilot(&orion_speech);
    case PUDDLE_JUMPER: return GetSpeechFromPilot(&puddle_speech);
    case NEBULA:        return GetSpeechFromPilot(&nebula_speech);
    case VOID:          return GetSpeechFromPilot(&void_speech);
    case AUREA:         return GetSpeechFromPilot(&aurea_speech);
    default:            return GetSpeechFromPilot(&orion_speech); // fallback
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
    UnloadMusicStream(intro_music);
	UnloadMusicStream(main_music);
	UnloadMusicStream(in_game_music);
	UnloadMusicStream(ending_music);
}

void PlayExplosionSound(void) {
    switch(GetRandomValue(0, 2)) {
        case 0:
            PlaySoundFxWithVolumeAndRandomPitch(&sound2, ENEMY_DEATH_SFX_VOLUME, 1.0f, 1.5f);
            break;
        case 1:
            PlaySoundFxWithVolumeAndRandomPitch(&sound19, ENEMY_DEATH_SFX_VOLUME, 1.0f, 1.5f);
            break;
        case 2:
            PlaySoundFxWithVolumeAndRandomPitch(&sound20, ENEMY_DEATH_SFX_VOLUME, 1.0f, 1.5f);
            break;
    }
}