#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 10
#define MAX_MADNESS 100.0f
#define INFRARED_DURATION_TICKS 5 // Cat timp dureaza abilitatea in actiuni/tick-uri

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position pos;
    float madness;
    int points_collected;
    int infra_red_ticks;
    bool is_alive;
    bool has_won;
} Player;