#include <stdlib.h>
#include <string.h>

#include "../C-Thread-Pool/thpool.h"
#include "../headers/utilities.h"

#define N_THREADS (8)
#define SORTING_THRESHOLD (256)


int partition(int *const data, const int low, const int high) {
    const int pivot = data[ (low + high) / 2 ];
    int i = low - 1;
    int j = high + 1;

    while (1) {
        do i++; while (data[i] < pivot);
        do j--; while (data[j] > pivot);
        if (i >= j) return j;
        swap(data, i, j);
    }
}


void _quicksort(int *const data, const int low, const int high) {
    if (low < high) {
        const int p = partition(data, low, high);
        _quicksort(data, low, p);
        _quicksort(data, p+1, high);
    }
}

void quicksort(int *const data, const int size) {
    _quicksort(data, 0, size - 1);
}


void _insertionsort(int *const data, const int low, const int high) {
    int x;
    int i, j;
    
    for (i = low + 1; i < high + 1; i++) {
        x = data[i];
        for (j = i - 1; ((j >= low) && (data[j] > x)); j--) {
            data[j+1] = data[j];
        }
        data[j+1] = x;
    }
}

void _hybrid_quicksort(int *const data, const int low, const int high) {
    if (low < high) {
        if (high - low < SORTING_THRESHOLD) {
            _insertionsort(data, low, high);
        } else {
            const int p = partition(data, low, high);
            _hybrid_quicksort(data, low, p);
            _hybrid_quicksort(data, p+1, high);
        }
    }
}

void hybrid_quicksort(int *const data, const int size) {
    _hybrid_quicksort(data, 0, size - 1);
}


typedef struct {
    int *data;
    int low;
    int high;
    threadpool pool;
} qs_params;

void _threaded_quicksort(void *args) {
    // This stands for "concrete args".
    qs_params *c_args = (qs_params *)args;

    if (c_args->low < c_args->high) {
        if (c_args->high - c_args->low < SORTING_THRESHOLD) {
            _insertionsort(c_args->data, c_args->low, c_args->high);
        } else {
            qs_params *left_child, *right_child;

            const int p = partition(c_args->data, c_args->low, c_args->high);

            left_child  = malloc(sizeof(qs_params));
            right_child = malloc(sizeof(qs_params));

            left_child->data    = c_args->data;
            left_child->low     = c_args->low;
            left_child->high    = p;
            left_child->pool    = c_args->pool;

            right_child->data   = c_args->data;
            right_child->low    = p + 1;
            right_child->high   = c_args->high;
            right_child->pool   = c_args->pool;

            thpool_add_work(c_args->pool, _threaded_quicksort, (void *)left_child);
            thpool_add_work(c_args->pool, _threaded_quicksort, (void *)right_child);
        }
    }

    free(args);
}

void threaded_quicksort(int *const data, const int size) {
    threadpool qs_pool;
    qs_params *initial_args;

    if ((qs_pool = thpool_init(N_THREADS)) == 0) {
        return;
    }

    initial_args = malloc(sizeof(qs_params));
    initial_args->data  = data;
    initial_args->low   = 0;
    initial_args->high  = size - 1;
    initial_args->pool  = qs_pool;

    if ((thpool_add_work(qs_pool, _threaded_quicksort, (void *)initial_args)) != 0) {
        free((void *)initial_args);
        return;
    }

    thpool_wait(qs_pool);
    thpool_destroy(qs_pool);
}