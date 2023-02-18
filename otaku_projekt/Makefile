SOURCES=$(wildcard *.c)
HEADERS=$(SOURCES:.c=.h)
FLAGS=-DDEBUG -g

all: main

main: $(SOURCES) $(HEADERS)
	mpicc $(SOURCES) $(FLAGS) -o main

clear: clean

clean:
	rm main a.out

run2: main
	mpirun -default-hostfile hosts -np 5 ./main

run: main
	mpirun -oversubscribe -np 5 ./main
