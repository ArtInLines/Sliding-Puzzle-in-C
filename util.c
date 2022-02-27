#include "util.h"
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

int rand_id() {
    // initiate x to be an arbitrary, high prime number
    static int x = 77002813;
    x ^= (x << 13);
    x ^= (x << 17);
    x ^= (x << 5);
    return x;
}


// Linked List:


listItem* create_item(int id, int weight, void *data, int size) {
    listItem *ptr = malloc(sizeof(listItem));
    if (ptr==NULL) {
        printf("Item couldn't be created!");
        return ptr;
    }
    ptr->id = id==0 ? rand_id() : id;
    ptr->weight = weight;
    ptr->data = data;
    ptr->size = size;
    ptr->prev = NULL;
    ptr->next = NULL;
}

listItem* copy_item(listItem *item, int keep_id) {
    listItem *ptr = malloc(sizeof(listItem));
    if (ptr == NULL) {
        printf("Item couldn't be copied!");
        return ptr;
    }
    ptr->id = keep_id ? item->id : rand_id();
    ptr->weight = item->weight;
    ptr->size = item->size;
    ptr->data = malloc(item->size);
    for (int i = 0, len = item->size / sizeof(int); i < len; i++) (ptr->data)[i] = (item->data)[i];
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

void remove_item(listItem *el) {
    if (el->prev) el->prev->next = el->next;
    if (el->next) el->next->prev = el->prev;
    free(el);
}

listItem* get_root(listItem *item) {
    listItem *tmp = item;
    while (tmp->prev) tmp = tmp->prev;
    return tmp;
}

listItem* find_by_id(int id, listItem *root) {
    if (root->id == id) return root;
    
    listItem *tmp = root;
    while (tmp->next) {
        tmp = tmp->next;
        if (tmp->id == id) return tmp;
    }
    
    tmp = root;
    while(tmp->prev) {
        tmp = tmp->prev;
        if (tmp->id == id) return tmp;
    }
    
    return NULL;
}

listItem* find_by_data(listItem *root, int (*f) (int*)) {
    listItem *tmp = root;
    if (f(tmp->data)) return tmp;
    while (tmp->next != NULL) {
        tmp = tmp->next;
        if (f(tmp->data)) return tmp;
    }
    return NULL;
}

listItem* find(listItem *root, int (*f) (listItem*)) {
    listItem *tmp = root;
    if (f(tmp)) return tmp;
    while (tmp->next != NULL) {
        tmp = tmp->next;
        if (f(tmp)) return tmp;
    }
    return NULL;
}

listItem* shift(listItem *root) {
    root->next->prev = NULL;
    listItem *new_root = root->next;
    free(root);
    return new_root;
}

listItem* sort(listItem *root) {
    // TODO
}

listItem* insert_sorted(listItem *root, listItem *new_el, int skip_root) {
    listItem *tmp = root;
    if (skip_root) {
        if (tmp->next == NULL) return insert(root, new_el);
        else tmp = tmp->next;
    }
    while (tmp->next != NULL) {
        if (new_el->weight <= tmp->weight) return insert(tmp->prev, new_el);
        else tmp = tmp->next;
    }    
    if (new_el->weight <= tmp->weight) return insert(tmp->prev, new_el);
    else return insert(tmp, new_el);
}

int get_list_len(listItem *root) {
    int len = 1;
    listItem *tmp = root;
    while (tmp->next) {
        tmp = tmp->next;
        len++;
    }
    return len;
}

void print_list_item(listItem *item) {
    printf("(Weight: %i, ID: %i)\n", item->weight, item->id);
}

void print_list_item_data(listItem *item) {
    for (int i = 0; i < 14; i++) printf("%i.: %i\n", i, (item->data)[i]);
}

void print_list(listItem *root) {
    while (root->next != NULL) {
        print_list_item(root);
        root = root->next;
    }
    print_list_item(root);
}