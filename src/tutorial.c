// tutorial.c

#include "tutorial.h"

#include "scene_manager.h"
#include "input.h"
#include "ship.h"
#include "draw_utils.h"
#include "scene_draw_effects.h"
#include "texture_manager.h"
#include "common.h"
#include "special_effects.h"
#include "raymath.h"
#include "bars.h"
#include "timer.h"
#include "background.h"
#include "stdlib.h"

Rectangle keyboard_keys[84]; // 12 * 7 = 84

void SetKeySources() {
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 12; i++) {
            keyboard_keys[j * 12 + i] = (Rectangle){ 16 * i, 16 * j, 16, 16 };
        }
    }
}

typedef struct Tutorial {
    Ship ship_movement;
    Ship ship_special;

    bool wormhole_created;
    SpecialEffect* teleport_sfx;
    
    bool wormhole_created2;
	SpecialEffect* teleport_sfx2;

    float bar_fill;
    float line_alpha;
    int cycle;

    bool backspace_pressed;
} Tutorial;

static Tutorial tutorial;

void InitTutorial(void) {
    tutorial.cycle = 0;

    tutorial.ship_movement.id = ORION;
    tutorial.ship_movement.position = (Vector2){ UI_LEFT_CENTER - DRAW_WH, SCREEN_HEIGHT * 0.45 - DRAW_WH };
    tutorial.ship_movement.draw_size = (Vector2){ DRAW_WH, DRAW_WH };
    tutorial.ship_movement.direction = CENTER;
    tutorial.ship_movement.color = WHITE;
    tutorial.ship_movement.alpha = 1.0f;
    tutorial.ship_movement.should_render = true;
    tutorial.ship_movement.is_tutorial = true;

    tutorial.ship_special.id = PUDDLE_JUMPER;
    tutorial.ship_special.position = (Vector2){ GAME_SCREEN_CENTER - DRAW_WH, SCREEN_HEIGHT * 0.45 + DRAW_WH };
    tutorial.ship_special.draw_size = (Vector2){ DRAW_WH, DRAW_WH };
    tutorial.ship_special.direction = CENTER;
    tutorial.ship_special.color = WHITE;
    tutorial.ship_special.alpha = 1.0f;
    tutorial.ship_special.should_render = true;
    tutorial.ship_special.is_tutorial = true;

	tutorial.wormhole_created = false;
    tutorial.backspace_pressed = false;

	tutorial.bar_fill = 0.0f;

	tutorial.teleport_sfx = NULL;
	tutorial.teleport_sfx2 = NULL;
    SetKeySources();
    InitEffects();
	InitFadeInEffect(1.5f, BLACK, 1.0f);
    InitBackground(BACKGROUND_STARS, WHITE, STRETCH_TO_SCREEN, 0.7f, 100.0f);
}

void ShipMovement() {
    float speed_factor = 50.0f;
    float delta_move = speed_factor * GetFrameTime();

    switch (tutorial.cycle) {

    case 0: { // Move right
        tutorial.ship_movement.direction = RIGHT;
        float targetX = UI_LEFT_CENTER + DRAW_WH;
        tutorial.ship_movement.position.x += delta_move;

        if (tutorial.ship_movement.position.x >= targetX) {
            tutorial.ship_movement.position.x = targetX;
            tutorial.cycle++;
        }
        break;
    }

    case 1: { // Move down
        tutorial.ship_movement.direction = CENTER;
        float targetY = SCREEN_HEIGHT * 0.45f + DRAW_WH;
        tutorial.ship_movement.position.y += delta_move;

        if (tutorial.ship_movement.position.y >= targetY) {
            tutorial.ship_movement.position.y = targetY;
            tutorial.cycle++;
        }
        break;
    }

    case 2: { // Move left
        tutorial.ship_movement.direction = LEFT;
        float targetX = UI_LEFT_CENTER - DRAW_WH;
        tutorial.ship_movement.position.x -= delta_move;

        if (tutorial.ship_movement.position.x <= targetX) {
            tutorial.ship_movement.position.x = targetX;
            tutorial.cycle++;
        }
        break;
    }

    case 3: { // Move up
        tutorial.ship_movement.direction = CENTER;
        float targetY = SCREEN_HEIGHT * 0.45f - DRAW_WH;
        tutorial.ship_movement.position.y -= delta_move;

        if (tutorial.ship_movement.position.y <= targetY) {
            tutorial.ship_movement.position.y = targetY;
            tutorial.cycle = 0;
        }
        break;
    }

    default:
        break;
    }
}

