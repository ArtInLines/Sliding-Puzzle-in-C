#include "general.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool is_solved(int len, u8 *board) {
    for (int i = 0; i < len-1; i++)
        if (board[i] != i+1) return false;
    // Last field doesn't have to be checked,
    // as long as all others are placed correctly
    return true;
}

bool is_solvable(Board board, AIL_Allocator *allocator) {
    if (solvable_helper(board, allocator) % 2) return false; // For odd amounts of transpositions, the puzzle is not solvable
    else return true; // For even amounts of transpositions, the puzzle is solvable
    // See ./literature for mathematical proves of this fact!
}

int solvable_helper(Board board, AIL_Allocator *allocator) {
    int i, transpositions_amount = 0, len = board.cols * board.rows;
    // Create copy of board where empty field is equal to the highest numbr (since it should be at the end of the board)
    Pos empty_field;
    u8 *fields_copy = allocator->alloc(allocator->data, len * sizeof(u8));
    for (i = 0; i < len; i++) {
        fields_copy[i] = board.fields[i];
        if (!fields_copy[i]) empty_field = index_to_pos(i, board.cols);
    }
    Board board_copy = copy_board(board, allocator);
    // Move the empty field to the last position with legal moves, so it can be ignored
    while (empty_field.x < board.rows-1) play_turn(DIR_DOWN, board_copy, &empty_field);
    while (empty_field.y < board.cols-1) play_turn(DIR_RIGHT, board_copy, &empty_field);

    // Loop through copied board (while ignoring empty field at last position)
    // When reaching an element out of order:
        // Swap the number with the number at the index, that this number should be at
        // i.e. if number 5 is at the first index, swap 5 with the number at the 5th index
        // Increase the amount of transpositions
    for (i = 0; i < len-1; i++) {
        while (board_copy.fields[i]-1 != i) {
            u8 idx = board_copy.fields[i] == 0 ? len-1 : board_copy.fields[i]-1;
            AIL_SWAP(board_copy.fields[i], board_copy.fields[idx]);
            transpositions_amount++;
        }
    }


    allocator->free_one(allocator->data, board_copy.fields);
    return transpositions_amount;
}

Pos get_empty_field(int column_size, u8 *board) {
    int i = 0;
    while (board[i]) i++;
    return index_to_pos(i, column_size);
}

Board create_initial_board(int cols, int rows, int variance, AIL_Allocator *allocator) {
    int len = cols * rows, i, r, empty_field = len - 1;
    Board board = { rows, cols, allocator->alloc(allocator->data, len * sizeof(int)) };

    // maybe do some more interesting calculation here someday:
    int swap_amount = variance;

    // Fill & shuffle board
    srand(time(NULL)); // init rand()
    for (i = 0; i < len; i++) board.fields[i] = i+1;
    board.fields[empty_field] = 0;
    for (i = 0; i < swap_amount || !is_solvable(board, allocator) || is_solved(len, board.fields); i++) {
        do { r = rand() % len; } while (r == empty_field);
        AIL_SWAP(board.fields[empty_field], board.fields[r]);
        empty_field = r;
    }

    return board;
}

Board copy_board(Board board, AIL_Allocator *allocator) {
    u16 len = board.rows * board.cols;
    u8 *fields = allocator->alloc(allocator->data, len * sizeof(u8));
    for (u16 i = 0; i < len; i++) fields[i] = board.fields[i];
    return (Board) {
        board.rows,
        board.cols,
        fields,
    };
}

Pos next_pos(Pos pos, Dir dir) {
    switch (dir) {
        case DIR_UP:
            pos.x--;
            break;
        case DIR_DOWN:
            pos.x++;
            break;
        case DIR_RIGHT:
            pos.y++;
            break;
        case DIR_LEFT:
            pos.y--;
            break;
        default:
            pos.x = UINT8_MAX;
            pos.y = UINT8_MAX;
    }
    return pos;
}

// If UINT8_MAX is returned, the move was illegal
u8 next_pos_idx(u8 pos, Dir dir, u8 rows, u8 cols) {
    switch (dir) {
        case DIR_UP:
            if (pos < cols) return UINT8_MAX;
            pos -= cols;
            break;
        case DIR_DOWN:
            pos += cols;
            if (pos >= rows*cols) return UINT8_MAX;
            break;
        case DIR_RIGHT:
            pos++;
            if (pos % cols == 0) return UINT8_MAX;
            break;
        case DIR_LEFT:
            if (pos % cols == 0) return UINT8_MAX;
            pos--;
            break;
        default:
            return UINT8_MAX;
    }
    return pos;
}

u8 next_pos_idx_unsafe(u8 pos, Dir dir, u8 cols) {
    switch (dir) {
        case DIR_UP:
            pos -= cols;
            break;
        case DIR_DOWN:
            pos += cols;
            break;
        case DIR_RIGHT:
            pos++;
            break;
        case DIR_LEFT:
            pos--;
            break;
        default:
            AIL_UNREACHABLE();
    }
    return pos;
}

bool is_legal_pos(Pos p, u8 rows, u8 cols) {
    return p.x < rows && p.y < cols;
}


// Print Board:

#define __board_print_row(cols, cell_width, border_char)                                                                           \
    for (int __print_row_idx = 0, __print_row_len = (cols) * (cell_width); __print_row_idx < __print_row_len; __print_row_idx++) { \
        printf("%c ", (border_char));                                                                                              \
    }                                                                                                                              \
    printf("%c\n", (border_char));

void show_board(Board board) {
    // Prints the following for a finished 8-puzzle board:
    // * * * * * * * * * *
    // *  1  *  2  *  3  *
    // * * * * * * * * * *
    // *  4  *  5  *  6  *
    // * * * * * * * * * *
    // *  7  *  8  *  -  *
    // * * * * * * * * * *

    char border_char = '*', empty_field_char = '-';
    int i, j, cell;
    float cell_width = 4;
    char before_str[4] = "  ", after_str[4] = "  ";

    __board_print_row(board.cols, cell_width, border_char);
    for (i = 0; i < board.rows; i++) {
        for (j = 0; j < board.cols; j++) {
            cell = board.fields[i*board.cols + j];

            if (cell >= 100) after_str[2] = 0;
            else after_str[2] = ' ';
            if (cell >= 10) before_str[2] = 0;
            else before_str[2] = ' ';

            printf("%c%s", border_char, before_str);
            if (cell) printf("%i", cell);
            else printf("%c", empty_field_char);
            printf(after_str);
        }
        printf("%c\n", border_char);
        __board_print_row(board.cols, cell_width, border_char);
    }
}

int compare_board(u8 *board1, u8 *board2, int len) {
    for (int i = 0; i < len; i++) if(board1[i] != board2[i]) return 0;
    return 1;
}