#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>

#include "io-utilities.hpp"
#include "stringPlus.hpp"
#include "mineSweeper.hpp"

#define DEFAULT_PROBABILITY 10
#define MAX_COMMAND_LENGTH 1024

bool dimension_args(int argc, char** argv) {
    return argc == 3 && is_non_negative_numeral(argv[1]) && is_non_negative_numeral(argv[2]);
}

bool dimension_and_probability_args(int argc, char** argv) {
    return argc == 4 && is_non_negative_numeral(argv[1]) && is_non_negative_numeral(argv[2]) &&
        is_non_negative_numeral(argv[3]);
}

bool file_args(int argc, char** argv) {
    return argc == 2;
}


mcw_game* create_game_based_on_args(int argc, char** argv) {
    if (dimension_args(argc, argv) || dimension_and_probability_args(argc, argv)) {
        int width = atoi(argv[1]);
        int height = atoi(argv[2]);
        int probability = dimension_and_probability_args(argc, argv) ? atoi(argv[3]) : DEFAULT_PROBABILITY;

        if (width < 1 || height < 1) {
            std:: cout<< "Sorry, but zero dimensions are not allowed." << std::endl;
            std::cout << std::endl;
            return NULL;
        }

        return initialize_random_game(width, height, probability);
    } else if (file_args(argc, argv)) {
        mcw_game* game = initialize_file_game(argv[1]);
        if (game == NULL) {
            puts("Sorry, but something appears to be amiss with the game file.");
            puts("");
        }

        return game;
    } else {
        return NULL;
    }
}

void display_help() {
    std:: cout<< "You have the following command choices:" << std::endl;
    std:: cout<< "- 'o <x> <y>' will open a square" << std::endl;
    std:: cout<< "- 'm <x> <y>' will mark a square" << std::endl;
    std:: cout<< "- 'h' will display this help listing" << std::endl;
    std:: cout<< "- 'n' will start a new game" << std::endl;
    std:: cout<< "- 'q' will quit" << std::endl;
}

typedef enum {
    command_handled,
    command_new,
    command_unrecognized,
    command_quit
} command_result;

command_result process_command(char* command_string, mcw_game* game) {
    char command;
    int x;
    int y;
    int successful_scans = sscanf(command_string, "%c %d %d", &command, &x, &y);
    if (successful_scans == 1) {
        if (command == 'h') {
            display_help();
            return command_handled;
        }

        if (command == 'n' || command == 'q') {
            // If the player quits or starts over, we display the full board.
            std:: cout<< "\nGame over! Here is the entire field:\n" << std::endl;
            show_all(game);
            display_game_state(game);

            if (command == 'n') {
                return command_new;
            }

            if (command == 'q') {
                return command_quit;
            }
        }
    } else if (successful_scans == 3) {
        if (!is_in_game_bounds(game, x, y)) {
            printf("Sorry, but either x=%d or y=%d is out of range.\n", x, y);
            return command_handled;
        }

        if (command == 'm') {
            mark_game_square(game, x, y);
            return command_handled;
        }

        if (command == 'o') {
            reveal_game_square(game, x, y);
            return command_handled;
        }
    }
    return command_unrecognized;
}

int main(int argc, char** argv) {
    
    srandom(time(NULL));

    mcw_game* game = create_game_based_on_args(argc, argv);
    if (game == NULL) {
        emit_utf_8(MARKER);
        printf(" Welcome to Mine-C-Weeper! ");
        emit_utf_8(WEEPER);
        puts("");
        std:: cout<< "Usage: mine-c-weeper <width> <height> [<weeper frequency>]" << std::endl;
        std:: cout<< "   or: mine-c-weeper <filename>" << std::endl;
        std:: cout<< std::endl;
        printf("The default weeper frequency is %d (i.e., %d%%).\n", DEFAULT_PROBABILITY, DEFAULT_PROBABILITY);
        std:: cout<< std::endl;
        std:: cout<< "Caution: File error-checking is minimal, so if the map file is in the wrong format," << std::endl;
        std:: cout<< "         this program may just terminate without warning." << std::endl;
        return 1;
    }

    bool done = false;
    display_help();
    while (!done) {
        std:: cout<< std::endl;

        int weeper_count = get_weeper_count(game);
        int flag_count = get_flag_count(game);
        printf("There are %d weeper%s here.\n", weeper_count, weeper_count == 1 ? "" : "s");
        printf("You have planted %d flag%s.\n", flag_count, flag_count == 1 ? "" : "s");
        std:: cout<< std::endl;
        display_game_state(game);
        std:: cout<< std::endl;
        std:: cout<< "What would you like to do? (enter 'h' for help)" << std::endl;

        char command_string[MAX_COMMAND_LENGTH];
        fgets(command_string, MAX_COMMAND_LENGTH, stdin);
        command_result result = process_command(command_string, game);

        switch (result) {
            case command_handled:
                break;

            case command_new:
                free_game(game);
                game = create_game_based_on_args(argc, argv);
                std:: cout << "\nOK, starting over!" << std::endl;
                break;

            case command_unrecognized:
                std:: cout << "\nSorry, that command was not recognized." << std::endl;
                break;

            case command_quit:
                std:: cout << "\nThank you for playing Mine-C-Weeper!" << std::endl;
                done = true;
                break;
        }

        if (!done) {
            bool player_lost = is_game_over_loss(game);
            bool player_won = is_game_over_win(game);
            if (player_lost || player_won) {
                std::string message = (player_lost) ? "Sorry, you have revealed a weeper!" :"Congratulations! You have cleared the field.";
                printf("\n%s Thank you for playing.\n\n", message.c_str());
                display_game_state(game);
                std:: cout << std::endl;
                std:: cout << "Here is the whole field, revealed!" << std::endl;;
                std:: cout << std::endl;
                show_all(game);
                display_game_state(game);
                std:: cout << std::endl;
                done = true;
            }
        }
    }

    free_game(game);
    return 0;
}
