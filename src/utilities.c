#include <stdio.h>

#include "../headers/utilities.h"

void swap(int *const data, const int i, const int j) {
    int temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

void print_array(int *const data, const unsigned int size) {
    for (unsigned int i = 0; i < size - 1; i++) {
        printf("%d ", data[i]);
    }
    printf("%d\n", data[size - 1]);
}

int is_ordered(int *const data, const unsigned int size) {
    for (unsigned int i = 1; i < size; i++) {
        if (data[i-1] > data[i]) {
            return -1;
        }
    }
    return 0;
}