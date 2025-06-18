// main_menu.c

#include "main_menu.h"
#include "common.h"
#include "scene_manager.h"
#include "background.h"
#include "draw_utils.h"

#include "raymath.h"

// Background
Background main_background;
float main_background_draw_scale = 2.0f;
bool main_alpha_flag = true;

// Option
MenuOption main_menu_option;

// Transition variables
float main_transition_duration_s = 1.5f;
float main_transition_timer = 0.0f;
bool is_in_transition = false;
bool is_fading_in = false;
bool main_transition_complete = false;

float main_text_alpha = 0.0f;
float transition_bg_alpha = 0.0f;

Scene select_next_scene;

void InitMenu() {
    main_menu_option = MENU_OPTION_START;

    main_background.color = WHITE;
    main_background.position_x = 0;
    main_background.position_y = 0;
    main_background.alpha = 0.0f;

    is_in_transition = true;
    is_fading_in = true;
    main_transition_timer = 0.0f;
    main_transition_complete = false;

    main_text_alpha = 0.0f;
}

void UpdateMenuTransition() {
    main_transition_timer += GetFrameTime();
    float progress = fmin(main_transition_timer / main_transition_duration_s, 1.0f);

    if (is_fading_in) {
        main_text_alpha = progress;
        main_background.alpha = progress * 0.8f;

        if (progress >= 1.0f) {
            is_in_transition = false;
            main_text_alpha = 1.0f;
            main_background.alpha = 0.8f;
        }
    }
    else {
        main_text_alpha = 1.0f - progress;
        main_background.alpha = transition_bg_alpha * (1.0f - progress);

        float scroll_speed = 100.0f + (progress * progress * 1500.0f);
        main_background.position_y = fmod(main_background.position_y + scroll_speed * GetFrameTime(), 1200);

        if (progress >= 1.0f) {
            main_transition_complete = true;
        }
    }
}

void UpdateMainMenuBackground() {
    if (main_alpha_flag) {
        main_background.alpha += 0.2f * GetFrameTime();
        if (main_background.alpha >= 0.8f) main_alpha_flag = false;
    }
    else {
        main_background.alpha -= 0.2f * GetFrameTime();
        if (main_background.alpha <= 0.4f) main_alpha_flag = true;
    }
}

void UpdateMenu() {
    if (main_transition_complete) {
        ChangeScene(select_next_scene);
        return;
    }

    if (is_in_transition) UpdateMenuTransition();
    
    UpdateMainMenuBackground();
    main_background.position_y = fmod(main_background.position_y + 100 * GetFrameTime(), main_background.texture.height * main_background_draw_scale);

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) main_menu_option = (main_menu_option - 1 + MENU_OPTION_COUNT) % MENU_OPTION_COUNT;
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) main_menu_option = (main_menu_option + 1) % MENU_OPTION_COUNT;
    else if (IsKeyPressed(KEY_ENTER)) {
        if (main_menu_option == MENU_OPTION_START) select_next_scene = SELECT_SHIP;
        else if (main_menu_option == MENU_OPTION_RANKING) select_next_scene = RANKING;
        else if (main_menu_option == MENU_OPTION_EXIT) select_next_scene = EXIT;

        is_in_transition = true;
        is_fading_in = false;
        main_transition_timer = 0.0f;
        transition_bg_alpha = main_background.alpha;
    }
}

void DrawMenuBackground() {
    float rotation = 0.0f;
	Color color = Fade(WHITE, main_background.alpha);
	Vector2 position1 = { 0, (int)main_background.position_y };
	Vector2 position2 = { 0, (int)main_background.position_y - main_background.texture.height * main_background_draw_scale };
    DrawTextureEx(main_background.texture, position1, rotation, main_background_draw_scale, color);
    DrawTextureEx(main_background.texture, position2, rotation, main_background_draw_scale, color);
}

void DrawMenu() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawMenuBackground();

    Color title_color = Fade(RAYWHITE, main_text_alpha);

	DrawOutlinedText("SPACE INVADERS", SCREEN_WIDTH / 2 - MeasureText("SPACE INVADERS", 50) / 2, 300, 50, title_color, Fade(RAYWHITE, main_text_alpha - 0.5f));

    Color color_game_start = (main_menu_option == MENU_OPTION_START) ? Fade(RED, main_text_alpha) : Fade(GRAY, main_text_alpha);
    Color color_ranking = (main_menu_option == MENU_OPTION_RANKING) ? Fade(RED, main_text_alpha) : Fade(GRAY, main_text_alpha);
    Color color_credits = (main_menu_option == MENU_OPTION_CREDITS) ? Fade(RED, main_text_alpha) : Fade(GRAY, main_text_alpha);
    Color color_exit = (main_menu_option == MENU_OPTION_EXIT) ? Fade(RED, main_text_alpha) : Fade(GRAY, main_text_alpha);

	DrawText("Iniciar Jogo", SCREEN_WIDTH / 2 - MeasureText("Iniciar Jogo", 30) / 2, 400, 30, color_game_start);
	DrawText("Placar", SCREEN_WIDTH / 2 - MeasureText("Placar", 30) / 2, 450, 30, color_ranking);
	DrawText("Agradecimentos", SCREEN_WIDTH / 2 - MeasureText("Agradecimentos", 30) / 2, 500, 30, color_credits);
	DrawText("Sair", SCREEN_WIDTH / 2 - MeasureText("Sair", 30) / 2, 550, 30, color_exit);

    EndDrawing();
}

void LoadMenuBackgroundTexture() {
    main_background.texture = LoadTexture("menubg.png");
}

void UnloadMenuBackgroundTexture() {
    UnloadTexture(main_background.texture);
}