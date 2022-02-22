#include "util.h"
#include <stdlib.h>


// Main Function
// Takes cli arguments, parses them & calls apropriate function
int main() {
    int column_size = 3, row_size = 3;
    // Create puzzle
        // Potentially add interface for creating a puzzle
    int *board = create_initial_board(column_size, row_size, 0);
    // Show board
    show_board(column_size, row_size, board);
    // Start A* algorithm
    // Each step of A*, show new board state (with a text saying the sliding-direction)
        // Show backtracking as steps too
    // When A* is done, end function
        // Don't forget to free all allocated memory!
    free(board);
    return 0;
}
