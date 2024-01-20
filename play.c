#include "general.h"
#include <stdlib.h>
#include <stdio.h>

void move(int *pos1, int *pos2, int column_size, int *board) {
    int posIndex = pos_to_index(pos1, column_size);
    int emptyIndex = pos_to_index(pos2, column_size);

    swap_ints(&board[posIndex], &board[emptyIndex]);
    pos2[0] = pos1[0];
    pos2[1] = pos1[1];
}

int play_turn(int direction, int column_size, int row_size, int *board, int *empty_field) {
    int *affected_field = get_new_pos(empty_field, direction);
    if (affected_field[0] == -1) return ILLEGAL_DIRECTION;

    // printf("Affected Field: %i,%i - Empty Field: %i,%i\n", affected_field[0], affected_field[1], empty_field[0], empty_field[1]);

    // Check if turn is possible
    if (affected_field[0] < 0 || affected_field[1] < 0 || affected_field[0] >= row_size || affected_field[1] >= column_size) return MOVE_OUTSIDE_BORDERS;

    move(affected_field, empty_field, column_size, board);
    return SUCCESS;
}

int invert_direction(int dir)
{
    if (dir < 5) return (~dir) & 3;
    return dir;
}

int get_direction(int inverted) {
    char key;
    int direction;

GET_CHAR_LOOP:
    printf("\n");
    printf("To let the computer help you with the next move, press e\n");
    printf("To let the computer finish this puzzle, press q\n");
    printf("Make a move using w-a-s-d:  ");
    getchar(); // I don't exactly know why this is needed tbh xd
    key = getchar();
    if      (key == 'w' || key == 'W') direction = UP;
    else if (key == 'a' || key == 'A') direction = LEFT;
    else if (key == 's' || key == 'S') direction = DOWN;
    else if (key == 'd' || key == 'D') direction = RIGHT;
    else if (key == 'e' || key == 'E') direction = GETHELP;
    else if (key == 'q' || key == 'Q') direction = FINISH;
    else direction = ERROR;

    if (inverted) direction = invert_direction(direction);
    if (direction == ERROR) {
        printf("You donkey, how hard is it to follow instructions! Try again!\n");
        goto GET_CHAR_LOOP;
    }
    return direction;
}

char* get_direction_string(int direction) {
    switch (direction) {
    case UP:
        return "Up";
    case RIGHT:
        return "Right";
    case DOWN:
        return "Down";
    case LEFT:
        return "Left";
    default:
        return "-";
    }
}