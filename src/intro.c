// intro.c

#include "intro.h"
#include "scene_manager.h"
#include "draw_utils.h"
#include "scene_draw_effects.h"
#include "common.h"
#include "input.h"
#include "timer.h"
#include "background.h"

static Camera2D camera;
static float scroll_offset;
static float scroll_speed;
static float loading_time;
static bool skip_pressed;
static bool intro_started;

#pragma region STORY
static const char* title = "Distress Beacon\0";
static const char* story = "No ano de 2279, a humanidade alcançou as estrelas.\n\n"
"Colônias florescem em Marte, enquanto luas de Júpiter como Europa e Ganimedes\n"
"abrigam centros de pesquisa avançados. Sondas automatizadas vasculham os\n"
"limites do universo, revelando mistérios cósmicos. A observação constante de\n"
"fenômenos como buracos negros, pulsares e nebulosas impulsionou descobertas\n"
"extraordinárias... e revelou algo ainda maior: a Terra não era o único lar de\n"
"vida inteligente.\n\n"

"O primeiro contato com civilizações extraterrestres mudou tudo. O medo deu \n"
"lugar à curiosidade, e a diplomacia abriu caminho para a cooperação. Em pouco\n"
"tempo, acordos científicos e tratados de paz uniram dezenas de raças espalhadas\n"
"por sistemas estelares distantes. Assim nasceu a Aliança Galáctica: uma união de\n"
"povos comprometidos com a preservação do conhecimento, da vida e da harmonia\n"
"entre mundos. Cada espécie trouxe consigo séculos de sabedoria, tecnologias\n"
"únicas e novas formas de enxergar o cosmos.\n\n"
"Mas nem todos no universo desejavam paz.\n\n"

"Enquanto conduzia um estudo de observação na Via Láctea, o Observatório Espacial\n"
"Kepler-II detectou um sinal incomum vindo da direção de Andrômeda, nossa galáxia\n"
"vizinha. Inicialmente confundido com uma emissão natural de um quasar, o padrão\n"
"se repetiu com regularidade assustadora.. uma mensagem codificada...\n"
"e quando decifrada, revelou uma ameaça.\n\n"

"Sem aviso, frotas hostis emergiram do espaço profundo, cruzando o Cinturão de\n"
"Asteroides e destruindo estações científicas ao longo do caminho. Agora, com a\n"
"paz da galáxia em risco e Ceres sendo usado como ponto estratégico de invasão,\n"
"a Aliança Galáctica ativou seu protocolo máximo de defesa. Cinco naves de elite,\n"
"cada uma alimentada pela ciência e tecnologia de uma civilização distinta foram\n"
"lançadas.\n\n"

"É sua missão controlar uma das naves e responder ao chamado de socorro,\n"
"Defendenda a Terra e preserve a paz na Via Láctea.\n\n"

"Contamos com você, capitão.";
#pragma endregion

// Forward declarations
void InitIntro(void) {
	camera.offset = (Vector2){ 0, 0 };
	camera.target = (Vector2){ 0, 0 };
	camera.zoom = 1.0f;
	camera.rotation = 0.0f;

	scroll_offset = SCREEN_HEIGHT;
	scroll_speed = 20.0f;
	loading_time = 5.0f;

	intro_started = false;
	skip_pressed = false;

	InitBackground(BACKGROUND_STARS, WHITE, -1, 1, 0);
}

void UpdateIntro(void) {

	loading_time -= GetFrameTime();
	if (loading_time <= 0.0f) {
		UpdateBackground();

		if (!intro_started) {
			InitFadeInEffect(1.5f, BLACK, 1.0f);
			intro_started = true;
		}
		else if (skip_pressed) {
			if (UpdateTimer()) {
				ChangeScene(START);
			}
		}

		scroll_offset += scroll_speed * GetFrameTime();
		camera.target.y = scroll_offset;



		if (IsActionButtonPressed() ||
			IsReturnButtonPressed() ||
			IsActionButton2Pressed() ||
			IsConfirmButtonPressed() ||
			scroll_offset>=3850.0f) {

			skip_pressed = true;

			InitTimer(1.5f);
			InitFadeOutEffect(1.6f, BLACK, GetCurrentScreenEffectAlpha());
		}
	}
}

void DrawIntro(void) {
	float tmr = GetElapsedTime();
	
		BeginDrawing();
		ClearBackground(BLACK);
		
		if (loading_time <= 0.0f) {
			DrawBackground();
			BeginMode2D(camera);
			
			DrawCenteredOutlinedText(title, GAME_SCREEN_CENTER, SCREEN_HEIGHT * 2.05, 60, WHITE, Fade(RAYWHITE, 0.5f));
			DrawCenteredMultilineText(story, GAME_SCREEN_CENTER, SCREEN_HEIGHT * 2.85, 40, WHITE);

			EndMode2D();
			UpdateAndDrawScreenEffects();
		}
		else {
			DrawText("Loading . . .", 10, 10, 30, WHITE);
		}
		
		EndDrawing();
	
}