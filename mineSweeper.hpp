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

mcw_game* initialize_random_game(int width, int height, int probability);

mcw_game* initialize_file_game(char* filename);

void display_game_field(mcw_game* game);

void show_all(mcw_game* game);

int get_weeper_count(mcw_game* game);

int get_flag_count(mcw_game* game);

int get_adjacent_weeper_count(mcw_game* game, int x, int y);

void display_game_state(mcw_game* game);

bool is_in_game_bounds(mcw_game* game, int x, int y);

void mark_game_square(mcw_game* game, int x, int y);

void reveal_game_square(mcw_game* game, int x, int y);

bool is_game_over_loss(mcw_game* game);

bool is_game_over_win(mcw_game* game);

#endif
