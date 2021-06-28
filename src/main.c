#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/benchmark.c"

#define N (1<<20)
#define K (5)


int main() {
    srandom(time(NULL));

    int *initial_sequence = (int *)malloc(N * sizeof(int));

    for (unsigned int i = 0; i < N; i++) {
        initial_sequence[i] = random() % 60000 - 30000;
    }

    measure(&quicksort, initial_sequence, N, K);
    measure(&hybrid_quicksort, initial_sequence, N, K);
    measure(&threaded_quicksort, initial_sequence, N, K);

    free((void *)initial_sequence);

    return 0;
}