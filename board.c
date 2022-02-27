#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int is_solved(int len, int *board) {
    int i = 0;
    while (i < len-1) if (board[i] != ++i) return 0;
    // Last field doesn't have to be checked,
    // as long as all others are placed correctly
    return 1;
}

int is_solvable(int len, int *board) {
    if (count_transpositions(len, board) % 2) return 0; // For odd amounts of transpositions, the puzzle is not solvable
    else return 1; // For even amounts of transpositions, the puzzle is solvable
    
    // See ./literature for mathematical proves of this fact!
}

int count_transpositions(int len, int *board) {
    int i, j, index, cell, transpositions_amount = 0;
    // Create copy of board without empty field
    int *board_copy = malloc((len-1) * sizeof(int));
    for (i = 0, j = 0; i < len; i++) {
        if (!board[i]) continue;
        board_copy[j] = board[i];
        j++;
    }
    
    // Loop through copied board
    // When reaching an element out of order:
        // Swap the number with the number at the index, that this number should be at
        // i.e. if number 5 is at the first index, swap 5 with the number at the 5th index
        // Increase the amount of transpositions   
    for (i = 0; i < len-1; i++) {
        while (board_copy[i]-1 != i) {
            printf("(%i %i)  -  (%i %i)  -  %i\n", i, board_copy[i]-1, board_copy[i], board_copy[board_copy[i]-1], transpositions_amount+1);
            
            swap_ints(&board_copy[i], &board_copy[board_copy[i]-1]);
            transpositions_amount++;
        }
    }

    free(board_copy);
    return transpositions_amount;
}

int* create_initial_board(int column_size, int row_size, int variance) {
    int len = column_size * row_size;
    int *board = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) board[i] = i+1;
    board[len-1] = 0;
    swap_ints(&board[len-1], &board[len-1-column_size]);
    swap_ints(&board[len-1-column_size], &board[len-2-column_size]);
    return board;
}

int* get_new_pos(int *old_pos, int dir) {
    int *new_pos = malloc(2 * sizeof(int));
    new_pos[0] = old_pos[0];
    new_pos[1] = old_pos[1];
    
    switch (dir) {
        case UP:
            new_pos[0]--;
            break;
        case DOWN:
            new_pos[0]++;
            break;
        case RIGHT:
            new_pos[1]++;
            break;
        case LEFT:
            new_pos[1]--;
            break;
        default:
            new_pos[0] = -1;
            new_pos[1] = -1;
    }
    return new_pos;
}


// Print Board:

void print_row(int *column_size, int *cell_width, char *border_char) {
    for (int i = 0, len = (*column_size) * (*cell_width); i < len; i++) {
        printf("%c ", *border_char);
    }
    printf("%c\n", *border_char);
}

void show_board(int column_size, int row_size, int *board) {
    // Prints the following for a finished 8-puzzle board:
    // * * * * * * * * * *
    // *  1  *  2  *  3  *
    // * * * * * * * * * *
    // *  4  *  5  *  6  *
    // * * * * * * * * * *
    // *  7  *  8  *  -  *
    // * * * * * * * * * *
    
    char border_char = '*', empty_field_char = '-';
    int len = column_size * row_size, cell_width = 3, i, j, cell;
    char before_str[3] = "  ", after_str[3] = "  ";
    
    if ((len-1)>10) {
        before_str[2] = ' ';
        cell_width++;
    }
    if ((len-1)>100) { 
        after_str[2] = ' ';
        cell_width++;
    }
    
    print_row(&column_size, &cell_width, &border_char);
    for (i = 0; i < row_size; i++) {
        for (j = 0; j < column_size; j++) {
            cell = board[i*column_size + j];
            printf("%c%s", border_char, before_str);
            if (cell) printf("%i", cell);
            else printf("%c", empty_field_char);
            printf(after_str);
        }
        printf("%c\n", border_char);
        print_row(&column_size, &cell_width, &border_char);
    }
}

int compare_board(int *board1, int *board2, int len) {
    for (int i = 0; i < len; i++) if(board1[i] != board2[i]) return 0;
    return 1;
}

// int my_pow(int x, int n) {
//     for (int i = 1; i < n; i++) x *= x;
//     return x;
// }

// int create_board_id(int *board, int len) {
//     int id = board[0];
//     for (int i = 1; i < len; i++) {
//         // id ^= (board[i] << (i % (sizeof(int) * 8)));
//         id += my_pow(10, i) * board[i];
//         printf("i: %i - ID: %i\n", i, id);
//     }
//     return id;
// }