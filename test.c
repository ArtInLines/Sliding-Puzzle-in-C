#include <stdio.h>
#include "util.h"


void test_priority() {
    int fails = 0;
    int test_amounts = 2;
    int tests[] = {
    // expected_hamming, expected_manhattan, column_size, row_size,
        // *board
        5, 10, 3, 3,
            8, 1, 3,
            4, 0, 2,
            7, 6, 5,
        4, 10, 4, 3,
            2, 8, 3, 0,
            5, 6, 7, 4,
            9, 10, 11, 1
    };
    
    int i, j, expected_hamming, expected_manhattan, hamming_res, manhattan_res, column_size, row_size, index = 0;
    
    for (i = 0; i < test_amounts; i++) {
        expected_hamming = tests[index++];
        expected_manhattan = tests[index++];
        column_size = tests[index++];
        row_size = tests[index++];
        
        // printf("\n%i - %i - %i - %i\n", expected_hamming, expected_manhattan, column_size, row_size);
        
        hamming_res = hamming(0, column_size, row_size, &tests[index]);
        manhattan_res = manhattan(0, column_size, row_size, &tests[index]);
        
        if (hamming_res == expected_hamming) printf("Hamming test #%i successful\n", i+1);
        else {
            fails++;
            printf("Hamming test #%i unsuccessful  -  Received %i instead of %i\n", i+1, hamming_res, expected_hamming);
        }
        
        if (manhattan_res == expected_manhattan) printf("Manhattan test #%i successful\n", i+1);
        else {
            fails++;
            printf("Manhattan test #%i unsuccessful  -  Received %i instead of %i\n", i+1, manhattan_res, expected_manhattan);
        }
        
        index += column_size*row_size;
    }
    
    printf("Tests ended with %i our of %i succesful tests", test_amounts*2-fails, test_amounts*2);
}