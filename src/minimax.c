#include "../include/minimax.h"
#include "../include/game.h"
#include "../include/utils.h"
#include <stdint.h>  // Ajout de cette ligne pour uint64_t
// Table de transposition globale
TranspositionEntry* transpositionTable;

int minimax(GameState state, int depth, int alpha, int beta, bool isMaximizing) {
    uint64_t hash = hashGameState(state);
    
    // Vérifier la table de transposition
    TranspositionEntry* entry = lookupTranspositionTable(hash);
    if (entry != NULL && entry->depth >= depth) {
        if (entry->flag == 0) // EXACT
            return entry->score;
        else if (entry->flag == 1 && entry->score > alpha) // LOWERBOUND
            alpha = entry->score;
        else if (entry->flag == 2 && entry->score < beta) // UPPERBOUND
            beta = entry->score;
            
        if (alpha >= beta)
            return entry->score;
    }
    
    // Conditions d'arrêt
    if (depth == 0 || isGameOver(state)) {
        int score = evaluatePosition(state);
        storeTranspositionTable(hash, depth, score, (Move){-1,-1,-1,-1,-1,-1}, 0);
        return score;
    }
    
    // Générer tous les coups possibles
    Move* possibleMoves = generateMoves(state);
    int moveCount = countPossibleMoves(possibleMoves);
    
    // Aucun mouvement possible = défaite
    if (moveCount == 0) {
        free(possibleMoves);
        return isMaximizing ? INT_MIN : INT_MAX;
    }
    
    // Trier les coups pour améliorer l'élagage
    orderMoves(possibleMoves, moveCount, state);
    
    int bestScore;
    Move bestMove = {-1,-1,-1,-1,-1,-1};
    int flag = 1; // Supposer LOWERBOUND par défaut
    
    if (isMaximizing) {
        bestScore = INT_MIN;
        for (int i = 0; i < moveCount; i++) {
            GameState newState = applyMove(state, possibleMoves[i]);
            int eval = minimax(newState, depth - 1, alpha, beta, false);
            
            if (eval > bestScore) {
                bestScore = eval;
                bestMove = possibleMoves[i];
            }
            
            alpha = (alpha > eval) ? alpha : eval;
            if (beta <= alpha) {
                flag = 2; // UPPERBOUND
                break;
            }
        }
    } else {
        bestScore = INT_MAX;
        for (int i = 0; i < moveCount; i++) {
            GameState newState = applyMove(state, possibleMoves[i]);
            int eval = minimax(newState, depth - 1, alpha, beta, true);
            
            if (eval < bestScore) {
                bestScore = eval;
                bestMove = possibleMoves[i];
            }
            
            beta = (beta < eval) ? beta : eval;
            if (beta <= alpha) {
                flag = 1; // LOWERBOUND
                break;
            }
        }
    }
    
    free(possibleMoves);
    
    // Si on n'a pas eu d'élagage, le score est exact
    if ((isMaximizing && bestScore < beta) || (!isMaximizing && bestScore > alpha))
        flag = 0; // EXACT
        
    storeTranspositionTable(hash, depth, bestScore, bestMove, flag);
    
    return bestScore;
}

int evaluatePosition(GameState state) {
    // Évaluation basée sur la mobilité et le territoire
    int whiteScore = 0;
    int blackScore = 0;
    
    // 1. Compter les mouvements possibles pour chaque amazone
    whiteScore += countMoves(state, AMAZON_WHITE);
    blackScore += countMoves(state, AMAZON_BLACK);
    
    // 2. Estimer le territoire contrôlé par chaque joueur
    int territories[2];
    evaluateTerritory(state, territories);
    whiteScore += territories[0] * 2;  // Donner plus de poids au territoire
    blackScore += territories[1] * 2;
    
    // Retourner le score relatif selon le joueur actuel
    return (state.currentPlayer == 0) ? 
           (whiteScore - blackScore) : 
           (blackScore - whiteScore);
}

void orderMoves(Move* moves, int moveCount, GameState state) {
    // Tableau de scores pour chaque coup
    int* scores = malloc(moveCount * sizeof(int));
    
    // Évaluer chaque coup avec une heuristique simple
    for (int i = 0; i < moveCount; i++) {
        GameState newState = applyMove(state, moves[i]);
        scores[i] = evaluatePosition(newState);
    }
    
    // Tri à bulles (simple mais suffisant pour de petits tableaux)
    for (int i = 0; i < moveCount - 1; i++) {
        for (int j = 0; j < moveCount - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                // Échanger les scores
                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;
                
                // Échanger les coups
                Move tempMove = moves[j];
                moves[j] = moves[j + 1];
                moves[j + 1] = tempMove;
            }
        }
    }
    
    free(scores);
}

void initTranspositionTable() {
    transpositionTable = calloc(TRANSPOSITION_TABLE_SIZE, sizeof(TranspositionEntry));
}

TranspositionEntry* lookupTranspositionTable(uint64_t hash) {
    int index = hash % TRANSPOSITION_TABLE_SIZE;
    TranspositionEntry* entry = &transpositionTable[index];
    
    if (entry->hash == hash)
        return entry;
    
    return NULL;
}

void storeTranspositionTable(uint64_t hash, int depth, int score, Move bestMove, int flag) {
    int index = hash % TRANSPOSITION_TABLE_SIZE;
    TranspositionEntry* entry = &transpositionTable[index];
    
    // Remplacer l'entrée si elle est plus profonde ou si c'est la même position
    if (entry->depth <= depth || entry->hash == hash) {
        entry->hash = hash;
        entry->depth = depth;
        entry->score = score;
        entry->bestMove = bestMove;
        entry->flag = flag;
    }
}