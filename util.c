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

// Clear Console
// system() is very resource-intensive & shouldn't be used,
// but for now it's fine
void clear_screen() {
    if (system("CLS")) system("clear");
}


// Linked List:


listItem* create_item(int weight, void *data, size_t size) {
    listItem *ptr = malloc(sizeof(listItem));
    if (ptr==NULL) {
        printf("Item couldn't be created!");
        return ptr;
    }
    ptr->weight = weight;
    ptr->data = data;
    ptr->size = size;
    ptr->prev = NULL;
    ptr->next = NULL;
}

listItem* insert(listItem *last_item, listItem *new_el) {
    if (last_item->next) {
        last_item->next->prev = new_el;
        new_el->next = last_item->next;
    }
    last_item->next = new_el;
    new_el->prev = last_item;
    return new_el;
}

listItem* remove(listItem *el) {
    if (el->prev) el->prev->next = el->next;
    if (el->next) el->next->prev = el->prev;
    free(el);
}

listItem* sort(listItem *root) {
    // TODO
}

listItem* insert_sorted(listItem *root, listItem *new_el) {
    while (root->next != NULL) {
        if (new_el->weight <= root->weight) return insert(root->prev, new_el);
        else root = root->next;
    }
    if (new_el->weight <= root->weight) return insert(root->prev, new_el);
    else return insert(root, new_el);
}

void print_list_item(listItem *item) {
    printf("(Weight: %i, DataSize: %i)\n", item->weight, item->size);
}

void print_list(listItem *root) {
    while (root->next != NULL) {
        print_list_item(root);
        root = root->next;
    }
    print_list_item(root);
}