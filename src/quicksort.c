#include <stdlib.h>
#include <string.h>

#include "../C-Thread-Pool/thpool.h"
#include "../headers/utilities.h"

#define N_THREADS			(16)
#define HYBRID_THRESHOLD	(1<<7)
#define THREADED_THRESHOLD	(1<<16)


#define SAFE_MIDDLE(a, b) (a + (b - a) / 2)


int partition(int *const data, const int low, const int high) {
    const int pivot = data[ SAFE_MIDDLE(low, high) ];
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

int quicksort(int *const data, const int size) {
    _quicksort(data, 0, size - 1);

    return 1;
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
    int lower_bound = low,
        upper_bound = high;

    while (lower_bound < upper_bound) {
        if ((upper_bound - lower_bound) < HYBRID_THRESHOLD) {
            _insertionsort(data, lower_bound, upper_bound);
            break;
        } else {
            const int p = partition(data, lower_bound, upper_bound);

            if (p <= SAFE_MIDDLE(lower_bound, upper_bound)) {
                _hybrid_quicksort(data, lower_bound, p);
                lower_bound = p + 1;
            } else {
                _hybrid_quicksort(data, p+1, upper_bound);
                upper_bound = p;
            }
        }
    }
}

int hybrid_quicksort(int *const data, const int size) {
    _hybrid_quicksort(data, 0, size - 1);

    return 1;
}


typedef struct {
    int *data;
    int low;
    int high;
    threadpool pool;
} qs_params;

void _threaded_quicksort(void *args) {
    qs_params *c_args = (qs_params *)args;

    while (c_args->low < c_args->high) {
        if (c_args->high - c_args->low < THREADED_THRESHOLD) {
            _hybrid_quicksort(c_args->data, c_args->low, c_args->high);
            break;
        } else {
            qs_params *largest_sub_array;

            const int p = partition(c_args->data, c_args->low, c_args->high);

            largest_sub_array = malloc(sizeof(qs_params));
            largest_sub_array->data = c_args->data;
            largest_sub_array->pool = c_args->pool;

            if (p <= SAFE_MIDDLE(c_args->low, c_args->high)) {
                largest_sub_array->low  = p + 1;
                largest_sub_array->high = c_args->high;

                c_args->high = p;
            } else {
                largest_sub_array->low  = c_args->low;
                largest_sub_array->high = p;

                c_args->low = p + 1;
            }

            thpool_add_work(c_args->pool, (void *)_threaded_quicksort, (void *)largest_sub_array);
        }
    }

    free(args);
}

int threaded_quicksort(int *const data, const int size) {
    threadpool qs_pool;
    qs_params *initial_args;

    if ((qs_pool = thpool_init(N_THREADS)) == NULL) {
        return 0;
    }

    initial_args = malloc(sizeof(qs_params));
    if (initial_args == NULL) {
        return 0;
    }

    initial_args->data  = data;
    initial_args->low   = 0;
    initial_args->high  = size - 1;
    initial_args->pool  = qs_pool;

    if ((thpool_add_work(qs_pool, _threaded_quicksort, (void *)initial_args)) != 0) {
        free(initial_args);
        return 0;
    }

    thpool_wait(qs_pool);
    thpool_destroy(qs_pool);

    return 1;
}