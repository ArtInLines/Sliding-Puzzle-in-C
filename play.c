#include "util.h"
#include <stdlib.h>

enum direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};


int g_column_size, g_row_size, *g_board, g_last_direction = -1, g_moved_back = 0, *g_empty_field_pos;


int init_game(int column_size, int row_size, int *board) {
    g_board = board;
    g_column_size = column_size;
    g_row_size = row_size;
    
    int i = 0;
    while (g_board[i]) i++;
    g_empty_field_pos = index_to_pos(i, g_column_size);
}

int play_turn(int direction) {
    // Check if turn is possible
    if (g_empty_field_pos[0] >= g_row_size-1 && direction == DOWN) return 0;
    else if (!g_empty_field_pos[0] && direction == UP) return 0;
    else if (g_empty_field_pos[1] >= g_column_size && direction == RIGHT) return 0;
    else if (!g_empty_field_pos[1] && direction == LEFT) return 0;
    
    if (g_last_direction == get_opposite_direction(direction)) g_moved_back++;
    else g_moved_back = 0;
    
    if (g_moved_back >= 2) return 0;
    
    int *affected_field;
}

int get_opposite_direction(int direction) {
    switch (direction) {
    case UP:
        return DOWN;
    
    case RIGHT:
        return LEFT;
        
    case DOWN:
        return UP;
        
    case LEFT:
        return RIGHT;
        
    default:
        return -1;
    }
}


