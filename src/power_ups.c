// power_ups.c

#include "power_ups.h"
#include "common.h"
#include "weapon.h"
#include "list.h"
#include "raymath.h"
#include "draw_utils.h"
#include "input.h"

#include <stdio.h>
#include <string.h>

Texture shoot_damage;
Texture shoot_cooldown;
Texture shoot_size;
Texture shoot_speed; // faltando

Texture weapon_pulse;
Texture weapon_photon;
Texture weapon_shotgun;
Texture weapon_homing;
Texture weapon_prism;

Texture util_shield; // faltando


PowerUpCard active_cards[3];
PowerUpCard power_up_type[POWERUP_COUNT];

int current_option = 0;
float power_up_alpha = 0.0f;


void InitPowerUps(void) {
   
    shoot_damage = LoadTexture("powerups/damage.png");
    shoot_cooldown = LoadTexture("powerups/speed.png");
    shoot_size = LoadTexture("powerups/size.png");
	shoot_speed = LoadTexture("powerups/bullet_speed.png");

    weapon_pulse = LoadTexture("powerups/pulse.png");
    weapon_photon = LoadTexture("powerups/photon.png");
    weapon_shotgun = LoadTexture("powerups/shotgun.png");
    weapon_homing = LoadTexture("powerups/homing.png");
	weapon_prism = LoadTexture("powerups/prism.png");

    util_shield = LoadTexture("powerups/shield.png");

    power_up_type[SHOOT_COOLDOWN] = (PowerUpCard){
        .type = SHOOT_COOLDOWN, 
        .name = "Rapid Fire",
        .type_string = "ATRIBUTO",
        .description = "Reduz o tempo\nentre tiros\n+%d%%",
		.texture = &shoot_cooldown
    };

    power_up_type[SHOOT_DAMAGE] = (PowerUpCard){
        .type = SHOOT_DAMAGE, 
        .name = "Potency",
		.type_string = "ATRIBUTO",
        .description = "Aumenta o dano\nde suas armas\n+%d%%",
		.texture = &shoot_damage
    };

    power_up_type[SHOOT_SIZE] = (PowerUpCard){
        .type = SHOOT_SIZE,
		.type_string = "ATRIBUTO",
        .name = "Bulkier Rounds",
        .description = "Aumenta o tamanho\nde seus projéteis\n+%d%%",
		.texture = &shoot_size
    };

    power_up_type[SHOOT_SPEED] = (PowerUpCard){
        .type = SHOOT_SPEED,
        .name = "Ballistics",
        .type_string = "ATRIBUTO",
        .description = "Aumenta a velocidade\nde seus projéteis\n+%d%%",
        .texture = &shoot_speed
    };

    power_up_type[WEAPON_PULSE] = (PowerUpCard){
        .type = WEAPON_PULSE, 
        .name = "Pulse",
		.type_string = "ARMA",
        .description = "Pulsos difusos\nde energia\nNível: %d",
        .value = PULSE,
		.texture = &weapon_pulse
    };

    power_up_type[WEAPON_PHOTON] = (PowerUpCard){
        .type = WEAPON_PHOTON,
        .name = "Photon",
		.type_string = "ARMA",
        .description = "Dispara photons\nde alto dano\nNível: %d",
        .value = PHOTON,
		.texture = &weapon_photon
    };

    power_up_type[WEAPON_SHOTGUN] = (PowerUpCard){
        .type = WEAPON_SHOTGUN, 
        .name = "Shotgun",
		.type_string = "ARMA",
        .description = "Vários projéteis\nde curto alcance\nNível: %d",
        .value = SHOTGUN,
		.texture = &weapon_shotgun
    };

    power_up_type[WEAPON_HOMING] = (PowerUpCard){
        .type = WEAPON_HOMING, 
        .name = "Homing",
		.type_string = "ARMA",
        .description = "Bumerangue energético\ne teleguiado\nNível: %d",
        .value = HOMING,
		.texture = &weapon_homing
    };

    power_up_type[WEAPON_PRISM] = (PowerUpCard){
        .type = WEAPON_PRISM,
        .name = "Prism",
        .type_string = "ARMA",
        .description = "Laser que refrata\nquando atinge o alvo\nNível: %d",
        .value = HOMING,
        .texture = &weapon_prism
    };

    power_up_type[SHIELD] = (PowerUpCard){
        .name = "Shield",
        .type = SHIELD,
        .type_string = "ESCUDO",
        .description = "Garante proteção\ncontra 3 hits.",
        .value = SHIELD,
        .texture = &util_shield
    };
}

