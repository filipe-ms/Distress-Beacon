// player.c

#include "player.h"
#include "common.h"

typedef struct Player {
	int ship_id;
	int score;
	int enemies_killed;
	int level;
	int experience;
	int exp_to_level;
} Player;

Player player;

void InitPlayer(void) {
	player.score = 0;
	player.enemies_killed = 0;
	player.level = 0;
	player.experience = 0;
	player.exp_to_level = BASE_EXP_TO_LEVEL;
}

bool AddExperience(int exp) {

	player.experience += exp;

	if (player.experience >= player.exp_to_level) {
		player.level++;
		player.experience -= player.exp_to_level;
		player.exp_to_level += EXP_TO_LEVEL_INCREMENT;
		return true;
	}

	return false;
}
void SetPlayerShip(int ship_id) { player.ship_id = ship_id; }
void AddScore(int score) { player.score += score; }
void AddEnemyKill(void) { player.enemies_killed++; }
int GetPlayerShip(void) { return player.ship_id; }
int GetPlayerScore(void) { return player.score; }
int GetPlayerEnemiesKilled(void) { return player.enemies_killed; }
int GetPlayerLevel(void) { return player.level; }
int GetPlayerExperience(void) { return player.experience; }
int GetPlayerExpToLevel(void) { return player.exp_to_level; }