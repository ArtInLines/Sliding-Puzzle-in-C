ifndef $(CC)
	CC = gcc
endif
ifndef $(CFLAGS)
	CFLAGS = -Wall -Wextra -Wpedantic -std=c99
endif

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