# Define OS=WIN if compiling for windows
# Define MODE=DEBUG if compiling in debug mode

ifeq ($(OS), WIN)
COMP   ?= cl
CFLAGS ?= /W1 /std:c++14
ifeq ($(MODE), DEBUG)
CFLAGS += /Zi
else
CFLAGS += /o2
endif

else
COMP   ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99
ifeq ($(MODE), DEBUG)
CFLAGS += -ggdb
else
CFLAGS += -O2
endif
endif

Files = board.c priority.c test.c util.c play.c pathfinding.c

all: main perftest

main: main.c
	$(COMP) $(CFLAGS) $(Files) -o main main.c

perftest: perftest.c
	$(COMP) $(CFLAGS) $(Files) -o perftest perftest.c