#include "util.h"

int distance(int x, int y) {
    if (x >= y) return x - y;
    else return y - x;
}