#include "element.h"
#include <stdlib.h>
#include <stdio.h>


element NULL_EL = {.id = 0, .weight = INT_MAX, .prevID = 0, .moves_amount = 0, .direction = 0, .empty_field = NULL, .board = NULL, .next = NULL, .prev = NULL };

element El_getNull() {
    return NULL_EL;
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
    return (El_eq(el1, el2) || El_lt(el1, el2));
}

int El_gte(element *el1, element *el2) {
    return (El_eq(el1, el2) || El_gt(el1, el2));
}

// Stringify

int El_stringifyCapacityVal = 50;

int El_stringifyCapacity() {
    return El_stringifyCapacityVal;
}

// See: http://www.cplusplus.com/reference/cstdio/snprintf/
int El_stringify(element *el, char *s) {
    int formatVal = sprintf(s, "(ID: %i:  %i  Dir: %i, Prev: %i)", el->id, el->weight, el->direction, el->prevID);
    return formatVal;
}