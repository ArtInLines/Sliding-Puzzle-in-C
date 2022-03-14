struct list_el {
    int id;                     // Unique ID of each element
    int weight;                 // weight -> sorted in ascending order
    int *data;                  // Arbitrary data payload
    int size;                   // Size of the data payload
    struct list_el* next;       // Pointer to next item
    struct list_el* prev;       // Pointer to previous item
    };
typedef struct list_el listItem;


int* A_star(int column_size, int row_size, int bias, int *empty_field, int *board, int print_progress);

int* A_star_mem_efficient(int column_size, int row_size, int *empty_field, int *board);


int next_id();

int rand_int();

void print_list(listItem *root);

void print_list_item(listItem *item);

void print_list_item_data(listItem *item);

listItem* insert_sorted(listItem *root, listItem *new_el, int skip_root);

listItem* sort(listItem *root);

void remove_item(listItem *el);

listItem* shift(listItem *root);

listItem* insert(listItem *last_item, listItem *new_el);

listItem* create_item(int id, int weight, void *data, int size);

listItem* copy_item(listItem *item, int keep_id);

listItem* find_by_id(int id, listItem *root);

listItem* find_by_data(listItem *root, int (*f) (int*));

int get_list_len(listItem *root);