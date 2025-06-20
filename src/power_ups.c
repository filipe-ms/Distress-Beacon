// power_ups.c

#include "power_ups.h"
#include "common.h"
#include "weapon.h"
#include "list.h"
#include "raymath.h"

#include <stdio.h>
#include <string.h>

Texture shoot_damage;
Texture shoot_cooldown;
Texture shoot_size;

Texture weapon_pulse;
Texture weapon_photon;
Texture weapon_shotgun;
Texture weapon_homing;

PowerUpCard active_cards[3];
PowerUpCard power_up_type[POWERUP_COUNT];

int current_option = 0;
float power_up_alpha = 0.0f;


void InitPowerUps(void) {
   
    shoot_damage = LoadTexture("powerups/damage.png");
    shoot_cooldown = LoadTexture("powerups/speed.png");
    shoot_size = LoadTexture("powerups/size.png");
    weapon_pulse = LoadTexture("powerups/pulse.png");
    weapon_photon = LoadTexture("powerups/photon.png");
    weapon_shotgun = LoadTexture("powerups/shotgun.png");
    weapon_homing = LoadTexture("powerups/homing.png");

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
        .name = "Bulky Rounds",
        .description = "Aumenta o tamanho\nde seus projéteis\n+%d%%",
		.texture = &shoot_size
    };

    power_up_type[WEAPON_PULSE] = (PowerUpCard){
        .type = WEAPON_PULSE, 
        .name = "Pulse",
		.type_string = "ARMA",
        .description = "Pulsos difusos\nde energia",
        .value = PULSE,
		.texture = &weapon_pulse
    };

    power_up_type[WEAPON_PHOTON] = (PowerUpCard){
        .type = WEAPON_PHOTON,
        .name = "Photon",
		.type_string = "ARMA",
        .description = "Dispara photons\nde alto dano",
        .value = PHOTON,
		.texture = &weapon_photon
    };

    power_up_type[WEAPON_SHOTGUN] = (PowerUpCard){
        .type = WEAPON_SHOTGUN, 
        .name = "Shotgun",
		.type_string = "ARMA",
        .description = "Vários projéteis\nde curto alcance",
        .value = SHOTGUN,
		.texture = &weapon_shotgun
    };

    power_up_type[WEAPON_HOMING] = (PowerUpCard){
        .type = WEAPON_HOMING, 
        .name = "Homing",
		.type_string = "ARMA",
        .description = "Bumerangue energético\ne teleguiado",
        .value = HOMING,
		.texture = &weapon_homing
    };
}

void UnloadPowerUpTextures(void) {
	UnloadTexture(shoot_damage);
	UnloadTexture(shoot_cooldown);
	UnloadTexture(shoot_size);
	UnloadTexture(weapon_pulse);
	UnloadTexture(weapon_photon);
	UnloadTexture(weapon_shotgun);
    UnloadTexture(weapon_homing);
}

