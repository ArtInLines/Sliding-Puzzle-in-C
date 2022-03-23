#include <stdlib.h>
#include <stdio.h>

#include "general.h"
#include "element.h"
#include "dynamicArray.h"
#include "binaryHeap.h"

#define DEFAULT_NEXT {NULL, NULL, NULL, NULL}


int *A_star(int column_size, int row_size, int bias, int *empty_field, int *board, int print_progress) {
    int i, index, turn_return, opposite_direction, len = column_size * row_size;
    int *path, directions[4] = {UP, RIGHT, DOWN, LEFT};
    element next_node, current_node; 
    binaryHeap *heap = binHeap_new(1);
    dynamicArr *used_nodes = dynArr_new(1);
    
    char s[1000];
    
    // Initial root node
    current_node = (element) { .id = get_id(), .weight = get_priority(0, bias, column_size, row_size, board), .direction = -1, .prevID = El_getNull().id, .moves_amount = 0, .empty_field = empty_field, .board = board, .prev = NULL, .next = DEFAULT_NEXT };
    next_node = (element) { .next = DEFAULT_NEXT };
    
    while (!is_solved(len, current_node.board)) {
        
        printf("Insert next nodes...\n");
        opposite_direction = get_opposite_direction(current_node.direction);
        next_node.prevID = current_node.id;
        next_node.prev = &current_node;
        next_node.moves_amount = current_node.moves_amount + 1;
        for (i = 0; i < 4; i++) {
            if (opposite_direction == directions[i]) continue;
            
            next_node.board = copy_board(len, current_node.board);
            next_node.empty_field = copy_board(2, current_node.empty_field);
            turn_return = play_turn(directions[i], column_size, row_size, next_node.board, next_node.empty_field, 0);
            
            if (turn_return != SUCCESS) continue;
            
            printf("direction %i is valid\n", directions[i]);
            
            next_node.id = get_id();
            next_node.direction = directions[i];
            next_node.weight = get_priority(next_node.moves_amount, bias, column_size, row_size, next_node.board);
            
            El_stringify(&next_node, s);
            printf("Next node for direction %i: %s\n", directions[i], s);
                        
            current_node.next[i] = &next_node;
            binHeap_insert(&heap, next_node);
            printf("direction %i inserted into heap\n", directions[i]);
        }
        
        printf("Insert current node into used_nodes tree\n");
        dynArr_push(&used_nodes, current_node);
        // Update the current_node's children's pointers, to quickly trace the way back again at the end
        for (i = 0; i < 4; i++) if (current_node.next[i] != NULL) current_node.next[i]->prev = &current_node;
        
        printf("Polling top node from heap to be the new current node\n");
        current_node = binHeap_poll(&heap);
        
        // For debugging
        El_stringify(&current_node, s);
        printf("Current Node: %s\n", s);
        
        binHeap_stringify(heap, s);
        printf("Heap:\n%s\n", s);
        
        dynArr_stringify(used_nodes, s);
        printf("Used Heap:\n%s\n", s);
    }
    
    path = malloc((current_node.moves_amount + 2) * sizeof(int));
    path[current_node.moves_amount + 1] = -1;
    for (i = current_node.moves_amount; i >= 0; i--) {
        path[i] = current_node.direction;
        current_node = *current_node.prev;
    }
    
    binHeap_free(heap);
    dynArr_free(used_nodes);
    
    return path;
}