// select_ship.c

#include "select_ship.h"
#include "common.h"
#include "player.h"
#include "scene_manager.h"
#include "background.h"
#include "raymath.h"

#include "ship.h"

// Constants
float select_background_draw_scale = 2.0f;
float select_transition_duration_s = 1.5f;
Vector2 select_ship_draw_size = { DRAW_WH, DRAW_WH };

// Others
Background select_background;

int ship_select_menu_option;

Ship select_ship_draw;

bool select_alpha_flag;
bool select_is_ship_selected;
bool select_transition_complete;

float select_transition_timer;
float select_text_alpha;

Vector2 select_pos1;
Vector2 select_pos2;
Vector2 select_pos3;

void InitSelectMenu() {
    select_alpha_flag = true;
    select_is_ship_selected = false;
	select_transition_complete = false;
    
    select_transition_timer = 0.0f;

    select_text_alpha = 0.0f;

    ship_select_menu_option = ORION;
    
    select_background.color = WHITE;
    select_background.position_x = 0;
    select_background.position_y = 0;
    select_background.alpha = 0.0f;

    select_pos1 = (Vector2){ SCREEN_WIDTH * 0.25, 300 };
    select_pos2 = (Vector2){ SCREEN_WIDTH / 2, 300 };
    select_pos3 = (Vector2){ SCREEN_WIDTH * 0.75, 300 };

    select_ship_draw.draw_size = select_ship_draw_size;
    select_ship_draw.direction = CENTER;
    select_ship_draw.color = WHITE;
    select_ship_draw.alpha = 0.0f;
}

void UpdateMenuAfterShipSelection() {

    select_transition_timer += GetFrameTime();

    if (ship_select_menu_option == AUREA) {
        if (select_pos1.y >= select_ship_draw.draw_size.y * -1) {
            select_pos1.y -= 1000 * GetFrameTime();
        }
    }
    else if (ship_select_menu_option == ORION) {
        if (select_pos2.y >= select_ship_draw.draw_size.y * -1) {
            select_pos2.y -= 1000 * GetFrameTime();
        }
    }
    else if (ship_select_menu_option == NEBULA) {
        if (select_pos3.y >= select_ship_draw.draw_size.y * -1) {
            select_pos3.y -= 1000 * GetFrameTime();
        }
    }

    if (select_ship_draw.alpha > 0) {
        select_ship_draw.alpha -= 1.0f * GetFrameTime();
		select_text_alpha -= 1.0f * GetFrameTime();
		select_background.alpha -= 1.0f * GetFrameTime();
    }

    if (select_transition_timer >= select_transition_duration_s) {
		select_transition_complete = true;
    }
}


void UpdateShipSelectMenu() {

    UpdateShip(&select_ship_draw);
    select_background.position_y = (float)fmod(select_background.position_y + 100 * GetFrameTime(), select_background.texture.height * select_background_draw_scale);

    if (select_transition_complete) {
        select_transition_complete = false;
        ChangeScene(GAME);
        return;
    }

    if (select_is_ship_selected) {
        UpdateMenuAfterShipSelection();
        return;
    }

    if (select_background.alpha < 0.8f) select_background.alpha += 0.25f * GetFrameTime();
    if (select_ship_draw.alpha < 1) select_ship_draw.alpha += 0.75f * GetFrameTime();
    if (select_text_alpha < 1) select_text_alpha += 0.25f * GetFrameTime();

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) ship_select_menu_option = (ship_select_menu_option - 1 + PLAYABLE_SHIPS) % PLAYABLE_SHIPS;
    else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) ship_select_menu_option = (ship_select_menu_option + 1) % PLAYABLE_SHIPS;
    else if (IsKeyPressed(KEY_ENTER)) {
        select_is_ship_selected = true;
        SetPlayerShip(ship_select_menu_option);
    }
}

void DrawSelectMenuBackground(void) {
    if (select_alpha_flag) {
        select_background.alpha += 0.1f * GetFrameTime();
        if (select_background.alpha >= 0.8f) select_alpha_flag = false;
    }
    else {
        select_background.alpha -= 0.1f * GetFrameTime();
        if (select_background.alpha <= 0.6f) select_alpha_flag = true;
    }
    float rotation = 0.0f;
    Vector2 position1 = { select_background.position_x, select_background.position_y };
    Vector2 position2 = { select_background.position_x, select_background.position_y - select_background.texture.height * select_background_draw_scale };
    DrawTextureEx(select_background.texture, position1, rotation, select_background_draw_scale, Fade(WHITE, select_background.alpha));
    DrawTextureEx(select_background.texture, position2, rotation, select_background_draw_scale, Fade(WHITE, select_background.alpha));
}

void DrawSelectMenu() {
    BeginDrawing();

    ClearBackground(BLACK);

    DrawSelectMenuBackground();

    Color ship_1 = (ship_select_menu_option == AUREA) ? Fade(RED, select_text_alpha) : Fade(GRAY, select_text_alpha);
    Color ship_2 = (ship_select_menu_option == ORION) ? Fade(RED, select_text_alpha) : Fade(GRAY, select_text_alpha);
    Color ship_3 = (ship_select_menu_option == NEBULA) ? Fade(RED, select_text_alpha) : Fade(GRAY, select_text_alpha);
    
    select_ship_draw.direction = CENTER;
    
    select_ship_draw.id = AUREA;
    select_ship_draw.position = select_pos1;
    DrawShip(&select_ship_draw);
    
    select_ship_draw.id = ORION;
    select_ship_draw.position = select_pos2;
    DrawShip(&select_ship_draw);

    select_ship_draw.id = NEBULA;
    select_ship_draw.position = select_pos3;
    DrawShip(&select_ship_draw);

    char* left_ship = "Aurea";
    char* middle_ship = "Orion";
    char* right_ship = "Nebula";

    DrawText(left_ship, (int)(SCREEN_WIDTH*0.25f - MeasureText(left_ship, 20) / 2.0f), 400, 20, ship_1);
    DrawText(middle_ship, (int)(SCREEN_WIDTH/2.0f - MeasureText(middle_ship, 20) / 2.0f), 400, 20, ship_2);
    DrawText(right_ship, (int)(SCREEN_WIDTH*0.75f - MeasureText(right_ship, 20) / 2.0f), 400, 20, ship_3);
    EndDrawing();
}

void LoadSelectMenuTextures() {
    select_background.texture = LoadTexture("shipselectbg.png");
}

void UnloadSelectMenuTextures() {
    UnloadTexture(select_background.texture);
}