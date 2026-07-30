CC=gcc
LINK=ar
OUTPUT=libneu-math.a

CFLAGS=

SRCS=src/*.c
INCLUDE=-Iinclude -I../errors/include

.PHONY: all clean

all: setup
	$(CC) $(CFLAGS) -c $(SRCS) $(INCLUDE)
	$(LINK) rcs ./bin/$(OUTPUT) ./*.o
	$(MAKE) cleanup

cleanup:
	rm ./*.o

setup:
	mkdir -p bin
