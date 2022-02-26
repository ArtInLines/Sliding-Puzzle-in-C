// Enums & Structs

struct list_el {
    int weight;                 // weight -> sorted in ascending order
    void *data;                 // Arbitrary data payload
    size_t size;                // Size of the data payload
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


// Utilities:

int distance(int x, int y);

int swap_ints(int *x, int *y);

int* index_to_pos(int index, int column_size);

int pos_to_index(int *pos, int column_size);

void clear_screen();

void print_list(listItem *root);

void print_list_item(listItem *item);

listItem* insert_sorted(listItem *root, listItem *new_el);

listItem* sort(listItem *root);

listItem* remove(listItem *el);

listItem* insert(listItem *last_item, listItem *new_el);

listItem* create_item(int weight, void *data, size_t size);

// Board:

int is_solved(int len, int *board);

int is_solvable(int len, int *board);

int* create_initial_board(int column_size, int row_size, int variance);

void show_board(int column_size, int row_size, int *board);

int count_transpositions(int len, int *board);

int get_direction();


// Priorities / Heuristics:

// Hamming priority function. The number of blocks in the wrong position, plus the number of moves made so far to get to the state. Intutively, a state with a small number of blocks in the wrong position is close to the goal state, and we prefer a state that have been reached using a small number of moves.
int hamming(int moves_amount, int column_size, int row_size, int *board);

// Manhattan priority function. The sum of the Manhattan distances (sum of the vertical and horizontal distance) from the blocks to their goal positions, plus the number of moves made so far to get to the state.
int manhattan(int moves_amount, int column_size, int row_size, int *board);


// Tests:

void test_priority();

void test_solvable();