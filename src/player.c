// player.c

#include "player.h"

// Possible Ships

void InitPlayer(Player *player) {
	player->score = 0;
	player->enemies_killed = 0;
	player->level = 0;
	player->experience = 0;
}

void AddExperience(Player* player, int exp) {

	player->experience += exp;

	int level_up_exp = 10 + 10 * player->level;

	if (player->experience >= level_up_exp) {
		player->level++;
		player->experience -= level_up_exp;
	}
}

void AddScore(Player* player, int score) {
	player->score += score;
}

void AddEnemyKill(Player* player) {
	player->enemies_killed++;
}