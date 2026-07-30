CC=gcc
CFLAGS=

SRCS=./gnuplot/gnuplot_i.c
INCLUDES=../openneuro/include
LDPATH=../openneuro/bin

all: setup
	$(CC) $(CFLAGS) -o ./bin/00_models_test ./00_models_example.c $(SRCS) -I$(INCLUDES) -L$(LDPATH) -lneu-api -lneu-models -lneu-math -lneu-errors -lm

setup:
	mkdir -p ./bin
