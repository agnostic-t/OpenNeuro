CC=gcc
LINK=ar
OUTPUT=libneu-errors.a

CFLAGS=

SRCS=src/*.c
INCLUDE=include

.PHONY: all clean

all: setup
	$(CC) $(CFLAGS) -c $(SRCS) -I $(INCLUDE)
	$(LINK) rcs ./bin/$(OUTPUT) ./*.o
	$(MAKE) cleanup

cleanup:
	rm ./*.o

setup:
	mkdir -p bin
