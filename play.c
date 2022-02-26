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
    int *affected_field = get_new_pos(empty_field, direction);
    if (affected_field[0] == -1) return ILLEGAL_DIRECTION;
    
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

int* copy_board(int len, int *board) {
    int *new_board = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) new_board[i] = board[i];
    return new_board;
}

// Exclusively A*

// The Data Fields of each item are structured in the following way:
    // 0: previous node
    // 1: moves amount
    // 2: direction
    // 3-4: empty field position
    // 5+: board
    
int get_data_size(int len){
    return (int) sizeof(int) * (len + 5);
}

int* create_data(int len, int prev_node, int moves_amount, int dir, int *empty_field, int *board) {
    int *data = malloc(get_data_size(len));
    int i = 0;
    data[i++] = prev_node;
    data[i++] = moves_amount;
    data[i++] = dir;
    data[i++] = empty_field[0];
    data[i++] = empty_field[1];
    for (int diff = i; i < len; i++) data[i] = g_board[i-diff];
}

int* A_star() {
    int i, prev_node, dir, moves_amount, turn_return, len = g_column_size * g_row_size;
    int *data, *current_board, *next_board, current_empty_field[2], directions[4] = {UP, RIGHT, DOWN, LEFT};
    listItem *current_node;
    
    data = create_data(len, 0, 0, -1, g_empty_field_pos, g_board);
    int data_size = get_data_size(len);
    
    listItem *node;
    listItem *root = create_item(get_priority(0, g_column_size, g_row_size, g_board), data, data_size);
    listItem *used_stack = create_item(0, data, data_size);
    
    while (1) {
        i = 0;
        data = root->data;
        prev_node = data[i++];
        moves_amount = data[i++];
        dir = data[i++];
        current_empty_field[0] = data[i++];
        current_empty_field[1] = data[i++];
        current_board = &data[i++];
        
        // Add new nodes for each direction (skipping illegal directions)
        for (i = 0; i < 4; i++) {
            next_board = copy_board(len, current_board);
            turn_return = play_turn_with_board(directions[i], next_board, current_empty_field);
            if (turn_return != SUCCESS) continue;
            data = create_data(len, root->id, moves_amount+1, directions[i], get_new_pos(current_empty_field, directions[i]), next_board);
            node = create_item(get_priority(moves_amount+1, g_column_size, g_row_size, next_board), data, data_size);
            insert_sorted(root, node, 1);
        }
        
        insert(used_stack, copy_item(root, 1));
        root = shift(root);
        
        if (root->weight == 0) break;
    }
    
    i = 0;
    int *path = malloc((moves_amount+2) * sizeof(int));
    do {
        path[i++] = (root->data)[2];
        prev_node = (root->data)[0];
    } while(prev_node);
    path[i] = 0;
    
    return path;
}

int* A_star_mem_efficient() {
    // A* but without storing a new board_copy for each node
    // instead only the direction for that node is stored
    // (as well as the parent node)
    // with the actual board being created on the fly from the og board.
}

int get_next_move() {
    int *path = A_star();
    return path[0];
}