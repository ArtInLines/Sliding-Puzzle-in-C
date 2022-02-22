#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int is_solvable(int len, int *board) {
    for (int i = 0; i < len; i++) {
        // ...
    }
}

int* create_initial_board(int column_size, int row_size, int variance) {
    int len = column_size * row_size;
    int *board = malloc(len * sizeof(int));
    int i = 0;
    while (i < len) board[i] = ++i;
    board[len-1] = 0;
    return board;
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