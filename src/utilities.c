#include <stdio.h>


void swap(int *const data, const int i, const int j) {
    int temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

void print_array(int *const data, unsigned int size) {
    for (unsigned int i = 0; i < size - 1; i++) {
        printf("%d ", data[i]);
    }
    printf("%d\n", data[size - 1]);
}