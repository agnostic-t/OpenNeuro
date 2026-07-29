CC=gcc
CFLAGS=

SRCS=math/mat.c gnuplot/gnuplot_i.c

all: setup
	$(CC) $(CFLAGS) -o ../bin/00_mat_test 00_mat_math.c $(SRCS)
	$(CC) $(CFLAGS) -o ../bin/01_lif_test 01_lif.c $(SRCS)

setup:
	mkdir -p ../bin
