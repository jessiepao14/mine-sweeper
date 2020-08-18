#ifndef _MINE_C_WEEPER_HPP
#define _MINE_C_WEEPER_HPP

#include <stdbool.h>

#define HIDDEN 0x1F535
#define WEEPER 0x1F4A3 
#define MARKER 0x1F6A9

typedef enum {
    open=0,
    weeper=1
} mcw_square;

typedef enum {
    hidden=0,
    revealed=1,
    flagged=2
} mcw_status;

typedef struct {
    int width;
    int height;
    mcw_square** field;
    mcw_status** status;
} mcw_game;

void free_game(mcw_game* game);

void hide_all(mcw_game *game);

void show_all(mcw_game *game);

void show_or_hide(mcw_game *game, int shown);

mcw_game* initialize_random_game(int width, int height, int probability);

mcw_game* initialize_file_game(char* filename);

void display_game_field(mcw_game* game);

int get_adjacent_weeper_count(mcw_game* game, int x, int y);

bool is_in_game_bounds(mcw_game *game, int x, int y);

#endif
