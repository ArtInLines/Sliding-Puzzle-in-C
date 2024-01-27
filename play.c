#include "general.h"
#include <stdlib.h>
#include <stdio.h>

void move(Pos to, Pos from, Board board) {
    int to_idx   = pos_to_index(to,   board.cols);
    int from_idx = pos_to_index(from, board.cols);

    u8 tmp = board.fields[to_idx];
    board.fields[to_idx] = board.fields[from_idx];
    board.fields[from_idx] = tmp;
}

int play_turn(Dir direction, Board board, Pos *empty_field) {
    Pos affected_field = next_pos(*empty_field, direction);
    if (affected_field.x == UINT8_MAX) return ILLEGAL_DIRECTION;

    // printf("Affected Field: %i,%i - Empty Field: %i,%i\n", affected_field.x, affected_field.y, empty_field[0], empty_field[1]);

    // Check if turn is possible
    if (!is_legal_pos(affected_field, board.rows, board.cols)) return MOVE_OUTSIDE_BORDERS;

    move(affected_field, *empty_field, board);
    *empty_field = affected_field;
    return SUCCESS;
}

Dir invert_direction(Dir dir)
{
    if (dir < 5) return (~dir) & 3;
    return dir;
}

Dir get_direction(bool inverted) {
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

char* get_direction_string(Dir d) {
    switch (d) {
        case UP:    return "Up";
        case RIGHT: return "Right";
        case DOWN:  return "Down";
        case LEFT:  return "Left";
        default:    return "-";
    }
}