CC=gcc
CFLAGS=-W -Wall -std=c89 -pedantic -O2
EXEC_FILES=prod
INC_DIR=headers

NAME=huffman
SRC=huffman.c arbre.c comp.c decomp.c utils.c
OBJ=$(SRC:.c=.o)

huffman: $(OBJ)
	$(CC) -o $(NAME) $(OBJ) `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` `pkg-config --libs-only-l MLV` -lm

%.o: %.c headers/%.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

clean:
	rm -rf *.o *~ prod
