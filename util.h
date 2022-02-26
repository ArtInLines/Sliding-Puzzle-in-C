// Enums & Structs

struct list_el {
    int id;            // Unique ID of each element
    int weight;                 // weight -> sorted in ascending order
    int *data;                  // Arbitrary data payload
    int size;                   // Size of the data payload
    struct list_el* next;       // Pointer to next item
    struct list_el* prev;       // Pointer to previous item
    };
typedef struct list_el listItem;

enum direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    GETHELP,
    FINISH,
    ERROR
};

enum play_turn_return_codes {
    SUCCESS,
    MOVE_OUTSIDE_BORDERS,
    REPEATED_MOVE_BACK,
    ILLEGAL_DIRECTION
};


// Play:

int init_game(int column_size, int row_size, int *board, int inverted);

int play_turn(int direction);

int play_turn_with_board(int direction, int *board, int *empty_field);

int get_opposite_direction(int direction);

int get_direction();

void move(int *pos1, int *pos2, int *board);

int* A_star();

int* A_star_mem_efficient();

int get_next_move();


// Utilities:

int distance(int x, int y);

int swap_ints(int *x, int *y);

int* index_to_pos(int index, int column_size);

int pos_to_index(int *pos, int column_size);

void clear_screen();

int rand_id();

void print_list(listItem *root);

void print_list_item(listItem *item);

listItem* insert_sorted(listItem *root, listItem *new_el, int skip_root);

listItem* sort(listItem *root);

void remove_item(listItem *el);

listItem* shift(listItem *root);

listItem* insert(listItem *last_item, listItem *new_el);

listItem* create_item(int weight, void *data, int size);

listItem* copy_item(listItem *item, int keep_id);


// Board:

int is_solved(int len, int *board);

int is_solvable(int len, int *board);

int* create_initial_board(int column_size, int row_size, int variance);

void show_board(int column_size, int row_size, int *board);

int count_transpositions(int len, int *board);

int get_direction();

int* get_new_pos(int *old_pos, int dir);


// Priorities / Heuristics:

// Hamming priority function. The number of blocks in the wrong position, plus the number of moves made so far to get to the state. Intutively, a state with a small number of blocks in the wrong position is close to the goal state, and we prefer a state that have been reached using a small number of moves.
int hamming(int moves_amount, int column_size, int row_size, int *board);

// Manhattan priority function. The sum of the Manhattan distances (sum of the vertical and horizontal distance) from the blocks to their goal positions, plus the number of moves made so far to get to the state.
int manhattan(int moves_amount, int column_size, int row_size, int *board);

int get_priority(int moves_amount, int column_size, int row_size, int *board);


// Tests:

void test_priority();

void test_solvable();