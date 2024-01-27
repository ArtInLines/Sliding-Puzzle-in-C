#include "general.h"
#define AIL_HM_IMPL
#include "ail/ail_hm.h"
#include <stdlib.h>
#include <stdio.h>

static u16 State_Size;
typedef u8* Bytes;
AIL_DA_INIT(Bytes);
AIL_HM_INIT(Bytes, u8);

typedef struct {
    u8 *fields;
    Dir *dirs;
} StateEl;
AIL_DA_INIT(StateEl);

// next_states fills an array of 4 states with the possible next states and set len to the amount of possible next states
// A state is layed out as follows:
// First the index of the currently empty field
// Next the fields bytes of the board

// [in]  state should hold the bytes for the current state
// [out] next should hold enough space for 4*(rows*cols + 1)) bytes
// [out] dirs should hold enough space for 4 Dirs. It is a parallel array to next
// [out] len stores the amount of states that were actually added
static inline void next_states(const u8 *restrict state, u8 *restrict next, Dir *restrict dirs, u8 *restrict len, u8 rows, u8 cols) {
    u16 board_size = rows*cols;
    u16 state_size = 1 + board_size;
    u8  pos = state[0];
    u16 j = 0;
    for (u8 i = 0; i < 4; i++) {
        u8 np = next_pos_idx(pos, (Dir)i, rows, cols);
        if (np != UINT8_MAX) {
            dirs[j] = (Dir)i;
            u8 *const ns = &next[j * state_size];
            memcpy(&ns[1], &state[1], board_size);
            ns[0] = np;
            u8 tmp = ns[1 + np];
            ns[1 + np]  = ns[1 + pos];
            ns[1 + pos] = tmp;
            j++;
        }
    }
    *len = j;
}

// No error checking is done in this function
static inline void play_turns(u8 *fields, u8 empty_idx, u8 cols, AIL_DA(Dir) dirs) {
    u8 ci = empty_idx;
    u8 ni;
    for (u32 i = 0; i < dirs.len; i++) {
        Dir d = dirs.data[i];
        ni = next_pos_idx_unsafe(ci, d, cols);
        u8 tmp = fields[ci];
        fields[ci] = fields[ni];
        fields[ni] = tmp;
        ci = ni;
    }
}

// Uses djb2 for now
// @TODO: Test if this is actually a good hash for our data
u32 fields_hash(Bytes fields) {
    u32 h = 5381;
    for (u16 i = 0; i < State_Size; i++) {
        h = ((h << 5) + h) + fields[i];
    }
    return h;
}

bool fields_eq(Bytes a, Bytes b) {
    for (u16 i = 0; i < State_Size; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// @TODO: Use custom allocator
AIL_DA(Dir) bfs(u8 empty_pos, Board board) {
    State_Size = board.rows*board.cols + 1;
    AIL_HM(Bytes, u8) visited = ail_hm_with_cap(Bytes, u8, 512*State_Size, &fields_hash, &fields_eq);
    AIL_DA(u8) visited_backlog = ail_da_new(u8);
    AIL_DA(StateEl) cstates = ail_da_new(StateEl);
    AIL_DA(StateEl) nstates = ail_da_new(StateEl);
    AIL_DA(Dir) path = ail_da_new(Dir);

    u8 *start = malloc(State_Size);
    start[0] = empty_pos;
    memcpy(&start[1], board.fields, board.rows*board.cols);
    ail_da_push(&cstates, ((StateEl){start, NULL}));

    u32 depth = 0;
    u8 next_amount;
    u8 *next = malloc(4*State_Size);
    Dir dirs[4];
    while (cstates.len) {
        for (u32 i = 0; i < cstates.len; i++) {
            StateEl state = cstates.data[i];

            // printf("\n\n");
            // printf("Current State: ( ");
            // for (u32 i = 0; i < depth; i++) printf("%s ", get_direction_string(state.dirs[i]));
            // printf(")\n");
            // show_board((Board){ board.rows, board.cols, &state.fields[1] });

            next_states(state.fields, next, dirs, &next_amount, board.rows, board.cols);
            for (u8 j = 0; j < next_amount; j++) {
                // printf("Next State: (%s)\n", get_direction_string(dirs[j]));
                // show_board((Board) { board.rows, board.cols, &next[j*State_Size + 1] });

                u32 idx;
                bool found;
                ail_hm_get_idx(&visited, &next[j*State_Size], idx, found);
                (void)idx;
                if (!found) {
                    if (is_solved(board.rows*board.cols, &next[j*State_Size + 1])) {
                        ail_da_pushn(&path, state.dirs, depth);
                        ail_da_push(&path, dirs[j]);
                        goto end;
                    }

                    u8 *ptr = malloc(State_Size + sizeof(Dir)*(depth + 1));
                    StateEl ns = {
                        .fields = ptr,
                        .dirs   = (Dir *)&ptr[State_Size],
                    };
                    memcpy(ns.fields, &next[j*State_Size], State_Size);
                    for (u32 k = 0; k < depth; k++) ns.dirs[k] = state.dirs[k]; // @Note: Memcpy didn't work here for some reason
                    ns.dirs[depth] = dirs[j];
                    ail_da_push(&nstates, ns);
                    u32 backlog_idx = visited_backlog.len;
                    ail_da_pushn(&visited_backlog, &next[j*State_Size], State_Size);
                    ail_hm_put(&visited, &visited_backlog.data[backlog_idx], 1);
                }
            }
            if (state.fields) free(state.fields);

        }
        cstates.len = 0;
        AIL_DA(StateEl) tmp = cstates;
        cstates = nstates;
        nstates = tmp;
        depth++;
    }
    ail_da_free(&path);
end:
    ail_da_free(&cstates);
    ail_da_free(&nstates);
    ail_hm_free(&visited);
    ail_da_free(&visited_backlog);
    free(next);

    // printf("\n\n");
    // printf("Start:\n");
    // show_board(board);
    // printf("Path of %d moves:\n", path.len);
    // for (u32 i = 0; i < path.len; i++)
    //     printf("  %s\n", get_direction_string(path.data[i]));
    // AIL_DBG_EXIT();
    return path;
}