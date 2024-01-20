// To build, run the following command:
// gcc main.c board.c priority.c test.c util.c play.c pathfinding.c -o main.exe

#include "general.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const int default_is_player_turn = 1;
const int default_variance = 20;
const int default_inverted = 0;
const int default_bias = 10;
const int default_sleep_time = 500;

void get_setup_values(int advanced_setup, int *column_size, int *row_size, int *is_player_turn, int *variance, int *inverted, int *bias, int *sleep_time) {
    char *puzzle_size_string = "Wondeful, you chose to play on a %ix%i board\n";

    if (!advanced_setup) {
        while (*column_size <= 1) {
            printf("Enter the puzzle's length please (>1): ");
            scanf("%d", column_size);
        }
        *row_size = *column_size;
        printf(puzzle_size_string, *column_size, *row_size);

        *is_player_turn = default_is_player_turn;
        *inverted = default_inverted;
        *variance = default_variance;
        *bias = default_bias;
        *sleep_time = default_sleep_time;
    } else {
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
        while (*bias <= 0) {
            printf("What should the bias for calculating the priority be? ");
            scanf("%d", bias);
        }
        printf("Do you want the movements to be inverted (1) or not (0)? ");
        scanf("%d", inverted);

        printf("How many miliseconds should the computer wait between each move? ");
        scanf("%d", sleep_time);

        printf("Do you wish to try the puzzle yourself (1) or let the computer solve it for you (0)? ");
        scanf("%d", is_player_turn);
    }
}

// Main Function
// Takes cli arguments, parses them & calls apropriate function
int main(void) {
    int column_size = 0, row_size = 0, is_player_turn = 1, turn_counter = 0, variance = 0, *empty_field, inverted = 0, bias = 0, sleep_time = 0, advanced_setup;
    clear_screen();
    // Set up game
    printf("Do you want a simplified (0) or advanced (1) setup? ");
    scanf("%d", &advanced_setup);
    get_setup_values(advanced_setup, &column_size, &row_size, &is_player_turn, &variance, &inverted, &bias, &sleep_time);

    // TODO: Allow the player to input their own initial puzzle state
    int len = column_size*row_size;
    int *board = create_initial_board(column_size, row_size, variance);
    empty_field = get_empty_field(column_size, board);

    // Show board
    clear_screen();
    printf("Here's the puzzle:\n\n");
    show_board(column_size, row_size, board);

    printf("Turn #%i\n", turn_counter);
    printf("Priority: %i\n", get_priority(turn_counter, bias, column_size, row_size, board) - turn_counter);


    while (is_player_turn && !is_solved(len, board)) {
        // Get direction & play
        int direction, play_turn_return_code = -1;
        while (play_turn_return_code != SUCCESS) {
            direction = get_direction(inverted);

            if (direction == GETHELP) {
                int *path = A_star(column_size, row_size, bias, empty_field, board, 1);
                direction = path[0];
                free(path);
            }
            else if (direction == FINISH) goto FINISH_PUZZLE;

            play_turn_return_code = play_turn(direction, column_size, row_size, board, empty_field);
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
        show_board(column_size, row_size, board);

        printf("Turn Counter: %i\n", turn_counter);
        printf("Priority: %i\n", get_priority(turn_counter, bias, column_size, row_size, board) - turn_counter);
    }

    if (!is_player_turn) {
    FINISH_PUZZLE:
        printf("Processing... \n");
        int *path = A_star(column_size, row_size, bias, empty_field, board, 1);
        printf("done!\n");
        int i = 0;
        while (path[i] != -1) {
            if (i) {
                Sleep(sleep_time);
                // printf("Press enter to continue\n");
                // getchar();
            }
            play_turn(path[i], column_size, row_size, board, empty_field);
            // Show board
            clear_screen();
            printf("Moved %s\n", get_direction_string(inverted ? invert_direction(path[i]) : path[i]));
            show_board(column_size, row_size, board);
            turn_counter++;
            i++;
        }
        printf("The computer's solution was another %i moves\n", i);
        free(board);
    }

    printf("\nThe puzzle was solved in %i moves! \\o/\n", turn_counter);

    // Don't forget to free all allocated memory!
    free(board);
    free(empty_field);

    // test_solvable();
    return 0;
}