void UnloadPowerUpTextures(void) {
	UnloadTexture(shoot_damage);
	UnloadTexture(shoot_cooldown);
	UnloadTexture(shoot_size);
	UnloadTexture(shoot_speed);
	UnloadTexture(weapon_pulse);
	UnloadTexture(weapon_photon);
	UnloadTexture(weapon_shotgun);
    UnloadTexture(weapon_homing);
    UnloadTexture(util_shield);
}

void PowerRandomizer(void) {

    for (int i = 0; i < 3; i++) {
        active_cards[i] = (PowerUpCard){ 0 };
    }

    List* available_powerups = List_Create(sizeof(PowerUpType));
    
    for (int i = 0; i < POWERUP_COUNT; i++) {
        PowerUpType current_type = (PowerUpType)i;
        bool is_weapon = (current_type >= WEAPON_PULSE && current_type <= WEAPON_HOMING);

        if (current_type == SHIELD && IsShieldActive()) { // se ele ja tiver sido pego pelo player e ainda nao acabou as 3 charges, n vai paprecer na pool
            continue;
        }

        if (is_weapon) {
            if (GetWeaponLevel(power_up_type[current_type].value) < 3) {
                List_Add(available_powerups, &current_type);
            }
        } else {
            List_Add(available_powerups, &current_type);
        }
    }

    for (int i = 0; i < 3; i++) {
        if (available_powerups->size == 0) break;

        int random_index = GetRandomValue(0, available_powerups->size - 1);
        PowerUpType powerup_type = *(PowerUpType*)List_GetByIndex(available_powerups, random_index); // precisa de cast pq o retorno é void pointer

        active_cards[i] = power_up_type[powerup_type];

        switch (powerup_type) {
        case SHOOT_COOLDOWN: {
            int value = GetRandomValue(7, 12);
            active_cards[i].value = value;
            sprintf(active_cards[i].description_buffer, active_cards[i].description, value);
            break;
        }
        case SHOOT_DAMAGE: {
            int value = GetRandomValue(10, 20);
            active_cards[i].value = value;
            sprintf(active_cards[i].description_buffer, active_cards[i].description, value);
            break;
        }
        case SHOOT_SIZE: {
            int value = GetRandomValue(5, 10);
            active_cards[i].value = value;
            sprintf(active_cards[i].description_buffer, active_cards[i].description, value);
            break;
        }
        case SHOOT_SPEED: {
			int value = GetRandomValue(8, 16);
			active_cards[i].value = value;
			sprintf(active_cards[i].description_buffer, active_cards[i].description, value);
			break;
        }

        case WEAPON_PULSE:
        case WEAPON_PHOTON:
        case WEAPON_SHOTGUN:
        case WEAPON_HOMING:
        case WEAPON_PRISM: {
            int level = GetWeaponLevel(active_cards[i].value) + 1;
            sprintf(active_cards[i].description_buffer, active_cards[i].description, level);
            break;
        }

        default:
            sprintf(active_cards[i].description_buffer, "%s", active_cards[i].description);
            break;
        }

        List_RemoveAt(available_powerups, random_index);
    }

    List_Destroy(available_powerups);
}

static void PickPowerUp(void) {
    PowerUpCard chosen_powerup = active_cards[current_option];

    switch (chosen_powerup.type) {
    case SHOOT_COOLDOWN: IncrementCooldownModifier(chosen_powerup.value); break;
    case SHOOT_DAMAGE:   IncrementDamageModifier(chosen_powerup.value);   break;
    case SHOOT_SIZE:     IncrementSizeModifier(chosen_powerup.value);     break;
	case SHOOT_SPEED:    IncrementSpeedModifier(chosen_powerup.value);    break;

    case WEAPON_PULSE:   PulseLevelUp();   break;
    case WEAPON_PHOTON:  PhotonLevelUp();  break;
    case WEAPON_SHOTGUN: ShotgunLevelUp(); break;
    case WEAPON_HOMING:  HomingLevelUp();  break;
	case WEAPON_PRISM:   PrismLevelUp();   break;

    case SHIELD: ActivateShield(); break;

    default: break;
    }
}

