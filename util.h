// Enums

enum direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
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

int get_opposite_direction(int direction);

int get_direction();

void move(int *pos);


// Utilities:

int distance(int x, int y);

int swap_ints(int *x, int *y);

int* index_to_pos(int index, int column_size);

int pos_to_index(int *pos, int column_size);

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