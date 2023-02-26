CC = gcc
CFLAGS = -Wall -Wextra
Files = board.c priority.c test.c util.c play.c pathfinding.c

# To Build Main Program:
main: main.c
	$(CC) $(CFLAGS) $(Files) -o main main.c

all: main.c perftest.c
	make main
	make perftest

# To build Perfomance Tester:
perftest: perftest.c
	$(CC) $(CFLAGS) $(Files) -o perftest perftest.c