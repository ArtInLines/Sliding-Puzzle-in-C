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
    // test_solvable();
    // return 0;
    
    
    int column_size = 0, row_size = 0, is_player_turn = 1, turn_counter = 0, variance = 0;
    // Create puzzle
        // Potentially add interface for creating a puzzle
    while (column_size <= 1) { printf("Enter the puzzle's column size (>1): "); scanf("%d", &column_size); }
    while (row_size <= 1) { printf("Enter the puzzle's row size (>1): "); scanf("%d", &row_size); }
    while (variance <= 0) { printf("Enter the initial game board's variance (>0): "); scanf("%d", &variance); } // Add better description
    printf("Do you wish to try the puzzle yourself (1) or let the computer solve it for you (0)? "); scanf("%d", &is_player_turn);
    
    // TODO: Allow the player to input their own initial puzzle state
    int len = column_size*row_size;
    int *board = create_initial_board(column_size, row_size, variance);
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
        printf("Processing... ");
        int *path = A_star();
        printf("done!\n");
        int i = 0;
        while (path[i] != -1) {
            if (i) {
                printf("Press any key to continue\n");
                getchar();
            }
            play_turn(path[i]);
            turn_counter++;
            i++;
            // Show board
            clear_screen();
            show_board(column_size, row_size, board);
        }
    }
    
    printf("\nThe puzzle was solved in %i moves! \\o/", turn_counter);
    
    // Don't forget to free all allocated memory!
    free(board);
    
    // test_solvable();
    return 0;
}
