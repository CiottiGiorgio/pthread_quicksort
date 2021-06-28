#include "../C-Thread-Pool/thpool.h"


int partition(int *const data, const int low, const int high);

void _quicksort(int *const data, const int low, const int high);
void quicksort(int *const data, const int size);

void _insertionsort(int *const data, const int low, const int high);

void _hybrid_quicksort(int *const data, const int low, const int high);
void hybrid_quicksort(int *const data, const int size);

void _threaded_quicksort(void *args);
int threaded_quicksort(int *const data, const int size);