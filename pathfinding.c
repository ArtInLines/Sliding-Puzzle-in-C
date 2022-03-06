#include "util.h"
#include <stdlib.h>
#include <stdio.h>

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

int* A_star(int column_size, int row_size, int bias, int *empty_field, int *board) {
    int i, prev_node, dir, moves_amount, turn_return, board_id, len = column_size * row_size;
    int *data, *current_board, *next_board, current_empty_field[2], *next_empty_field, directions[4] = {UP, RIGHT, DOWN, LEFT};
    listItem *current_node;
    
    data = create_data(len, 0, 0, -1, empty_field, board);
    int data_size = get_data_size(len);
    
    listItem *node;
    listItem *root = create_item(0, get_priority(0, bias, column_size, row_size, board), data, data_size);
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
            if ((root->data)[2] == get_opposite_direction(directions[i])) continue;
            
            next_board = copy_board(len, current_board);
            next_empty_field = copy_board(2, current_empty_field);
            turn_return = play_turn(directions[i], column_size, row_size, next_board, next_empty_field, 0);

            if (turn_return != SUCCESS) continue;
            
            // printf("\nMove %i: \n", directions[i]);
            // show_board(column_size, row_size, next_board);
            
            // printf("\nDirection: %i\n", directions[i]);
            // printf("next_board[4]: %i\n", next_board[4]);
            
            data = create_data(len, root->id, moves_amount+1, directions[i], next_empty_field, next_board);
            // board_id = create_board_id(next_board, len);
            node = create_item(0, get_priority(moves_amount+1, bias, column_size, row_size, next_board), data, data_size);
            // print_list_item(node);
            // print_list_item_data(node);
            
            if (find_by_board(used_stack, next_board, len) != NULL) continue;
            
            // printf("New Node: "); print_list_item(node);
            // printf("Root: "); print_list_item(root);
            
            insert_sorted(root, node, 1);
            
            // printf("Complete List:\n");
            // print_list(root);
        }
        // printf("Stack:\n");
        // print_list(root);
        // printf("Used Stack:\n");
        // print_list(used_stack);
        
        insert(used_stack, copy_item(root, 1));
        root = shift(root);
        
        printf("\rStack Height: %i   -   Used Stack Height: %i   -   Best Priority: %i   -   Root ID: %i   -   Turn Counter: %i  ", get_list_len(root), get_list_len(used_stack), root->weight, root->id, (root->data)[1]);
        
        // printf("\n\n\n");
        // show_board(column_size, row_size, &(root->data)[5]);
        // printf("Current root: "); print_list_item(root);
        // printf("Stack:\n"); print_list(root);
        // printf("\nUsed Stack:\n"); print_list(used_stack);
        
        // printf("Looked at %i possible moves\n", get_list_len(used_stack));
        
        if (is_solved(len, &(root->data)[5])) break;
    }
    
    // show_board(column_size, row_size, &(root->data)[5]);
    
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

int* A_star_mem_efficient(int column_size, int row_size, int *empty_field, int *board) {
    // A* but without storing a new board_copy for each node
    // instead only the direction for that node is stored
    // (as well as the parent node)
    // with the actual board being created on the fly from the og board.
}