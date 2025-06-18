// main_menu.c

#include "main_menu.h"
#include "common.h"
#include "scene_manager.h"
#include "background.h"
#include "draw_utils.h"

#include "raymath.h"

// Background
Background background;
float main_background_draw_scale = 2.0f;
bool main_alpha_flag = true;

// Option
MenuOption ship_select_menu_option;

// Transition variables
float main_transition_duration_s = 1.5f;
float select_transition_timer = 0.0f;
bool is_in_transition = false;
bool is_fading_in = false;
bool main_transition_complete = false;

float select_text_alpha = 0.0f;
float transition_bg_alpha = 0.0f;

Scene select_next_scene;

void InitMenu() {
    ship_select_menu_option = MENU_OPTION_START;

    background.color = WHITE;
    background.position_x = 0;
    background.position_y = 0;
    background.alpha = 0.0f;

    is_in_transition = true;
    is_fading_in = true;
    select_transition_timer = 0.0f;
    main_transition_complete = false;

    select_text_alpha = 0.0f;
    
}

void UpdateMenuTransition() {
    select_transition_timer += GetFrameTime();
    float progress = fmin(select_transition_timer / main_transition_duration_s, 1.0f);

    if (is_fading_in) {
        select_text_alpha = progress;
        background.alpha = progress * 0.8f;

        if (progress >= 1.0f) {
            is_in_transition = false;
            select_text_alpha = 1.0f;
            background.alpha = 0.8f;
        }
    }
    else {
        select_text_alpha = 1.0f - progress;
        background.alpha = transition_bg_alpha * (1.0f - progress);

        float scroll_speed = 100.0f + (progress * progress * 1500.0f);
        background.position_y = fmod(background.position_y + scroll_speed * GetFrameTime(), 1200);

        if (progress >= 1.0f) {
            main_transition_complete = true;
        }
    }
}

void UpdateMainMenuBackground() {
    if (main_alpha_flag) {
        background.alpha += 0.2f * GetFrameTime();
        if (background.alpha >= 0.8f) main_alpha_flag = false;
    }
    else {
        background.alpha -= 0.2f * GetFrameTime();
        if (background.alpha <= 0.4f) main_alpha_flag = true;
    }
}

void UpdateMenu() {
    if (main_transition_complete) {
        ChangeScene(select_next_scene);
        return;
    }

    if (is_in_transition) UpdateMenuTransition();
    
    UpdateMainMenuBackground();
    background.position_y = fmod(background.position_y + 100 * GetFrameTime(), background.texture.height * main_background_draw_scale);

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) ship_select_menu_option = (ship_select_menu_option - 1 + MENU_OPTION_COUNT) % MENU_OPTION_COUNT;
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) ship_select_menu_option = (ship_select_menu_option + 1) % MENU_OPTION_COUNT;
    else if (IsKeyPressed(KEY_ENTER)) {
        if (ship_select_menu_option == MENU_OPTION_START) select_next_scene = SELECT_SHIP;
        else if (ship_select_menu_option == MENU_OPTION_RANKING) select_next_scene = RANKING;
        else if (ship_select_menu_option == MENU_OPTION_EXIT) select_next_scene = EXIT;

        is_in_transition = true;
        is_fading_in = false;
        select_transition_timer = 0.0f;
        transition_bg_alpha = background.alpha;
    }
}

void DrawMenuBackground() {
    float rotation = 0.0f;
	Color color = Fade(WHITE, background.alpha);
	Vector2 position1 = { 0, (int)background.position_y };
	Vector2 position2 = { 0, (int)background.position_y - background.texture.height * main_background_draw_scale };
    DrawTextureEx(background.texture, position1, rotation, main_background_draw_scale, color);
    DrawTextureEx(background.texture, position2, rotation, main_background_draw_scale, color);
}

void DrawMenu() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawMenuBackground();

    Color title_color = Fade(RAYWHITE, select_text_alpha);

	DrawOutlinedText("SPACE INVADERS", SCREEN_WIDTH / 2 - MeasureText("SPACE INVADERS", 50) / 2, 300, 50, title_color, Fade(RAYWHITE, select_text_alpha - 0.5f));

    Color color_game_start = (ship_select_menu_option == MENU_OPTION_START) ? Fade(RED, select_text_alpha) : Fade(GRAY, select_text_alpha);
    Color color_ranking = (ship_select_menu_option == MENU_OPTION_RANKING) ? Fade(RED, select_text_alpha) : Fade(GRAY, select_text_alpha);
    Color color_credits = (ship_select_menu_option == MENU_OPTION_CREDITS) ? Fade(RED, select_text_alpha) : Fade(GRAY, select_text_alpha);
    Color color_exit = (ship_select_menu_option == MENU_OPTION_EXIT) ? Fade(RED, select_text_alpha) : Fade(GRAY, select_text_alpha);

	DrawText("Iniciar Jogo", SCREEN_WIDTH / 2 - MeasureText("Iniciar Jogo", 30) / 2, 400, 30, color_game_start);
	DrawText("Placar", SCREEN_WIDTH / 2 - MeasureText("Placar", 30) / 2, 450, 30, color_ranking);
	DrawText("Agradecimentos", SCREEN_WIDTH / 2 - MeasureText("Agradecimentos", 30) / 2, 500, 30, color_credits);
	DrawText("Sair", SCREEN_WIDTH / 2 - MeasureText("Sair", 30) / 2, 550, 30, color_exit);

    EndDrawing();
}

void LoadMenuBackgroundTexture() {
    background.texture = LoadTexture("menubg.png");
}

void UnloadMenuBackgroundTexture() {
    UnloadTexture(background.texture);
}