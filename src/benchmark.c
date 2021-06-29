#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../headers/utilities.h"
#include "../headers/quicksort.h"


double measure(void (*f)(int *const, const int), int *const initial_sequence, const int size, const unsigned int repetitions) {
    int *test_sequence = malloc(size * sizeof(int));

    struct timespec start, finish;
    double elapsed = 0;

    for (unsigned int i = 0; i < repetitions; i++) {
        memcpy((void *)test_sequence, (void *)initial_sequence, size * sizeof(int));

        clock_gettime(CLOCK_MONOTONIC, &start);
        f(test_sequence, size);
        clock_gettime(CLOCK_MONOTONIC, &finish);

        elapsed += (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    }

    //print_array(initial_sequence, size);
    //print_array(test_sequence, size);

    elapsed /= repetitions;

    free((void *)test_sequence);

    return elapsed;
}