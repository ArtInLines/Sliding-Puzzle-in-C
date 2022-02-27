#include "util.h"
#include <stdlib.h>
#include <stdio.h>

// #ifdef _WIN32
// #include <windows.h>
// #else
// #include <unistd.h>
// #endif


// Main Function
// Takes cli arguments, parses them & calls apropriate function
int main() {
    int column_size = 3, row_size = 3, len = column_size*row_size, is_player_turn = 1, turn_counter = 0;
    // Create puzzle
        // Potentially add interface for creating a puzzle
    int *board = create_initial_board(column_size, row_size, 0);
    // Start Game
    init_game(column_size, row_size, board, 0);
    // Show board
    clear_screen();
    show_board(column_size, row_size, board);
    
    
    while (is_player_turn && !is_solved(len, board)) {
        // Get direction & play
        int direction, play_turn_return_code = -1;
        while (play_turn_return_code != SUCCESS) {
            direction = get_direction();
            
            if (direction == GETHELP) direction = get_next_move();
            else if (direction == FINISH) goto FINISH_PUZZLE;
            
            play_turn_return_code = play_turn(direction);
            switch (play_turn_return_code) {
                case MOVE_OUTSIDE_BORDERS:
                    printf("The empty field can't be moved outside the puzzle's borders! Try again!\n");
                    break;
                
                case REPEATED_MOVE_BACK:
                    printf("This move was restricted to stop you from moving back and forth forever in a loop. Try again!\n");
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
    }
    
    if (!is_player_turn) {
    FINISH_PUZZLE:
        printf("Processing...\n");
        int *path = A_star();
        int i = 0;
        // while (path[i] != -1) {
        //     // if (i) sleep(100);
        //     if (i) {
        //         printf("Press any key to continue");
        //         getchar();
        //         putchar((int) '\n');
        //     }
        //     play_turn(path[i]);
        // }
    }
    
    printf("\nThe puzzle was solved in %i moves! \\o/", turn_counter);
    
    // Don't forget to free all allocated memory!
    free(board);
    
    // test_solvable();
    return 0;
}
