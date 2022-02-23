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