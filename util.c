#include "general.h"
#include <stdlib.h>
#include <stdio.h>

int distance(int x, int y) {
    if (x >= y) return x - y;
    else return y - x;
}

int swap_ints(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int* index_to_pos(int index, int column_size) {
    int *pos = malloc(2 * sizeof(int));
    pos[0] = (int) index / column_size;
    pos[1] = index % column_size;
    return pos;
}

int pos_to_index(int *pos, int column_size) {
    return pos[0]*column_size + pos[1];
}

// Clear Console
// system() is very resource-intensive & shouldn't be used,
// but for now it's fine
void clear_screen() {
    if (system("CLS")) system("clear");
}

int rand_int() {
    // initiate x to be an arbitrary, high prime number
    static int x = 77002813;
    x ^= (x << 13);
    x ^= (x << 17);
    x ^= (x << 5);
    return x;
}

int next_id() {
    static int x = 0;
    x++;
    return x;
}

int get_id() {
    // return rand_id();
    return next_id();
}