void PowerRandomizer(void) {

    for (int i = 0; i < 3; i++) {
        active_cards[i] = (PowerUpCard){ 0 };
    }

    List* available_powerups = List_Create(sizeof(PowerUpType));
    
    for (int i = 0; i < POWERUP_COUNT; i++) {
        PowerUpType current_type = (PowerUpType)i;
        bool is_weapon = (current_type >= WEAPON_PULSE && current_type <= WEAPON_HOMING);

        if (is_weapon) {
            if (!IsWeaponActive(power_up_type[current_type].value)) {
                List_Add(available_powerups, &current_type);
            }
        } else {
            List_Add(available_powerups, &current_type);
        }
    }

    for (int i = 0; i < 3; i++) {
        if (available_powerups->size == 0) break;

        int random_index = GetRandomValue(0, available_powerups->size - 1);
        PowerUpType powerup_type = *(PowerUpType*)List_GetByIndex(available_powerups, random_index); // precisa de cast pq o retorno � void pointer

        active_cards[i] = power_up_type[powerup_type];

        switch (powerup_type) {
        case SHOOT_COOLDOWN: {
            int value = GetRandomValue(5, 10);
            active_cards[i].value = value;
            sprintf(active_cards[i].description_buffer, active_cards[i].description, value);
            break;
        }
        case SHOOT_DAMAGE: {
            int value = GetRandomValue(10, 25);
            active_cards[i].value = value;
            sprintf(active_cards[i].description_buffer, active_cards[i].description, value);
            break;
        }
        case SHOOT_SIZE: {
            int value = GetRandomValue(15, 30);
            active_cards[i].value = value;
            sprintf(active_cards[i].description_buffer, active_cards[i].description, value);
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

    case WEAPON_PULSE:   ActivatePulse();   break;
    case WEAPON_PHOTON:  ActivatePhoton();  break;
    case WEAPON_SHOTGUN: ActivateShotgun(); break;
    case WEAPON_HOMING:  ActivateHoming();  break;

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

    if (IsKeyPressed(KEY_RIGHT)) {
        current_option = (current_option + 1) % 3;
    }
    else if (IsKeyPressed(KEY_LEFT)) {
        current_option = (current_option - 1 + 3) % 3;
    }
    else if (IsKeyPressed(KEY_ENTER)) {
        *flag = false;
        PickPowerUp();
    }
}

static void DrawMultilineText(const char* text, float centerX, float y, int fontSize, Color color, float alpha) {
    const char* str = text;
    float lineHeight = (float)fontSize + 4.0f;

    while (*str) {
        int len = 0;
        while (str[len] != '\n' && str[len] != '\0') {
            len++;
        }

        if (len > 0) {
            const char* line = TextSubtext(str, 0, len); // Função da raylib que faz uma substring
            float lineWidth = (float)MeasureText(line, fontSize);
            float x = centerX - (lineWidth / 2.0f);
            DrawText(line, (int)x, (int)y, fontSize, Fade(color, alpha));
        }

        str += len;
        if (*str == '\n') {
            str++;
            y += lineHeight;
        }
    }
}

static void DrawPowerUpCard(PowerUpCard* card, Rectangle card_rec, bool is_selected, float alpha) {
    float card_center_x = card_rec.x + card_rec.width / 2.0f;

    DrawRectangleRounded(card_rec, 0.1f, 10, Fade(BLACK, 0.75f * alpha));
    Color border_color = is_selected ? Fade(RED, alpha) : Fade(GRAY, alpha);
    DrawRectangleRoundedLinesEx(card_rec, 0.1f, 10, 4.0f, border_color);

    float name_size = 26.0f;

    float name_y = card_rec.y + 20.0f;
    float name_width = (float)MeasureText(card->name, name_size);
    DrawText(card->name, (int)(card_center_x - name_width / 2.0f), (int)name_y, name_size, Fade(GREEN, alpha));

    Texture* texture = card->texture;
    float scale = 2.5f;
    float texture_width = (float)texture->width * scale;
    Vector2 texture_pos = { card_center_x - (texture_width / 2.0f), name_y + 40.0f };
    DrawTextureEx(*texture, texture_pos, 0, scale, Fade(WHITE, alpha));

    const char* type_text = card->type_string;
    int typeFontSize = 20;
    float type_y = texture_pos.y + (texture->height * scale) + 15.0f;
    Color type_color;
    if (strcmp(type_text, "ARMA") == 0) type_color = RED;
    else type_color = BLUE;
    float type_width = (float)MeasureText(type_text, typeFontSize);
    Color shadow_color = (Color){ type_color.r, type_color.g, type_color.b, alpha };
    DrawText(type_text, (int)(card_center_x - type_width / 2.0f) - 2, (int)type_y - 2, typeFontSize, Fade(shadow_color, alpha-0.5f));
    DrawText(type_text, (int)(card_center_x - type_width / 2.0f), (int)type_y, typeFontSize, Fade(type_color, alpha));

    float description_y = type_y + 35.0f;
    DrawMultilineText(card->description_buffer, card_center_x, description_y, 20, WHITE, alpha);
}

void DrawLevelUpSelectMenu(bool flag) {
    if (!flag) return;

    const char* title = "Selecione um upgrade";
    int titleFontSize = 40;
    float titleWidth = (float)MeasureText(title, titleFontSize);
    DrawText(title, (int)((GAME_SCREEN_WIDTH - titleWidth) / 2.0f) - 2, (int)(SCREEN_HEIGHT / 2.0f - 280.0f) - 2, titleFontSize, Fade(RAYWHITE, power_up_alpha - 0.5f));
    DrawText(title, (int)((GAME_SCREEN_WIDTH - titleWidth) / 2.0f), (int)(SCREEN_HEIGHT / 2.0f - 280.0f), titleFontSize, Fade(WHITE, power_up_alpha));

    float card_width = 210.0f;
    float card_height = 320.0f;
    float spacing = 20.0f;
    float total_width = (card_width * 3) + (spacing * 2);
    float start_x = (GAME_SCREEN_WIDTH - total_width) / 2.0f;
    float y_pos = (SCREEN_HEIGHT - card_height) / 2.0f;

    for (int i = 0; i < 3; i++) {
        Rectangle card_rec = { start_x + i * (card_width + spacing), y_pos, card_width, card_height };
        bool is_selected = (current_option == i);
        DrawPowerUpCard(&active_cards[i], card_rec, is_selected, power_up_alpha);
    }
}