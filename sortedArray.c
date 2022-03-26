#include <stdlib.h>
#include <stdio.h>
#include "element.h"
#include "sortedArray.h"

// For typedefs see header file

// Private functions not exposed via header file:
sortedArr *sortArr_hiddenInsert(sortedArr **A, int index, element el);


sortedArr *sortArr_new() {
    sortedArr *arr = malloc(sizeof(sortedArr));
    arr->len = 0;
    arr->capacity = 1;
    arr->list = malloc((arr->capacity) * sizeof(element));
    return arr;
}

void sortArr_free(sortedArr *arr) {
    // BUG: Program exits silently when freeing this memory
    for (int i = 0; i < arr->len; i++) El_free(&(arr->list[i]));
    free(arr->list);
    free(arr);
}

element sortArr_peek(sortedArr *arr) {
    return sortArr_get(arr, 0);
}

element sortArr_get(sortedArr *arr, int index) {
    return arr->list[index];
}

element *sortArr_getRef(sortedArr *arr, int index) {
    return &arr->list[index];
}

int sortArr_length(sortedArr *arr) {
    return arr->len;
}

sortedArr *sortArr_removeAt(sortedArr **A, int index) {
    sortedArr *arr = *A;
    if (arr->capacity >= 2 * (arr->len - 1)) {
        sortedArr *tmp = arr;
        *A = sortArr_new(arr->len - 1);
        arr = *A;
        arr->len = tmp->len - 1;

        int i;
        for (i = 0; i < index; i++) arr->list[i] = tmp->list[i];
        for (i = index; i < tmp->len; i++) arr->list[i] = tmp->list[i+1];

        sortArr_free(tmp);
        printf("Removed element at pos %i\n", index);
    } else {
        arr->len--;
        for (int i = index; i < arr->len; i++) arr->list[i] = arr->list[i+1];
    }
    return arr;
}

int sortArr_binSearchApprox(sortedArr *arr, element *el) {
    int halfIndex, leftPadding = 0, rightPadding = 0;
    element *currentEl;
    
    do {
        halfIndex = (int) leftPadding + ((arr->len - rightPadding - leftPadding) / 2);
        currentEl = &arr->list[halfIndex];
        
        if (El_same(el, currentEl)) return halfIndex;
        else if (El_lt(el, currentEl)) rightPadding = arr->len - halfIndex;
        else if (El_gt(el, currentEl)) leftPadding = halfIndex;
        else {
            while (El_eq(el, currentEl)) {
                halfIndex++;
                currentEl = &arr->list[halfIndex];
                if (El_same(el, currentEl)) return halfIndex;
            }
            while (El_eq(el, currentEl)) {
                halfIndex--;
                currentEl = &arr->list[halfIndex];
                if (El_same(el, currentEl)) return halfIndex;
            }
            return -1;
        }
    } while (arr->len - (leftPadding + rightPadding) > 1);
    
    if (halfIndex == arr->len - 1 && El_gte(el, currentEl)) return arr->len;
    else return halfIndex;
}

int sortArr_binSearch(sortedArr *arr, element *el) {
    int index = sortArr_binSearchApprox(arr, el);
    if (!El_same(el, sortArr_getRef(arr, index))) return -1;
    else return index;
}

sortedArr *sortArr_insert(sortedArr **A, element el) {
    int index = sortArr_binSearchApprox(*A, &el);
    printf("Index to insert element at: %i\n", index);
    printf("Length: %i, Capacity: %i\n", (*A)->len, (*A)->capacity);
    return sortArr_hiddenInsert(A, index, el);
}

sortedArr *sortArr_hiddenInsert(sortedArr **A, int index, element el) {
    // printf("Inserting %i at position %i  -  Length: %i, Capacity: %i\n", el.id, index, (*A)->len, (*A)->capacity);
    
    sortedArr *arr = *A;
    if (arr->capacity > arr->len) {
        arr->len++;
        for (int i = arr->len-1; i > index; i--) arr->list[i] = arr->list[i-1];
        arr->list[index] = el;
        return arr;
    } else {
        sortedArr *tmp = arr;
        *A = sortArr_new(arr->capacity * 2);
        arr = *A;
        arr->len = tmp->len+1;
        
        int i;
        for (i = tmp->len; i > index; i--) arr->list[i] = tmp->list[i-1];
        arr->list[index] = el;
        for (i = index-1; i >= 0; i--) arr->list[i] = tmp->list[i];
        
        sortArr_free(tmp);
        return arr;
    }
}

element sortArr_pop(sortedArr **arr) {
    element el = (*arr)->list[(*arr)->len-1];
    sortArr_removeAt(arr, (*arr)->len - 1);
    return el;
}

int sortArr_contains(sortedArr *arr, element el) {
    for (int i = 0; i < arr->len; i++) {
        if (El_eq(&arr->list[i], &el)) return 1;
    }
    return 0;
}

int sortArr_indexOf(sortedArr *arr, element el) {
    for (int i = 0; i < arr->len; i++) {
        if (El_eq(&arr->list[i], &el)) return i;
    }
    return -1;
}

int sortArr_stringify(sortedArr *arr, char *s) {
    int i, formatVal;
    char elS[10];
    
    El_stringify(sortArr_getRef(arr, 0), s);
    
    for (i = 1; i < arr->len; i++) {
        El_stringify(sortArr_getRef(arr, i), elS);
        formatVal = sprintf(s, "%s, %s", s, elS);
    }
    
    return formatVal;
}