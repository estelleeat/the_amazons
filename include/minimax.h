#ifndef MINIMAX_H
#define MINIMAX_H

#include "amazones.h"

// Définition de la table de transposition
#define TRANSPOSITION_TABLE_SIZE 1000000

typedef struct {
    uint64_t hash;
    int depth;
    int score;
    Move bestMove;
    int flag; // 0 = EXACT, 1 = LOWERBOUND, 2 = UPPERBOUND
} TranspositionEntry;

// Fonctions minimax
int minimax(GameState state, int depth, int alpha, int beta, bool isMaximizing);
int evaluatePosition(GameState state);
void orderMoves(Move* moves, int moveCount, GameState state);
void initTranspositionTable();
TranspositionEntry* lookupTranspositionTable(uint64_t hash);
void storeTranspositionTable(uint64_t hash, int depth, int score, Move bestMove, int flag);

#endif // MINIMAX_H