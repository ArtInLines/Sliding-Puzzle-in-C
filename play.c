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

void move(int *pos1, int *pos2, int column_size, int *board) {
    int posIndex = pos_to_index(pos1, column_size);
    int emptyIndex = pos_to_index(pos2, column_size);
    
    swap_ints(&board[posIndex], &board[emptyIndex]);
    pos2[0] = pos1[0];
    pos2[1] = pos1[1];
}

int play_turn_with_everything(int direction, int column_size, int row_size, int last_direction, int moved_back_counter, int *board, int *empty_field) {
    int *affected_field = get_new_pos(empty_field, direction);
    if (affected_field[0] == -1) return ILLEGAL_DIRECTION;
    
    // printf("Affected Field: %i,%i - Empty Field: %i,%i\n", affected_field[0], affected_field[1], empty_field[0], empty_field[1]);
    
    // Check if turn is possible
    if (affected_field[0] < 0 || affected_field[1] < 0 || affected_field[0] >= row_size || affected_field[1] >= column_size) return MOVE_OUTSIDE_BORDERS;
    
    // Check if player moved back too often
    // to prevent loops of moving back and forth
    if (last_direction == get_opposite_direction(direction)) moved_back_counter++;
    else moved_back_counter = 0;
    if (moved_back_counter >= 2) return REPEATED_MOVE_BACK;
    last_direction = direction;
    
    move(affected_field, empty_field, column_size, board);
    return SUCCESS;
}

int play_turn_with_board(int direction, int *board, int *empty_field) {
    return play_turn_with_everything(direction, g_column_size, g_row_size, g_last_direction, g_moved_back, board, empty_field);
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
        printf("To let the computer finish this puzzle, press q\n");
        printf("Make a move using w-a-s-d:  ");
        key = getchar();
        while ('\n'!=getchar());
        // putchar(key);
        // printf("\nGotten key %c with code %i\n", key, (int) key);
        if (key == 'w' || key == 'W') direction = UP;
        else if (key == 'a' || key == 'A') direction = LEFT;
        else if (key == 's' || key == 'S') direction = DOWN;
        else if (key == 'd' || key == 'D') direction = RIGHT;
        else if (key == 'e' || key == 'E') direction = GETHELP;
        else if (key == 'q' || key == 'Q') direction = FINISH;
        else direction = ERROR;
        
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
    data[i] = prev_node; i++;
    data[i] = moves_amount; i++;
    data[i] = dir; i++;
    data[i] = empty_field[0]; i++;
    data[i] = empty_field[1]; i++;
    for (int diff = i, i = 0; i < len; i++) data[i+diff] = board[i];
    
    // Debugging
    // printf("Creating Data:\n");
    // for (i = 0; i < len+5; i++) printf("%i. -> %i\n", i, data[i]);
    
    return data;
}

listItem* find_by_board(listItem *root, int *board, int len) {
    listItem *tmp = root;
    if (compare_board(board, &(tmp->data)[5], len)) return tmp;
    while (tmp->next!=NULL) {
        tmp = tmp->next;
        if (compare_board(board, &(tmp->data)[5], len)) return tmp;
    }
    return NULL;
}

int* A_star() {
    int i, prev_node, dir, moves_amount, turn_return, board_id, len = g_column_size * g_row_size;
    int *data, *current_board, *next_board, current_empty_field[2], *next_empty_field, directions[4] = {UP, RIGHT, DOWN, LEFT};
    listItem *current_node;
    
    data = create_data(len, 0, 0, -1, g_empty_field_pos, g_board);
    int data_size = get_data_size(len);
    
    listItem *node;
    listItem *root = create_item(0, get_priority(0, g_column_size, g_row_size, g_board), data, data_size);
    listItem *used_stack = create_item(0, 0, data, data_size);
    
    // printf("A* before while loop\n");
    
    while (1) {
        i = 0;
        data = root->data;
        prev_node = data[i]; i++;
        moves_amount = data[i]; i++;
        dir = data[i]; i++;
        current_empty_field[0] = data[i]; i++;
        current_empty_field[1] = data[i]; i++;
        current_board = &(data[i]);
        
        // Add new nodes for each direction (skipping illegal directions)
        for (i = 0; i < 4; i++) {
            next_board = copy_board(len, current_board);
            next_empty_field = copy_board(2, current_empty_field);
            turn_return = play_turn_with_board(directions[i], next_board, next_empty_field);
            // printf("turn_return = %i\n", turn_return);
            if (turn_return != SUCCESS) continue;
            // printf("\nMove %i: \n", directions[i]);
            // show_board(g_column_size, g_row_size, next_board);
            
            // printf("\nDirection: %i\n", directions[i]);
            // printf("next_board[4]: %i\n", next_board[4]);
            
            data = create_data(len, root->id, moves_amount+1, directions[i], next_empty_field, next_board);
            // board_id = create_board_id(next_board, len);
            node = create_item(0, get_priority(moves_amount+1, g_column_size, g_row_size, next_board), data, data_size);
            // print_list_item(node);
            // print_list_item_data(node);
            
            if (find_by_board(used_stack, next_board, len) != NULL) continue;
            
            // printf("New Node: "); print_list_item(node);
            // printf("Root: "); print_list_item(root);
            
            insert_sorted(root, node, 1);
            
            // printf("Complete List:\n");
            // print_list(root);
        }
        // print_list(root);
        // print_list(used_stack);
        
        insert(used_stack, copy_item(root, 1));
        root = shift(root);
        
        // printf("\n\n\n");
        // show_board(g_column_size, g_row_size, &(root->data)[5]);
        // printf("Current root: "); print_list_item(root);
        // printf("Stack:\n"); print_list(root);
        // printf("\nUsed Stack:\n"); print_list(used_stack);
        
        printf("Looked at %i possible moves\n", get_list_len(used_stack));
        
        if (is_solved(len, &(root->data)[5])) break;
    }
    
    // printf("A* before path creation: moves_amount=%i\n", moves_amount+2);
    
    int *path = malloc((moves_amount+2) * sizeof(int));
    path[moves_amount+1] = -1;
    do {
        moves_amount = (root->data)[1];
        path[moves_amount-1] = (root->data)[2];
        prev_node = (root->data)[0];
        root = find_by_id(prev_node, used_stack);
        // printf("i: %i -> path[i]: %i | moves_amount: %i\n", moves_amount-1, path[moves_amount-1], moves_amount);
    } while((root->data)[1]);
    
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