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

typedef struct ShipSelectMenu {
	int option;
    Ship ship;
	
    Vector2 select_pos1;
	Vector2 select_pos2;
	Vector2 select_pos3;
    Vector2 select_pos4;
    Vector2 select_pos5;

    bool is_ship_selected;
} ShipSelectMenu;

static ShipSelectMenu ship_menu;

void InitSelectMenu() {

    ship_menu.option = ORION;

    ship_menu.select_pos1 = (Vector2){ GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.4 };

    ship_menu.select_pos2 = (Vector2){ GAME_SCREEN_CENTER - 0.2 * GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.6 };
    ship_menu.select_pos3 = (Vector2){ GAME_SCREEN_CENTER + 0.2 * GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.6 };

    ship_menu.select_pos4 = (Vector2){ GAME_SCREEN_CENTER + 0.4 * GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.5 };
    ship_menu.select_pos5 = (Vector2){ GAME_SCREEN_CENTER - 0.4 * GAME_SCREEN_WIDTH, SCREEN_HEIGHT * 0.5 };
    

    ship_menu.ship.draw_size = (Vector2){ DRAW_WH, DRAW_WH };
    ship_menu.ship.direction = CENTER;
    ship_menu.ship.color = WHITE;
	ship_menu.ship.alpha = 1.0f;
	ship_menu.is_ship_selected = false;

	InitBackground(BACKGROUND_SELECT_SHIP, WHITE, STRETCH_TO_SCREEN, 0.7f, 100.0f);
	InitFadeInEffect(1.5f, BLACK, 1.0f);
	TriggerPilotAnimation(5.0f);
}

void UpdateShipSelectMenu() {
    UpdateBackground();
    UpdateShip(&ship_menu.ship);

    if (ship_menu.is_ship_selected) {
        if (GetPlayerShip()      == ORION)         ship_menu.select_pos1.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == AUREA)         ship_menu.select_pos2.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == NEBULA)        ship_menu.select_pos3.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == PUDDLE_JUMPER) ship_menu.select_pos4.y -= 1500 * GetFrameTime();
        else if (GetPlayerShip() == VOID)          ship_menu.select_pos5.y -= 1500 * GetFrameTime();
        
		if (UpdateTimer()) ChangeScene(GAME);
    }

    if (!ship_menu.is_ship_selected) {
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            ship_menu.option = (ship_menu.option - 1 + PLAYABLE_SHIPS) % PLAYABLE_SHIPS;
			TriggerPilotAnimation(5.0f);
        }
        else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            ship_menu.option = (ship_menu.option + 1) % PLAYABLE_SHIPS;
            TriggerPilotAnimation(5.0f);
        }
        else if (IsKeyPressed(KEY_ENTER)) {
            ship_menu.is_ship_selected = true;
            SetPlayerShip(ship_menu.option);
            InitTimer(2.0f);
			InitFadeOutEffect(2.1f, BLACK, GetCurrentScreenEffectAlpha());
        }
    }
}

#pragma region LEFT_SIDE_INFO

static void DrawPilotHead(int pos_x, int pos_y) {
    int scale = 16;
    float res_scale = 8 * scale;
    DrawRectangle(UI_LEFT_CENTER - res_scale / 2.0f, SCREEN_HEIGHT * 0.2f - res_scale / 2, res_scale, res_scale, BLACK);
    DrawPilot(ship_menu.option, pos_x, pos_y, scale, WHITE);
}

static void DrawLeftSideInfo() {

    DrawRectangle(0, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);

    int border_thick = 5;
    int margin_offset = border_thick * 4;

    DrawCenteredPixelBorder(UI_LEFT_CENTER, SCREEN_HEIGHT / 2, UI_WIDTH - margin_offset, SCREEN_HEIGHT - margin_offset, border_thick, WHITE);

    DrawCenteredOutlinedText(GetPilotName(ship_menu.option), UI_LEFT_CENTER, (int)(SCREEN_HEIGHT * 0.10f), 40, GOLD, Fade(GOLD, 0.3f));

    DrawPilotHead(UI_LEFT_CENTER, SCREEN_HEIGHT * 0.2);

    DrawCenteredPixelBorder(UI_LEFT_CENTER, SCREEN_HEIGHT * 0.4, UI_WIDTH - border_thick * 16, SCREEN_HEIGHT * 0.2, border_thick, WHITE);

    DrawCenteredMultilineText(GetPilotPresentation(ship_menu.option), UI_LEFT_CENTER, SCREEN_HEIGHT * 0.4, 30, WHITE);
}

#pragma endregion LEFT_SIDE_INFO

#pragma region RIGHT_SIDE_INFO
static void DrawRightSideInfo() {
    DrawRectangle(GAME_SCREEN_END, 0, UI_WIDTH, SCREEN_HEIGHT, BLACK);

    int border_width = 5;

    DrawPixelBorder(GAME_SCREEN_END + border_width * 2, border_width * 2, UI_WIDTH - border_width * 4, SCREEN_HEIGHT - border_width * 4, 5, WHITE);

    DrawCenteredOutlinedText("Nave", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.1f, 40, WHITE, Fade(RAYWHITE, 0.5f));

    DrawCenteredText(GetShipName(ship_menu.option), UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.15f, 40, GetShipColor(ship_menu.option));

    DrawCenteredOutlinedText("Abilidade especial", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.3f, 40, WHITE, Fade(RAYWHITE, 0.5f));

    DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.365, UI_WIDTH * 0.7, SCREEN_HEIGHT*0.05f, Fade(WHITE, 0.2f));

    DrawCenteredText(GetShipSpecial(ship_menu.option), UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.35f, 35, WHITE);

    DrawCenteredOutlinedText("Descrição", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.45, 40, WHITE, Fade(RAYWHITE, 0.5f));

    DrawCenteredMultilineText(GetShipSpecialDescription(ship_menu.option), UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.55f, 30, WHITE);
}

#pragma endregion RIGHT_SIDE_INFO

void DrawSelectMenu() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawBackground();

    Color ship_1 = (ship_menu.option == ORION)  ? RED : GRAY;
    Color ship_2 = (ship_menu.option == AUREA)  ? RED : GRAY;
    Color ship_3 = (ship_menu.option == NEBULA) ? RED : GRAY;
	Color ship_4 = (ship_menu.option == PUDDLE_JUMPER) ? RED : GRAY;
    Color ship_5 = (ship_menu.option == VOID) ? RED : GRAY;

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

    DrawLeftSideInfo();

    DrawRightSideInfo();

    UpdateAndDrawScreenEffects();

    EndDrawing();
}