#ifndef AMAZONES_H
#define AMAZONES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

// Définition des types
typedef enum {
    EMPTY,
    ARROW,
    AMAZON_WHITE,
    AMAZON_BLACK
} CellType;

typedef struct {
    CellType board[10][10];  // Plateau 10x10 standard
    int currentPlayer;       // 0 pour blanc, 1 pour noir
} GameState;

typedef struct {
    int fromX, fromY;        // Position de départ de l'amazone
    int toX, toY;            // Position d'arrivée de l'amazone
    int arrowX, arrowY;      // Position de la flèche
} Move;

// Fonction principale
Move findBestMove(GameState state, int maxDepth);
Move findBestMoveWithTimeLimit(GameState state, int timeLimit);

#endif // AMAZONES_H