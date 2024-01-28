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
} BFSStateEl;
AIL_DA_INIT(BFSStateEl);

typedef struct {
    u8 *fields;
    u32 depth;
    Dir dir;
} DFSStateEl;
AIL_DA_INIT(DFSStateEl);

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

#define SET_STATE_SIZE(board) State_Size = (board).rows*(board).cols + 1

AIL_DA(Dir) dfs(u8 empty_pos, Board board, AIL_Allocator *allocator) {
    SET_STATE_SIZE(board);
    AIL_DA(Dir) path           = ail_da_new_with_alloc(Dir, AIL_DA_INIT_CAP, allocator);
    AIL_HM(Bytes, u8) visited  = ail_hm_new_with_alloc(Bytes, u8, 1024*State_Size, &fields_hash, &fields_eq, allocator);
    AIL_DA(u8) visited_backlog = ail_da_new_with_alloc(u8, AIL_DA_INIT_CAP, allocator);
    AIL_DA(DFSStateEl) stack   = ail_da_new_with_alloc(DFSStateEl, AIL_DA_INIT_CAP, allocator);

    u8 *start = allocator->alloc(allocator->data, State_Size);
    start[0] = empty_pos;
    memcpy(&start[1], board.fields, board.rows*board.cols);
    DFSStateEl start_el = { .depth = UINT32_MAX, .fields = start };
    ail_da_push(&stack, start_el);

    while (stack.len) {
        u8 *next = allocator->alloc(allocator->data, 4*State_Size);
        Dir dirs[4];
        u8 amount;
        DFSStateEl state = stack.data[--stack.len];
        if (AIL_LIKELY(state.depth != UINT32_MAX)) {
            path.len = state.depth;
            ail_da_push(&path, state.dir);
        }
        next_states(state.fields, next, dirs, &amount, board.rows, board.cols);
        for (u8 i = 0; i < amount; i++) {
            u32 idx; AIL_UNUSED(idx);
            bool found;
            ail_hm_get_idx(&visited, &next[i*State_Size], idx, found);
            if (!found) {
                if (is_solved(board.rows*board.cols, &next[i*State_Size + 1])) {
                    ail_da_push(&path, dirs[i]);
                    goto done;
                }

                u32 backlog_idx = visited_backlog.len;
                ail_da_pushn(&visited_backlog, &next[i*State_Size], State_Size);
                ail_hm_put(&visited, &visited_backlog.data[backlog_idx], 1);

                DFSStateEl ns = {
                    .depth  = path.len,
                    .dir    = dirs[i],
                    .fields = &visited_backlog.data[backlog_idx],
                };
                ail_da_push(&stack, ns);
            }
        }
        allocator->free_one(allocator->data, next);
    }

done:
    ail_hm_free(&visited);
    ail_da_free(&visited_backlog);
    ail_da_free(&stack);
    allocator->free_one(allocator->data, start);
    return path;
}

// @TODO: Use custom allocator
AIL_DA(Dir) bfs(u8 empty_pos, Board board, AIL_Allocator *allocator) {
    SET_STATE_SIZE(board);
    AIL_HM(Bytes, u8) visited  = ail_hm_new_with_alloc(Bytes, u8, 512*State_Size, &fields_hash, &fields_eq, allocator);
    AIL_DA(u8) visited_backlog = ail_da_new_with_alloc(u8,         AIL_DA_INIT_CAP, allocator);
    AIL_DA(BFSStateEl) cstates = ail_da_new_with_alloc(BFSStateEl, AIL_DA_INIT_CAP, allocator);
    AIL_DA(BFSStateEl) nstates = ail_da_new_with_alloc(BFSStateEl, AIL_DA_INIT_CAP, allocator);
    AIL_DA(Dir)        path    = ail_da_new_with_alloc(Dir,        AIL_DA_INIT_CAP, allocator);

    u8 *start = allocator->alloc(allocator->data, State_Size);
    start[0] = empty_pos;
    memcpy(&start[1], board.fields, board.rows*board.cols);
    ail_da_push(&cstates, ((BFSStateEl){start, NULL}));

    u32 depth = 0;
    u8 next_amount;
    u8 *next = allocator->alloc(allocator->data, 4*State_Size);
    Dir dirs[4];
    while (cstates.len) {
        for (u32 i = 0; i < cstates.len; i++) {
            BFSStateEl state = cstates.data[i];
            next_states(state.fields, next, dirs, &next_amount, board.rows, board.cols);
            for (u8 j = 0; j < next_amount; j++) {
                u32 idx; AIL_UNUSED(idx);
                bool found;
                ail_hm_get_idx(&visited, &next[j*State_Size], idx, found);
                if (!found) {
                    if (is_solved(board.rows*board.cols, &next[j*State_Size + 1])) {
                        ail_da_pushn(&path, state.dirs, depth);
                        ail_da_push(&path, dirs[j]);
                        goto done;
                    }

                    u8 *ptr = allocator->alloc(allocator->data, State_Size + sizeof(Dir)*(depth + 1));
                    BFSStateEl ns = {
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
            if (state.fields) allocator->free_one(allocator->data, state.fields);

        }
        cstates.len = 0;
        AIL_DA(BFSStateEl) tmp = cstates;
        cstates = nstates;
        nstates = tmp;
        depth++;
    }
    ail_da_free(&path);
done:
    ail_da_free(&cstates);
    ail_da_free(&nstates);
    ail_hm_free(&visited);
    ail_da_free(&visited_backlog);
    allocator->free_one(allocator->data, next);
    return path;
}