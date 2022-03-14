#include <stdlib.h>
#include <stdio.h>
#include "element.h"
#include "dynamicArray.h"

// For typedefs see header file

dynamicArr *dynArr_new(int length) {
    dynamicArr *arr = malloc(sizeof(dynamicArr));
    arr->len = 0;
    arr->capacity = length <= 0 ? 1 : length;
    arr->list = malloc((arr->capacity) * sizeof(element));
    return arr;
}

element dynArr_peek(dynamicArr *arr) {
    return dynArr_get(arr, 0);
}

element dynArr_get(dynamicArr *arr, int index) {
    return arr->list[index];
}

element *dynArr_getRef(dynamicArr *arr, int index) {
    return &arr->list[index];
}

void dynArr_set(dynamicArr *arr, int index, element newValue) {
    arr->list[index] = newValue;
}

int dynArr_length(dynamicArr *arr) {
    return arr->len;
}

dynamicArr *dynArr_removeAt(dynamicArr **A, int index) {
    dynamicArr *arr = *A;
    if (arr->capacity >= 2 * (arr->len - 1)) {
        dynamicArr *tmp = arr;
        *A = dynArr_new(arr->len - 1);
        arr = *A;
        arr->len = tmp->len - 1;

        int i;
        for (i = 0; i < index; i++) arr->list[i] = tmp->list[i];
        for (i = index; i < tmp->len; i++) arr->list[i] = tmp->list[i+1];

        free(tmp->list);
        free(tmp);
        return arr;
    } else {
        arr->len--;
        for (int i = index; i < arr->len; i++) arr->list[i] = arr->list[i+1];
        return arr;
    }
}

dynamicArr *dynArr_insert(dynamicArr **A, int index, element el) {
    dynamicArr *arr = *A;
    printf("Inserting %i at position %i\nLength: %i, Capacity: %i\n", el.id, index, (*A)->len, (*A)->capacity);
    if (arr->capacity > arr->len) {
        arr->len++;
        for (int i = arr->len-1; i > index; i--) arr->list[i] = arr->list[i-1];
        arr->list[index] = el;
        return arr;
    } else {
        dynamicArr *tmp = arr;
        *A = dynArr_new(arr->capacity * 2);
        arr = *A;
        arr->len = tmp->len+1;
        
        int i;
        for (i = tmp->len; i > index; i--) arr->list[i] = tmp->list[i-1];
        arr->list[index] = el;
        for (i = index-1; i >= 0; i--) arr->list[i] = tmp->list[i];
        
        free(tmp->list);
        free(tmp);
        return arr;
    }
}

dynamicArr *dynArr_push(dynamicArr **arr, element el) {
    return dynArr_insert(arr, (*arr)->len, el);
}

element dynArr_pop(dynamicArr **arr) {
    element el = (*arr)->list[(*arr)->len-1];
    dynArr_removeAt(arr, (*arr)->len - 1);
    return el;
}

int dynArr_contains(dynamicArr *arr, element el) {
    for (int i = 0; i < arr->len; i++) {
        if (El_eq(&arr->list[i], &el)) return 1;
    }
    return 0;
}

int dynArr_indexOf(dynamicArr *arr, element el) {
    for (int i = 0; i < arr->len; i++) {
        if (El_eq(&arr->list[i], &el)) return i;
    }
    return -1;
}

int dynArr_findIndexOf(dynamicArr *arr, int (*fn) (element, int, dynamicArr*)) {
    for (int i = 0; i < arr->len; i++) {
        if (fn(arr->list[i], i, arr)) return i;
    }
    return -1;
}

element *dynArr_find(dynamicArr *arr, int (*fn) (element, int, dynamicArr*)) {
    for (int i = 0; i < arr->len; i++) {
        if (fn(arr->list[i], i, arr)) return &arr->list[i];
    }
    return NULL;
}

dynamicArr *dynArr_map(dynamicArr **arr, element (*fn) (element, int, dynamicArr*)) {
    for (int i = 0; i < (*arr)->len; i++) {
        (*arr)->list[i] = fn((*arr)->list[i], i, *arr);
    }
    return *arr;
}

dynamicArr *dynArr_immutableMap(dynamicArr **arr, element (*fn) (element, int, dynamicArr*)) {
    dynamicArr *newArr = dynArr_new((*arr)->len);
    for (int i = 0; i < (*arr)->len; i++) {
        newArr->list[i] = fn((*arr)->list[i], i, *arr);
    }
    return newArr;
}

dynamicArr *dynArr_filter(dynamicArr **arr, int (*fn) (element, int, dynamicArr*)) {
    // TODO
}

element dynArr_reduce(dynamicArr *arr, element (*fn) (element, element, int, dynamicArr*)) {
    // TODO
}

void *dynArr_fullReduce(dynamicArr *arr, void* (*firstEl) (element, int, dynamicArr*), void* (*fn) (element, element, int, dynamicArr*)) {
    // TODO
}

// Stringify

int dynArr_stringifyCapacity(dynamicArr *arr) {
    return (El_stringifyCapacity() + 2) * arr->len + 1;
}

int dynArr_stringify(dynamicArr *arr, char *s) {
    int i, formatVal, capacity = dynArr_stringifyCapacity(arr);
    char elS[10];
    
    El_stringify(dynArr_getRef(arr, 0), s);
    
    for (i = 1; i < arr->len; i++) {
        El_stringify(dynArr_getRef(arr, i), elS);
        formatVal = sprintf(s, "%s, %s", s, elS);
    }
    
    return formatVal;
}