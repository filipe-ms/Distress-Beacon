#include "ship_references.h"

#include "common.h"

char* GetShipName(int ship_id) {
    switch (ship_id) {
    case ORION: return "Orion";
    case AUREA: return "Aurea";
    case NEBULA: return "Nebula";
    case PUDDLE_JUMPER: return "Puddle Jumper";
    case VOID: return "Void";
    default: return "Desconhecido";
    }
}

Color GetShipColor(int ship_id) {
	switch (ship_id) {
	case ORION:         return RAYWHITE;
	case AUREA:         return YELLOW;
	case NEBULA:        return PURPLE;
	case PUDDLE_JUMPER: return GREEN;
	case VOID:          return RED;
	default:            return GRAY;
	}
}

char* GetPilotName(int ship_id) {
    if (ship_id == ORION)          return "Déboru";
    if (ship_id == AUREA)          return "Je Kounill";
    if (ship_id == NEBULA)         return "Dentriel";
    if (ship_id == PUDDLE_JUMPER)  return "Tial-Ki";
    if (ship_id == VOID)           return "Apouphis";
    return "Desconhecido";
}

char* GetShipSpecial(int ship_id) {
	if (ship_id == ORION)          return "Dash";
	if (ship_id == PUDDLE_JUMPER)  return "Portal";
	return "Abilidade desconhecida";
}

char* GetShipSpecialDescription(int ship_id) {
    if (ship_id == ORION) return "Impulsiona a nave\nem alta velocidade para\na direção escolhida.";
    return "Descrição faltando.";
}

char* GetPilotPresentation(int ship_id) {
    if (ship_id == ORION) return "Sou o vencedor do grand\nprix de Rio Doce.";
    if (ship_id == NEBULA) return "É dento.";
    return "Default";
}