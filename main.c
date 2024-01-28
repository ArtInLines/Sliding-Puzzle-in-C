// #define AIL_ALLOC_PRINT_MEM
#define AIL_ALL_IMPL
// #define AIL_MD_IMPL
// #define AIL_MD_MEM_DEBUG
// #define AIL_MD_MEM_PRINT
// #include "ail/ail_md.h"

#include "general.h"
#include "board.c"
#include "priority.c"
#include "play.c"
#include "pathfinding.c"
#include "screen.h"
#include <stdlib.h>
#include <stdio.h>

const int default_is_player_turn = 1;
const int default_variance = 20;
const int default_inverted = 0;
const int default_bias = 10;
const int default_sleep_time = 500;
search_t *const default_search = &bfs;

void get_setup_values(int advanced_setup, int *column_size, int *row_size, int *is_player_turn, int *variance, int *inverted, int *bias, int *sleep_time, search_t **search) {
    char *puzzle_size_string = "Wonderful, you chose to play on a %ix%i board\n";

    if (!advanced_setup) {
        while (*column_size <= 1) {
            printf("Enter the puzzle's length please (>1): ");
            scanf("%d", column_size);
        }
        *row_size = *column_size;
        printf(puzzle_size_string, *column_size, *row_size);

        *is_player_turn = default_is_player_turn;
        *inverted       = default_inverted;
        *variance       = default_variance;
        *bias           = default_bias;
        *sleep_time     = default_sleep_time;
        *search         = default_search;
    } else {
        // @TODO: Add ability to just press enter and thus use the default setting
        while (*column_size <= 1) {
            printf("Enter the puzzle's column size (>1): ");
            scanf("%d", column_size);
        }
        while (*row_size <= 1) {
            printf("Enter the puzzle's row size (>1): ");
            scanf("%d", row_size);
        }
        printf(puzzle_size_string, *column_size, *row_size);
        while (*variance <= 0) {
            // Add better description
            printf("Enter the initial game board's variance (>0): ");
            scanf("%d", variance);
        }
        printf("Do you want the movements to be inverted (1) or not (0)? ");
        scanf("%d", inverted);

        bool valid_algo = false;
        char algo[128] = {0};
        while (!valid_algo) {
            printf("What algorithm do you want the computer to use (dfs, bfs)? ");
            scanf("%s", algo);
            valid_algo = true;
            if      (strcmp(algo, "dfs") == 0) *search = &dfs;
            else if (strcmp(algo, "bfs")     == 0) *search = &bfs;
            else valid_algo = false;
        }

        while (*bias <= 0) {
            printf("What should the bias for calculating the priority be? ");
            scanf("%d", bias);
        }

        printf("How many miliseconds should the computer wait between each move? ");
        scanf("%d", sleep_time);

        printf("Do you wish to try the puzzle yourself (1) or let the computer solve it for you (0)? ");
        scanf("%d", is_player_turn);
    }
}

// Main Function
// Takes cli arguments, parses them & calls apropriate function
int main(void) {
    AIL_Allocator allocator = ail_alloc_std;
    search_t *search;
    int column_size = 0, row_size = 0, is_player_turn = 1, turn_counter = 0, variance = 0, inverted = 0, bias = 0, sleep_time = 0, advanced_setup;
    clear_screen();
    // Set up game
    printf("Do you want a simplified (0) or advanced (1) setup? ");
    scanf("%d", &advanced_setup);
    get_setup_values(advanced_setup, &column_size, &row_size, &is_player_turn, &variance, &inverted, &bias, &sleep_time, &search);

    // TODO: Allow the player to input their own initial puzzle state
    int len = column_size*row_size;
    Board board = create_initial_board(column_size, row_size, variance, &allocator);
    Pos empty_field = get_empty_field(column_size, board.fields);

    // Show board
    clear_screen();
    printf("Here's the puzzle:\n\n");
    show_board(board);

    printf("Turn #%i\n", turn_counter);
    printf("Priority: %i\n", get_priority(turn_counter, bias, board) - turn_counter);


    while (is_player_turn && !is_solved(len, board.fields)) {
        // Get direction & play
        int direction, play_turn_return_code = -1;
        while (play_turn_return_code != SUCCESS) {
            direction = get_direction(inverted);

            if (direction == GETHELP) {
                AIL_DA(Dir) path = search(pos_to_index(empty_field, board.cols), board, &allocator);
                direction = path.data[0];
                ail_da_free(&path);
            }
            else if (direction == FINISH) goto FINISH_PUZZLE;

            play_turn_return_code = play_turn(direction, board, &empty_field);
            switch (play_turn_return_code) {
                case MOVE_OUTSIDE_BORDERS:
                    printf("The empty field can't be moved outside the puzzle's borders! Try again!\n");
                    break;

                case ILLEGAL_DIRECTION:
                    printf("Whatever you did, this is not allowed! Try again!\n");
                    break;
            }
        }

        turn_counter++;
        // Show board
        clear_screen();
        show_board(board);

        printf("Turn Counter: %i\n", turn_counter);
        printf("Priority: %i\n", get_priority(turn_counter, bias, board) - turn_counter);
    }

    if (!is_player_turn) {
    FINISH_PUZZLE:
        printf("Processing... \n");
        AIL_DA(Dir) path = search(pos_to_index(empty_field, board.cols), board, &allocator);
        printf("done!\n");
        for (u32 i = 0; i < path.len; i++) {
            if (i) {
                Sleep(sleep_time);
                // printf("Press enter to continue\n");
                // getchar();
            }
            play_turn(path.data[i], board, &empty_field);
            // Show board
            clear_screen();
            printf("Moved %s\n", get_direction_string(inverted ? invert_direction(path.data[i]) : path.data[i]));
            show_board(board);
            turn_counter++;
        }
        printf("The computer's solution was another %d moves\n", path.len);

        ail_da_free(&path);
    }

    printf("\nThe puzzle was solved in %i moves! \\o/\n", turn_counter);

    // Don't forget to free all allocated memory!
    allocator.free_one(allocator.data, board.fields);

    return 0;
}
