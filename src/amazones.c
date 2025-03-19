#include "../include/amazones.h"
#include "../include/minimax.h"
#include "../include/game.h"
#include "../include/utils.h"

Move findBestMove(GameState state, int maxDepth) {
    Move bestMove;
    int bestValue = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;
    
    // Générer tous les coups possibles pour le joueur actuel
    Move* possibleMoves = generateMoves(state);
    int moveCount = countPossibleMoves(possibleMoves);
    
    // Trier les coups pour améliorer l'élagage alpha-beta
    orderMoves(possibleMoves, moveCount, state);
    
    for (int i = 0; i < moveCount; i++) {
        // Appliquer le coup
        GameState newState = applyMove(state, possibleMoves[i]);
        
        // Évaluer ce coup avec minimax
        int moveValue = minimax(newState, maxDepth - 1, alpha, beta, false);
        
        // Mettre à jour le meilleur coup
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = possibleMoves[i];
        }
        
        // Mise à jour alpha
        alpha = (alpha > moveValue) ? alpha : moveValue;
    }
    
    free(possibleMoves);
    
    return bestMove;
}

Move findBestMoveWithTimeLimit(GameState state, int timeLimit) {
    Move bestMove;
    
    // Initialiser la table de transposition
    initTranspositionTable();
    
    double startTime = elapsedTime();
    
    for (int depth = 1; depth <= 10; depth++) {
        // Vérifier si on a encore du temps
        if (elapsedTime() - startTime > timeLimit * 0.8)
            break;
            
        Move currentBestMove = findBestMove(state, depth);
        bestMove = currentBestMove;
        
        printf("Profondeur %d explorée, meilleur coup: ", depth);
        printMove(bestMove);
        printf("\n");
    }
    
    return bestMove;
}