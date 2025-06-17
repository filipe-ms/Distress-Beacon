// player.c

#include "player.h"

// Possible Ships

void InitPlayer(Player *player) {
	player->score = 0;
	player->enemies_killed = 0;
}