void ShipSpecial() {
    float speed_factor = 50.0f;
    float delta_move = speed_factor * GetFrameTime();

    Vector2 pos1 = { GAME_SCREEN_CENTER - DRAW_WH, SCREEN_HEIGHT * 0.45 + DRAW_WH };
    Vector2 pos2 = { GAME_SCREEN_CENTER + DRAW_WH, SCREEN_HEIGHT * 0.45 - DRAW_WH };

    switch (tutorial.cycle) {

    case 0: { // Wait (placing wormhole)
		if (tutorial.teleport_sfx) {
			DestroyEffect(tutorial.teleport_sfx);
			tutorial.teleport_sfx = NULL;
		}
		if (tutorial.teleport_sfx2) {
			DestroyEffect(tutorial.teleport_sfx2);
			tutorial.teleport_sfx2 = NULL;
		}

        tutorial.ship_special.direction = CENTER;
        tutorial.ship_special.position = pos1;
        tutorial.ship_special.should_render = true;
        tutorial.line_alpha = 0;

        if (!tutorial.wormhole_created) {
            tutorial.teleport_sfx = CreateUnmanagedEffect(WORMHOLE, tutorial.ship_special.position, 6.0f);
            tutorial.wormhole_created = true;
        }

        break;
    }
    case 1: { // Moving away...
        tutorial.ship_special.direction = RIGHT;

        tutorial.ship_special.position.x += delta_move;
        tutorial.ship_special.position.y -= delta_move;

        if (tutorial.ship_special.position.x >= pos2.x) {
            tutorial.ship_special.position.x = pos2.x;
        }

        if (tutorial.ship_special.position.y <= pos2.y) {
            tutorial.ship_special.position.y = pos2.y;
        }


        tutorial.wormhole_created = false;
        break;
    }

    case 2: { // Waiting for tele back
        tutorial.ship_special.direction = CENTER;
        if (!tutorial.wormhole_created2) {
            tutorial.teleport_sfx2 = CreateUnmanagedEffect(WORMHOLE, tutorial.ship_special.position, 2.0f);
            tutorial.wormhole_created2 = true;
        }

        tutorial.line_alpha += 0.5 * GetFrameTime();
        tutorial.line_alpha = Clamp(tutorial.line_alpha, 0.0f, 1.0f);

        break;
    }

    case 3: { // Teleporting back
        tutorial.ship_special.should_render = false;
        tutorial.wormhole_created2 = false;

        tutorial.ship_special.position.x -= delta_move;
        tutorial.ship_special.position.y += delta_move;

        if (tutorial.ship_special.position.x <= pos1.x) {
            tutorial.ship_special.position.x = pos1.x;
        }

        if (tutorial.ship_special.position.y >= pos1.y) {
            tutorial.ship_special.position.y = pos1.y;
        }

        tutorial.line_alpha = (tutorial.ship_special.position.x - pos1.x) / (pos2.x - pos1.x);
        tutorial.line_alpha = Clamp(tutorial.line_alpha, 0.0f, 1.0f);

        break;
    }

    default: {
        break;
    }

    }
}

void LevelAndExperience() {
    switch (tutorial.cycle) {

    case 0: { // Filling
		tutorial.bar_fill += 0.55f * GetFrameTime();
        if (tutorial.bar_fill > 1) tutorial.bar_fill = 1;

        break;
    }
    case 1: { // Level Up
        break;
    }

    case 2: { // Card options
       
        tutorial.bar_fill = 0;
        break;
    }

    case 3: { // Choice
       

        break;
    }

    default: {
        break;
    }

    }
}

Color GetButtonColor(int btn) {
    if (tutorial.cycle == 0 && btn == RIGHT) { return GRAY; }
	else if (tutorial.cycle == 1 && btn == DOWN) { return GRAY; }
	else if (tutorial.cycle == 2 && btn == LEFT) { return GRAY; }
	else if (tutorial.cycle == 3 && btn == UP) { return GRAY; }
    else { return WHITE; }
}

void UpdateTutorial(void) {
    UpdateBackground();
    ShipMovement(); // Cycle toma update aqui
    ShipSpecial();
    UpdateShip(&tutorial.ship_movement);
    UpdateShip(&tutorial.ship_special);

    LevelAndExperience();

    UpdateEffects();
    
    if (tutorial.backspace_pressed) {
        if (UpdateTimer()) ChangeScene(START);
    } else if (IsReturnButtonPressed() || IsConfirmButtonPressed()) {
        InitTimer(2.0f);
        InitFadeOutEffect(2.1f, BLACK, GetCurrentScreenEffectAlpha());
        tutorial.backspace_pressed = true;
    }
}

