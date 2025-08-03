// select_ship.c

#include "common.h"
#include "background.h"
#include "scene_draw_effects.h"
#include "raymath.h"
#include "timer.h"
#include "ship.h"
#include "draw_utils.h"
#include "ship_references.h"
#include "input.h"
#include "left_ui.h"
#include "special_effects.h"
#include "general_utils.h"
#include "stdlib.h"
#include "scene_manager.h"
#include "dev_interface_left_ui.h"

static SpecialEffect* planet_black_hole;
static SpecialEffect* planet_galaxy;
static SpecialEffect* planet_moon_1;
static SpecialEffect* planet_moon_2;
static SpecialEffect* planet_1;
static SpecialEffect* planet_2;
static SpecialEffect* planet_3;
static SpecialEffect* planet_sun;

typedef enum {
    STEP_1_GALAXY_ZOOMING_IN,
    STEP_1_GALAXY_ZOOMING_DIALOG_1,
    STEP_1_GALAXY_ZOOMING_DIALOG_2,
    STEP_1_GALAXY_ZOOMING_DIALOG_3,
    STEP_1_GALAXY_ZOOMING_DIALOG_4,
    STEP_1_GALAXY_ZOOMING_DIALOG_5,
    STEP_1_GALAXY_ZOOMING_DIALOG_6,
    STEP_2_SUN_APPEARING,
    STEP_2_SUN_MOVING_OUT,
    STEP_3_PLANET_3_MOVING_OUT,
    STEP_3_PLANET_2_MOVING_OUT,
    STEP_3_PLANET_1_ZOOMING_IN,
    STEP_4_BLACKHOLE_APPEARING_DIALOG_1,
    STEP_4_BLACKHOLE_APPEARING,
    STEP_4_BLACKHOLE_APPEARING_DIALOG_2,
    STEP_4_BLACKHOLE_APPEARING_DIALOG_3,
    STEP_4_BLACKHOLE_APPEARING_DIALOG_4,
    STEP_4_BLACKHOLE_APPEARING_DIALOG_5,
    STEP_4_BLACKHOLE_APPEARING_DIALOG_6,
    STEP_5_WARPDRIVE_ENGAGE_1,
    STEP_5_WARPDRIVE_ENGAGE_2,
    STEP_5_WARPDRIVE_ENGAGE_3,
    STEP_5_WARPDRIVE_ENGAGE_4,
} EndingScene_AnimationState;

static const Vector2 black_hole_original_size = (Vector2) {260, 260};
static const Vector2 sun_original_size = (Vector2) {800, 800};
static const Vector2 planet3_original_size = (Vector2) {400, 400};
static const Vector2 planet2_original_size = (Vector2) {400, 400};
static const Vector2 planet1_original_size = (Vector2) {300, 300};

static const Vector2 center = (Vector2) { GAME_SCREEN_CENTER, SCREEN_HEIGHT / 2.0f };
static const Color transparent = (Color){ .r = 1, .g = 1, .b = 1, .a = 0 };

static EndingScene_AnimationState animation_state;
static const float galaxy_expansion_timer = 16.0f; // 12.0f
static float galaxy_expansion_elapsed_time;

static float elapsed_time;

static Ship cutscene_ship;

static void EndingScene_InitEffects() {
    InitEffects();
    
    planet_sun = CreateUnmanagedEffect(PLANET_SUN, center, 0);
    planet_sun->size = sun_original_size;

    planet_3 = CreateUnmanagedEffect(PLANET_3, center, 0);
    planet_3->size = planet3_original_size;

    planet_2 = CreateUnmanagedEffect(PLANET_2, center, 0);
    planet_2->size = planet2_original_size;

    planet_moon_1 = CreateUnmanagedEffect(PLANET_MOON_1, center, 0);
    planet_moon_2 = CreateUnmanagedEffect(PLANET_MOON_2, center, 0);
    planet_1 = CreateUnmanagedEffect(PLANET_1, center, 0);
    
    planet_black_hole = CreateUnmanagedEffect(PLANET_BLACK_HOLE, center, 0);

    planet_galaxy = CreateUnmanagedEffect(PLANET_GALAXY, center, 0);

    planet_moon_1->color = planet_moon_2->color =
        planet_1->color = planet_black_hole->color =
        planet_2->color = planet_3->color =
        planet_sun->color = transparent;

    animation_state = STEP_1_GALAXY_ZOOMING_IN;
    elapsed_time = 0;

    InitFadeInEffect(5.0f, BLACK, 1.0f);

    UI_ClearTopPilotText();
    UI_ClearBottomPilot();
    UI_ClearBottomPilotText();
}

