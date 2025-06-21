// main_menu.c

#include "main_menu.h"
#include "common.h"
#include "scene_manager.h"
#include "background.h"
#include "draw_utils.h"
#include "scene_draw_utils.h"
#include "timer.h"

#include "raymath.h"

// Option
typedef struct MainMenu {
	MainMenuOption option;
    bool option_picked;
	Scene next_scene;
    float alpha;
} MainMenu;

static MainMenu main_menu;

void InitMainMenu() {
    main_menu.option = MENU_OPTION_START;
    main_menu.option_picked = false;
	InitBackground(BACKGROUND_STARS, WHITE, STRETCH_TO_SCREEN, 1.0f, 100.0f);
    InitFadeInThenPulse(2.0f, BLACK, 1.0f, 3.0f, 0.0f, 0.4f);
}

void UpdateMainMenu() {
    UpdateBackground();

    if (main_menu.option_picked) {
        float acceleration = 1500.0f;
        SetBackgroundScrollSpeed(GetBackgroundScrollSpeed() + acceleration * GetFrameTime());
		if (UpdateTimer()) {
			ChangeScene(main_menu.next_scene);
		}
        return;
    }

	if (!main_menu.option_picked) {
        if      (IsKeyPressed(KEY_UP)   || IsKeyPressed(KEY_W)) main_menu.option = (main_menu.option - 1 + MENU_OPTION_COUNT) % MENU_OPTION_COUNT;
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) main_menu.option = (main_menu.option + 1) % MENU_OPTION_COUNT;
        else if (IsKeyPressed(KEY_ENTER)) {
            if      (main_menu.option == MENU_OPTION_START)   main_menu.next_scene = SELECT_SHIP;
            else if (main_menu.option == MENU_OPTION_RANKING) main_menu.next_scene = RANKING;
            else if (main_menu.option == MENU_OPTION_EXIT)    main_menu.next_scene = EXIT;
		    // TODO: Implementar agradecimentos
		    main_menu.option_picked = true;
			InitFadeOutEffect(2.0f, BLACK, GetCurrentScreenEffectAlpha());
            InitTimer(2.0f);
        }
    }
}

static void DrawCenteredText(const char* text, int y, int font_size, Color color) {
    int text_width = MeasureText(text, font_size);
    DrawText(text, SCREEN_WIDTH / 2 - text_width / 2, y, font_size, color);
}

void DrawMainMenu() {
    BeginDrawing();
    ClearBackground(BLACK);
	DrawBackground();

    Color color_game_start = (main_menu.option == MENU_OPTION_START)   ? RED : GRAY;
    Color color_ranking =    (main_menu.option == MENU_OPTION_RANKING) ? RED : GRAY;
    Color color_credits =    (main_menu.option == MENU_OPTION_CREDITS) ? RED : GRAY;
    Color color_exit =       (main_menu.option == MENU_OPTION_EXIT)    ? RED : GRAY;


    if (GetCurrentScreenEffectType() == EFFECT_FADING_IN) {
        main_menu.alpha = fabs(GetCurrentScreenEffectAlpha() - 1.0f);
    }
    else if (GetCurrentScreenEffectType() == EFFECT_FADING_OUT) {
        main_menu.alpha -= GetFrameTime() * 0.4f;
    }
    else {
        main_menu.alpha = 1.0f;
	}

    UpdateAndDrawScreenEffects();

    DrawOutlinedText("DISTRESS BEACON", SCREEN_WIDTH  / 2 - MeasureText("DISTRESS BEACON", 50) / 2, SCREEN_HEIGHT * 0.35f, 50, Fade(WHITE, main_menu.alpha), Fade(RAYWHITE, main_menu.alpha - 0.5f));

	DrawCenteredText("Iniciar Jogo",    SCREEN_HEIGHT * 0.45f, 30, Fade(color_game_start, main_menu.alpha));
	DrawCenteredText("Ranking",         SCREEN_HEIGHT * 0.5f, 30, Fade(color_ranking, main_menu.alpha));
	DrawCenteredText("Créditos",        SCREEN_HEIGHT * 0.55f, 30, Fade(color_credits, main_menu.alpha));
	DrawCenteredText("Sair",            SCREEN_HEIGHT * 0.6f, 30, Fade(color_exit, main_menu.alpha));
    
    EndDrawing();
}