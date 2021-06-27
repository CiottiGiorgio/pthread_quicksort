#include <stdio.h>

#include "../headers/quicksort.h"
#include "../headers/utilities.h"

#define N 11

int main() {
    int sequence[N] = {5, -2, 5, 1, 0, 2, 3, 0, 9, 100, -999};

    print_array(sequence, N);

    //quicksort(sequence, N);
    //_insertionsort(sequence, 0, N - 1);
    hybrid_quicksort(sequence, N);

    print_array(sequence, N);

    return 0;
}