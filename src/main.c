#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../headers/benchmark.h"
#include "../headers/quicksort.h"


int main(int argc, char *argv[]) {
    int (*f)(int *const, const int);
    unsigned int N;
    unsigned int K;
    int *initial_sequence;

    if (argc != 4) {
        exit(1);
    }

    if (strcmp(argv[1], "0") == 0) {
        f = quicksort;
    } else if (strcmp(argv[1], "1") == 0) {
        f = hybrid_quicksort;
    } else if (strcmp(argv[1], "2") == 0) {
        f = threaded_quicksort;
    } else {
        exit(2);
    }

    if ((N = atoi(argv[2])) == 0) {
        exit(3);
    }

    if ((K = atoi(argv[3])) == 0) {
        exit(4);
    }

    if ((initial_sequence = malloc(N * sizeof(int))) == NULL) {
        exit(5);
    }

    srandom((unsigned int)1000);

    for (unsigned int i = 0; i < N; i++) {
        initial_sequence[i] = random() % 60000 - 30000;
    }

    printf("%lf\n", measure(f, initial_sequence, N, K));

    free(initial_sequence);

    exit(0);
}