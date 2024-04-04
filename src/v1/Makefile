CC=gcc
CFLAGS=-W -Wall -std=c89 -pedantic -O2
EXEC_FILES=prod

NAME_1=huff
SRC_1=huff.c arbre.c
OBJ_1=$(SRC_1:.c=.o)

NAME_2=testes
SRC_2=test.c arbre.c
OBJ_2=$(SRC_2:.c=.o)

testes: $(OBJ_2)
	$(CC) -o $(NAME_2) $(OBJ_2) `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` `pkg-config --libs-only-l MLV` -lm

huffv1: $(OBJ_1)
	$(CC) -o $(NAME_1) $(OBJ_1) `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` `pkg-config --libs-only-l MLV` -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

clean:
	rm -rf *.o *~ prod
