// select_ship.c

#include "select_ship.h"
#include "common.h"
#include "player.h"
#include "scene_manager.h"
#include "background.h"
#include "scene_draw_effects.h"
#include "raymath.h"
#include "timer.h"
#include "ship.h"
#include "pilot.h"
#include "draw_utils.h"
#include "ship_references.h"
#include "input.h"
#include "dev_interface_left_ui.h"

#include "left_ui.h"

typedef struct ShipSelectMenu {
    Ship ship;
	
    Vector2 select_pos1;
	Vector2 select_pos2;
	Vector2 select_pos3;
    Vector2 select_pos4;
    Vector2 select_pos5;

    bool is_ship_selected;
	bool is_backspace_pressed;
} ShipSelectMenu;

static ShipSelectMenu ship_menu;
static const float pilot_speech_timer = 3.0f;

void InitSelectMenu() {

    SetPlayerShip(ORION);
    SetTopPilotDefault();
    InitLeftUIPanel();

    ship_menu.select_pos1 = (Vector2){ GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.4 };

    ship_menu.select_pos2 = (Vector2){ GAME_SCREEN_CENTER - 0.2 * GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.6 };
    ship_menu.select_pos3 = (Vector2){ GAME_SCREEN_CENTER + 0.2 * GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.6 };

    ship_menu.select_pos4 = (Vector2){ GAME_SCREEN_CENTER + 0.4 * GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.5 };
    ship_menu.select_pos5 = (Vector2){ GAME_SCREEN_CENTER - 0.4 * GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.5 };
    
    ship_menu.ship.draw_size = (Vector2){ DRAW_WH, DRAW_WH };
    ship_menu.ship.direction = CENTER;
    ship_menu.ship.color = WHITE;
	ship_menu.ship.alpha = 1.0f;
    ship_menu.ship.should_render = true;
	ship_menu.is_ship_selected = false;
	ship_menu.is_backspace_pressed = false;

	InitBackground(BACKGROUND_SELECT_SHIP, WHITE, STRETCH_TO_SCREEN, 0.7f, 100.0f);
	InitFadeInEffect(1.5f, BLACK, 1.0f);
    SetTopPilotText(GetPilotPresentation(GetPlayerShip()));
    UI_TriggerTopPilotSpeech(pilot_speech_timer);
}

static int GetPrevShip() {
    if (GetPlayerShip() == ORION) return AUREA;
    if (GetPlayerShip() == AUREA) return VOID;
    if (GetPlayerShip() == VOID) return PUDDLE_JUMPER;
    if (GetPlayerShip() == PUDDLE_JUMPER) return NEBULA;
    if (GetPlayerShip() == NEBULA) return ORION;
    return ORION;
}

static int GetNextShip() {
    if (GetPlayerShip() == ORION) return NEBULA;
    if (GetPlayerShip() == AUREA) return ORION;
    if (GetPlayerShip() == VOID) return AUREA;
    if (GetPlayerShip() == PUDDLE_JUMPER) return VOID;
    if (GetPlayerShip() == NEBULA) return PUDDLE_JUMPER;
    return ORION;
}

void UpdateShipSelectMenu() {
    UpdateBackground();
    UpdateShip(&ship_menu.ship);
    UpdatePilot();

    if (ship_menu.is_ship_selected) {
        if (GetPlayerShip()      == ORION)         ship_menu.select_pos1.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == AUREA)         ship_menu.select_pos2.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == NEBULA)        ship_menu.select_pos3.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == PUDDLE_JUMPER) ship_menu.select_pos4.y -= 1500 * GetFrameTime();
        else if (GetPlayerShip() == VOID)          ship_menu.select_pos5.y -= 1500 * GetFrameTime();
        
        if (UpdateTimer()) {
            ChangeScene(GAME);
            ClearTopPilotText();
        }
    }

	if (ship_menu.is_backspace_pressed) {
		if (UpdateTimer()) {
            ClearTopPilotText();
			ChangeScene(START);
		}
	}

    if (!ship_menu.is_ship_selected) {
        if (IsInputLeftPressed()) {
            SetPlayerShip(GetPrevShip());
			SetTopPilotText(GetPilotPresentation(GetPlayerShip()));
			SetTopPilotDefault();
            TriggerTopPilotAnimation(pilot_speech_timer);
        }
        else if (IsInputRightPressed()) {
            SetPlayerShip(GetNextShip());
            SetTopPilotText(GetPilotPresentation(GetPlayerShip()));
            SetTopPilotDefault();
            TriggerTopPilotAnimation(pilot_speech_timer);
        }
        else if (IsConfirmButtonPressed()) {
            ship_menu.is_ship_selected = true;
            TriggerTopPilotAnimation(0.0f);
            InitTimer(2.0f);
            InitFadeOutEffect(2.1f, BLACK, GetCurrentScreenEffectAlpha());
        }
        else if (IsReturnButtonPressed()) {
            ship_menu.is_backspace_pressed = true;
            SetPlayerShip(ORION);
            InitTimer(2.0f);
            InitFadeOutEffect(2.1f, BLACK, GetCurrentScreenEffectAlpha());
        }
    }
}

