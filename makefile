CC=clang-12

bin/main: src/main.c bin/quicksort.o bin/utilities.o bin/thpool.o bin/benchmark.o bin/libhoard.so
	$(CC) src/main.c -pthread -Os -g -o bin/main bin/libhoard.so bin/quicksort.o bin/utilities.o bin/thpool.o

bin/utilities.o: headers/utilities.h src/utilities.c
	$(CC) src/utilities.c -c -Os -g -o bin/utilities.o

bin/benchmark.o: src/benchmark.c
	$(CC) src/benchmark.c -c -Os -g -o bin/benchmark.os

bin/quicksort.o: headers/quicksort.h src/quicksort.c
	$(CC) src/quicksort.c -c -Os -g -o bin/quicksort.o

bin/thpool.o: C-Thread-Pool/thpool.h C-Thread-Pool/thpool.c
	$(CC) C-Thread-Pool/thpool.c -pthread -c -Os -g -o bin/thpool.o