void Ending1Scene_Init() {
    InitLeftUIPanel();
    EndingScene_InitEffects();
    InitBackground(BACKGROUND_ENDING, WHITE, -1, 1, 4.0f);

    cutscene_ship.id = GetPlayerShip();
    cutscene_ship.position = Vector2Subtract(center, (Vector2){ 0, -250 });
    cutscene_ship.draw_size = (Vector2){ DRAW_WH, DRAW_WH };
    cutscene_ship.direction = CENTER;
    cutscene_ship.alpha = 1.0f;
    cutscene_ship.color = WHITE;
    cutscene_ship.should_render = false;
    cutscene_ship.is_tutorial = true;
}

void Ending1Scene_Unload() {
    DestroyEffect(planet_sun);
    DestroyEffect(planet_3);
    DestroyEffect(planet_2);
    DestroyEffect(planet_1);
    DestroyEffect(planet_black_hole);
    DestroyEffect(planet_galaxy);
    DestroyEffect(planet_moon_1);
    DestroyEffect(planet_moon_2);
}

float planet_increase_size_elapsed_time = 0.0f;

static void Ending1Scene_IncreasePlanetSize() {

    const Vector2 size = { 160, 160 };
    const float total_time = 24.0f; // 12.0f

    planet_increase_size_elapsed_time = Clamp(planet_increase_size_elapsed_time + GetFrameTime(), 0, total_time);
    
    float factor = planet_increase_size_elapsed_time / total_time;

    planet_black_hole->size = Vector2Add(black_hole_original_size, Vector2MultiplyScalarF(size, factor));
    planet_sun->size = Vector2Add(sun_original_size, Vector2MultiplyScalarF(size, factor));
    planet_3->size = Vector2Add(planet3_original_size, Vector2MultiplyScalarF(size, factor));
    planet_2->size = Vector2Add(planet2_original_size, Vector2MultiplyScalarF(size, factor));
    planet_1->size = Vector2Add(planet1_original_size, Vector2MultiplyScalarF(size, factor));

    planet_3->rotation = factor * 60.0f;
}

static void Ending1Scene_IncreaseGalaxySize() {
    const Vector2 initial_size = { 160, 160 };

    galaxy_expansion_elapsed_time = Clamp(galaxy_expansion_elapsed_time + GetFrameTime(), 0, galaxy_expansion_timer);

    float factor = galaxy_expansion_elapsed_time / galaxy_expansion_timer;
    float cosFactor = (cosf(factor * PI) * -1 + 1) / 2.0f;

    planet_galaxy->size = Vector2Add(initial_size, Vector2MultiplyScalarF(initial_size, cosFactor * 5));
    planet_galaxy->rotation = -40 + -5  * galaxy_expansion_elapsed_time / galaxy_expansion_timer;
}

