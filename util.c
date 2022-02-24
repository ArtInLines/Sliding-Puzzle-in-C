#include "util.h"

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
    int pos[2];
    pos[0] = (int) index / column_size;
    pos[1] = index % column_size;
    return pos;
}