#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/benchmark.c"

#define N (1<<24)
#define K (5)


int main() {
    srandom(time(NULL));

    int *initial_sequence = (int *)malloc(N * sizeof(int));

    for (unsigned int i = 0; i < N; i++) {
        initial_sequence[i] = random() % 60000 - 30000;
    }

    printf("Quicksort:\t\t%lf s\n", measure(&quicksort, initial_sequence, N, K));
    printf("Hybrid quicksort:\t%lf s\n", measure(&hybrid_quicksort, initial_sequence, N, K));
    printf("Threaded quicksort:\t%lf s\n", measure(&threaded_quicksort, initial_sequence, N, K));

    free((void *)initial_sequence);

    return 0;
}