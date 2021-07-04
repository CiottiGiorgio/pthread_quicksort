#include <stdlib.h>
#include <string.h>

#include "../C-Thread-Pool/thpool.h"
#include "../headers/utilities.h"

#define N_THREADS			(16)
#define HYBRID_THRESHOLD	(1<<7)
#define THREADED_THRESHOLD	(1<<16)


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
    int lower_bound = low,
        upper_bound = high;

    while (lower_bound < upper_bound) {
        if ((upper_bound - lower_bound) < HYBRID_THRESHOLD) {
            _insertionsort(data, lower_bound, upper_bound);
            break;
        } else {
            const int p = partition(data, lower_bound, upper_bound);

            if (p <= (lower_bound + upper_bound) / 2) {
                _hybrid_quicksort(data, lower_bound, p);
                lower_bound = p + 1;
            } else {
                _hybrid_quicksort(data, p+1, upper_bound);
                upper_bound = p;
            }
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
    qs_params *c_args = (qs_params *)args;

    if (c_args->low < c_args->high) {
        if (c_args->high - c_args->low < THREADED_THRESHOLD) {
            _hybrid_quicksort(c_args->data, c_args->low, c_args->high);
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

            // Why waste a perfectly good thread? Let's reuse it!
            // Although to be fair this could build up a stack as would the other versions + it doesnt free args until all the stack is complete.
            //  (Which means that ausiliary memory is release all at the end and not as soon as it is not needed anymore).
            // It is unclear at this moment wether reusing the current thread actually yields more performance. We are using a thread pool so we don't pay the overhead
            //  of spinning up a new thread.
            // _threaded_quicksort((void*)right_child);
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