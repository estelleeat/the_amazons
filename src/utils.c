#include "../include/utils.h"
#include <time.h>

uint64_t hashGameState(GameState state) {
    uint64_t hash = 0;
    
    // Hachage simple (peut être amélioré avec Zobrist)
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            hash = hash * 31 + state.board[x][y];
        }
    }
    
    hash = hash * 31 + state.currentPlayer;
    
    return hash;
}

int countMoves(GameState state, CellType amazonType) {
    int count = 0;
    
    // Sauvegarder le joueur actuel
    int originalPlayer = state.currentPlayer;
    
    // Définir le joueur en fonction du type d'amazone
    state.currentPlayer = (amazonType == AMAZON_WHITE) ? 0 : 1;
    
    // Générer tous les coups possibles
    Move* moves = generateMoves(state);
    count = countPossibleMoves(moves);
    free(moves);
    
    // Restaurer le joueur
    state.currentPlayer = originalPlayer;
    
    return count;
}

void evaluateTerritory(GameState state, int territories[2]) {
    bool visited[10][10] = {{false}};
    territories[0] = territories[1] = 0;
    
    // Pour chaque case du plateau
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            // Si c'est une amazone
            if (state.board[x][y] == AMAZON_WHITE || state.board[x][y] == AMAZON_BLACK) {
                int player = (state.board[x][y] == AMAZON_WHITE) ? 0 : 1;
                
                // Flood fill à partir de cette amazone
                territories[player] += floodFill(state, x, y, visited);
            }
        }
    }
}

int floodFill(GameState state, int startX, int startY, bool visited[10][10]) {
    // Initialiser une file pour le parcours en largeur
    typedef struct {
        int x, y;
    } Point;
    
    Point queue[100];
    int front = 0, rear = 0;
    
    // Ajouter le point de départ à la file
    queue[rear++] = (Point){startX, startY};
    visited[startX][startY] = true;
    
    int count = 0;
    
    // Directions pour le parcours
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    while (front < rear) {
        Point current = queue[front++];
        count++;
        
        // Explorer les voisins
        for (int d = 0; d < 8; d++) {
            int newX = current.x + dx[d];
            int newY = current.y + dy[d];
            
            // Vérifier si on est dans le plateau
            if (newX < 0 || newX >= 10 || newY < 0 || newY >= 10)
                continue;
                
            // Vérifier si la case est vide et non visitée
            if (state.board[newX][newY] == EMPTY && !visited[newX][newY]) {
                queue[rear++] = (Point){newX, newY};
                visited[newX][newY] = true;
            }
        }
    }
    
    return count;
}

double elapsedTime() {
    static clock_t start = 0;
    
    if (start == 0)
        start = clock();
        
    return ((double)(clock() - start)) / CLOCKS_PER_SEC;
}