#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/utilities.h"
#include "../headers/quicksort.h"

#define N (1<<17)
#define K (5)


static void benchmark() {
    srandom(time(NULL));

    int *initial_sequence = (int *)malloc(N * sizeof(int));
    int *test_sequence = (int *)malloc(N * sizeof(int));

    for (unsigned int i = 0; i < N; i++) {
        initial_sequence[i] = random() % 60000 - 30000;
    }

    clock_t begin, end;
    double timeElapsed = 0;

    for (unsigned int i = 0; i < K; i++) {
        memcpy((void *)test_sequence, (void *)initial_sequence, N * sizeof(int));

        begin = clock();
        //quicksort(test_sequence, N);
        //hybrid_quicksort(test_sequence, N);
        threaded_quicksort(test_sequence, N);
        end = clock();

        timeElapsed += (double)(end - begin) / CLOCKS_PER_SEC;
    }

    //print_array(initial_sequence, N);
    //print_array(test_sequence, N);

    timeElapsed /= K;
    printf("%s\n", is_ordered(test_sequence, N) == 0 ? "Success!" : "Failure.");
    printf("Average time:\t%lf s\n", timeElapsed);
}