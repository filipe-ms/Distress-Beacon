#include "ship_references.h"

#include "common.h"

static int StringEqualsIgnoreCase(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return 0;
        }
        s1++;
        s2++;
    }
    return (*s1 == *s2);
}


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

	if (ship_id == DUCK)           return "Dodgers";
	if (ship_id == ROBOT)          return "Epsilon";
	if (ship_id == ALIEN)          return "Gronk";
	if (ship_id == PIRATE)         return "Comandante";
	if (ship_id == INVERSE)        return "Inverso";
	if (ship_id == INTERFERENCE)   return "Interferência";

    return "";
}

char* GetShipSpecial(int ship_id) {
	if (ship_id == ORION)			return "Dash";
	if (ship_id == PUDDLE_JUMPER)	return "Portal";
	if (ship_id == AUREA)			return "Drones de Ataque";
	if (ship_id == NEBULA)			return "Disparo de Blaster";
	if (ship_id == VOID)			return "Horizonte de Eventos";
	return "Abilidade desconhecida";
}

char* GetShipSpecialDescription(int ship_id) {
	if (ship_id == ORION)			return "Envolve a nave em chamas e\ntorna-a invulnerável pela\nduração.";
	if (ship_id == PUDDLE_JUMPER)	return "Cria portais e é capaz de\nviajar entre eles.";
	if (ship_id == AUREA)			return "Ativa dois drones que\ndisparam contra os inimigos!";
	if (ship_id == NEBULA)			return "Carrega um tiro de blaster.\nPossui três níveis\nde intensidade.";
	if (ship_id == VOID)			return "Cria um horizonte de eventos\nque danifica as naves inimigas.";
    return "Descrição faltando.";
}

char* GetPilotPresentation(int ship_id) {
    if (ship_id == ORION) return "Sou o piloto mais\nrápido da galáxia!";
    if (ship_id == NEBULA) return "Sou capaz de ver\nmelhor do que qualquer\nhumano.";
	if (ship_id == VOID) return "Afastarei os invasores\ncom minhas próprias mãos!";
	if (ship_id == AUREA) return "Kounill no volante,\nemoção constante!";
	if (ship_id == PUDDLE_JUMPER) return "Possuo a melhor tecnologia!\nMeus portais nos levarão\nà vitória!";
    return "Vamos defender o planeta\nda horda inimiga!";
}

Color GetShipColorByName(const char* ship_name) {
	if (StringEqualsIgnoreCase(ship_name, "Orion")) {
		return GetShipColor(ORION);
	}
	else if (StringEqualsIgnoreCase(ship_name, "Aurea")) {
		return GetShipColor(AUREA);
	}
	else if (StringEqualsIgnoreCase(ship_name, "Nebula")) {
		return GetShipColor(NEBULA);
	}
	else if (StringEqualsIgnoreCase(ship_name, "Puddle Jumper")) {
		return GetShipColor(PUDDLE_JUMPER);
	}
	else if (StringEqualsIgnoreCase(ship_name, "Void")) {
		return GetShipColor(VOID);
	}
	return GRAY;
}