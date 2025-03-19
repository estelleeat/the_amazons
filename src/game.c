#include "../include/game.h"

void initializeGame(GameState* state) {
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            state->board[x][y] = EMPTY;
        }
    }
    
    // Placer les amazones blanches (positions standard)
    state->board[0][3] = AMAZON_WHITE;
    state->board[3][0] = AMAZON_WHITE;
    state->board[6][0] = AMAZON_WHITE;
    state->board[9][3] = AMAZON_WHITE;
    
    // Placer les amazones noires (positions standard)
    state->board[0][6] = AMAZON_BLACK;
    state->board[3][9] = AMAZON_BLACK;
    state->board[6][9] = AMAZON_BLACK;
    state->board[9][6] = AMAZON_BLACK;
    
    // Blanc commence
    state->currentPlayer = 0;
}

bool isGameOver(GameState state) {
    // Le jeu est terminé si le joueur actuel n'a pas de coups possibles
    Move* moves = generateMoves(state);
    int moveCount = countPossibleMoves(moves);
    free(moves);
    return moveCount == 0;
}

Move* generateMoves(GameState state) {
    // Allocation de mémoire pour les coups potentiels (max théorique)
    Move* moves = malloc(10 * 10 * 10 * 10 * sizeof(Move));
    int moveCount = 0;
    
    // Identifier quelle amazone appartient au joueur actuel
    CellType currentAmazon = (state.currentPlayer == 0) ? AMAZON_WHITE : AMAZON_BLACK;
    
    // Pour chaque case du plateau
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            // Si c'est une amazone du joueur actuel
            if (state.board[x][y] == currentAmazon) {
                // Pour chaque direction de déplacement possible (8 directions)
                int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
                int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
                
                for (int d = 0; d < 8; d++) {
                    // Déterminer toutes les positions d'arrivée possibles
                    for (int steps = 1; steps < 10; steps++) {
                        int toX = x + dx[d] * steps;
                        int toY = y + dy[d] * steps;
                        
                        // Vérifier si on est dans le plateau
                        if (toX < 0 || toX >= 10 || toY < 0 || toY >= 10)
                            break;
                            
                        // Vérifier si la case est vide
                        if (state.board[toX][toY] != EMPTY)
                            break;
                            
                        // Pour chaque direction de tir possible
                        for (int ad = 0; ad < 8; ad++) {
                            for (int arrowSteps = 1; arrowSteps < 10; arrowSteps++) {
                                int arrowX = toX + dx[ad] * arrowSteps;
                                int arrowY = toY + dy[ad] * arrowSteps;
                                
                                // Vérifier si on est dans le plateau
                                if (arrowX < 0 || arrowX >= 10 || arrowY < 0 || arrowY >= 10)
                                    break;
                                    
                                // Vérifier si la case est vide (ou la position de départ)
                                if (state.board[arrowX][arrowY] != EMPTY && !(arrowX == x && arrowY == y))
                                    break;
                                    
                                // Ajouter le coup à la liste
                                moves[moveCount].fromX = x;
                                moves[moveCount].fromY = y;
                                moves[moveCount].toX = toX;
                                moves[moveCount].toY = toY;
                                moves[moveCount].arrowX = arrowX;
                                moves[moveCount].arrowY = arrowY;
                                moveCount++;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Marquer la fin de la liste
    moves[moveCount].fromX = -1;
    
    return moves;
}

int countPossibleMoves(Move* moves) {
    int count = 0;
    while (moves[count].fromX != -1) {
        count++;
    }
    return count;
}

GameState applyMove(GameState state, Move move) {
    GameState newState = state;
    
    // Déplacer l'amazone
    newState.board[move.toX][move.toY] = newState.board[move.fromX][move.fromY];
    newState.board[move.fromX][move.fromY] = EMPTY;
    
    // Placer la flèche
    newState.board[move.arrowX][move.arrowY] = ARROW;
    
    // Changer de joueur
    newState.currentPlayer = 1 - newState.currentPlayer;
    
    return newState;
}

void printBoard(GameState state) {
    printf("  0 1 2 3 4 5 6 7 8 9\n");
    for (int y = 0; y < 10; y++) {
        printf("%d ", y);
        for (int x = 0; x < 10; x++) {
            switch (state.board[x][y]) {
                case EMPTY:
                    printf(". ");
                    break;
                case ARROW:
                    printf("* ");
                    break;
                case AMAZON_WHITE:
                    printf("W ");
                    break;
                case AMAZON_BLACK:
                    printf("B ");
                    break;
            }
        }
        printf("\n");
    }
}

void printMove(Move move) {
    printf("(%d,%d) -> (%d,%d), flèche: (%d,%d)", 
           move.fromX, move.fromY, 
           move.toX, move.toY, 
           move.arrowX, move.arrowY);
}