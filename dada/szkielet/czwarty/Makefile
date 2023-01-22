FLAGS=-DDEBUG
all: main

main: main.c main.h util.c util.h
	mpicc $(FLAGS) main.c util.c -o main

run: main
	mpirun -np 4 main

clean:
	rm -f main a.out
