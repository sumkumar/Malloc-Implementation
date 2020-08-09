a.out: main.o impl.o
	gcc main.o impl.o

main.o: main.c header.h
	gcc -c main.c

impl.o: impl.c header.h
	gcc -c impl.c
