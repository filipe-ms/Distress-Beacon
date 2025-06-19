#pragma once

#define MAX_ENTRIES 10
#define MAX_NAME_LENGTH 20

typedef struct {
    char name[MAX_NAME_LENGTH + 1];
    char shipName[MAX_NAME_LENGTH + 1];
    int score;
} RankingEntry;

extern RankingEntry entries[MAX_ENTRIES];
extern int playerCount;

void InitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

void AddToRanking(const char* name, int ship_type, int score);
void LoadRanking(void);