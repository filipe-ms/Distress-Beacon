// select_ship.c

#include "common.h"
#include "background.h"
#include "scene_draw_effects.h"
#include "raymath.h"
#include "timer.h"
#include "ship.h"
#include "pilot.h"
#include "draw_utils.h"
#include "ship_references.h"
#include "input.h"
#include "left_ui.h"
#include "special_effects.h"
#include "general_utils.h"
#include "stdlib.h"

SpecialEffect* planet_black_hole;
SpecialEffect* planet_galaxy;
SpecialEffect* planet_moon_1;
SpecialEffect* planet_moon_2;
SpecialEffect* planet_1;
SpecialEffect* planet_2;
SpecialEffect* planet_3;
SpecialEffect* planet_sun;

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
} EndingScene_AnimationState;

const Vector2 sun_original_size = (Vector2) {800, 800};
const Vector2 planet3_original_size = (Vector2) {400, 400};
const Vector2 planet2_original_size = (Vector2) {400, 400};
const Vector2 planet1_original_size = (Vector2) {300, 300};

const Vector2 center = (Vector2) { GAME_SCREEN_CENTER, SCREEN_HEIGHT / 2.0f };
const Color transparent = (Color){ .r = 1, .g = 1, .b = 1, .a = 0 };

EndingScene_AnimationState animation_state;
float elapsed_time;

static void EndingScene_InitEffects() {
    InitEffects();
    
    planet_sun = CreateUnmanagedEffect(PLANET_SUN, center, 0);
    planet_sun->size = sun_original_size;

    planet_3 = CreateUnmanagedEffect(PLANET_3, center, 0);
    planet_3->size = planet3_original_size;

    planet_2 = CreateUnmanagedEffect(PLANET_2, center, 0);
    planet_2->size = planet2_original_size;

    planet_black_hole = CreateUnmanagedEffect(PLANET_BLACK_HOLE, center, 0);

    planet_moon_1 = CreateUnmanagedEffect(PLANET_MOON_1, center, 0);
    planet_moon_2 = CreateUnmanagedEffect(PLANET_MOON_2, center, 0);
    planet_1 = CreateUnmanagedEffect(PLANET_1, center, 0);
    
    planet_galaxy = CreateUnmanagedEffect(PLANET_GALAXY, center, 0);

    planet_moon_1->color = planet_moon_2->color =
        planet_1->color = planet_black_hole->color =
        planet_2->color = planet_3->color =
        planet_sun->color = transparent;

    animation_state = STEP_1_GALAXY_ZOOMING_IN;
    elapsed_time = 0;

    InitFadeInEffect(5.0f, BLACK, 1.0f);
}

void Ending1Scene_Init() {
    InitLeftUIPanel();
    EndingScene_InitEffects();
    InitBackground(BACKGROUND_ENDING, WHITE, -1, 1, 4.0f);
}

float planet_increase_size_elapsed_time = 0.0f;

static void Ending1Scene_IncreasePlanetSize() {

    const Vector2 size = { 160, 160 };
    const float total_time = 24.0f; // 12.0f

    planet_increase_size_elapsed_time = Clamp(planet_increase_size_elapsed_time + GetFrameTime(), 0, total_time);
    
    float factor = planet_increase_size_elapsed_time / total_time;

    planet_sun->size = Vector2Add(sun_original_size, Vector2MultiplyScalarF(size, factor));
    planet_3->size = Vector2Add(planet3_original_size, Vector2MultiplyScalarF(size, factor));
    planet_2->size = Vector2Add(planet2_original_size, Vector2MultiplyScalarF(size, factor));
    planet_1->size = Vector2Add(planet1_original_size, Vector2MultiplyScalarF(size, factor));

    planet_3->rotation = factor * 60.0f;
}

