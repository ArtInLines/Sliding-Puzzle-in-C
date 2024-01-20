// TO build run the following command:
// gcc perftest.c board.c priority.c test.c util.c play.c pathfinding.c -o perftest.exe

#include "general.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

double get_time(void) {
  return (double)clock() / CLOCKS_PER_SEC;
}

double create_initial_board_test(int column_size, int row_size, int variance) {
    double begin_time, end_time;
    begin_time = get_time();
    int *board = create_initial_board(column_size, row_size, variance);
    end_time = get_time();
    free(board);
    return end_time - begin_time;
}

double A_star_test(int column_size, int row_size, int bias, int variance) {
    int *empty_field, *board, *path;
    double begin_time, end_time;

    board = create_initial_board(column_size, row_size, variance);
    empty_field = get_empty_field(column_size, board);

    begin_time = get_time();
    path = A_star(column_size, row_size, bias, empty_field, board, 0);
    end_time = get_time();
    free(path);

    return end_time - begin_time;
}

void run_test(int column_size, int row_size, int bias, int variance) {
    double create_board_time, a_star_time;

    printf("\rsize=%ix%i, variance=%i, bias=%i, ", column_size, row_size, variance, bias);

    create_board_time = create_initial_board_test(column_size, row_size, variance);
    printf("create_board_time=%.4fs, ", create_board_time);

    a_star_time = A_star_test(column_size, row_size, bias, variance);
    printf("solve_time=%3.4fs", a_star_time);

}

int main(void) {
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

        run_test(column_size, row_size, bias, variance);
        printf("\n\n");
    }
    return 0;
}