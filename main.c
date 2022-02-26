#include "util.h"
#include <stdlib.h>
#include <stdio.h>

void let_player_move() {
    int direction, play_turn_return_code = -1;
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
}

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
    
    
    // Start A* algorithm
    // Each step of A*, show new board state (with a text saying the sliding-direction)
        // Show backtracking as steps too
    
    
    while (!is_solved(len, board)) {
        // Get direction & play
        if (is_player_turn) let_player_move();
        else A_star_next_step();
        
        turn_counter++;
        // Show board
        clear_screen();
        show_board(column_size, row_size, board);
    }
    
    printf("\nThe puzzle was solved in %i moves! \\o/", turn_counter);
    
    // Don't forget to free all allocated memory!
    free(board);
    
    // test_solvable();
    return 0;
}
