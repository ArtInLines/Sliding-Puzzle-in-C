// Enums & Structs

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
    ILLEGAL_DIRECTION
};


// Play:

int play_turn(int direction, int column_size, int row_size, int *board, int *empty_field, int inverted);

void move(int *pos1, int *pos2, int column_size, int *board);

int get_opposite_direction(int direction);

int get_direction(int inverted);

char* get_direction_string(int direction);


// Utilities:

int distance(int x, int y);

int swap_ints(int *x, int *y);

int* index_to_pos(int index, int column_size);

int pos_to_index(int *pos, int column_size);

void clear_screen();

int rand_int();

int next_id();

int get_id();


// Board:

int is_solved(int len, int *board);

int is_solvable(int column_size, int row_size, int *board);

int solvable_helper(int column_size, int row_size, int *board);

int* create_initial_board(int column_size, int row_size, int variance);

int* copy_board(int len, int *board);

void show_board(int column_size, int row_size, int *board);

void print_row(int *column_size, float *cell_width, char *border_char);

int* get_new_pos(int *old_pos, int dir);

int create_board_id(int *board, int len);

int compare_board(int *board1, int *board2, int len);

int* get_empty_field(int column_size, int *board);


// Priorities / Heuristics:

// Hamming priority function. The number of blocks in the wrong position, plus the number of moves made so far to get to the state. Intutively, a state with a small number of blocks in the wrong position is close to the goal state, and we prefer a state that have been reached using a small number of moves.
int hamming(int moves_amount, int bias, int column_size, int row_size, int *board);

// Manhattan priority function. The sum of the Manhattan distances (sum of the vertical and horizontal distance) from the blocks to their goal positions, plus the number of moves made so far to get to the state.
int manhattan(int moves_amount, int bias, int column_size, int row_size, int *board);

int get_priority(int moves_amount, int bias, int column_size, int row_size, int *board);


// Tests:

void test_priority();

void test_solvable();