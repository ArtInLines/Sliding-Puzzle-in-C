#ifndef GENERAL_H_
#define GENERAL_H_

#define AIL_TYPES_IMPL
#include "ail/ail.h"
#include <stdbool.h>

// Enums & Structs

typedef struct Pos {
    i8 x, y;
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

typedef enum Dir {
    UP      = 0, // 00
    DOWN    = 3, // 11
    RIGHT   = 1, // 01
    LEFT    = 2, // 10
    GETHELP = 5,
    FINISH  = 6,
    ERROR   = 7,
} Dir;

typedef enum TurnResult {
    SUCCESS,
    MOVE_OUTSIDE_BORDERS,
    ILLEGAL_DIRECTION
} TurnResult;


// Play:
int play_turn(Dir direction, Board board, Pos *empty_field);
void move(Pos to, Pos from, Board board);
int get_direction(int inverted);
int invert_direction(int dir);
char* get_direction_string(int direction);


// Pathfinding:
int* A_star(int column_size, int row_size, int bias, int *empty_field, u8 *board, int print_progress);
int* A_star_mem_efficient(int column_size, int row_size, int *empty_field, u8 *board);


// Utilities:
int distance(int x, int y);
Pos index_to_pos(int index, int column_size);
int pos_to_index(Pos pos, int column_size);
void clear_screen();
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


// Board:
bool is_solved(int len, u8 *board);
bool is_solvable(Board board);
int solvable_helper(Board board);
Board create_initial_board(int cols, int rows, int variance);
Board copy_board(Board board);
void show_board(Board board);
Pos next_pos(Pos pos, Dir dir);
int compare_board(u8 *board1, u8 *board2, int len);
Pos get_empty_field(int column_size, u8 *board);


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