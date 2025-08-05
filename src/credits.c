// credits.c

#include "credits.h"
#include "draw_utils.h"
#include "scene_draw_effects.h"
#include "background.h"
#include "input.h"
#include "timer.h"
#include "scene_manager.h"
#include "common.h"
#include "audio_manager.h"

static bool return_to_menu;

void InitCredits(void) {
	return_to_menu = false;
	InitFadeInEffect(1.5f, BLACK, 1.0f);
	InitBackground(BACKGROUND_STARS, WHITE, STRETCH_TO_SCREEN, 0.7f, 100.0f);
}

void UpdateCredits(void) {
	UpdateBackground();

	if (return_to_menu) {
		if (UpdateTimer()) {
			ChangeScene(START);
		}
		return;
	}

	if (IsConfirmButtonPressed() || IsReturnButtonPressed()) {
		PlaySoundFxWithVolumeAndRandomPitch(&sound22, 1, 1, 1);
		return_to_menu = true;
		InitTimer(2.0f);
		InitFadeOutEffect(2.1f, BLACK, GetCurrentScreenEffectAlpha());
	}
}

void DrawCredits(void) {
	BeginDrawing();
	ClearBackground(BLACK);
	DrawBackground();

	float h1 = 0.1;
	float h2 = 0.25;
	float h3 = 0.6;

	int text_size = 32;
	int spacing = SCREEN_HEIGHT * h2;

	DrawCenteredOutlinedText("Agradecimentos", GAME_SCREEN_CENTER, SCREEN_HEIGHT * h1, 64, WHITE, Fade(RAYWHITE, 0.5f));

	{ // Equipe
		DrawCenteredOutlinedText("Equipe", UI_LEFT_CENTER, SCREEN_HEIGHT * h2, 48, WHITE, Fade(RAYWHITE, 0.5f));

		DrawCenteredRectangle(UI_LEFT_CENTER, GAME_SCREEN_HEIGHT * 0.34, 500, 100, BLACK);
		DrawCenteredPixelBorder(UI_LEFT_CENTER, SCREEN_HEIGHT * 0.34, 500, 100, 5, WHITE);
		DrawCenteredText("Luís Santos", UI_LEFT_CENTER, spacing + text_size * 2.5, text_size, WHITE);

		DrawCenteredRectangle(UI_LEFT_CENTER, GAME_SCREEN_HEIGHT * 0.44, 500, 100, BLACK);
		DrawCenteredPixelBorder(UI_LEFT_CENTER, SCREEN_HEIGHT * 0.44, 500, 100, 5, WHITE);
		DrawCenteredText("Anderson Gabriel", UI_LEFT_CENTER, spacing + text_size * 6, text_size, WHITE);

		DrawCenteredRectangle(UI_LEFT_CENTER, GAME_SCREEN_HEIGHT * 0.54, 500, 100, BLACK);
		DrawCenteredPixelBorder(UI_LEFT_CENTER, SCREEN_HEIGHT * 0.54, 500, 100, 5, WHITE);
		DrawCenteredText("Débora Souza", UI_LEFT_CENTER, spacing + text_size * 9.3, text_size, WHITE);
	}

	{ // Orientadores
		DrawCenteredOutlinedText("Orientadores", GAME_SCREEN_CENTER, SCREEN_HEIGHT * h2, 48, WHITE, Fade(RAYWHITE, 0.5f));

		DrawCenteredRectangle(GAME_SCREEN_CENTER, GAME_SCREEN_HEIGHT * 0.34, 500, 100, BLACK);
		DrawCenteredPixelBorder(GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.34, 500, 100, 5, WHITE);
		DrawCenteredText("Pamela Bezerra",			GAME_SCREEN_CENTER, spacing + text_size * 2.5, text_size, WHITE);

		DrawCenteredRectangle(GAME_SCREEN_CENTER, GAME_SCREEN_HEIGHT * 0.44, 500, 100, BLACK);
		DrawCenteredPixelBorder(GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.44, 500, 100, 5, WHITE);
		DrawCenteredText("Tiago Barros",	GAME_SCREEN_CENTER, spacing + text_size * 6, text_size, WHITE);
	}

	{ // Colaboradores
		DrawCenteredOutlinedText("Colaboradores", GAME_SCREEN_CENTER, SCREEN_HEIGHT * h3, 48, WHITE, Fade(RAYWHITE, 0.5f));

		DrawCenteredRectangle(GAME_SCREEN_CENTER, GAME_SCREEN_HEIGHT * 0.7, 500, 100, BLACK);
		DrawCenteredPixelBorder(GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.7, 500, 100, 5, WHITE);
		DrawCenteredText("Carlos Santos", GAME_SCREEN_CENTER, SCREEN_HEIGHT * h3 + text_size * 3, text_size, WHITE);

	}

	{ // Assets
		DrawCenteredOutlinedText("Assets", UI_RIGHT_CENTER, SCREEN_HEIGHT * h2, 48, WHITE, Fade(RAYWHITE, 0.5f));
		
		int left_start = UI_RIGHT_CENTER;
		
		{ // ansdor
			DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.34, 500, 100, BLACK);
			DrawCenteredPixelBorder(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.34, 500, 100, 5, WHITE);
			DrawCenteredText("ansdor", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.3, 38, WHITE);
			DrawCenteredText("Botões de teclado", left_start, SCREEN_HEIGHT * 0.34, text_size, WHITE);
		}

		{ // Gustavo Vituri
			DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.47, 500, 170, BLACK);
			DrawCenteredPixelBorder(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.47, 500, 170, 5, WHITE);
			DrawCenteredText("Gustavo Vituri", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.41, 38, WHITE);
			DrawCenteredMultilineText("Naves, Inimigos, Pilotos\nProjéteis e Efeitos Especiais", left_start, spacing + text_size * 8, text_size, WHITE);
		}
		
		{ // Quintino Pixels
			DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.6, 500, 100, BLACK);
			DrawCenteredPixelBorder(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.6, 500, 100, 5, WHITE);
			DrawCenteredText("Quintino Pixels", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.565, 38, WHITE);
			DrawCenteredMultilineText("Ícones de Melhorias", left_start, spacing + text_size * 12.5, text_size, WHITE);
		}

		{ // BDragon1727
			DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.73, 500, 170, BLACK);
			DrawCenteredPixelBorder(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.73, 500, 170, 5, WHITE);
			DrawCenteredText("BDragon1727", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.665, 38, WHITE);
			DrawCenteredMultilineText("Barras de Progresso e\nEfeitos Especiais", left_start, spacing + text_size * 16.7, text_size, WHITE);
		}

		{ // Deep-Fold
			DrawCenteredRectangle(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.86, 500, 100, BLACK);
			DrawCenteredPixelBorder(UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.86, 500, 100, 5, WHITE);
			DrawCenteredText("Deep-Fold", UI_RIGHT_CENTER, SCREEN_HEIGHT * 0.825, 38, WHITE);
			DrawCenteredMultilineText("Backgrounds, Planetas", left_start, spacing + text_size * 21.3, text_size, WHITE);
		}

	}

	DrawCenteredText("Pressione Enter ou Backspace para voltar ao menu principal.", GAME_SCREEN_CENTER, SCREEN_HEIGHT * 0.93, 48, WHITE);
	UpdateAndDrawScreenEffects();
	EndDrawing();
}