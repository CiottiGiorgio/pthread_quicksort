CC=clang-12

main: bin/quicksort.o bin/utilities.o
	$(CC) -g -o bin/main src/main.c bin/quicksort.o bin/utilities.o

bin/quicksort.o: headers/quicksort.h src/quicksort.c
	$(CC) -g -c -o bin/quicksort.o src/quicksort.c

bin/utilities.o: headers/utilities.h src/utilities.c
	$(CC) -g -c -o bin/utilities.o src/utilities.c