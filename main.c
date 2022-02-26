#include "util.h"
#include <stdlib.h>


// Main Function
// Takes cli arguments, parses them & calls apropriate function
int main() {
    int column_size = 3, row_size = 3, direction;
    // Create puzzle
        // Potentially add interface for creating a puzzle
    int *board = create_initial_board(column_size, row_size, 0);
    // Start Game
    init_game(column_size, row_size, board);
    // Start A* algorithm
    // Each step of A*, show new board state (with a text saying the sliding-direction)
        // Show backtracking as steps too
    while (1) {
        // Show board
        show_board(column_size, row_size, board);
        
        // Get direction
        direction = 0;
        play_turn(direction);
    }
    
    // When A* is done, end function
        // Don't forget to free all allocated memory!
    free(board);
    
    // test_solvable();
    return 0;
}
