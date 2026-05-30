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

bool is_near_torch(const GameState *game) {
    for(int dy = -2; dy <= 2; dy++) {
        for(int dx = -2; dx <= 2; dx++) {
            int ny = game->player.pos.y + dy;
            int nx = game->player.pos.x + dx;
            if (ny >= 0 && ny < MAP_HEIGHT && nx >= 0 && nx < MAP_WIDTH) {
                if (game->grid[ny][nx] == 'T') return true;
            }
        }
    }
    return false;
}

void update_madness(GameState *game) {
    if (is_near_torch(game)) {
        game->player.madness -= 5.0f; // Scade madness langa torta
        if (game->player.madness < 0) game->player.madness = 0;
    } else {
        game->player.madness += 2.5f; // Creste in intuneric
    }

    if (game->player.madness >= MAX_MADNESS) {
        game->player.is_alive = false;
    }
}

void check_collisions(GameState *game) {
    int px = game->player.pos.x;
    int py = game->player.pos.y;

    if (game->grid[py][px] == '*') {
        game->player.points_collected++;
        game->grid[py][px] = '.'; 
    }

    if (game->player.points_collected == 3 && game->player.madness < MAX_MADNESS) {
        game->player.has_won = true;
    }
}

void process_tick(GameState *game, char input) {
    game->ticks_passed++;
    int nx = game->player.pos.x;
    int ny = game->player.pos.y;

    if (input == 'w') ny--;
    if (input == 's') ny++;
    if (input == 'a') nx--;
    if (input == 'd') nx++;
    if (input == 'i') game->player.infra_red_ticks = INFRARED_DURATION_TICKS;

    if (game->grid[ny][nx] != '#') { 
        game->player.pos.x = nx;
        game->player.pos.y = ny;
    }

    if (game->player.infra_red_ticks > 0) {
        game->player.infra_red_ticks--;
    }

    check_collisions(game);
    update_madness(game);
}

void render_game_to_buffer(const GameState *game, char *buffer) {
    char line[256];
    buffer[0] = '\0'; // Golim bufferul
    
    sprintf(line, "--- BLIND MAZE ---\n");
    strcat(buffer, line);
    sprintf(line, "Puncte: %d/3 | Madness: %.1f%% | Infra-Red: %s\n", 
           game->player.points_collected, 
           game->player.madness, 
           game->player.infra_red_ticks > 0 ? "ACTIV" : "Inactiv (apasa 'i')");
    strcat(buffer, line);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            bool in_sight = (abs(game->player.pos.x - x) <= 1 && abs(game->player.pos.y - y) <= 1);
            bool is_torch_and_ir_active = (game->grid[y][x] == 'T' && game->player.infra_red_ticks > 0);

            if (game->player.pos.x == x && game->player.pos.y == y) {
                strcat(buffer, "P");
            } else if (in_sight || is_torch_and_ir_active) {
                sprintf(line, "%c", game->grid[y][x]);
                strcat(buffer, line);
            } else {
                strcat(buffer, " "); // Fog of war
            }
        }
        strcat(buffer, "\n");
    }

    strcat(buffer, "\nControale: w/a/s/d (miscare) | i (infra-red) | q (iesire)\n");

    if (game->player.has_won) {
        strcat(buffer, "\nFELICITARI! Ai colectat toate punctele si ai supravietuit!\n");
    } else if (!game->player.is_alive) {
        strcat(buffer, "\nGAME OVER. Nivelul de Madness a atins 100%.\n");
    }
}