#pragma region RIGHT_SIDE_INFO
static void DrawRightSideInfo() {
    DrawRectangle(GAME_SCREEN_END, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);

    int border_width = 5;

    DrawPixelBorder(GAME_SCREEN_END + border_width * 2, border_width * 2, UI_WIDTH - border_width * 4, SCREEN_HEIGHT - border_width * 4, 5, WHITE);

    DrawCenteredOutlinedText("Nave", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.1f, 40, WHITE, Fade(RAYWHITE, 0.5f));

    DrawCenteredText(GetShipName(GetPlayerShip()), UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.15f, 40, GetShipColor(GetPlayerShip()));

    DrawCenteredOutlinedText("Abilidade especial", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.3f, 40, WHITE, Fade(RAYWHITE, 0.5f));

    DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.365, UI_WIDTH * 0.75, SCREEN_HEIGHT*0.05f, Fade(WHITE, 0.2f));

    DrawCenteredText(GetShipSpecial(GetPlayerShip()), UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.35f, 35, WHITE);

    DrawCenteredOutlinedText("Descrição", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.45, 40, WHITE, Fade(RAYWHITE, 0.5f));

    DrawCenteredMultilineText(GetShipSpecialDescription(GetPlayerShip()), UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.55f, 30, WHITE);
}

#pragma endregion RIGHT_SIDE_INFO

void DrawSelectMenu() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawBackground();

    Color ship_1 = (GetPlayerShip() == ORION)  ? RED : GRAY;
    Color ship_2 = (GetPlayerShip() == AUREA)  ? RED : GRAY;
    Color ship_3 = (GetPlayerShip() == NEBULA) ? RED : GRAY;
	Color ship_4 = (GetPlayerShip() == PUDDLE_JUMPER) ? RED : GRAY;
    Color ship_5 = (GetPlayerShip() == VOID) ? RED : GRAY;

    ship_menu.ship.direction = CENTER;
    
    ship_menu.ship.id = ORION;
    ship_menu.ship.position = ship_menu.select_pos1;
    DrawShip(&ship_menu.ship);
    
    ship_menu.ship.id = AUREA;
    ship_menu.ship.position = ship_menu.select_pos2;
    DrawShip(&ship_menu.ship);

    ship_menu.ship.id = NEBULA;
    ship_menu.ship.position = ship_menu.select_pos3;
    DrawShip(&ship_menu.ship);

    ship_menu.ship.id = PUDDLE_JUMPER;
    ship_menu.ship.position = ship_menu.select_pos4;
    DrawShip(&ship_menu.ship);

	ship_menu.ship.id = VOID;
	ship_menu.ship.position = ship_menu.select_pos5;
	DrawShip(&ship_menu.ship);

    DrawCenteredText(GetShipName(ORION), ship_menu.select_pos1.x, ship_menu.select_pos1.y + 100, 20, ship_1);
    DrawCenteredText(GetShipName(AUREA), ship_menu.select_pos2.x, ship_menu.select_pos2.y + 100, 20, ship_2);
    DrawCenteredText(GetShipName(NEBULA), ship_menu.select_pos3.x, ship_menu.select_pos3.y + 100, 20, ship_3);
    DrawCenteredText(GetShipName(PUDDLE_JUMPER), ship_menu.select_pos4.x, ship_menu.select_pos4.y + 100, 20, ship_4);
    DrawCenteredText(GetShipName(VOID), ship_menu.select_pos5.x, ship_menu.select_pos5.y + 100, 20, ship_5);

    { // Left UI panel
        DrawLeftUIPanel();
    }

    DrawRightSideInfo();

    UpdateAndDrawScreenEffects();

    EndDrawing();
}