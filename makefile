# Define OS=WIN if compiling for windows
# Define MODE=RELEASE if compiling in release mode

MODE ?= DEBUG

ifeq ($(OS),WIN)
COMP   ?= cl
CFLAGS ?= /W1 /std:c++14
ifeq ($(MODE),RELEASE)
CFLAGS += /o2
else
CFLAGS += /Zi
endif

else
COMP   ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99
ifeq ($(MODE), RELEASE)
CFLAGS += -O2
else
CFLAGS += -ggdb
endif
endif

all: main perftest

main: main.c
	$(COMP) $(CFLAGS) -o main main.c

perftest: perftest.c
	$(COMP) $(CFLAGS) -o perftest perftest.c