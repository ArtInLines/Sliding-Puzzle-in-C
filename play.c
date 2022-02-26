#include "util.h"
#include <stdlib.h>
#include <stdio.h>


int g_column_size, g_row_size, *g_board, g_last_direction = -1, g_moved_back = 0, *g_empty_field_pos, g_inverted;


int init_game(int column_size, int row_size, int *board, int inverted) {
    g_board = board;
    g_column_size = column_size;
    g_row_size = row_size;
    g_inverted = inverted;
    
    int i = 0;
    while (g_board[i]) i++;
    g_empty_field_pos = index_to_pos(i, g_column_size);
}

void move(int *pos) {
    swap_ints(&g_board[pos_to_index(pos, g_column_size)], &g_board[pos_to_index(g_empty_field_pos, g_column_size)]);
}

int play_turn(int direction) {
    int affected_field[2];
    affected_field[0] = g_empty_field_pos[0];
    affected_field[1] = g_empty_field_pos[1];
    
    switch (direction) {
    case UP:
        affected_field[0]++;
        break;
    case DOWN:
        affected_field[0]--;
        break;
    case RIGHT:
        affected_field[1]++;
        break;
    case LEFT:
        affected_field[1]--;
        break;
    default:
        return ILLEGAL_DIRECTION;
    }
    
    // Check if turn is possible
    if (affected_field[0] < 0 || !affected_field[1] < 0 || affected_field[0] >= g_row_size || affected_field[1] >= g_column_size) return MOVE_OUTSIDE_BORDERS;
    
    // Check if player moved back too often
    // to prevent loops of moving back and forth
    if (g_last_direction == get_opposite_direction(direction)) g_moved_back++;
    else g_moved_back = 0;
    if (g_moved_back >= 2) return REPEATED_MOVE_BACK;
    
    move(affected_field);
    return SUCCESS;
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
        return ERROR;
    }
}

int get_direction() {
    char key; int direction;
    GET_CHAR_LOOP:
        printf("Make a move using w-a-s-d:  ");
        key = getchar();
        while ('\n'!=getchar());
        // putchar(key);
        // printf("\nGotten key %c with code %i\n", key, (int) key);
        switch (key) {
            case 'w':
                direction = UP;
                break;
            case 'a':
                direction = LEFT;
                break;
            case 's':
                direction = DOWN;
                break;
            case 'd':
                direction = RIGHT;
                break;
            default:
                direction = ERROR;
                break;
        }
        if (g_inverted) direction = get_opposite_direction(direction);
    if (direction == ERROR) {
        printf("You donkey, press w, a, s, or d! Try again!\n");
        goto GET_CHAR_LOOP;
    }
    return direction;
}