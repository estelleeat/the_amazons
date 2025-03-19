#include "include/amazones.h"
#include "include/game.h"
#include "include/minimax.h"

int main() {
    GameState state;
    initializeGame(&state);
    
    printf("État initial du jeu:\n");
    printBoard(state);
    
    // Jouer jusqu'à ce que le jeu soit terminé
    while (!isGameOver(state)) {
        printf("\nJoueur %d, c'est à vous de jouer.\n", state.currentPlayer);
        
        // Utiliser l'IA pour trouver le meilleur coup
        printf("L'IA réfléchit...\n");
        Move bestMove = findBestMoveWithTimeLimit(state, 5); // 5 secondes de réflexion
        
        printf("L'IA joue: ");
        printMove(bestMove);
        printf("\n");
        
        // Appliquer le coup
        state = applyMove(state, bestMove);
        
        // Afficher le nouvel état du jeu
        printf("\nNouveau plateau:\n");
        printBoard(state);
    }
    
    printf("\nJeu terminé! Le joueur %d a gagné!\n", 1 - state.currentPlayer);
    
    return 0;
}