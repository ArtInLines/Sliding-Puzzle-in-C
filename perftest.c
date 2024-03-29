#include "general.h"
#include "board.c"
#include "priority.c"
#include "play.c"
#include "pathfinding.c"
#include "screen.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

double get_time(void) {
  return (double)clock() / CLOCKS_PER_SEC;
}

double create_initial_board_test(Board *board, int column_size, int row_size, int variance, AIL_Allocator *allocator) {
    double begin_time, end_time;
    begin_time = get_time();
    *board     = create_initial_board(column_size, row_size, variance, allocator);
    end_time   = get_time();
    return end_time - begin_time;
}

// double A_star_test(Board board, int bias) {
//     double begin_time, end_time;
//     Pos empty_field = get_empty_field(board.cols, board.fields);
//     int p[2]        = {empty_field.x, empty_field.y};
//     begin_time      = get_time();
//     int *path       = A_star(board.cols, board.rows, bias, p, board.fields, 0);
//     end_time        = get_time();
//     free(path);

//     return end_time - begin_time;
// }

void run_test(int column_size, int row_size, int bias, int variance, AIL_Allocator *allocator) {
    double create_board_time, a_star_time;

    printf("\rsize=%ix%i, variance=%i, bias=%i, ", column_size, row_size, variance, bias);

    Board board;
    create_board_time = create_initial_board_test(&board, column_size, row_size, variance, allocator);
    printf("create_board_time=%.4fs, ", create_board_time);

    // a_star_time = A_star_test(board, bias);
    // printf("solve_time=%3.4fs", a_star_time);

}

int main(void) {
    AIL_Allocator allocator = ail_alloc_std;
    int column_size, row_size, bias, variance;
    clear_screen();

    while (1) {
        printf("Column Size? ");
        scanf("%d", &column_size);

        printf("Row Size? ");
        scanf("%d", &row_size);

        printf("Bias? ");
        scanf("%d", &bias);

        printf("Variance (negative => Max. Integer)? ");
        scanf("%d", &variance);
        if (variance < 0) variance = INT_MAX;

        run_test(column_size, row_size, bias, variance, &allocator);
        printf("\n\n");
    }
    return 0;
}