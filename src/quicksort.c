#include <stdlib.h>

#include "../C-Thread-Pool/thpool.h"

#include "../headers/utilities.h"


#define SORTING_THRESHOLD 32


int partition(int *const data, const int low, const int high) {
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
} qsargs;

void _threaded_quicksort(void *args) {
    qsargs *aargs = (qsargs *)args;

    if (aargs->low < aargs->high) {
        if (aargs->high - aargs->low < SORTING_THRESHOLD) {
            _insertionsort(aargs->data, aargs->low, aargs->high);
        } else {
            qsargs *child1_args, *child2_args;

            const int p = partition(aargs->data, aargs->low, aargs->high);

            child1_args = (qsargs *)malloc(sizeof(qsargs));
            child1_args->data   = aargs->data;
            child1_args->low    = aargs->low;
            child1_args->high   = p;
            child1_args->pool   = aargs->pool;

            child2_args = (qsargs *)malloc(sizeof(qsargs));
            child2_args->data   = aargs->data;
            child2_args->low    = p + 1;
            child2_args->high   = aargs->high;
            child2_args->pool   = aargs->pool;

            thpool_add_work(aargs->pool, _threaded_quicksort, (void *)child1_args);
            thpool_add_work(aargs->pool, _threaded_quicksort, (void *)child2_args);
        }
    }

    free(aargs);
}

int threaded_quicksort(int *const data, const int size) {
    threadpool th_pool;

    if ((th_pool = thpool_init(16)) == 0) {
        return -1;
    }

    qsargs *initial_args = (qsargs *)malloc(sizeof(qsargs));
    initial_args->data = data;
    initial_args->low = 0;
    initial_args->high = size - 1;
    initial_args->pool = th_pool;

    if ((thpool_add_work(th_pool, _threaded_quicksort, (void *)initial_args)) != 0) {
        return -1;
    }

    thpool_wait(th_pool);
    thpool_destroy(th_pool);

    return 0;
}