#include "util.h"
// #include <stdio.h> // For debugging

int get_priority(int moves_amount, int bias,  int column_size, int row_size, int *board) {
    return manhattan(moves_amount, bias, column_size, row_size, board);
}

// Hamming priority function. The number of blocks in the wrong position, plus the number of moves made so far to get to the state. Intutively, a state with a small number of blocks in the wrong position is close to the goal state, and we prefer a state that have been reached using a small number of moves.
int hamming(int moves_amount, int bias, int column_size, int row_size, int *board) {
    for (int i = 0, len = column_size * row_size; i < len; i++) {
        // if the value isn't the empty cell and is not in its correct position,
        // increment the minimum moves needed by 1
        if (board[i] && board[i] != i+1) moves_amount += bias;
    }
    return moves_amount;
}

// Manhattan priority function. The sum of the Manhattan distances (sum of the vertical and horizontal distance) from the blocks to their goal positions, plus the number of moves made so far to get to the state.
// Bias helps to make the amount of moves less relevant to the end result
int manhattan(int moves_amount, int bias, int column_size, int row_size, int *board) {
    int current_col, current_row, wanted_col, wanted_row;
    
    for (int i = 0, len = column_size * row_size; i < len; i++) {
        // if the value isn't the empty cell and is not in its correct position,
        // increment the minimum moves needed by the Manhattan distance
        // (also called taxicab distance, see https://en.wikipedia.org/wiki/Taxicab_geometry)
        if (board[i] && board[i] != i+1) {
            current_col = (int) i / column_size;
            wanted_col = (int) (board[i]-1) / column_size;
            current_row = i % column_size;
            wanted_row = (board[i]-1) % column_size;
            
            // Debugging:
            // printf("Board[i] = %i   Col: %i -> %i = %i   Row: %i -> %i = %i\n", board[i], current_col, wanted_col, distance(current_col, wanted_col), current_row, wanted_row, distance(current_row, wanted_row));
            
            moves_amount += bias * (distance(current_col, wanted_col) + distance(current_row, wanted_row));
        }
    }
    return moves_amount;
}