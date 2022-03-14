#include <stdlib.h>
#include <stdio.h>

#include "element.h"
#include "binaryHeap.h"
#include "general.h"

#define DEFAULT_NEXT {NULL, NULL, NULL, NULL}


int *A_star(int column_size, int row_size, int bias, int *empty_field, int *board, int print_progress) {
    int i, index, turn_return, len = column_size * row_size;
    int *path, *next_board, *next_empty_field, directions[4] = {UP, RIGHT, DOWN, LEFT};
    element next_node, current_node; 
    binaryHeap *heap =binHeap_new(1), *used_heap = binHeap_new(1);
    
    char s[1000];
    
    // Initial root node
    current_node = (element) { .id = get_id(), .weight = get_priority(0, bias, column_size, row_size, board), .direction = -1, .prevID = El_getNull().id, .moves_amount = 0, .empty_field = empty_field, .board = board, .prev = NULL, .next = DEFAULT_NEXT };
    
    while (!is_solved(len, current_node.board)) {
        printf("Not solved yet\n");
        for (i = 0; i < 4; i++) {
            if (current_node.direction == get_opposite_direction(directions[i])) continue;
            
            next_board = copy_board(len, current_node.board);
            next_empty_field = copy_board(2, current_node.empty_field);
            turn_return = play_turn(directions[i], column_size, row_size, next_board, next_empty_field, 0);
            
            if (turn_return != SUCCESS) continue;
            
            printf("direction %i is valid\n", directions[i]);
            
            next_node = (element) { .id = get_id(), .prevID = current_node.id, .prev = &current_node, .next = DEFAULT_NEXT, .board = next_board, .empty_field = next_empty_field, .weight = get_priority(current_node.moves_amount + 1, bias, column_size, row_size, next_board), .direction = directions[i], .moves_amount = current_node.moves_amount + 1, };
            
            current_node.next[i] = &next_node;
            binHeap_insert(&heap, next_node);
        }
        
        printf("For loop over\n");
        index = binHeap_insert(&used_heap, current_node);
        for (i = 0; i < 4; i++) {
            // Update the current_node's children's pointers, to quickly trace the way back again at the end
            if (current_node.next[i] != NULL) current_node.next[i]->prev = &current_node;
        }
        current_node = binHeap_poll(&heap);
        
        El_stringify(&current_node, s);
        printf("Current Node: %s\n", s);
        
        binHeap_stringify(heap, s);
        printf("Heap:\n%s\n", s);
        
        binHeap_stringify(used_heap, s);
        printf("Used Heap:\n%s\n", s);
    }
    
    path = malloc((current_node.moves_amount + 2) * sizeof(int));
    path[current_node.moves_amount + 1] = -1;
    for (i = current_node.moves_amount; i >= 0; i--) {
        path[i] = current_node.direction;
        current_node = *current_node.prev;
    }
    
    return path;
}