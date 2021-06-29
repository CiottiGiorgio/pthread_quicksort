CC=clang-12

bin/main: src/main.c bin/quicksort.o bin/utilities.o bin/thpool.o bin/benchmark.o bin/libhoard.so
	$(CC) src/main.c -pthread -O3 -o bin/main bin/libhoard.so bin/quicksort.o bin/utilities.o bin/thpool.o

bin/utilities.o: headers/utilities.h src/utilities.c
	$(CC) src/utilities.c -c -O3 -o bin/utilities.o

bin/benchmark.o: src/benchmark.c
	$(CC) src/benchmark.c -c -O3 -o bin/benchmark.o

bin/quicksort.o: headers/quicksort.h src/quicksort.c
	$(CC) src/quicksort.c -c -O3 -o bin/quicksort.o

bin/thpool.o: C-Thread-Pool/thpool.h C-Thread-Pool/thpool.c
	$(CC) C-Thread-Pool/thpool.c -pthread -c -O3 -o bin/thpool.o