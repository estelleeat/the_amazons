#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>  
#include "amazones.h"

// Fonctions utilitaires
uint64_t hashGameState(GameState state);
int countMoves(GameState state, CellType amazonType);
void evaluateTerritory(GameState state, int territories[2]);
int floodFill(GameState state, int startX, int startY, bool visited[10][10]);
double elapsedTime();

#endif // UTILS_H