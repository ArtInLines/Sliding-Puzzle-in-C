#include "general.h"

int get_priority(int moves_amount, int bias,  Board board) {
    return manhattan(moves_amount, bias, board);
}

// Hamming priority function. The number of blocks in the wrong position, plus the number of moves made so far to get to the state. Intutively, a state with a small number of blocks in the wrong position is close to the goal state, and we prefer a state that have been reached using a small number of moves.
int hamming(int moves_amount, int bias, Board board) {
    for (int i = 0, len = board.cols * board.rows; i < len; i++) {
        // if the value isn't the empty cell and is not in its correct position,
        // increment the minimum moves needed by 1
        if (board.fields[i] && board.fields[i] != i+1) moves_amount += bias;
    }
    return moves_amount;
}

// Manhattan priority function. The sum of the Manhattan distances (sum of the vertical and horizontal distance) from the blocks to their goal positions, plus the number of moves made so far to get to the state.
// Bias helps to make the amount of moves less relevant to the end result
int manhattan(int moves_amount, int bias, Board board) {
    int current_col, current_row, wanted_col, wanted_row;

    for (int i = 0, len = board.cols * board.rows; i < len; i++) {
        // if the value isn't the empty cell and is not in its correct position,
        // increment the minimum moves needed by the Manhattan distance
        // (also called taxicab distance, see https://en.wikipedia.org/wiki/Taxicab_geometry)
        if (board.fields[i] && board.fields[i] != i+1) {
            current_col = (int) i / board.cols;
            wanted_col = (int) (board.fields[i]-1) / board.cols;
            current_row = i % board.cols;
            wanted_row = (board.fields[i]-1) % board.cols;
            moves_amount += bias * (DIST(current_col, wanted_col) + DIST(current_row, wanted_row));
        }
    }
    return moves_amount;
}