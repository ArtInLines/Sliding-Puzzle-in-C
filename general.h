#ifndef GENERAL_H_
#define GENERAL_H_

#define AIL_ALL_IMPL
#include "ail/ail.h"
#include "ail/ail_alloc.h"
#include <stdbool.h>

// Enums & Structs
typedef struct Pos {
    u8 x, y;
} Pos;

typedef struct Board {
    u8  rows, cols;
    u8 *fields;
} Board;

struct list_el {
    int id;            // Unique ID of each element
    int weight;                 // weight -> sorted in ascending order
    int *data;                  // Arbitrary data payload
    int size;                   // Size of the data payload
    struct list_el* next;       // Pointer to next item
    struct list_el* prev;       // Pointer to previous item
    };
typedef struct list_el listItem;

AIL_PACK_BEGIN()
enum Dir {
    DIR_UP      = 0, // 00
    DIR_DOWN    = 3, // 11
    DIR_RIGHT   = 1, // 01
    DIR_LEFT    = 2, // 10
    DIR_GETHELP = 5,
    DIR_FINISH  = 6,
    DIR_ERROR   = 7,
};
AIL_PACK_END()
typedef enum Dir Dir;
AIL_DA_INIT(Dir);

typedef enum TurnResult {
    SUCCESS,
    MOVE_OUTSIDE_BORDERS,
    ILLEGAL_DIRECTION
} TurnResult;

typedef AIL_DA(Dir) (search_t)(u8 empty_pos, Board board, AIL_Allocator *allocator);

// Utilities:
#define DIST(x, y) ((x) < (y))*((y) - (x)) + ((x) >= (y))*((x) - (y))

static inline Pos index_to_pos(u8 idx, u8 cols) {
    return (Pos) {
        idx / cols,
        idx % cols
    };
}

static inline u8 pos_to_index(Pos pos, u8 cols) {
    return pos.x*cols + pos.y;
}

// Play:
int play_turn(Dir direction, Board board, Pos *empty_field);
void move(Pos to, Pos from, Board board);
Dir get_direction(bool inverted);
Dir invert_direction(Dir dir);
char* get_direction_string(Dir d);


// Pathfinding:
search_t bfs;
search_t dfs;


// Board:
bool is_solved(int len, u8 *board);
bool is_solvable(Board board, AIL_Allocator *allocator);
int solvable_helper(Board board, AIL_Allocator *allocator);
Board create_initial_board(int cols, int rows, int variance, AIL_Allocator *allocator);
Board copy_board(Board board, AIL_Allocator *allocator);
void show_board(Board board);
Pos next_pos(Pos pos, Dir dir);
u8 next_pos_idx(u8 pos, Dir dir, u8 rows, u8 cols);
u8 next_pos_idx_unsafe(u8 pos, Dir dir, u8 cols);
int compare_board(u8 *board1, u8 *board2, int len);
Pos get_empty_field(int column_size, u8 *board);
bool is_legal_pos(Pos p, u8 rows, u8 cols);


// Priorities / Heuristics:
// Hamming priority function. The number of blocks in the wrong position, plus the number of moves made so far to get to the state. Intutively, a state with a small number of blocks in the wrong position is close to the goal state, and we prefer a state that have been reached using a small number of moves.
int hamming(int moves_amount, int bias, Board board);
// Manhattan priority function. The sum of the Manhattan distances (sum of the vertical and horizontal distance) from the blocks to their goal positions, plus the number of moves made so far to get to the state.
int manhattan(int moves_amount, int bias, Board board);
int get_priority(int moves_amount, int bias, Board board);


// Tests:
void test_priority();
void test_solvable();

#endif // GENERAL_H_