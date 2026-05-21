#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void init_game(GameState *game) {
    const char *template_map[MAP_HEIGHT] = {
        "####################",
        "#P....#......*.....#",
        "#####.#.T.##########",
        "#*....#............#",
        "#.#####.######.T...#",
        "#.......#....#.....#",
        "#########.##.#.#####",
        "#T........##.#.*...#",
        "#...########.#####.#",
        "####################"
    };

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            game->grid[y][x] = template_map[y][x];
            if (template_map[y][x] == 'P') {
                game->player.pos.x = x;
                game->player.pos.y = y;
                game->grid[y][x] = '.'; 
            }
        }
    }

    game->player.madness = 0.0f;
    game->player.points_collected = 0;
    game->player.infra_red_ticks = 0;
    game->player.is_alive = true;
    game->player.has_won = false;
    game->ticks_passed = 0;
}
