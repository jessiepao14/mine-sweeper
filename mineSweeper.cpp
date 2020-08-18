#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "io-utilities.hpp"
#include "mineSweeper.hpp"

#define BUFFER_SIZE 1024
#define MAX_PROBABILITY 100

#define OPEN_CHAR '.'
#define WEEPER_CHAR 'W'

mcw_game* alloc_game(int width, int height) {
    mcw_game* game = (mcw_game*) (malloc(sizeof(mcw_game)));
    game->width = width;
    game->height = height;
    game->field = (mcw_square**) malloc(height * sizeof(mcw_square*));
    game->status = (mcw_status**) malloc(height * sizeof(mcw_status*));
    for (int i = 0; i < height; i++) {
        game->field[i] = (mcw_square*) malloc(width * sizeof(mcw_square));
        game->status[i] = (mcw_status*) malloc(width * sizeof(mcw_status));
        for (int x = 0; x < width; x++) {
            game->field[i][x] = open;
            game->status[i][x] = hidden;
        }
    }
    return game;
}

void free_game(mcw_game* game) {
    for (int i = 0; i < game->height; i++) {
        free(game->field[i]);
        free(game->status[i]);
    }
    free(game->status);
    free(game->field);
    free(game);
}

void show_or_hide(mcw_game* game, int shown) {
    for (int i = 0; i < game->height; i++) {
        for (int x = 0; x < game->width; x++) {
            game->status[i][x] = (mcw_status) shown;
        }
    }
}
void show_all(mcw_game* game) {
    show_or_hide(game, revealed);
}

void hide_all(mcw_game* game) {
    show_or_hide(game, hidden);
}

mcw_game* initialize_file_game(char* filename) {
    FILE* game_file = fopen(filename, "r");
    if (!game_file) {
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    mcw_game* game = NULL;
    int y = 0;
    while (!feof(game_file)) {
        fgets(buffer, BUFFER_SIZE, game_file);
        if (game == NULL) {
            int width;
            int height;
            int successful_scans = sscanf(buffer, "%d %d", &width, &height);
            if (successful_scans < 2) {
                fclose(game_file);
                return NULL;
            } else {
                game = alloc_game(width, height);
                if (game == NULL) {
                    fclose(game_file);
                    return NULL;
                }
            }
        } else {
            if (y >= game->height) {
                break;
            }

            int x;
            for (x = 0; x < game->width; x++) {
                if (buffer[x] == OPEN_CHAR || buffer[x] == WEEPER_CHAR) {
                    game->field[y][x] = buffer[x] == OPEN_CHAR ? open : weeper;
                } else {
                    fclose(game_file);
                    return NULL;
                }
            }

            y++;
        }
    }

    fclose(game_file);
    return game;
}

int get_adjacent_weeper_count(mcw_game* game, int x, int y) {
    int count = 0;
    if(game->field[y][x] != weeper || is_in_game_bounds(game, x, y)) {
        for(int i = y - 1; i <= y + 1; i++) {
            for (int w = x - 1; w <= x + 1; w++) {
                if (is_in_game_bounds(game, w, i) && game->field[i][w] == weeper) {
                    count++;
                }
            }
        }
    }
    return count;
}

void display_game_field(mcw_game* game) {
    std::cout << ("  ");
    for (int i = 0; i < game->width; i ++) {
        printf(" %d", i);
    }
    std::cout << std::endl;
    for (int y = 0; y < game->height; y++) {
        printf(" %d", y);
        for (int x = 0; x < game->width; x++) {
            if(game->field[y][x] == weeper) {
                emit_utf_8(WEEPER);
            } else {
                int adj = get_adjacent_weeper_count(game, x, y);
                adj == 0 ? printf("  "):printf(" %d", adj);
            }
        }
        std::cout << std::endl;
    }
}

mcw_game* initialize_random_game(int width, int height, int probability) {
    mcw_game* game = alloc_game(width, height);
    if (game == NULL) {
        return NULL;
    }

    int threshold = probability > MAX_PROBABILITY ? MAX_PROBABILITY : probability;

    int x, y;
    for (y = 0; y < game->height; y++) {
        for (x = 0; x < game->width; x++) {
            int mine_roll = random() % MAX_PROBABILITY;
            game->field[y][x] = mine_roll < threshold ? weeper : open;
        }
    }
    hide_all(game);
    return game;
}

bool is_in_game_bounds(mcw_game* game, int x, int y) {
    if (x >= 0 && x < game->width && y >= 0 && y < game->height) {
        return true;
    }
    return false;
}
