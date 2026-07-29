CC=gcc
CFLAGS=

SRCS=math/mat.c gnuplot/gnuplot_i.c

all: setup
	$(CC) $(CFLAGS) -o ../bin/00_mat_test 00_mat_math.c $(SRCS)
	$(CC) $(CFLAGS) -o ../bin/01_lif_test 01_lif.c $(SRCS)
	$(CC) $(CFLAGS) -o ../bin/02_eif_test 02_eif.c $(SRCS) -lm
	$(CC) $(CFLAGS) -o ../bin/03_adex_test 03_adex.c $(SRCS) -lm
	$(CC) $(CFLAGS) -o ../bin/04_izhikevich_test 04_izhikevich.c $(SRCS) -lm
	$(CC) $(CFLAGS) -o ../bin/05_hodjkin_haxly_test 05_hodjkin_haxly.c $(SRCS) -lm

setup:
	mkdir -p ../bin
