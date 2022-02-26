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

void move(int *pos1, int *pos2, int *board) {
    int posIndex = pos_to_index(pos1, g_column_size);
    int emptyIndex = pos_to_index(pos2, g_column_size);
    
    swap_ints(&board[posIndex], &board[emptyIndex]);
    pos2[0] = pos1[0];
    pos2[1] = pos1[1];
}

int play_turn_with_board(int direction, int *board, int *empty_field) {
    int affected_field[2];
    affected_field[0] = empty_field[0];
    affected_field[1] = empty_field[1];
    
    switch (direction) {
    case UP:
        affected_field[0]--;
        break;
    case DOWN:
        affected_field[0]++;
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
    
    printf("Affected Field: %i,%i - Empty Field: %i,%i\n", affected_field[0], affected_field[1], empty_field[0], empty_field[1]);
    
    // Check if turn is possible
    if (affected_field[0] < 0 || !affected_field[1] < 0 || affected_field[0] >= g_row_size || affected_field[1] >= g_column_size) return MOVE_OUTSIDE_BORDERS;
    
    // Check if player moved back too often
    // to prevent loops of moving back and forth
    if (g_last_direction == get_opposite_direction(direction)) g_moved_back++;
    else g_moved_back = 0;
    if (g_moved_back >= 2) return REPEATED_MOVE_BACK;
    g_last_direction = direction;
    
    move(affected_field, empty_field, board);
    return SUCCESS;
}

int play_turn(int direction) {
    return play_turn_with_board(direction, g_board, g_empty_field_pos);
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
        printf("\n");
        printf("To let the computer help you with the next move, press e\n");
        printf("To let the computer finish this puzzle, press 'q\n");
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
            case 'e':
                direction = GETHELP;
                break;
            case 'q':
                direction = FINISH;
                break;
            default:
                direction = ERROR;
                break;
        }
        if (g_inverted) direction = get_opposite_direction(direction);
    if (direction == ERROR) {
        printf("You donkey, how hard is it to follow instructions! Try again!\n");
        goto GET_CHAR_LOOP;
    }
    return direction;
}

int* A_star() {
    int i, len = g_column_size * g_row_size;
    int *og_board = malloc(len * sizeof(int));
    for (i = 0; i < len; i++) og_board[i] = g_board[i];
    
    // TODO
}