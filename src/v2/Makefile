CC=gcc
CFLAGS=-W -Wall -std=c89 -pedantic -O2
MLVFLAGS=`pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` `pkg-config --libs-only-l MLV` -lm

SRC=arbre.c comp.c decomp.c utils.c
OBJ=$(SRC:.c=.o)

huff_v1: huff_v1.o $(OBJ)
	$(CC) -o huff_v1 huff_v1.o $(OBJ) $(MLVFLAGS)

huff_v2: huff_v2.o $(OBJ)
	$(CC) -o huff_v2 huff_v2.o $(OBJ) $(MLVFLAGS)

huff_v3: huff_v3.o $(OBJ)
	$(CC) -o huff_v3 huff_v3.o $(OBJ) $(MLVFLAGS)

huff_v4: huff_v4.o $(OBJ)
	$(CC) -o huff_v4 huff_v4.o $(OBJ) $(MLVFLAGS)

huff_v5: huff_v5.o $(OBJ)
	$(CC) -o huff_v5 huff_v5.o $(OBJ) $(MLVFLAGS)

huff_v6: huff_v6.o $(OBJ)
	$(CC) -o huff_v6 huff_v6.o $(OBJ) $(MLVFLAGS)

%.o: %.c headers/%.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

clean:
	rm -rf *.o *~ huff_v1 huff_v2 huff_v3 huff_v4 huff_v5 huff_v6
