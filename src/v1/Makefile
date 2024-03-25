CC=gcc
CFLAGS=-W -Wall -std=c89 -pedantic -O2
EXEC_FILES=prod

NAME_1=huff
SRC_1=huff.c
OBJ_1=$(SRC_1:.c=.o)

partie1: $(OBJ_1)
	$(CC) -o $(NAME_1) $(OBJ_1) `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` `pkg-config --libs-only-l MLV` -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

clean:
	rm -rf *.o *~ prod
