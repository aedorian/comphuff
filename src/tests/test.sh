#!/usr/bin/env bash
gcc -W -Wall -ansi -pedantic -O2 test_compression.c -o test -lm
./test > /dev/null
python3 graphique.py