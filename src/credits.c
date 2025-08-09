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

static Camera2D camera;
static float scroll_offset;
static float scroll_speed;
static float loading_time;
static float thank_offset;
static float scroll_limit;
static bool return_to_menu;


static const char* content = "EQUIPE\n"
"\n"
"Luís Santos\n"
"Anderson Gabriel\n"
"Débora Souza\n"

"\n\n"
"ORIENTADORES\n"
"\n"
"Pamela Bezerra\n"
"Tiago Barros\n"
"\n\n"
"COLABORADORES\n"
"\n"
"Carlos Santos\n"
"\n\n"
"ASSETS\n"
"\n"
"ansdor\n"
"[ Botões de Teclado ]\n"
"\n"
"Gustavo Vituri\n"
"[ Naves, Inimigos, Pilotos Projéteis e Efeitos Especiais ]\n"
"\n"
"Quintino Pixels\n"
"[ Ícones de Melhorias ]\n"
"\n"
"BDragon1727\n"
"[ Barras de Progresso e Efeitos Especiais ]\n"
"\n"
"Deep-Fold\n"
"[ Planetas e Backgrounds ]\n"
"\n\n"
"EFEITOS SONOROS\n"
"\n"
"Kronos1001\n"
"R0T0R\n"
"peepholecircus\n"
"mikst656\n"
"Leszek_Szary\n"
"Trading_Nation\n"
"outroelison\n"
"u_wzgyzo3pae\n"
"RescopicSound\n"
"StereogenicStudio\n"
"CharlieWD100\n"
"floraphonic\n"
"ecfike\n"
"studiomandragore\n"
"qubodup\n"
"miraclei\n"
"plasterbrain\n"
"\n\n"
"MÚSICAS\n"
"\n"
"Dani Stob\n"
"\n\n"
"Alguns assets e sons foram criados pela própria equipe\n"
"\n\n\n"
"Jogo desenvolvido com a Raylib 5.5\n"
"(www.raylib.com)\n"
"\n\n\n"
"Assets e músicas encontrados no site itch.io\n"
"\n\n\n"
"Efeitos sonoros encontrados no site pixabay.com\n";

void InitCredits(void) {
	camera.offset = (Vector2){ 0, 0 };
	camera.target = (Vector2){ 0, 0 };
	camera.zoom = 1.0f;
	camera.rotation = 0.0f;

	scroll_offset = SCREEN_HEIGHT;
	scroll_speed = 20.0f;
	loading_time = 5.0f;

	return_to_menu = false;

	thank_offset = 0;
	scroll_limit = 6200;

	InitBackground(BACKGROUND_STARS, WHITE, -1, 1, 0);
}

void UpdateCredits(void) {
	UpdateBackground();

	if (return_to_menu) {
		if (UpdateTimer()) {
			ChangeScene(START);
		}
		return;
	}

	scroll_offset += scroll_speed * GetFrameTime();
	camera.target.y = scroll_offset;

	if (IsActionButtonPressed() ||
		IsReturnButtonPressed() ||
		IsActionButton2Pressed() ||
		IsConfirmButtonPressed() ||
		scroll_offset >= scroll_limit) {

		return_to_menu = true;

		InitTimer(1.5f);
		InitFadeOutEffect(1.6f, BLACK, GetCurrentScreenEffectAlpha());
	}

	TraceLog(LOG_INFO, "scroll_offset: %f", scroll_offset);
}

void DrawCredits(void) {
	BeginDrawing();

	ClearBackground(BLACK);
	DrawBackground();

	BeginMode2D(camera);
	{
		DrawCenteredOutlinedText("AGRADECIMENTOS", GAME_SCREEN_CENTER, SCREEN_HEIGHT * 2.05, 60, WHITE, Fade(RAYWHITE, 0.5f));
		DrawCenteredMultilineText(content, GAME_SCREEN_CENTER, SCREEN_HEIGHT * 3.7, 40, WHITE);
		if (scroll_offset < 5900) {
			DrawCenteredOutlinedText("Obrigado por jogar nosso jogo!", GAME_SCREEN_CENTER, SCREEN_HEIGHT * 6, 40, WHITE, Fade(RAYWHITE, 0.5f));
		}
		else {
			if(scroll_offset < scroll_limit) thank_offset += scroll_speed * GetFrameTime();
			DrawCenteredOutlinedText("Obrigado por jogar nosso jogo!", GAME_SCREEN_CENTER, SCREEN_HEIGHT * 6 + thank_offset, 40, WHITE, Fade(RAYWHITE, 0.5f));
		}
	}
	EndMode2D();
	UpdateAndDrawScreenEffects();

	EndDrawing();
}