void UpdateLevelUpSelectMenu(bool* flag) {
    if (!(*flag)) {
        power_up_alpha = 0.0f;
        return;
    }

    if (power_up_alpha < 1.0f) {
        power_up_alpha += 0.5f * GetFrameTime();
    }

    if (IsInputRightPressed()) {
        current_option = (current_option + 1) % 3;
    }
    else if (IsInputLeftPressed()) {
        current_option = (current_option - 1 + 3) % 3;
    }
    else if (IsConfirmButtonPressed()) {
        *flag = false;
        PickPowerUp();
    }
}

static void DrawCardText(const char* text, float centerX, float y, int fontSize, Color color, bool outlined, float alpha) {
    const char* str = text;
    float currentY = y;
    float lineHeight = (float)fontSize + 4.0f;

    while (*str) {
        int len = 0;
        while (str[len] != '\n' && str[len] != '\0') len++;

        if (len > 0) {
            const char* line = TextSubtext(str, 0, len);
            float lineWidth = (float)MeasureText(line, fontSize);
            int posX = (int)(centerX - lineWidth / 2.0f);

            if (outlined) {
                DrawOutlinedText(line, posX, (int)currentY, fontSize, Fade(color, alpha), Fade(color, 0.5f * alpha));
            }
            else {
                DrawText(line, posX, (int)currentY, fontSize, Fade(color, alpha));
            }
        }

        str += len;
        if (*str == '\n') {
            str++;
            currentY += lineHeight;
        }
    }
}

static Color GetTypeColor(const char* type) {
    if (strcmp(type, "ARMA") == 0) return RED;
    else return BLUE;
}

static void DrawPowerUpCard(PowerUpCard* card, Rectangle cardRect, bool is_selected, float alpha) {

    float card_roundness = 0.3f;
    int card_segments = 20;
    float card_texture_scale = 2.5f;
    float border_thickness = 3.0f;

    const float card_center_x = cardRect.x + cardRect.width / 2.0f;

    DrawRectangleRounded(cardRect, card_roundness, card_segments, Fade(BLACK, 0.75f * alpha));
    const Color border_color = is_selected ? Fade(RED, alpha) : Fade(GRAY, alpha);
    DrawRectangleRoundedLinesEx(cardRect, card_roundness, card_segments, border_thickness, border_color);

    float current_y = cardRect.y + 20.0f;

    DrawCardText(card->name, card_center_x, current_y, 26, GREEN, false, alpha);
    current_y += 40.0f;

    const float texture_width = (float)card->texture->width * card_texture_scale;
    const float texture_height = (float)card->texture->height * card_texture_scale;
    const Vector2 texture_pos = { card_center_x - texture_width / 2.0f, current_y };
    DrawTextureEx(*card->texture, texture_pos, 0, card_texture_scale, Fade(WHITE, alpha));
    current_y += texture_height + 15.0f;

    DrawCardText(card->type_string, card_center_x, current_y, 20, GetTypeColor(card->type_string), true, alpha);
    current_y += 35.0f;

    DrawCardText(card->description_buffer, card_center_x, current_y, 20, WHITE, false, alpha);
}

void DrawLevelUpSelectMenu(bool should_draw) {
    if (!should_draw) return;

    int card_width = 220;
    int card_spacing = 50;
    int card_height = 320;

    DrawCardText("Selecione um upgrade", GAME_SCREEN_CENTER, SCREEN_HEIGHT / 2.0f - 280.0f, 40, WHITE, true, power_up_alpha);

    const float cards_y = (SCREEN_HEIGHT / 2.0f) - (card_height / 2.0f);
    const float center_card_x = GAME_SCREEN_CENTER - card_width / 2.0f;
    const float card_positions_x[3] = {
        center_card_x - card_width - card_spacing,
        center_card_x,
        center_card_x + card_width + card_spacing
    };

    for (int i = 0; i < 3; i++) {
        if (i < 3 && active_cards[i].texture != NULL) {
            const bool is_selected = (current_option == i);
            const Rectangle cardRect = { card_positions_x[i], cards_y, card_width, card_height };
            DrawPowerUpCard(&active_cards[i], cardRect, is_selected, power_up_alpha);
        }
    }
}