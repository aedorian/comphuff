$CFLAGS =-W -Wall -ansi -pedantic -O2


all: analyse

analyse: test_compression.o gen_fich.o
	gcc $(CFLAGS) $^ -o $@

test_compression.o: test_compression.c
	gcc $(CFLAGS) -c test_compression.c -o $@ -lm

gen_fich.o: gen_fich.c gen_fich.h
	gcc $(CFLAGS) -c gen_fich.c -o $@