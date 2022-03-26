#include "general.h"
#include "element.h"
#include <stdlib.h>
#include <stdio.h>


element NULL_EL = {.id = 0, .weight = INT_MAX, .prevID = 0, .moves_amount = 0, .direction = 0, .empty_field = NULL, .board = NULL, .next = {NULL, NULL, NULL, NULL}, .prev = NULL };

element El_getNull() {
    return NULL_EL;
}

element *El_new(int weight, int moves_amount, int direction, int *empty_field, int *board, element *prev) {
    element *el = malloc(sizeof(element));
    el->weight = weight;
    el->moves_amount = moves_amount;
    el->direction = direction;
    el->empty_field = empty_field;
    el->board = board;
    el->prev = prev;
    el->prevID = prev == NULL ? NULL_EL.id : prev->id;
    el->id = next_id();
    
    return el;
}

void El_free(element *el) {
    // BUG: Freeing element leads to program exiting silently
    printf("Gonna free element now\n");
    free(el->empty_field);
    printf("Freed empty fieldd\n");
    free(el->board);
    printf("Freed board\n");
    free(el);
    printf("Freed element\n");
}

int El_isInvalid(element *el) {
    return El_same(el, &NULL_EL);
}

int El_same(element *el1, element *el2) {
    return (el1->id == el2->id);
}

int El_eq(element *el1, element *el2) {
    return (el1->weight == el2->weight);
}

int El_lt(element *el1, element *el2) {
    return (el1->weight < el2->weight);
}

int El_gt(element *el1, element *el2) {
    return (el1->weight > el2->weight);
}

int El_lte(element *el1, element *el2){
    return (el1->weight <= el2->weight);
}

int El_gte(element *el1, element *el2) {
    return (el1->weight >= el2->weight);
}

int El_stringify(element *el, char *s) {
    int formatVal = sprintf(s, "(ID: %i:  %i  Dir: %i, Prev: %i)", el->id, el->weight, el->direction, el->prevID);
    return formatVal;
}