// select_ship.c

#include "select_ship.h"
#include "common.h"
#include "player.h"
#include "scene_manager.h"
#include "background.h"
#include "scene_draw_utils.h"
#include "raymath.h"
#include "timer.h"
#include "ship.h"


typedef struct ShipSelectMenu {
	int option;
    Ship ship;
	
    Vector2 select_pos1;
	Vector2 select_pos2;
	Vector2 select_pos3;
    Vector2 select_pos4;

    bool is_ship_selected;
} ShipSelectMenu;

static ShipSelectMenu ship_menu;

void InitSelectMenu() {

    ship_menu.option = ORION;

    ship_menu.select_pos1 = (Vector2){ SCREEN_WIDTH * 0.2, SCREEN_HEIGHT * 0.6 };
    ship_menu.select_pos2 = (Vector2){ SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.4 };
    ship_menu.select_pos3 = (Vector2){ SCREEN_WIDTH * 0.6, SCREEN_HEIGHT * 0.6 };
    ship_menu.select_pos4 = (Vector2){ SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.4 };

    ship_menu.ship.draw_size = (Vector2){ DRAW_WH, DRAW_WH };
    ship_menu.ship.direction = CENTER;
    ship_menu.ship.color = WHITE;
	ship_menu.ship.alpha = 1.0f;
	ship_menu.is_ship_selected = false;

	InitBackground(BACKGROUND_SELECT_SHIP, WHITE, STRETCH_TO_SCREEN, 0.7f, 100.0f);
	InitFadeInEffect(1.5f, BLACK, 1.0f);
}

void UpdateShipSelectMenu() {
    UpdateBackground();
    UpdateShip(&ship_menu.ship);

    if (ship_menu.is_ship_selected) {
        if (GetPlayerShip()      == AUREA)         ship_menu.select_pos1.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == ORION)         ship_menu.select_pos2.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == NEBULA)        ship_menu.select_pos3.y -= 1500 * GetFrameTime();
		else if (GetPlayerShip() == PUDDLE_JUMPER) ship_menu.select_pos4.y -= 1500 * GetFrameTime();
        
		if (UpdateTimer()) ChangeScene(GAME);
    }

    if (!ship_menu.is_ship_selected) {
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) ship_menu.option = (ship_menu.option - 1 + PLAYABLE_SHIPS) % PLAYABLE_SHIPS;
        else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) ship_menu.option = (ship_menu.option + 1) % PLAYABLE_SHIPS;
        else if (IsKeyPressed(KEY_ENTER)) {
            ship_menu.is_ship_selected = true;
            SetPlayerShip(ship_menu.option);
            InitTimer(2.0f);
			InitFadeOutEffect(2.0f, BLACK, GetCurrentScreenEffectAlpha());
        }
    }
}

void DrawSelectMenu() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawBackground();

    Color ship_1 = (ship_menu.option == AUREA)  ? RED : GRAY;
    Color ship_2 = (ship_menu.option == ORION)  ? RED : GRAY;
    Color ship_3 = (ship_menu.option == NEBULA) ? RED : GRAY;
	Color ship_4 = (ship_menu.option == PUDDLE_JUMPER) ? RED : GRAY;

    ship_menu.ship.direction = CENTER;
    
    ship_menu.ship.id = AUREA;
    ship_menu.ship.position = ship_menu.select_pos1;
    DrawShip(&ship_menu.ship);
    
    ship_menu.ship.id = ORION;
    ship_menu.ship.position = ship_menu.select_pos2;
    DrawShip(&ship_menu.ship);

    ship_menu.ship.id = NEBULA;
    ship_menu.ship.position = ship_menu.select_pos3;
    DrawShip(&ship_menu.ship);

    ship_menu.ship.id = PUDDLE_JUMPER;
    ship_menu.ship.position = ship_menu.select_pos4;
    DrawShip(&ship_menu.ship);

    char* aurea_text = "Aurea";
    char* orion_text = "Orion";
    char* nebula_text = "Nebula";
    char* puddle_jumper_text = "Puddle Jumper";

    DrawText(aurea_text, (int)(ship_menu.select_pos1.x - MeasureText(aurea_text, 20) / 2.0f), ship_menu.select_pos1.y + 100, 20, ship_1);
    DrawText(orion_text, (int)(ship_menu.select_pos2.x - MeasureText(orion_text, 20) / 2.0f), ship_menu.select_pos2.y + 100, 20, ship_2);
    DrawText(nebula_text, (int)(ship_menu.select_pos3.x - MeasureText(nebula_text, 20) / 2.0f), ship_menu.select_pos3.y + 100, 20, ship_3);
    DrawText(puddle_jumper_text, (int)(ship_menu.select_pos4.x - MeasureText(puddle_jumper_text, 20) / 2.0f), ship_menu.select_pos4.y + 100, 20, ship_4);

    UpdateAndDrawScreenEffects();
    EndDrawing();
}