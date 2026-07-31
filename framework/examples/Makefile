CC=gcc
CFLAGS=

SRCS=./gnuplot/gnuplot_i.c
INCLUDES=../openneuro/include
LDPATH=../openneuro/bin
LIBS=-lneu-api -lneu-models -lneu-math -lneu-errors -lm

all: setup
	$(CC) $(CFLAGS) -o ./bin/00_models_test ./00_models.c $(SRCS) -I$(INCLUDES) -L$(LDPATH) $(LIBS)
	$(CC) $(CFLAGS) -o ./bin/01_network_test ./01_network.c $(SRCS) -I$(INCLUDES) -L$(LDPATH) $(LIBS)

setup:
	mkdir -p ./bin
