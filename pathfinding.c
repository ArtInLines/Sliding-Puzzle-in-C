#include <stdlib.h>
#include <stdio.h>

#include "element.h"
#include "binaryHeap.h"
#include "general.h"

#define DEFAULT_NEXT {NULL, NULL, NULL, NULL}


int *A_star(int column_size, int row_size, int bias, int *empty_field, int *board, int print_progress) {
    int i, index, turn_return, len = column_size * row_size;
    int *path, *next_board, *next_empty_field, directions[4] = {UP, RIGHT, DOWN, LEFT};
    element new_node, node; 
    binaryHeap *heap =binHeap_new(0), *used_heap = binHeap_new(0);
    
    // Initial root node
    node = (element) { .id = get_id(), .weight = get_priority(0, bias, column_size, row_size, board), .direction = -1, .prevID = El_getNull().id, .moves_amount = 0, .empty_field = empty_field, .board = board, .prev = NULL, .next = DEFAULT_NEXT };
    binHeap_insert(&heap, node);
    
    while (!is_solved(len, node.board)) {
        for (i = 0; i < 4; i++) {
            printf("Test 1\n");
            if (node.direction == get_opposite_direction(directions[i])) continue;
            
            next_board = copy_board(len, node.board);
            next_empty_field = copy_board(2, node.empty_field);
            turn_return = play_turn(directions[i], column_size, row_size, next_board, next_empty_field, 0);
            
            if (turn_return != SUCCESS) continue;
            
            new_node = (element) { .id = get_id(), .prevID = node.id, .prev = &node, .next = DEFAULT_NEXT, .board = next_board, .empty_field = next_empty_field, .weight = get_priority(node.moves_amount + 1, bias, column_size, row_size, next_board), .direction = directions[i], .moves_amount = node.moves_amount + 1, };
            
            printf("Test 2\n");
            node.next[i] = &new_node;
            binHeap_insert(&heap, new_node);
        }
        
        printf("Test\n");
        node = binHeap_poll(&heap);
        printf("Node ID: %i\n", node.id);
        index = binHeap_insert(&used_heap, node);
        node = binHeap_get(used_heap, index);
        for (i = 0; i < 4; i++) {
            // Update the node's children's pointers, to quickly trace the way back again at the end
            if (node.next[i] != NULL) node.next[i]->prev = &node;
        }
        node = binHeap_peek(heap);
    }
    
    path = malloc((node.moves_amount + 2) * sizeof(int));
    path[node.moves_amount + 1] = -1;
    for (i = node.moves_amount; i >= 0; i--) {
        path[i] = node.direction;
        node = *node.prev;
    }
    
    return path;
}