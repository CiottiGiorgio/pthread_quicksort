#include "../headers/utilities.h"


int partition(int *const data, const unsigned int low, const unsigned int high) {
    const int pivot = data[ (low + high) / 2 ];
    int i = low - 1;
    int j = high + 1;

    while (1) {
        do {
            i++;
        } while (data[i] < pivot);
        do {
            j--;
        } while (data[j] > pivot);
        if (i >= j) {
            return j;
        }
        swap(data, i, j);
    }
}


void _quicksort(int *const data, const unsigned int low, const unsigned int high) {
    if (low < high) {
        const int p = partition(data, low, high);
        _quicksort(data, low, p);
        _quicksort(data, p+1, high);
    }
}

void quicksort(int *const data, const unsigned int size) {
    _quicksort(data, 0, size - 1);
}


void _insertionsort(int *const data, const unsigned int low, const unsigned int high) {
    int x;
    int i, j;
    
    for (i = low + 1; i < (int)(low + high + 1); i++) {
        x = data[i];
        for (j = i - 1; ((j >= (int)low) && (data[j] > x)); j--) {
            data[j+1] = data[j];
        }
        data[j+1] = x;
    }
}

void _hybrid_quicksort(int *const data, const unsigned int low, const unsigned int high) {
    if (low < high) {
        if (high - low < 32) {
            _insertionsort(data, low, high);
        } else {
            const int p = partition(data, low, high);
            _hybrid_quicksort(data, low, p);
            _hybrid_quicksort(data, p+1, high);
        }
    }
}

void hybrid_quicksort(int *const data, const unsigned int size) {
    _hybrid_quicksort(data, 0, size - 1);
}