#include "util.h"
#include <stdlib.h>
#include <stdio.h>

// Main Function
// Takes cli arguments, parses them & calls apropriate function
int main() {
    int column_size = 3, row_size = 3, direction, play_turn_return_code;
    // Create puzzle
        // Potentially add interface for creating a puzzle
    int *board = create_initial_board(column_size, row_size, 0);
    // Start Game
    init_game(column_size, row_size, board, 0);
    // Start A* algorithm
    // Each step of A*, show new board state (with a text saying the sliding-direction)
        // Show backtracking as steps too
    do {
        // Show board
        show_board(column_size, row_size, board);
        
        // Get direction & play
        play_turn_return_code = -1;
        while (play_turn_return_code != SUCCESS) {
            direction = get_direction();
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
    } while (!is_solved(column_size*row_size, board));
    
    
    // Don't forget to free all allocated memory!
    free(board);
    
    // test_solvable();
    return 0;
}