static void Ending1Scene_UpdatePlanetAnimation() {
    bool has_reached_max = false;
    const float audio_speech_time = 3.0f;
    const float dialog_time = 5.0f;
    const float fade_times = 4.0f;
    const float sun_appearing_total_time = 4.0f;
    const float sun_moving_out_timer = 6.0f;
    const float planet_3_moving_out_timer = 6.0f;
    const float planet_2_moving_out_timer = 6.0f;
    const float planet_1_zooming_in_timer = 6.0f;
    const float blackhole_appearing_total_time = 6.0f;

    const Vector2 offset_right = { 80 * 3, 0 };
    const Vector2 offset_left = { -80 * 3, 0 };

    TraceLog(LOG_WARNING, "%d", cutscene_ship.direction);

    switch (animation_state) {
        case STEP_1_GALAXY_ZOOMING_IN:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, fade_times, NULL, &has_reached_max);

                Ending1Scene_IncreaseGalaxySize();

                if (has_reached_max) {
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_1;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_1:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                Ending1Scene_IncreaseGalaxySize();

                if (has_reached_max) {
                    UI_SetBottomPilot(ROBOT);
                    UI_BottomPilotSpeechAct("Captain, we are nearing\nsignal source!", audio_speech_time);
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_2;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_2:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                Ending1Scene_IncreaseGalaxySize();

                if (has_reached_max) {
                    UI_TopPilotSpeechAct("Scan the coordinates\nfor life forms!", audio_speech_time);
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_3;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_3:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                Ending1Scene_IncreaseGalaxySize();

                if (has_reached_max) {
                    UI_SetBottomPilot(ALIEN);
                    UI_BottomPilotSpeechAct("Found faint life\nsigns on P3X-ZHH.", audio_speech_time);
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_4;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_4:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                Ending1Scene_IncreaseGalaxySize();

                if (has_reached_max) {
                    UI_TopPilotSpeechAct("Sublights engines at\n100%. Engage!", audio_speech_time);
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_5;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_5:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                Ending1Scene_IncreaseGalaxySize();

                if (has_reached_max) {        
                    UI_ClearBottomPilot();
                    UI_ClearBottomPilotText();

                    InitFadeOutEffect(fade_times, BLACK, 0.0f);

                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_6;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_6:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, fade_times, NULL, &has_reached_max);

                if (has_reached_max) {        
                    UI_ClearTopPilotText();
                    InitFadeInEffect(fade_times, BLACK, 1.0f);
                    animation_state = STEP_2_SUN_APPEARING;
                    planet_galaxy->color = transparent;

                    cutscene_ship.should_render = true;
                    planet_sun->color = WHITE;
                    planet_black_hole->color = WHITE;
                    planet_3->color = WHITE;
                    planet_2->color = WHITE;
                    planet_1->color = WHITE;

                    elapsed_time = 0;
                }
            }
            break;
        case STEP_2_SUN_APPEARING:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, sun_appearing_total_time, NULL, &has_reached_max);

                if (has_reached_max) {
                    animation_state = STEP_2_SUN_MOVING_OUT;
                    elapsed_time = 0;
                    cutscene_ship.direction = LEFT;
                }
            }    
            break;
        case STEP_2_SUN_MOVING_OUT:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, sun_moving_out_timer, NULL, &has_reached_max);
                planet_sun->position = Vector2Add(center, Vector2MultiplyScalarF(offset_right, elapsed_time));
                
                Ending1Scene_IncreasePlanetSize();

                if (elapsed_time > sun_moving_out_timer / 2.0f) {
                    cutscene_ship.direction = CENTER;
                }
    
                if (has_reached_max) {
                    animation_state = STEP_3_PLANET_3_MOVING_OUT;
                    elapsed_time = 0;
                    cutscene_ship.direction = RIGHT;
                }
            }    
            break;
        case STEP_3_PLANET_3_MOVING_OUT:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, planet_3_moving_out_timer, NULL, &has_reached_max);
                planet_3->position = Vector2Add(center, Vector2MultiplyScalarF(offset_left, elapsed_time));
                
                Ending1Scene_IncreasePlanetSize();
    
                if (elapsed_time > planet_3_moving_out_timer / 2.0f) {
                    cutscene_ship.direction = CENTER;
                }

                if (has_reached_max) {
                    animation_state = STEP_3_PLANET_2_MOVING_OUT;
                    elapsed_time = 0;
                    cutscene_ship.direction = LEFT;
                }
            }    
            break;
        case STEP_3_PLANET_2_MOVING_OUT:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, planet_2_moving_out_timer, NULL, &has_reached_max);
                planet_2->position = Vector2Add(center, Vector2MultiplyScalarF(offset_right, elapsed_time));
                
                Ending1Scene_IncreasePlanetSize();
    
                if (has_reached_max) {
                    animation_state = STEP_3_PLANET_1_ZOOMING_IN;
                    elapsed_time = 0;
                    cutscene_ship.direction = CENTER;
                }
            }    
            break;
        case STEP_3_PLANET_1_ZOOMING_IN:
            {
                const Vector2 size = { 160, 160 };

                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, planet_1_zooming_in_timer, NULL, &has_reached_max);
                
                Ending1Scene_IncreasePlanetSize();

                if (has_reached_max) {
                    UI_SetBottomPilot(ALIEN);
                    UI_BottomPilotSpeechAct("Detecting life signs\non the surface\nat the southern\nhemisphere.", 3.0f);
                    animation_state = STEP_4_BLACKHOLE_APPEARING_DIALOG_1;
                    elapsed_time = 0;
                }
            }    
            break;
        case STEP_4_BLACKHOLE_APPEARING_DIALOG_1:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);
               
                Ending1Scene_IncreasePlanetSize();

                if (has_reached_max) {
                    UI_TopPilotSpeechAct("Good job, ensign.\nAll hands to rescue.", audio_speech_time);

                    animation_state = STEP_4_BLACKHOLE_APPEARING;
                    elapsed_time = 0;
                }
            }    
            break;
        case STEP_4_BLACKHOLE_APPEARING:
            const Vector2 offset = { 40, -40 };

            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, blackhole_appearing_total_time, NULL, &has_reached_max);

            planet_black_hole->position = Vector2Add(center, Vector2MultiplyScalarF(offset, elapsed_time));

            Ending1Scene_IncreasePlanetSize();

            if (has_reached_max) {
                UI_SetBottomPilot(ROBOT);
                UI_BottomPilotSpeechAct("Captain, the sensors are\ndetecting a massive graviton\nfield behind the planet.", audio_speech_time);

                animation_state = STEP_4_BLACKHOLE_APPEARING_DIALOG_2;
                elapsed_time = 0;
            }
            break;
        case STEP_4_BLACKHOLE_APPEARING_DIALOG_2:            
            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

            Ending1Scene_IncreasePlanetSize();

            if (has_reached_max) {
                UI_TopPilotSpeechAct("The pirates had\naritifical blackhole\nweapons. Huh?", audio_speech_time);

                animation_state = STEP_4_BLACKHOLE_APPEARING_DIALOG_3;
                elapsed_time = 0;
            }
            break;
        case STEP_4_BLACKHOLE_APPEARING_DIALOG_3:            
            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

            Ending1Scene_IncreasePlanetSize();

            if (has_reached_max) {
                UI_TopPilotSpeechAct("Code RED. Beam them out\nof there A.S.A.P.", audio_speech_time);
                InitFadeOutEffect(fade_times, BLACK, 0);

                animation_state = STEP_4_BLACKHOLE_APPEARING_DIALOG_4;
                elapsed_time = 0;
            }
            break;
        case STEP_4_BLACKHOLE_APPEARING_DIALOG_4:
            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, fade_times, NULL, &has_reached_max);

            Ending1Scene_IncreasePlanetSize();

            if (has_reached_max) {
                InitFadeInEffect(fade_times, BLACK, 1);
                UI_SetBottomPilot(ALIEN);
                UI_BottomPilotSpeechAct("Sir, everyone is safe\nand reporting to sickbay.", audio_speech_time);

                animation_state = STEP_4_BLACKHOLE_APPEARING_DIALOG_5;
                elapsed_time = 0;
            }
            break;
        case STEP_4_BLACKHOLE_APPEARING_DIALOG_5:
            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, fade_times, NULL, &has_reached_max);

            Ending1Scene_IncreasePlanetSize();

            if (has_reached_max) {
                UI_TopPilotSpeechAct("Engage warpdrive before\nwe are caught by the\ngravitational pull, helmsman.", audio_speech_time);

                animation_state = STEP_4_BLACKHOLE_APPEARING_DIALOG_6;
                elapsed_time = 0;
            }
            break;
        case STEP_4_BLACKHOLE_APPEARING_DIALOG_6:
            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, fade_times, NULL, &has_reached_max);

            Ending1Scene_IncreasePlanetSize();

            if (has_reached_max) {
                UI_SetBottomPilot(ALIEN);
                UI_BottomPilotSpeechAct("Aye aye, captain.", audio_speech_time);

                UI_TopPilotSpeechAct(NULL, 0);
                
                animation_state = STEP_5_WARPDRIVE_ENGAGE_1;
                elapsed_time = 0;
            }
            break;
        case STEP_5_WARPDRIVE_ENGAGE_1:
            const float warpdrive_begin_time = 3.0f;

            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, warpdrive_begin_time, NULL, &has_reached_max);
 
            Ending1Scene_IncreasePlanetSize();

            if (has_reached_max) {
                SpecialEffect* sfx = CreateManagedEffectDuration(WORMHOLE, cutscene_ship.position, 10.0f);
                sfx->order = RENDERING_ORDER_AFTER_SHIP;
                animation_state = STEP_5_WARPDRIVE_ENGAGE_2;
                elapsed_time = 0;
            }
            break;
        case STEP_5_WARPDRIVE_ENGAGE_2:
            const float warpdrive = 5.0f;

            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, warpdrive, NULL, &has_reached_max);
 
            Ending1Scene_IncreasePlanetSize();

            if (has_reached_max) {
                cutscene_ship.should_render = false;
                animation_state = STEP_5_WARPDRIVE_ENGAGE_3;
                elapsed_time = 0;
            }
            break;
        case STEP_5_WARPDRIVE_ENGAGE_3:
            const float warpdrive_end = 5.0f;

            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, warpdrive_end, NULL, &has_reached_max);

            Ending1Scene_IncreasePlanetSize();
            
            if (has_reached_max) {
                InitFadeOutEffect(fade_times, BLACK, 0);
                animation_state = STEP_5_WARPDRIVE_ENGAGE_4;
                elapsed_time = 0;
            }
            
            break;
        case STEP_5_WARPDRIVE_ENGAGE_4:
            elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, fade_times, NULL, &has_reached_max);

            Ending1Scene_IncreasePlanetSize();
            
            if (has_reached_max) {
                Ending1Scene_Unload();
                ChangeScene(WINNER);
            }
            
            break;
    }
}

void Ending1Scene_Update() {
    UpdatePilot();

    UpdateEffects();
    UpdateBackground();
    UpdateShip(&cutscene_ship);

    Ending1Scene_UpdatePlanetAnimation();
}

void Ending1Scene_Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawBackground();
    DrawEffects(RENDERING_ORDER_BEFORE_ENEMY);
    DrawEffects(RENDERING_ORDER_BEFORE_SHIP);

    DrawShip(&cutscene_ship);

    TraceLog(LOG_WARNING, "%d", cutscene_ship.direction);

    DrawEffects(RENDERING_ORDER_AFTER_SHIP);

    // Left and right panels
    UpdateAndDrawScreenEffects();

    DrawUserInterface();

    EndDrawing();
}