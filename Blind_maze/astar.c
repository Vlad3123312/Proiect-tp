#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Functie euristica (Manhattan distance)
int heuristic(Position a, Position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Structura A* - poate fi apelata din server pentru un bot sau hint-uri
void find_path_astar(GameState *game, Position target) {
    printf("[A* DEBUG] Se calculeaza ruta de la (%d,%d) catre (%d,%d)...\n", 
            game->player.pos.x, game->player.pos.y, target.x, target.y);
    
    
}
