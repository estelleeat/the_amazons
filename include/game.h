#ifndef GAME_H
#define GAME_H

#include "amazones.h"

// Fonctions de gestion du jeu
void initializeGame(GameState* state);
bool isGameOver(GameState state);
Move* generateMoves(GameState state);
int countPossibleMoves(Move* moves);
GameState applyMove(GameState state, Move move);
void printBoard(GameState state);
void printMove(Move move);

#endif // GAME_H