void DrawTutorial(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawBackground();

    float h1 = 0.08;
    float h2 = 0.17;
    float h3 = 0.25;
    float h4 = 0.45;

    DrawCenteredOutlinedText("Tutorial", GAME_SCREEN_CENTER, SCREEN_HEIGHT * h1, 64, WHITE, Fade(RAYWHITE, 0.5));

    DrawCenteredRectangle(UI_LEFT_CENTER, SCREEN_HEIGHT * 0.47, 500, 750, UI_BG_COLOR);
    DrawCenteredPixelBorder(UI_LEFT_CENTER, SCREEN_HEIGHT * 0.47, 500, 750, 5, WHITE);
    DrawCenteredRectangle(UI_LEFT_CENTER, SCREEN_HEIGHT * h4, 200, 200, BLACK);
    { // Movimentação
        DrawCenteredOutlinedText("Movimentação", UI_LEFT_CENTER, SCREEN_HEIGHT * h2, 24, WHITE, Fade(RAYWHITE, 0.5));

        Rectangle dest_left = { (UI_LEFT_CENTER - 24) - 50, SCREEN_HEIGHT * h3, 48, 48 };
        DrawTexturePro(keyboard_buttons, keyboard_keys[55], dest_left, (Vector2) { 0 }, 0.0f, GetButtonColor(LEFT));

        Rectangle dest_right = { (UI_LEFT_CENTER - 24) + 50, SCREEN_HEIGHT * h3, 48, 48 };
        DrawTexturePro(keyboard_buttons, keyboard_keys[56], dest_right, (Vector2) { 0 }, 0.0f, GetButtonColor(RIGHT));

        Rectangle dest_up = { (UI_LEFT_CENTER - 24), SCREEN_HEIGHT * h3 - 48, 48, 48 };
        DrawTexturePro(keyboard_buttons, keyboard_keys[57], dest_up, (Vector2) { 0 }, 0.0f, GetButtonColor(UP));

        Rectangle dest_down = { (UI_LEFT_CENTER - 24), SCREEN_HEIGHT * h3, 48, 48 };
        DrawTexturePro(keyboard_buttons, keyboard_keys[58], dest_down, (Vector2) { 0 }, 0.0f, GetButtonColor(DOWN));

        DrawCenteredPixelBorder(UI_LEFT_CENTER, SCREEN_HEIGHT * h4, 200, 200, 5, WHITE);

        DrawCenteredMultilineText("Utilize as setas\ndirecionais para\nse mover", UI_LEFT_CENTER, SCREEN_HEIGHT * 0.65, 34, WHITE);
    }

    DrawCenteredRectangle(GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.47, 500, 750, UI_BG_COLOR);
    DrawCenteredPixelBorder(GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.47, 500, 750, 5, WHITE);
    DrawCenteredRectangle(GAME_SCREEN_CENTER, SCREEN_HEIGHT * h4, 200, 200, BLACK);
    { // Especiais
        DrawCenteredOutlinedText("Especiais", GAME_SCREEN_CENTER, SCREEN_HEIGHT * h2, 24, WHITE, Fade(RAYWHITE, 0.5));

		Rectangle dest_space = { (GAME_SCREEN_CENTER - 24) - 50, SCREEN_HEIGHT * h3 - 50, 48, 48 };
		DrawTexturePro(keyboard_buttons, keyboard_keys[54], dest_space, (Vector2) { 0 }, 0.0f, GetButtonColor(RIGHT));
        DrawCenteredText("ou", GAME_SCREEN_CENTER - 48, SCREEN_HEIGHT * h3 - 5, 24, WHITE);

		Rectangle dest_z = { (GAME_SCREEN_CENTER - 24) - 50, SCREEN_HEIGHT * h3 + 20, 48, 48 };
		DrawTexturePro(keyboard_buttons, keyboard_keys[19], dest_z, (Vector2) { 0 }, 0.0f, GetButtonColor(RIGHT));

		Rectangle dest_x = { (GAME_SCREEN_CENTER - 24) + 50, SCREEN_HEIGHT * h3 - 10, 48, 48 };
		DrawTexturePro(keyboard_buttons, keyboard_keys[20], dest_x, (Vector2) { 0 }, 0.0f, GetButtonColor(LEFT));

        DrawCenteredPixelBorder(GAME_SCREEN_CENTER, SCREEN_HEIGHT * h4, 200, 200, 5, WHITE);

        Vector2 pos1 = { GAME_SCREEN_CENTER - DRAW_WH, SCREEN_HEIGHT * 0.45 + DRAW_WH };
        Vector2 pos2 = { GAME_SCREEN_CENTER + DRAW_WH, SCREEN_HEIGHT * 0.45 - DRAW_WH };
        DrawCircleV(pos1, 8, Fade(BLUE, tutorial.line_alpha));
        DrawCircleV(pos2, 8, Fade(BLUE, tutorial.line_alpha));
        DrawLineEx(pos1, pos2, 16, BLACK);
        DrawLineEx(pos1, pos2, 16, Fade(BLUE, tutorial.line_alpha));

        DrawCenteredMultilineText("Espaço ou Z ativam\no especial da nave.\n\nA Puddle Jumper também\nutiliza o X para o especial.", GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.67, 34, WHITE);
    }

    DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.47, 500, 750, UI_BG_COLOR);
    DrawCenteredPixelBorder(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.47, 500, 750, 5, WHITE);
    DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * h4, 200, 200, BLACK);
    { // Level up
        DrawCenteredOutlinedText("Level Up", UI_RIGHT_CENTER, SCREEN_HEIGHT * h2, 24, WHITE, Fade(RAYWHITE, 0.5));
        int bar_width = 300;
        int bar_height = 25;
        float fill_percentage = tutorial.bar_fill;
        Rectangle exp_bar = { UI_RIGHT_CENTER - bar_width/2, SCREEN_HEIGHT * h3, bar_width, bar_height };
        DrawProgressBarRec(PROGRESS_BAR_ROUNDED_EMPTY_WIDE, exp_bar, 1, 1);
        DrawProgressBarRec(PROGRESS_BAR_ROUNDED_BLUE_WIDE, exp_bar, fill_percentage, 1);
        DrawText("Exp", UI_RIGHT_CENTER - 140, SCREEN_HEIGHT * 0.23, 24, WHITE);

        Rectangle dest_left = { (UI_RIGHT_CENTER - 24) - 53, SCREEN_HEIGHT * 0.48, 48, 48 };
        Color color1 = tutorial.cycle == 3 ? GRAY : WHITE;
        DrawTexturePro(keyboard_buttons, keyboard_keys[55], dest_left, (Vector2) { 0 }, 0.0f, color1);

        Color color2 = tutorial.cycle == 1 || tutorial.cycle == 2 ? GRAY : WHITE;
        Rectangle dest_right = { (UI_RIGHT_CENTER - 24) + 53, SCREEN_HEIGHT * 0.48, 48, 48 };
        DrawTexturePro(keyboard_buttons, keyboard_keys[56], dest_right, (Vector2) { 0 }, 0.0f, color2);

        Color color3 = tutorial.cycle == 0 ? GRAY : WHITE;
        Rectangle dest_enter = { (UI_RIGHT_CENTER - 24), SCREEN_HEIGHT * 0.48, 48, 48 };
        DrawTexturePro(keyboard_buttons, keyboard_keys[53], dest_enter, (Vector2) { 0 }, 0.0f, color3);

        DrawCenteredPixelBorder(UI_RIGHT_CENTER, SCREEN_HEIGHT * h4, 200, 200, 5, WHITE);

        DrawCenteredPixelBorder(UI_RIGHT_CENTER - 50,   SCREEN_HEIGHT * 0.42, 40, 55, 5, tutorial.cycle == 0 ? RED : WHITE);
        DrawCenteredPixelBorder(UI_RIGHT_CENTER,        SCREEN_HEIGHT * 0.42, 40, 55, 5, tutorial.cycle == 1 || tutorial.cycle == 3 ? RED : WHITE);
        DrawCenteredPixelBorder(UI_RIGHT_CENTER + 50,   SCREEN_HEIGHT * 0.42, 40, 55, 5, tutorial.cycle == 2 ? RED : WHITE);

        DrawCenteredMultilineText("Ganhe experiência\nderrotando inimigos.\n\nUse as setas para\nescolher uma melhoria e\nconfirme com enter.", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.68, 34, WHITE);
    }

    DrawCenteredText("Pressione Enter ou Backspace para voltar ao menu principal.", GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.93, 48, WHITE);

    

    DrawEffects(RENDERING_ORDER_BEFORE_SHIP);

    DrawShip(&tutorial.ship_movement);
    DrawShip(&tutorial.ship_special);
    UpdateAndDrawScreenEffects();
    EndDrawing();
}