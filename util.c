#include "util.h"
#include <stdlib.h>

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