static void Ending1Scene_UpdatePlanetAnimation() {
    bool has_reached_max = false;
    const float dialog_time = 4.0f;
    const float fade_times = 5.0f;

    const Vector2 offset_right = { 80 * 3, 0 };
    const Vector2 offset_left = { -80 * 3, 0 };

    switch (animation_state) {
        case STEP_1_GALAXY_ZOOMING_IN:
            {
                const Vector2 initial_size = { 160, 160 };
                const float total_time = 4.0f; // 12.0f

                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, total_time, NULL, &has_reached_max);
                planet_galaxy->size = Vector2MultiplyScalarF(initial_size, elapsed_time / 2.0f);
                planet_galaxy->rotation = -40 + -5  * elapsed_time / total_time;

                if (has_reached_max) {
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_1;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_1:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                if (has_reached_max) {
                    SetBottomPilotDefault(ROBOT);
                    SetBottomPilotText("Captain, we are nearing\nsignal source!");
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_2;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_2:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                if (has_reached_max) {
                    SetTopPilotDefault();
                    SetTopPilotText("Scan the coordinates\nfor life forms!");
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_3;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_3:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                if (has_reached_max) {
                    SetBottomPilotDefault(ALIEN);
                    SetBottomPilotText("Found faint life\nsigns on P3X-ZHH.");
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_4;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_4:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                if (has_reached_max) {
                    SetTopPilotDefault();
                    SetTopPilotText("Sublights engines at\n100%. Engage!");
                    animation_state = STEP_1_GALAXY_ZOOMING_DIALOG_5;
                    elapsed_time = 0;
                }
            }
            break;
        case STEP_1_GALAXY_ZOOMING_DIALOG_5:
            {
                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, dialog_time, NULL, &has_reached_max);

                if (has_reached_max) {
                    SetTopPilotDefault();
                    SetTopPilotText(NULL);
                    SetBottomPilotDefault(NONE);
                    SetBottomPilotText(NULL);
                    
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
                    InitFadeInEffect(fade_times, BLACK, 1.0f);
                    animation_state = STEP_2_SUN_APPEARING;
                    planet_galaxy->color = transparent;

                    planet_sun->color = WHITE;
                    planet_3->color = WHITE;
                    planet_2->color = WHITE;
                    planet_1->color = WHITE;

                    elapsed_time = 0;
                }
            }
            break;
        case STEP_2_SUN_APPEARING:
            {
                const float total_time = 4.0f; // 12.0f

                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, total_time, NULL, &has_reached_max);

                if (has_reached_max) {
                    animation_state = STEP_2_SUN_MOVING_OUT;
                    elapsed_time = 0;
                }
            }    
            break;
        case STEP_2_SUN_MOVING_OUT:
            {
                const float total_time = 8.0f; // 12.0f

                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, total_time, NULL, &has_reached_max);
                planet_sun->position = Vector2Add(center, Vector2MultiplyScalarF(offset_right, elapsed_time));
                
                Ending1Scene_IncreasePlanetSize();
    
                if (has_reached_max) {
                    animation_state = STEP_3_PLANET_3_MOVING_OUT;
                    elapsed_time = 0;
                }
            }    
            break;
        case STEP_3_PLANET_3_MOVING_OUT:
            {
                const float total_time = 8.0f; // 12.0f

                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, total_time, NULL, &has_reached_max);
                planet_3->position = Vector2Add(center, Vector2MultiplyScalarF(offset_left, elapsed_time));
                
                Ending1Scene_IncreasePlanetSize();
    
                if (has_reached_max) {
                    animation_state = STEP_3_PLANET_2_MOVING_OUT;
                    elapsed_time = 0;
                }
            }    
            break;
        case STEP_3_PLANET_2_MOVING_OUT:
            {
                const float total_time = 8.0f; // 12.0f

                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, total_time, NULL, &has_reached_max);
                planet_2->position = Vector2Add(center, Vector2MultiplyScalarF(offset_right, elapsed_time));
                
                Ending1Scene_IncreasePlanetSize();
    
                if (has_reached_max) {
                    animation_state = STEP_3_PLANET_1_ZOOMING_IN;
                    elapsed_time = 0;
                }
            }    
            break;
        case STEP_3_PLANET_1_ZOOMING_IN:
            {
                const Vector2 size = { 160, 160 };
                const float total_time = 8.0f; // 12.0f

                elapsed_time = ClampWithFlagsF(elapsed_time + GetFrameTime(), 0, total_time, NULL, &has_reached_max);
                
                Ending1Scene_IncreasePlanetSize();
    
                if (has_reached_max) {
                    animation_state = STEP_3_PLANET_1_ZOOMING_IN;
                    elapsed_time = 0;
                }
            }    
            break;

            /*// Pilotos não jogáveis
#define DUCK 5
#define ROBOT 6
#define ALIEN 7
#define PIRATE 8
#define INVERSE 9
#define INTERFERENCE 10 */
            break;
    }
}

void Ending1Scene_Update() {
    UpdatePilot();

    UpdateEffects();
    UpdateBackground();

    Ending1Scene_UpdatePlanetAnimation();
}

void Ending1Scene_Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawBackground();
    DrawEffects(RENDERING_ORDER_BEFORE_SHIP);

    // Left and right panels
    DrawLeftUIPanel();
    DrawRectangle(GAME_SCREEN_END, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);

    UpdateAndDrawScreenEffects();

    EndDrawing();
}